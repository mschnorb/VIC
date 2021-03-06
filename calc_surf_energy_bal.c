#include <stdio.h>
#include <stdlib.h>
#include "vicNl.h"

static char vcid[] = "$Id$";

double calc_surf_energy_bal(double             latent_heat_Le,
			    double             LongUnderIn,
			    double             NetLongSnow, // net LW at snow surface
			    double             NetShortGrnd, // net SW transmitted thru snow
			    double             NetShortSnow, // net SW at snow surface
			    double             OldTSurf,
			    double             ShortUnderIn,
			    double             SnowAlbedo,
			    double             SnowLatent,
			    double             SnowLatentSub,
			    double             SnowSensible,
			    double             Tair, // T of canopy or air
			    double             VPDcanopy,
			    double             VPcanopy,
			    double             advection,
			    double             coldcontent,
			    double             delta_coverage, // change in coverage fraction
			    double             dp,
			    double             ice0,
			    double             melt_energy,
			    double             moist,
			    double             precipitation_mu,
			    double             snow_coverage,
			    double             snow_depth,
			    double             BareAlbedo,
			    double             surf_atten,
			    double             vapor_flux,
			    VegConditions     &aero_resist,
			    AeroResistUsed    &aero_resist_used,
			    VegConditions     &displacement,
			    double            *melt,
			    double            *ppt,
			    double            *rainfall,
			    VegConditions     &ref_height,
			    VegConditions     &roughness,
			    double            *snowfall,
			    VegConditions     &wind_speed,
			    const float       *root,
			    int                INCLUDE_SNOW,
			    VegConditions::VegSurfType UnderStory,
			    int                Nnodes,
			    int                dt,
			    int                hour,
			    int                nlayer,
			    int                overstory,
			    int                rec,
			    int                veg_class,
			    bool               isArtificialBareSoil,
			    atmos_data_struct *atmos,
			    const dmy_struct  *dmy,
			    energy_bal_struct *energy,
			    layer_data_struct *layer_dry,
			    layer_data_struct *layer_wet,
			    snow_data_struct  *snow,
			    const soil_con_struct *soil_con,
			    veg_var_struct    *veg_var_dry,
			    veg_var_struct    *veg_var_wet,
			    int                nrecs,
			    const ProgramState* state)
/**************************************************************
  calc_surf_energy_bal.c  Greg O'Donnell and Keith Cherkauer  Sept 9 1997
  
  This function calculates the surface temperature, in the
  case of no snow cover.  Evaporation is computed using the
  previous ground heat flux, and then used to comput latent heat
  in the energy balance routine.  Surface temperature is found
  using the Root Brent method (Numerical Recipies).
  

  modifications:
    02-29-00  Included variables needed to compute energy flux
              through the snow pack.  The ground surface energy
              balance will now be a mixture of snow covered
	      and bare ground, controlled by the snow cover 
	      fraction set in solve_snow.c                 KAC
    6-8-2000  Modified to make use of spatially distributed 
              soil frost                                   KAC
    03-09-01  Added QUICK_SOLVE options for finite difference
              soil thermal solution.  By iterating on only a
              few choice nodes near the soil surface the 
              simulation time can be significantly reduced
              with minimal additional energy balance errors.  KAC
    11-18-02  Modified to include the effects of blowing snow
              on the surface energy balance.                 LCB
    07-30-03  Made sure that local NOFLUX variable is always set
              to the options flag value.                      KAC
    04-Jun-04 Placed "ERROR" at beginning of screen dump in
	      error_print_surf_energy_bal.				TJB
    16-Jul-04 Cast the last 6 variables in the parameter list
	      passed to root_brent, error_calc_surf_energy_bal
	      and solve_surf_energy_bal as double, since for
	      some reason letting them remain ints or floats
	      caused them to become garbage in the child
	      functions.						TJB
    16-Jul-04 Modified the cap on vapor_flux to re-scale
	      blowing_flux and surface_flux proportionally
	      so that vapor_flux still = their sum.			TJB
    05-Aug-04 Removed lag_one, sigma_slope, and fetch from
	      parameter list, since these were only used in
	      the call to root_brent/func_surf_energy_bal(),
	      which no longer needs them.				TJB
    24-Aug-04 Modified the re-scaling of surface_flux to reduce
	      round-off errors.						TJB
    21-Sep-04 Added ErrorString to store error messages from
	      root_brent.						TJB
    28-Sep-04 Added aero_resist_used to store the aerodynamic
	      resistance used in flux calculations.			TJB
  2007-Apr-06 Modified to handle grid cell errors by returning to the
	      main subroutine, rather than ending the simulation.	GCT/KAC
  2007-Apr-24 Changed the read-in order of iveg, and VEG in
	      error_print_surf_energy_bal to be consistent with the
	      call order, also added year, day, and hour to the
	      argument list.						JCA
  2007-Apr-24 Features included for IMPLICIT frozen soils option.	JCA 
	      including:
	        passing in nrecs
	        passing nrec, nrecs, and iveg to func_surf_energy_bal
	        passing bulk_density, soil_density, and quartz to
	          func_surf_energy_bal
  2007-Apr-24 Features included for EXP_TRANS option for frozen soils
	      algorithm.						JCA
  2007-Apr-24 Passing in Zsum_node rather than recalculating.		JCA
  2007-Aug-08 Features included for EXCESS_ICE option for frozen soils
	      algorithm.						JCA
	      including:
	        passing in entire soil_con structure.
  2007-Aug-31 Checked root_brent return value against -998 rather
	      than -9998.						JCA
  2007-Sep-01 Checked for return value of ERROR from
	      solve_surf_energy_bal.					JCA
  2007-Nov-09 Modified code to reset NOFLUX boundary to global option
	      value before computing final soil column temperatures.
	      Previously NOFLUX was set to FALSE for initial QUICK_SOLVE
	      estimates, but never reset to reflect actual bottom
	      boundary conditions for final pass through solver.	KAC
  2009-Feb-09 Removed dz_node from variables passed to
	      func_surf_energy_bal.					KAC via TJB
  2009-May-22 Added TFALLBACK value to options.CONTINUEONERROR.  This
	      allows simulation to continue when energy balance fails
	      to converge by using previous T value.			TJB
  2009-Jun-19 Added T flag to indicate whether TFALLBACK occurred.	TJB
  2009-Sep-19 Added T fbcount to count TFALLBACK occurrences.		TJB
  2009-Nov-15 Fixed initialization of Tsurf_fbcount.			TJB
  2009-Nov-15 Changed definitions of D1 and D2 to work for arbitrary
	      node spacing.						TJB
  2009-Dec-11 Replaced "assert" statements with "if" statements.	TJB
  2011-May-24 Replaced call to finish_frozen_soil_calcs() with a call
	      to calc_layer_average_thermal_props(); expanded the set of
	      cases for which this function is called to include
	      FROZEN_SOIL FALSE and QUICK_FLUX TRUE, so that a soil T
	      profile can be estimated and output in these cases.	TJB
  2011-May-31 Removed options.GRND_FLUX.  Now soil temperatures and
	      ground flux are always computed.				TJB
  2011-Aug-09 Now initialize soil thermal properties for all modes of
	      operation.						TJB
***************************************************************/
{
  int      FIRST_SOLN[2];
  int      NOFLUX;
  int      EXP_TRANS;
  int      VEG;
  int      i;
  int      nidx;
  int      tmpNnodes;

  double   Cs1;
  double   Cs2;
  double   D1;
  double   D2;
  double   LongBareIn;
  double   NetLongBare;
  double   NetShortBare;
  double   T1;
  double   T1_old;
  double   T2;
  double   Ts_old;
  double   Tsnow_surf;
  double   Tsurf; 
  char     Tsurf_fbflag; 
  int      Tsurf_fbcount; 
  double   albedo;
  double   atmos_density;
  double   atmos_pressure;
  double   bubble;
  double   delta_t;
  double   emissivity;
  double   error;
  double   expt;
  double   kappa1;
  double   kappa2;
  double   kappa_snow;
  double   max_moist;
  double   ra;
  double   refrozen_water;

  double   Wdew[2];
  double   Tnew_node[MAX_NODES];
  char     Tnew_fbflag[MAX_NODES];
  int      Tnew_fbcount[MAX_NODES];
  layer_data_struct layer[MAX_LAYERS];

  double   T_lower, T_upper;
  double   LongSnowIn;
  double   TmpNetLongSnow;
  double   TmpNetShortSnow;
  double   old_swq, old_depth;
  char ErrorString[MAXSTRING];

  /**************************************************
    Set All Variables For Use
  **************************************************/
  /* Initialize T_fbflag */
  Tsurf_fbflag = 0;
  Tsurf_fbcount = 0;
  for (nidx=0; nidx<Nnodes; nidx++) {
    Tnew_fbflag[nidx] = 0;
    Tnew_fbcount[nidx] = 0;
  }

  if(isArtificialBareSoil == false) {
    if(state->veg_lib[veg_class].LAI[dmy->month-1] > 0.0) VEG = TRUE;
    else VEG = FALSE;
  }
  else VEG = FALSE;

  T2                  = energy->T[Nnodes-1]; // soil column bottom temp
  Ts_old              = energy->T[0]; // previous surface temperature
  T1_old              = energy->T[1]; // previous first node temperature
  atmos_density       = atmos->density[hour]; // atmospheric density
  atmos_pressure      = atmos->pressure[hour]; // atmospheric pressure
  emissivity          = 1.; // longwave emissivity
  kappa1              = energy->kappa[0]; // top node conductivity
  kappa2              = energy->kappa[1]; // second node conductivity
  Cs1                 = energy->Cs[0]; // top node heat capacity
  Cs2                 = energy->Cs[1]; // second node heat capacity
  D1                  = soil_con->Zsum_node[1]-soil_con->Zsum_node[0]; // top node thickness
  D2                  = soil_con->Zsum_node[2]-soil_con->Zsum_node[1]; // second node thickness
  delta_t             = (double)dt * 3600.;
  max_moist           = soil_con->max_moist[0] / (soil_con->depth[0]*1000.);
  bubble              = soil_con->bubble[0];
  expt                = soil_con->expt[0];
  Tsnow_surf          = snow->surf_temp;
  Wdew[WET]           = veg_var_wet->Wdew;
  if(state->options.DIST_PRCP) Wdew[DRY] = veg_var_dry->Wdew;
  FIRST_SOLN[0] = TRUE;
  FIRST_SOLN[1] = TRUE;
  if ( snow->depth > 0. ) 
    kappa_snow = K_SNOW * (snow->density) * (snow->density) / snow_depth;
  else 
    kappa_snow = 0;

  /** compute incoming and net radiation **/
  NetShortBare  = ( ShortUnderIn * (1. - ( snow_coverage + delta_coverage ) ) 
		    * (1. - BareAlbedo) + ShortUnderIn * ( delta_coverage ) 
		    * ( 1. - SnowAlbedo ) );
  LongBareIn    = (1. - snow_coverage ) * LongUnderIn;
  if ( INCLUDE_SNOW || snow->swq == 0 ) { 
    TmpNetLongSnow  = NetLongSnow;
    TmpNetShortSnow = NetShortSnow;
    LongSnowIn      = snow_coverage * LongUnderIn;
  }
  else {
    TmpNetShortSnow = 0.; 
    TmpNetLongSnow  = 0.; 
    LongSnowIn      = 0.;
  }

  /**************************************************
    Find Surface Temperature Using Root Brent Method
  **************************************************/
  if(state->options.FULL_ENERGY) {

    /** If snow included in solution, temperature cannot exceed 0C  **/
    if ( INCLUDE_SNOW ) {
      T_lower = energy->T[0]-SURF_DT;
      T_upper = 0.;
    }
    else {
      T_lower = 0.5*(energy->T[0]+Tair)-SURF_DT;
      T_upper = 0.5*(energy->T[0]+Tair)+SURF_DT;
    }

    if ( state->options.QUICK_SOLVE && !state->options.QUICK_FLUX ) {
      // Set iterative Nnodes using the depth of the thaw layer
      tmpNnodes = 0;
      for ( nidx = Nnodes-5; nidx >= 0; nidx-- ) 
	if ( energy->T[nidx] >= 0 && energy->T[nidx+1] < 0 ) tmpNnodes = nidx+1;
      if ( tmpNnodes == 0 ) { 
	if ( energy->T[0] <= 0 && energy->T[1] >= 0 ) tmpNnodes = Nnodes;
	else tmpNnodes = 3;
      }
      else tmpNnodes += 4;
      NOFLUX = FALSE;

/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
      EXP_TRANS = FALSE;  // Why would we do this???
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/
/***********************************************************************/

    }
    else { 
      tmpNnodes = Nnodes;
      NOFLUX = state->options.NOFLUX;
      EXP_TRANS = state->options.EXP_TRANS;
    }

    SurfEnergyBal surfEnergyBalIterative(rec, nrecs, dmy->month, VEG, veg_class, delta_t, Cs1, Cs2, D1, D2,
        T1_old, T2, Ts_old, bubble, dp,
        expt, ice0, kappa1, kappa2,
        max_moist, moist, root,
        UnderStory, overstory, NetShortBare, NetShortGrnd,
        TmpNetShortSnow, Tair, atmos_density,
        atmos_pressure,
        emissivity, LongBareIn, LongSnowIn, precipitation_mu, surf_atten,
        VPcanopy, VPDcanopy,
        Wdew, displacement, aero_resist, aero_resist_used,
        rainfall, ref_height, roughness, wind_speed, latent_heat_Le,
        energy->advection, OldTSurf, snow->pack_temp,
        Tsnow_surf, kappa_snow, melt_energy,
        snow_coverage,
        snow->density, snow->swq, snow->surf_water,
        &energy->deltaCC, &energy->refreeze_energy,
        &snow->vapor_flux, &snow->blowing_flux, &snow->surface_flux,
        tmpNnodes, energy->Cs_node, energy->T, Tnew_node, Tnew_fbflag, Tnew_fbcount,
        energy->ice_content, energy->kappa_node, energy->moist,
        soil_con, layer_wet, layer_dry, veg_var_wet, veg_var_dry,
        INCLUDE_SNOW, NOFLUX, EXP_TRANS, snow->snow,
        FIRST_SOLN, &NetLongBare, &TmpNetLongSnow, &T1,
        &energy->deltaH, &energy->fusion, &energy->grnd_flux,
        &energy->latent, &energy->latent_sub,
        &energy->sensible, &energy->snow_flux, &energy->error, state);
    Tsurf = surfEnergyBalIterative.root_brent(T_lower, T_upper, ErrorString);
 
    if(surfEnergyBalIterative.resultIsError(Tsurf)) {
      if (state->options.TFALLBACK) {
        Tsurf = Ts_old;
        Tsurf_fbflag = 1;
        Tsurf_fbcount++;
      }
      else {
        fprintf(stderr, "SURF_DT = %.2f\n", SURF_DT);
        error = error_print_surf_energy_bal(Tsurf, dmy->year, dmy->month, dmy->day, dmy->hour, VEG,
					   veg_class, delta_t, Cs1, Cs2, D1, D2, 
					   T1_old, T2, Ts_old, 
					   bubble, dp,
					   expt, ice0, kappa1, kappa2, 
					   max_moist,
					   moist, root,
					   UnderStory, overstory, NetShortBare, 
					   NetShortGrnd, TmpNetShortSnow, Tair, 
					   atmos_density, atmos_pressure, 
					   emissivity, LongBareIn, LongSnowIn, 
					   precipitation_mu, surf_atten, VPcanopy, VPDcanopy, 
					   Wdew, displacement, aero_resist, aero_resist_used, 
					   rainfall, ref_height, roughness, 
					   wind_speed, latent_heat_Le, energy->advection, 
					   OldTSurf, snow->pack_temp, 
					   Tsnow_surf, 
					   kappa_snow, melt_energy, 
					   snow_coverage, snow->density, 
					   snow->swq, snow->surf_water, 
					   &energy->deltaCC, 
					   &energy->refreeze_energy, 
					   &snow->vapor_flux, Nnodes, energy->Cs_node, 
					   energy->T, Tnew_node, energy->ice_content, energy->kappa_node,
					   energy->moist,
					   layer_wet, layer_dry, veg_var_wet, 
					   veg_var_dry, 
					   INCLUDE_SNOW,
					   NOFLUX, EXP_TRANS,
					   snow->snow, FIRST_SOLN, &NetLongBare, 
					   &TmpNetLongSnow, &T1, &energy->deltaH, 
					   &energy->fusion, &energy->grnd_flux, 
					   &energy->latent, 
					   &energy->latent_sub, 
					   &energy->sensible, 
					   &energy->snow_flux, &energy->error, ErrorString, soil_con, state);
        return ( ERROR );
      }
    }

    /**************************************************
      Recalculate Energy Balance Terms for Final Surface Temperature
    **************************************************/

    if ( Ts_old * Tsurf < 0 && state->options.QUICK_SOLVE ) {
      tmpNnodes = Nnodes;
      NOFLUX = state->options.NOFLUX;
      FIRST_SOLN[0] = TRUE;
      
      SurfEnergyBal surfEnergyBalIter2(rec, nrecs, dmy->month, VEG, veg_class,
          delta_t, Cs1, Cs2, D1, D2, T1_old, T2, Ts_old, bubble, dp, expt,
          ice0, kappa1, kappa2, max_moist, moist, root, UnderStory, overstory,
          NetShortBare, NetShortGrnd, TmpNetShortSnow, Tair, atmos_density,
          atmos_pressure, emissivity, LongBareIn, LongSnowIn, precipitation_mu,
          surf_atten, VPcanopy, VPDcanopy, Wdew, displacement, aero_resist,
          aero_resist_used, rainfall, ref_height, roughness, wind_speed,
          latent_heat_Le, energy->advection, OldTSurf, snow->pack_temp,
          Tsnow_surf, kappa_snow, melt_energy, snow_coverage, snow->density,
          snow->swq, snow->surf_water, &energy->deltaCC,
          &energy->refreeze_energy, &snow->vapor_flux, &snow->blowing_flux,
          &snow->surface_flux, tmpNnodes, energy->Cs_node, energy->T, Tnew_node,
          Tnew_fbflag, Tnew_fbcount, energy->ice_content, energy->kappa_node,
          energy->moist, soil_con, layer_wet, layer_dry, veg_var_wet,
          veg_var_dry, INCLUDE_SNOW, NOFLUX, EXP_TRANS, snow->snow, FIRST_SOLN,
          &NetLongBare, &TmpNetLongSnow, &T1, &energy->deltaH, &energy->fusion,
          &energy->grnd_flux, &energy->latent, &energy->latent_sub,
          &energy->sensible, &energy->snow_flux, &energy->error, state);
      
      Tsurf = surfEnergyBalIter2.root_brent(T_lower, T_upper, ErrorString);


      if(surfEnergyBalIter2.resultIsError(Tsurf)) {
        if (state->options.TFALLBACK) {
          Tsurf = Ts_old;
          Tsurf_fbflag = 1;
          Tsurf_fbcount++;
        }
        else {
	  error = error_print_surf_energy_bal(Tsurf, dmy->year, dmy->month, dmy->day, dmy->hour, VEG,
	             veg_class, delta_t, Cs1, Cs2, D1,
					     D2, T1_old, T2, Ts_old, 
					     bubble, dp,
					     expt, ice0, kappa1, kappa2, 
					     max_moist,
					     moist, root,
					     UnderStory, overstory, 
					     NetShortBare, NetShortGrnd, 
					     TmpNetShortSnow, Tair, 
					     atmos_density, atmos_pressure,
					     emissivity, LongBareIn, LongSnowIn, 
					     precipitation_mu, surf_atten, VPcanopy, 
					     VPDcanopy, Wdew, displacement, 
					     aero_resist, aero_resist_used, rainfall, ref_height, 
					     roughness, wind_speed, latent_heat_Le, 
					     energy->advection, 
					     OldTSurf, snow->pack_temp, 
					     Tsnow_surf, 
					     kappa_snow, melt_energy, 
					     snow_coverage, snow->density, 
					     snow->swq, snow->surf_water, 
					     &energy->deltaCC, 
					     &energy->refreeze_energy, 
					     &snow->vapor_flux, Nnodes, energy->Cs_node, 
					     energy->T, Tnew_node, energy->ice_content, energy->kappa_node,
					     energy->moist,
					     layer_wet, layer_dry, veg_var_wet, 
					     veg_var_dry, INCLUDE_SNOW, 
					     NOFLUX, EXP_TRANS,
					     snow->snow, FIRST_SOLN, &NetLongBare, 
					     &TmpNetLongSnow, &T1, 
					     &energy->deltaH, &energy->fusion, 
					     &energy->grnd_flux, 
					     &energy->latent, 
					     &energy->latent_sub, 
					     &energy->sensible, 
					     &energy->snow_flux, &energy->error, ErrorString, soil_con, state);
          return ( ERROR );
        }
      }
    }
  }
  else {

    /** Frozen soil model run with no surface energy balance **/
    Tsurf  = Tair;
    NOFLUX = state->options.NOFLUX;
    EXP_TRANS = state->options.EXP_TRANS;

  }
  
  if ( state->options.QUICK_SOLVE && !state->options.QUICK_FLUX )
    // Reset model so that it solves thermal fluxes for full soil column
    FIRST_SOLN[0] = TRUE;
  SurfEnergyBal surfEnergyBal(rec, nrecs, dmy->month, VEG, veg_class,
      delta_t, Cs1, Cs2, D1, D2, T1_old, T2, Ts_old, bubble, dp, expt, ice0,
      kappa1, kappa2, max_moist, moist, root, UnderStory, overstory,
      NetShortBare, NetShortGrnd, TmpNetShortSnow, Tair, atmos_density,
      atmos_pressure, emissivity, LongBareIn, LongSnowIn, precipitation_mu,
      surf_atten, VPcanopy, VPDcanopy, Wdew, displacement, aero_resist,
      aero_resist_used, rainfall, ref_height, roughness, wind_speed, latent_heat_Le,
      energy->advection, OldTSurf, snow->pack_temp, Tsnow_surf, kappa_snow,
      melt_energy, snow_coverage, snow->density, snow->swq, snow->surf_water,
      &energy->deltaCC, &energy->refreeze_energy, &snow->vapor_flux,
      &snow->blowing_flux, &snow->surface_flux, Nnodes, energy->Cs_node,
      energy->T, Tnew_node, Tnew_fbflag, Tnew_fbcount, energy->ice_content,
      energy->kappa_node, energy->moist, soil_con, layer_wet, layer_dry,
      veg_var_wet, veg_var_dry, INCLUDE_SNOW, NOFLUX, EXP_TRANS, snow->snow,
      FIRST_SOLN, &NetLongBare, &TmpNetLongSnow, &T1, &energy->deltaH,
      &energy->fusion, &energy->grnd_flux, &energy->latent, &energy->latent_sub,
      &energy->sensible, &energy->snow_flux, &energy->error, state);
  error = surfEnergyBal.calculate(Tsurf);

  if(error == ERROR)
    return(ERROR);
  else
    energy->error = error;
  
  /***************************************************
    Recalculate Soil Moisture and Thermal Properties
  ***************************************************/
    if(state->options.QUICK_FLUX || !(state->options.FULL_ENERGY || (state->options.FROZEN_SOIL && soil_con->FS_ACTIVE))) {

      Tnew_node[0] = Tsurf;
      Tnew_node[1] = T1;
      Tnew_node[2] = T2;

    }
  calc_layer_average_thermal_props(energy, layer_wet, layer_dry, layer,
      soil_con, Nnodes, veg_class, Tnew_node, state);

  /** Store precipitation that reaches the surface */
  if (!snow->snow && !INCLUDE_SNOW) {
    if (isArtificialBareSoil == false) {
      if (state->veg_lib[veg_class].LAI[dmy->month - 1] <= 0.0) {
        veg_var_wet->throughfall = rainfall[WET];
        ppt[WET] = veg_var_wet->throughfall;
        if (state->options.DIST_PRCP) {
          veg_var_dry->throughfall = rainfall[DRY];
          ppt[DRY] = veg_var_dry->throughfall;
        }
      } else {
        ppt[WET] = veg_var_wet->throughfall;
        if (state->options.DIST_PRCP)
          ppt[DRY] = veg_var_dry->throughfall;
      }
    } else {
      ppt[WET] = rainfall[WET];
      if (state->options.DIST_PRCP)
        ppt[DRY] = rainfall[DRY];
    }
  }

  /****************************************
    Store understory energy balance terms 
  ****************************************/

// energy->sensible + energy->latent + energy->latent_sub + NetShortBare + NetLongBare + energy->grnd_flux + energy->deltaH + energy->fusion + energy->snow_flux

  energy->NetShortGrnd = NetShortGrnd;
  if ( INCLUDE_SNOW ) {
    energy->NetLongUnder  = NetLongBare + TmpNetLongSnow;
    energy->NetShortUnder = NetShortBare + TmpNetShortSnow + NetShortGrnd;
    energy->latent        = (energy->latent);
    energy->latent_sub    = (energy->latent_sub);
    energy->sensible      = (energy->sensible);
  }
  else {
    energy->NetLongUnder  = NetLongBare + NetLongSnow;
    energy->NetShortUnder = NetShortBare + NetShortSnow + NetShortGrnd;
/*     energy->latent        = (SnowLatent + (1. - snow_coverage)  */
/* 			     * energy->latent); */
/*     energy->latent_sub    = (SnowLatentSub  */
/* 			     + (1. - snow_coverage) * energy->latent_sub); */
/*     energy->sensible      = (SnowSensible  */
/* 			     + (1. - snow_coverage) * energy->sensible); */
    energy->latent        = (SnowLatent + energy->latent);
    energy->latent_sub    = (SnowLatentSub + energy->latent_sub);
    energy->sensible      = (SnowSensible + energy->sensible);
  }
  energy->LongUnderOut  = LongUnderIn - energy->NetLongUnder;
  energy->AlbedoUnder   = ((1. - ( snow_coverage + delta_coverage ) ) 
			   * BareAlbedo + ( snow_coverage + delta_coverage ) 
			   * SnowAlbedo );
  energy->melt_energy   = melt_energy;
  energy->Tsurf         = (snow->coverage * snow->surf_temp 
			   + (1. - snow->coverage) * Tsurf);

  /*********************************************************************
    adjust snow water balance for vapor mass flux if snowpack included 
  *********************************************************************/

//NEED TO ADJUST SNOW COVERAGE FRACTION - AND STORAGE

  if ( INCLUDE_SNOW ) {

    // don't allow vapor_flux to exceed snow pack
    if (-(snow->vapor_flux) > snow->swq) {
      // if vapor_flux exceeds snow pack, we not only need to
      // re-scale vapor_flux, we need to re-scale surface_flux and blowing_flux
      snow->blowing_flux *= -( snow->swq / snow->vapor_flux );
//      snow->surface_flux *= -( snow->swq / snow->vapor_flux );
      snow->vapor_flux = -(snow->swq);
      snow->surface_flux = snow->vapor_flux - snow->blowing_flux;
    }

    /* adjust snowpack for vapor flux */
    old_swq           = snow->swq;
    snow->swq        += snow->vapor_flux;
    snow->surf_water += snow->vapor_flux;
    snow->surf_water  = ( snow->surf_water < 0 ) ? 0. : snow->surf_water;

    /* compute snowpack melt or refreeze */
    if (energy->refreeze_energy >= 0.0) {
      refrozen_water = energy->refreeze_energy / ( Lf * RHO_W ) * delta_t; 
      if ( refrozen_water > snow->surf_water) {
        refrozen_water = snow->surf_water;
        energy->refreeze_energy = refrozen_water * Lf * RHO_W / delta_t;
      } 
      snow->surf_water -= refrozen_water;
      if (snow->surf_water < 0.0) snow->surf_water = 0.0;
      (*melt)           = 0.0;

    }
    else {
      
      /* Calculate snow melt */      
      (*melt) = fabs(energy->refreeze_energy) / (Lf * RHO_W) * delta_t;
      snow->swq -= *melt;
      if ( snow->swq < 0 ) { 
	(*melt) += snow->swq;
	snow->swq = 0;
      }
    }

    if ( snow->swq > 0 ) {

      // set snow energy terms
      snow->surf_temp   = ( Tsurf > 0 ) ? 0 : Tsurf;
      snow->coldcontent = CH_ICE * snow->surf_temp * snow->swq;

      // recompute snow depth
      old_depth   = snow->depth;
      snow->depth = 1000. * snow->swq / snow->density; 
      
      /** Check for Thin Snowpack which only Partially Covers Grid Cell
	  exists only if not snowing and snowpack has started to melt **/
#if SPATIAL_SNOW
#error // SPATIAL_SNOW is an untested code path. Continue at your own risk!
      snow->coverage = calc_snow_coverage(&snow->store_snow, 
					  soil_con->depth_full_snow_cover, 
					  snow_coverage, snow->swq,
					  old_swq, snow->depth, old_depth, 
					  (*melt) - snow->vapor_flux, 
					  &snow->max_swq, snowfall, 
					  &snow->store_swq, 
					  &snow->swq_slope,
					  &snow->store_coverage);
      
#else
      if ( snow->swq > 0 ) snow->coverage = 1.;
      else snow->coverage = 0.;
#endif // SPATIAL_SNOW

      if ( IS_INVALID(snow->surf_temp) || snow->surf_temp > 0 )

	energy->snow_flux = ( energy->grnd_flux + energy->deltaH 
			      + energy->fusion );

    }
    else {
      /* snowpack melts completely */
      snow->density    = 0.;
      snow->depth      = 0.;
      snow->surf_water = 0;
      snow->pack_water = 0;
      snow->surf_temp  = 0;
      snow->pack_temp  = 0;
      snow->coverage   = 0;
#if SPATIAL_SNOW
      snow->store_swq = 0.;
#endif // SPATIAL_SNOW
    }
    snow->vapor_flux *= -1;
  }

  /** record T flag values **/
  energy->Tsurf_fbflag = Tsurf_fbflag;
  energy->Tsurf_fbcount += Tsurf_fbcount;
  for (nidx=0; nidx<Nnodes; nidx++) {
    energy->T_fbflag[nidx] = Tnew_fbflag[nidx];
    energy->T_fbcount[nidx] += Tnew_fbcount[nidx];
  }

  /** Return soil surface temperature **/
  return (Tsurf);
    
}

double error_print_surf_energy_bal(double Ts, int year, int month, int day, int hour,
    int VEG, int veg_class,
    double delta_t,
    /* soil layer terms */
    double Cs1, double Cs2, double D1, double D2, double T1_old, double T2,
    double Ts_old, double bubble, double dp, double expt,
    double ice0, double kappa1, double kappa2,
    double max_moist, double moist,
    const float *root,
    /* meteorological forcing terms */
    VegConditions::VegSurfType UnderStory, int overstory,
    double NetShortBare,  // net SW that reaches bare ground
    double NetShortGrnd,  // net SW that penetrates snowpack
    double NetShortSnow,  // net SW that reaches snow surface
    double Tair, double atmos_density, double atmos_pressure,
    double emissivity, double LongBareIn, double LongSnowIn,
    double precipitation_mu, double surf_atten, double vp, double vpd,
    double *Wdew, VegConditions &displacement, VegConditions &aero_resist, AeroResistUsed &ra_used,
    double *rainfall, VegConditions &ref_height, VegConditions &roughness, VegConditions &wind_speed,
    /* latent heat terms */
    double Le,
    /* snowpack terms */
    double Advection, double OldTSurf, double TPack, double Tsnow_surf,
    double kappa_snow, double melt_energy, double snow_coverage,
    double snow_density, double snow_swq, double snow_water,
    double *deltaCC, double *refreeze_energy, double *VaporMassFlux,
    /* soil node terms */
    int Nnodes,
    double *Cs_node, double *T_node, double *Tnew_node, double *ice_node, double *kappa_node,
    double *moist_node,
    /* model structures */
    layer_data_struct *layer_wet, layer_data_struct *layer_dry,
    veg_var_struct *veg_var_wet, veg_var_struct *veg_var_dry,
    /* control flags */
    int INCLUDE_SNOW, int NOFLUX, int EXP_TRANS,
    int SNOWING,
    int *FIRST_SOLN,
    /* returned energy balance terms */
    double *NetLongBare, // net LW from snow-free ground
    double *NetLongSnow, // net longwave from snow surface - if INCLUDE_SNOW
    double *T1, double *deltaH, double *fusion, double *grnd_flux,
    double *latent_heat, double *latent_heat_sub, double *sensible_heat,
    double *snow_flux, double *store_error, char *ErrorString, const soil_con_struct* soil_con,
    const ProgramState* state) {
/**********************************************************************
  Modifications:
  2009-Mar-03 Fixed format string for print statement, eliminates
	      compiler WARNING.						KAC via TJB
**********************************************************************/

  fprintf(stderr, "%s", ErrorString);
  fprintf(stderr, "ERROR: calc_surf_energy_bal failed to converge to a solution in root_brent.  Variable values will be dumped to the screen, check for invalid values.\n");

  /* Print Variables */
  /* general model terms */
  fprintf(stderr, "year = %i\n", year);
  fprintf(stderr, "month = %i\n", month);
  fprintf(stderr, "day = %i\n", day);
  fprintf(stderr, "hour = %i\n", hour);
  fprintf(stderr, "VEG = %i\n", VEG);
  fprintf(stderr, "veg_class = %i\n", veg_class);
  fprintf(stderr, "delta_t = %f\n",  delta_t);

  /* soil layer terms */
  fprintf(stderr, "Cs1 = %f\n",  Cs1);
  fprintf(stderr, "Cs2 = %f\n",  Cs2);
  fprintf(stderr, "D1 = %f\n",  D1);
  fprintf(stderr, "D2 = %f\n",  D2);
  fprintf(stderr, "T1_old = %f\n",  T1_old);
  fprintf(stderr, "T2 = %f\n",  T2);
  fprintf(stderr, "Ts_old = %f\n",  Ts_old);
  fprintf(stderr, "b_infilt = %f\n",  soil_con->b_infilt);
  fprintf(stderr, "bubble = %f\n",  bubble);
  fprintf(stderr, "dp = %f\n",  dp);
  fprintf(stderr, "expt = %f\n",  expt);
  fprintf(stderr, "ice0 = %f\n",  ice0);
  fprintf(stderr, "kappa1 = %f\n",  kappa1);
  fprintf(stderr, "kappa2 = %f\n",  kappa2);
  fprintf(stderr, "max_infil = %f\n",  soil_con->max_infil);
  fprintf(stderr, "max_moist = %f\n",  max_moist);
  fprintf(stderr, "moist = %f\n",  moist);

  fprintf(stderr, "*Wcr = %f\n",  *soil_con->Wcr);
  fprintf(stderr, "*Wpwp = %f\n",  *soil_con->Wpwp);
  fprintf(stderr, "*depth = %f\n",  *soil_con->depth);
  fprintf(stderr, "*resid_moist = %f\n",  *soil_con->resid_moist);

  fprintf(stderr, "*root = %f\n",  *root);

  /* meteorological forcing terms */
  fprintf(stderr, "UnderStory = %i\n", UnderStory);
  fprintf(stderr, "overstory = %i\n", overstory);

  fprintf(stderr, "NetShortBare = %f\n",  NetShortBare); 
  fprintf(stderr, "NetShortGrnd = %f\n",  NetShortGrnd); 
  fprintf(stderr, "NetShortSnow = %f\n",  NetShortSnow); 
  fprintf(stderr, "Tair = %f\n",  Tair);
  fprintf(stderr, "atmos_density = %f\n",  atmos_density);
  fprintf(stderr, "atmos_pressure = %f\n",  atmos_pressure);
  fprintf(stderr, "elevation = %f\n",  soil_con->elevation);
  fprintf(stderr, "emissivity = %f\n",  emissivity);
  fprintf(stderr, "LongBareIn = %f\n",  LongBareIn); 
  fprintf(stderr, "LongSnowIn = %f\n",  LongSnowIn); 
  fprintf(stderr, "mu = %f\n",  precipitation_mu);
  fprintf(stderr, "surf_atten = %f\n",  surf_atten);
  fprintf(stderr, "vp = %f\n",  vp);
  fprintf(stderr, "vpd = %f\n",  vpd);

  fprintf(stderr, "*Wdew = %f\n",  *Wdew);
  fprintf(stderr, "*displacement = %f\n",  displacement.snowCovered);
  fprintf(stderr, "*ra = %f\n",  aero_resist.snowCovered);
  fprintf(stderr, "*ra_used = %f\n",  ra_used.surface);
  fprintf(stderr, "*rainfall = %f\n",  *rainfall);
  fprintf(stderr, "*ref_height = %f\n",  ref_height.snowCovered);
  fprintf(stderr, "*roughness = %f\n",  roughness.snowCovered);
  fprintf(stderr, "*wind = %f\n",  wind_speed.snowCovered);
 
  /* latent heat terms */
  fprintf(stderr, "Le = %f\n",   Le);

  /* snowpack terms */
  fprintf(stderr, "Advection = %f\n",  Advection);
  fprintf(stderr, "OldTSurf = %f\n",  OldTSurf);
  fprintf(stderr, "TPack = %f\n",  TPack);
  fprintf(stderr, "Tsnow_surf = %f\n",  Tsnow_surf);
  fprintf(stderr, "kappa_snow = %f\n",  kappa_snow);
  fprintf(stderr, "melt_energy = %f\n",  melt_energy);
  fprintf(stderr, "snow_coverage = %f\n",  snow_coverage);
  fprintf(stderr, "snow_density = %f\n",  snow_density);
  fprintf(stderr, "snow_swq = %f\n",  snow_swq);
  fprintf(stderr, "snow_water = %f\n",  snow_water);

  fprintf(stderr, "*deltaCC = %f\n",  *deltaCC);
  fprintf(stderr, "*refreeze_energy = %f\n",  *refreeze_energy);
  fprintf(stderr, "*VaporMassFlux = %f\n",  *VaporMassFlux);

  /* soil node terms */
  fprintf(stderr, "Nnodes = %i\n", Nnodes);

  /* spatial frost terms */
#if SPATIAL_FROST    
  fprintf(stderr, "*frost_fract = %f\n",  *frost_fract);
#endif

  /* control flags */
  fprintf(stderr, "INCLUDE_SNOW = %i\n", INCLUDE_SNOW);
  fprintf(stderr, "FS_ACTIVE = %i\n", soil_con->FS_ACTIVE);
  fprintf(stderr, "NOFLUX = %i\n", NOFLUX);
  fprintf(stderr, "EXP_TRANS = %i\n", EXP_TRANS);
  fprintf(stderr, "SNOWING = %i\n", SNOWING);

  fprintf(stderr, "*FIRST_SOLN = %i\n", *FIRST_SOLN);

  /* returned energy balance terms */
  fprintf(stderr, "*NetLongBare = %f\n",  *NetLongBare); 
  fprintf(stderr, "*NetLongSnow = %f\n",  *NetLongSnow); 
  fprintf(stderr, "*T1 = %f\n",  *T1);
  fprintf(stderr, "*deltaH = %f\n",  *deltaH);
  fprintf(stderr, "*fusion = %f\n",  *fusion);
  fprintf(stderr, "*grnd_flux = %f\n",  *grnd_flux);
  fprintf(stderr, "*latent_heat = %f\n",  *latent_heat);
  fprintf(stderr, "*latent_heat_sub = %f\n",  *latent_heat_sub);
  fprintf(stderr, "*sensible_heat = %f\n",  *sensible_heat);
  fprintf(stderr, "*snow_flux = %f\n",  *snow_flux);
  fprintf(stderr, "*store_error = %f\n",  *store_error);

  write_layer(layer_wet, veg_class, state->options.Nlayer, soil_con->frost_fract);

  if(state->options.DIST_PRCP)
    write_layer(layer_dry, veg_class, state->options.Nlayer, soil_con->frost_fract);

  write_vegvar(&(veg_var_wet[0]),veg_class);
  if(state->options.DIST_PRCP)
    write_vegvar(&(veg_var_dry[0]),veg_class);

  if(!state->options.QUICK_FLUX) {
    fprintf(stderr,"Node\tT\tTnew\tZsum\tkappa\tCs\tmoist\tbubble\texpt\tmax_moist\tice\n");
    for(int i=0;i<Nnodes;i++)
      fprintf(stderr,"%i\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\t%.4f\n",
	      i, T_node[i], Tnew_node[i], soil_con->Zsum_node[i], kappa_node[i],
	      Cs_node[i], moist_node[i], soil_con->bubble_node[i], soil_con->expt_node[i],
	      soil_con->max_moist_node[i], ice_node[i]);
  }
  
  fprintf(stderr,"**********\n**********\nFinished writing calc_surf_energy_bal variables.\nTry increasing SURF_DT to get model to complete cell.\nThen check output for instabilities.\n**********\n**********\n");

  return (ERROR);
    
}

