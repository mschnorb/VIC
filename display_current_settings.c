#include <stdio.h>
#include <stdlib.h>
#include "vicNl.h"

static char vcid[] = "$Id$";

void ProgramState::display_current_settings(int mode,filenames_struct *names)
/**********************************************************************
  display_current_settings	Ted Bohn			2003

  This routine displays the current settings of options defined in
  user_def.h and the global parameter file.

  NOTE: This file must be kept in sync with any additions, removals,
  or modifications to names of parameters in user_def.h or get_global_param.c.

  Modifications:
  2005-03-08 Added EQUAL_AREA option.				TJB
  2005-03-24 Added ALMA_OUTPUT option.				TJB
  2005-04-23 Changed ARNO_PARAMS to NIJSSEN2001_BASEFLOW.	TJB
  2005-11-29 SAVE_STATE is now set in global param file         GCT
  2006-09-13 Replaced NIJSSEN2001_BASEFLOW with BASEFLOW option. TJB/GCT
  2006-Sep-23 Implemented flexible output configuration
              and aggregation of output variables.		TJB
  2006-Oct-10 Moved printing of soil_dir inside if{} block.	TJB
  2006-Oct-16 Merged infiles and outfiles structs into filep_struct;
	      This included moving global->statename to names->statefile. TJB
  2006-Nov-07 Removed LAKE_MODEL option.			TJB
  2007-Jan-03 Added ALMA_INPUT option.				TJB
  2007-Jan-15 Added PRT_HEADER option.				TJB
  2007-Apr-24 Added IMPLICIT option.				JCA
  2007-Apr-24 Added EXP_TRANS option.				JCA
  2007-Aug-08 Added EXCESS_ICE option.				JCA
  2008-Apr-21 Added SNOW_ALBEDO option.				KAC via TJB
  2008-Apr-21 Added SNOW_DENSITY option.			TJB
  2009-Jan-12 Added COMPUTE_TREELINE and JULY_TAVG_SUPPLIED options.	TJB
  2009-Jan-16 Added AERO_RESIST_CANSNOW option.			TJB
  2009-May-17 Added AR_406_LS to AERO_RESIST_CANSNOW.		TJB
  2009-May-18 Added PLAPSE option.				TJB
  2009-May-20 Added GRND_FLUX_TYPE option.			TJB
  2009-May-22 Added TFALLBACK value to options.CONTINUEONERROR.	TJB
  2009-Sep-19 Moved TFALLBACK to its own separate option.	TJB
  2009-Nov-15 Redirected output to stderr.			TJB
  2010-Apr-28 Replaced GLOBAL_LAI with VEGPARAM_LAI and LAI_SRC.TJB
  2011-May-31 Removed GRND_FLUX option.				TJB
  2011-Jun-03 Added ORGANIC_FRACT option.			TJB
  2011-Nov-04 Added options for accessing new forcing estimation
	      features.						TJB
**********************************************************************/
{

  int file_num;

  if (mode == DISP_VERSION) {
    fprintf(stdout,"***** VIC Version %s *****\n",SOURCE_VERSION);
    return;
  }
  else {
    fprintf(stdout,"\n***** VIC Version %s - Current Model Settings *****\n",SOURCE_VERSION);
  }

  fprintf(stdout,"\n");
  fprintf(stdout,"COMPILE-TIME OPTIONS (set in user_def.h)\n");
  fprintf(stdout,"----------------------------------------\n");

  fprintf(stdout,"\n");
  fprintf(stdout,"Output to Screen:\n");
#if OUTPUT_FORCE_STATS
  fprintf(stdout,"OUTPUT_FORCE_STATS\tTRUE\n");
#else
  fprintf(stdout,"OUTPUT_FORCE_STATS\tFALSE\n");
#endif
#if VERBOSE
  fprintf(stdout,"VERBOSE\t\t\tTRUE\n");
#else
  fprintf(stdout,"VERBOSE\t\t\tFALSE\n");
#endif

  fprintf(stdout,"\n");
  fprintf(stdout,"Input Files:\n");
#if NO_REWIND
  fprintf(stdout,"NO_REWIND\t\tTRUE\n");
#else
  fprintf(stdout,"NO_REWIND\t\tFALSE\n");
#endif

  fprintf(stdout,"\n");
  fprintf(stdout,"Output Files:\n");
#if LINK_DEBUG
  fprintf(stdout,"LINK_DEBUG\t\tTRUE\n");
#else
  fprintf(stdout,"LINK_DEBUG\t\tFALSE\n");
#endif

  fprintf(stdout,"\n");
  fprintf(stdout,"Simulation Parameters:\n");
#if CLOSE_ENERGY
  fprintf(stdout,"CLOSE_ENERGY\t\tTRUE\n");
#else
  fprintf(stdout,"CLOSE_ENERGY\t\tFALSE\n");
#endif
#if LOW_RES_MOIST
  fprintf(stdout,"LOW_RES_MOIST\t\tTRUE\n");
#else
  fprintf(stdout,"LOW_RES_MOIST\t\tFALSE\n");
#endif
#if QUICK_FS
  fprintf(stdout,"QUICK_FS\t\tTRUE\n");
  fprintf(stdout,"QUICK_FS_TEMPS\t%d\n",QUICK_FS_TEMPS);
#else
  fprintf(stdout,"QUICK_FS\t\tFALSE\n");
#endif
#if SPATIAL_FROST
  fprintf(stdout,"SPATIAL_FROST\t\tTRUE\n");
  fprintf(stdout,"FROST_SUBAREAS\t\t%d\n",FROST_SUBAREAS);
#else
  fprintf(stdout,"SPATIAL_FROST\t\tFALSE\n");
#endif
#if SPATIAL_SNOW
  fprintf(stdout,"SPATIAL_SNOW\t\tTRUE\n");
#else
  fprintf(stdout,"SPATIAL_SNOW\t\tFALSE\n");
#endif
#if EXCESS_ICE
  fprintf(stdout,"EXCESS_ICE\t\tTRUE\n");
#else
  fprintf(stdout,"EXCESS_ICE\t\tFALSE\n");
#endif

  fprintf(stdout,"\n");
  fprintf(stdout,"Maximum Array Sizes:\n");
  fprintf(stdout,"MAX_BANDS\t\t%2d\n",MAX_BANDS);
  fprintf(stdout,"MAX_FRONTS\t\t%2d\n",MAX_FRONTS);
  fprintf(stdout,"MAX_LAKE_NODES\t\t%2d\n",MAX_LAKE_NODES);
  fprintf(stdout,"MAX_LAYERS\t\t%2d\n",MAX_LAYERS);
  fprintf(stdout,"MAX_NODES\t\t%2d\n",MAX_NODES);
  fprintf(stdout,"\n");
  fprintf(stdout,"Snow Constants:\n");
  fprintf(stdout,"TraceSnow\t\t%f\n",TraceSnow);
  fprintf(stdout,"\n");
  fprintf(stdout,"Other Constants:\n");
  fprintf(stdout,"LAI_WATER_FACTOR\t%f\n",LAI_WATER_FACTOR);
  fprintf(stdout,"LWAVE_COR\t\t%f\n",LWAVE_COR);
  fprintf(stdout,"MAXIT_FE\t\t%2d\n",MAXIT_FE);

  if (mode == DISP_COMPILE_TIME) {
    return;
  }

  fprintf(stdout,"\n");
  fprintf(stdout,"RUN-TIME OPTIONS (set in global parameter file)\n");
  fprintf(stdout,"-----------------------------------------------\n");

  fprintf(stdout,"Simulation Dimensions:\n");
  fprintf(stdout,"NLAYER\t\t\t%d\n",options.Nlayer);
  if ( options.EQUAL_AREA ) {
    fprintf(stdout,"EQUAL_AREA\t\tTRUE\n");
  }
  else {
    fprintf(stdout,"EQUAL_AREA\t\tFALSE\n");
  }
  fprintf(stdout,"RESOLUTION\t\t%f\n",global_param.resolution);
  fprintf(stdout,"TIME_STEP\t\t%d\n",global_param.dt);
  fprintf(stdout,"SNOW_STEP\t\t%d\n",options.SNOW_STEP);
  fprintf(stdout,"STARTYEAR\t\t%d\n",global_param.startyear);
  fprintf(stdout,"STARTMONTH\t\t%d\n",global_param.startmonth);
  fprintf(stdout,"STARTDAY\t\t%d\n",global_param.startday);
  fprintf(stdout,"STARTHOUR\t\t%d\n",global_param.starthour);
  if ( global_param.nrecs > 0 )
    fprintf(stdout,"NRECS\t\t%d\n",global_param.nrecs);
  else {
    fprintf(stdout,"ENDYEAR\t\t\t%d\n",global_param.endyear);
    fprintf(stdout,"ENDMONTH\t\t%d\n",global_param.endmonth);
    fprintf(stdout,"ENDDAY\t\t\t%d\n",global_param.endday);
  }

  fprintf(stdout,"\n");
  fprintf(stdout,"Simulation Parameters:\n");
  if (options.AERO_RESIST_CANSNOW == AR_406)
    fprintf(stdout,"AERO_RESIST_CANSNOW\t\tAR_406\n");
  else if (options.AERO_RESIST_CANSNOW == AR_406_LS)
    fprintf(stdout,"AERO_RESIST_CANSNOW\t\tAR_406_LS\n");
  else if (options.AERO_RESIST_CANSNOW == AR_406_FULL)
    fprintf(stdout,"AERO_RESIST_CANSNOW\t\tAR_406_FULL\n");
  else if (options.AERO_RESIST_CANSNOW == AR_410)
    fprintf(stdout,"AERO_RESIST_CANSNOW\t\tAR_410\n");
  else if (options.AERO_RESIST_CANSNOW == AR_COMBO)
    fprintf(stdout,"AERO_RESIST_CANSNOW\t\tAR_COMBO\n");
  if (options.BLOWING)
    fprintf(stdout,"BLOWING\t\t\tTRUE\n");
  else
    fprintf(stdout,"BLOWING\t\t\tFALSE\n");
  if (options.COMPUTE_TREELINE)
    fprintf(stdout,"COMPUTE_TREELINE\t\tTRUE\n");
  else
    fprintf(stdout,"COMPUTE_TREELINE\t\tFALSE\n");
  if (options.CONTINUEONERROR == TRUE)
    fprintf(stdout,"CONTINUEONERROR\t\tTRUE\n");
  else
    fprintf(stdout,"CONTINUEONERROR\t\tFALSE\n");
  if (options.CORRPREC)
    fprintf(stdout,"CORRPREC\t\tTRUE\n");
  else
    fprintf(stdout,"CORRPREC\t\tFALSE\n");
  if (options.DIST_PRCP)
    fprintf(stdout,"DIST_PRCP\t\tTRUE\n");
  else
    fprintf(stdout,"DIST_PRCP\t\tFALSE\n");
  if (options.EXP_TRANS)
    fprintf(stdout,"EXP_TRANS\t\tTRUE\n");
  else
    fprintf(stdout,"EXP_TRANS\t\tFALSE\n");
  if (options.FROZEN_SOIL)
    fprintf(stdout,"FROZEN_SOIL\t\tTRUE\n");
  else
    fprintf(stdout,"FROZEN_SOIL\t\tFALSE\n");
  if (options.FULL_ENERGY)
    fprintf(stdout,"FULL_ENERGY\t\tTRUE\n");
  else
    fprintf(stdout,"FULL_ENERGY\t\tFALSE\n");
  if (options.GRND_FLUX_TYPE == GF_406)
    fprintf(stdout,"GRND_FLUX_TYPE\t\tGF_406\n");
  else if (options.GRND_FLUX_TYPE == GF_410)
    fprintf(stdout,"GRND_FLUX_TYPE\t\tGF_410\n");
  else if (options.GRND_FLUX_TYPE == GF_FULL)
    fprintf(stdout,"GRND_FLUX_TYPE\t\tGF_FULL\n");
  if (options.LW_TYPE == LW_TVA)
    fprintf(stdout,"LW_TYPE\t\tLW_TVA\n");
  else if (options.LW_TYPE == LW_ANDERSON)
    fprintf(stdout,"LW_TYPE\t\tLW_ANDERSON\n");
  else if (options.LW_TYPE == LW_BRUTSAERT)
    fprintf(stdout,"LW_TYPE\t\tLW_BRUTSAERT\n");
  else if (options.LW_TYPE == LW_SATTERLUND)
    fprintf(stdout,"LW_TYPE\t\tLW_SATTERLUND\n");
  else if (options.LW_TYPE == LW_IDSO)
    fprintf(stdout,"LW_TYPE\t\tLW_IDSO\n");
  else if (options.LW_TYPE == LW_PRATA)
    fprintf(stdout,"LW_TYPE\t\tLW_PRATA\n");
  if (options.LW_CLOUD == LW_CLOUD_DEARDORFF)
    fprintf(stdout,"LW_CLOUD\t\tLW_CLOUD_DEARDORFF\n");
  else
    fprintf(stdout,"LW_CLOUD\t\tLW_CLOUD_BRAS\n");
  if (options.IMPLICIT)
    fprintf(stdout,"IMPLICIT\t\tTRUE\n");
  else
    fprintf(stdout,"IMPLICIT\t\tFALSE\n");
  if (options.NOFLUX)
    fprintf(stdout,"NOFLUX\t\t\tTRUE\n");
  else
    fprintf(stdout,"NOFLUX\t\t\tFALSE\n");
  if (options.MTCLIM_SWE_CORR)
    fprintf(stdout,"MTCLIM_SWE_CORR\t\tTRUE\n");
  else
    fprintf(stdout,"MTCLIM_SWE_CORR\t\tFALSE\n");
  if (options.PLAPSE)
    fprintf(stdout,"PLAPSE\t\tTRUE\n");
  else
    fprintf(stdout,"PLAPSE\t\tFALSE\n");
  if (options.QUICK_FLUX)
    fprintf(stdout,"QUICK_FLUX\t\tTRUE\n");
  else
    fprintf(stdout,"QUICK_FLUX\t\tFALSE\n");
  if (options.QUICK_SOLVE)
    fprintf(stdout,"QUICK_SOLVE\t\tTRUE\n");
  else
    fprintf(stdout,"QUICK_SOLVE\t\tFALSE\n");
  if (options.SNOW_ALBEDO == USACE)
    fprintf(stdout,"SNOW_ALBEDO\t\tUSACE\n");
  else if (options.SNOW_ALBEDO == SUN1999)
    fprintf(stdout,"SNOW_ALBEDO\t\tSUN1999\n");
  if (options.SNOW_DENSITY == DENS_BRAS)
    fprintf(stdout,"SNOW_DENSITY\t\tDENS_BRAS\n");
  else if (options.SNOW_DENSITY == DENS_SNTHRM)
    fprintf(stdout,"SNOW_DENSITY\t\tDENS_SNTHRM\n");
  fprintf(stdout,"SW_PREC_THRESH\t\t%f\n",options.SW_PREC_THRESH);
  if (options.TFALLBACK == TRUE)
    fprintf(stdout,"TFALLBACK\t\tTRUE\n");
  else
    fprintf(stdout,"TFALLBACK\t\tFALSE\n");

  if (options.VP_INTERP == TRUE)
    fprintf(stdout,"VP_INTERP\t\tTRUE\n");
  else
    fprintf(stdout,"VP_INTERP\t\tFALSE\n");

  if (options.VP_ITER == VP_ITER_NONE)
    fprintf(stdout,"VP_ITER\t\tVP_ITER_NONE\n");
  else if (options.VP_ITER == VP_ITER_ALWAYS)
    fprintf(stdout,"VP_ITER\t\tVP_ITER_ALWAYS\n");
  else if (options.VP_ITER == VP_ITER_ANNUAL)
    fprintf(stdout,"VP_ITER\t\tVP_ITER_ANNUAL\n");
  else if (options.VP_ITER == VP_ITER_CONVERGE)
    fprintf(stdout,"VP_ITER\t\tVP_ITER_CONVERGE\n");

  if (options.TEMP_TH_TYPE == VIC_412)
    fprintf(stdout,"TEMP_TH_TYPE\t\tVIC_412\n");
  else
    fprintf(stdout,"TEMP_TH_TYPE\t\tKIENZLE\n");

  fprintf(stdout,"PREC_EXPT\t\t%f\n",options.PREC_EXPT);
  fprintf(stdout,"WIND_H\t\t\t%f\n",global_param.wind_h);
  fprintf(stdout,"MEASURE_H\t\t%f\n",global_param.measure_h);
  fprintf(stdout,"NODES\t\t\t%d\n",options.Nnode);
  fprintf(stdout,"MIN_WIND_SPEED\t\t%f\n",options.MIN_WIND_SPEED);

  fprintf(stdout,"\n");
  fprintf(stdout,"Input Forcing Data:\n");
  for (file_num=0; file_num<2; file_num++) {
    if (IS_VALID(global_param.forceyear[file_num]) && global_param.forceyear[file_num] > 0) {
      fprintf(stdout,"Forcing File %d:\t\t%s*\n",file_num+1,names->f_path_pfx[file_num]);
      fprintf(stdout,"FORCEYEAR\t\t%d\n",global_param.forceyear[file_num]);
      fprintf(stdout,"FORCEMONTH\t\t%d\n",global_param.forcemonth[file_num]);
      fprintf(stdout,"FORCEDAY\t\t%d\n",global_param.forceday[file_num]);
      fprintf(stdout,"FORCEHOUR\t\t%d\n",global_param.forcehour[file_num]);
      fprintf(stdout,"N_TYPES\t\t\t%d\n",param_set.N_TYPES[file_num]);
      fprintf(stdout,"FORCE_DT\t\t%d\n",param_set.FORCE_DT[file_num]);
      if (param_set.FORCE_ENDIAN[file_num] == LITTLE)
        fprintf(stdout,"FORCE_ENDIAN\t\tLITTLE\n");
      else
        fprintf(stdout,"FORCE_ENDIAN\t\tBIG\n");
      if (param_set.FORCE_FORMAT[file_num] == BINARY)
        fprintf(stdout,"FORCE_FORMAT\t\tBINARY\n");
      else if (param_set.FORCE_FORMAT[file_num] == ASCII)
        fprintf(stdout,"FORCE_FORMAT\t\tASCII\n");
      else
        fprintf(stdout,"FORCE_FORMAT\t\tNETCDF\n");
    }
  }
  fprintf(stdout,"GRID_DECIMAL\t\t%d\n",options.GRID_DECIMAL);
  if (options.ALMA_INPUT)
    fprintf(stdout,"ALMA_INPUT\t\tTRUE\n");
  else
    fprintf(stdout,"ALMA_INPUT\t\tFALSE\n");

  fprintf(stdout,"\n");
  fprintf(stdout,"Input Soil Data:\n");
  fprintf(stdout,"Soil file\t\t%s\n",names->soil);
  if (options.ARC_SOIL) {
    fprintf(stdout,"ARC_SOIL\t\tTRUE\n");
    fprintf(stdout,"Soil dir\t\t%s\n",names->soil_dir);
  }
  else
    fprintf(stdout,"ARC_SOIL\t\tFALSE\n");
  if (options.BASEFLOW == ARNO)
    fprintf(stdout,"BASEFLOW\t\tARNO\n");
  else if (options.BASEFLOW == NIJSSEN2001)
    fprintf(stdout,"BASEFLOW\t\tNIJSSEN2001\n");
  if (options.JULY_TAVG_SUPPLIED)
    fprintf(stdout,"JULY_TAVG_SUPPLIED\t\tTRUE\n");
  else
    fprintf(stdout,"JULY_TAVG_SUPPLIED\t\tFALSE\n");
  if (options.ORGANIC_FRACT)
    fprintf(stdout,"ORGANIC_FRACT\t\tTRUE\n");
  else
    fprintf(stdout,"ORGANIC_FRACT\t\tFALSE\n");

  fprintf(stdout,"\n");
  fprintf(stdout,"Input Veg Data:\n");
  fprintf(stdout,"Veg library file\t%s\n",names->veglib);
  fprintf(stdout,"Veg param file\t\t%s\n",names->veg);
  fprintf(stdout,"ROOT_ZONES\t\t%d\n",options.ROOT_ZONES);
  fprintf(stdout, "GLACIER_ID\t\t%d\n", options.GLACIER_ID);
  if (options.VEGPARAM_LAI)
    fprintf(stdout,"VEGPARAM_LAI\t\tTRUE\n");
  else
    fprintf(stdout,"VEGPARAM_LAI\t\tFALSE\n");
  if (options.LAI_SRC == LAI_FROM_VEGPARAM)
    fprintf(stdout,"LAI_SRC\t\tLAI_FROM_VEGPARAM\n");
  else if (options.LAI_SRC == LAI_FROM_VEGLIB)
    fprintf(stdout,"LAI_SRC\t\tLAI_FROM_VEGLIB\n");

  fprintf(stdout,"\n");
  fprintf(stdout,"Input Elevation Data:\n");
  if (options.SNOW_BAND > 1)
    fprintf(stdout,"SNOW_BAND\t\t%d\t%s\n",options.SNOW_BAND,names->snowband);
  else if (options.SNOW_BAND == 1)
    fprintf(stdout,"SNOW_BAND\t\t%d\t(no input file needed for SNOW_BAND=1)\n",options.SNOW_BAND);
  else
    fprintf(stdout,"SNOW_BAND\t\t%d\n",options.SNOW_BAND);

  fprintf(stdout,"\n");
  fprintf(stdout,"Input Lake Data:\n");
  if (options.LAKES)
    fprintf(stdout,"LAKES\t\tTRUE\t%s\n",names->lakeparam);
  else
    fprintf(stdout,"LAKES\t\tFALSE\n");
  if (options.LAKE_PROFILE)
    fprintf(stdout,"LAKE_PROFILE\t\tTRUE\n");
  else
    fprintf(stdout,"LAKE_PROFILE\t\tFALSE\n");

  fprintf(stdout,"\n");
  fprintf(stdout,"Input State File:\n");
  if (options.INIT_STATE) {
    fprintf(stdout,"INIT_STATE\t\tTRUE\t%s\n",names->init_state);
    if (options.STATE_FORMAT == StateOutputFormat::BINARY_STATEFILE)
      fprintf(stdout,"STATE_FORMAT\tBINARY_STATEFILE\n");
    else if (options.STATE_FORMAT == StateOutputFormat::ASCII_STATEFILE)
      fprintf(stdout,"STATE_FORMAT\tASCII\n");
    else
      fprintf(stdout,"STATE_FORMAT\tNETCDF_STATEFILE");
  }
  else
    fprintf(stdout,"INIT_STATE\t\tFALSE\n");

  fprintf(stdout,"\n");
  fprintf(stdout,"Output State File:\n");
  if (options.SAVE_STATE) {
    fprintf(stdout,"SAVE_STATE\t\tTRUE\n");
    fprintf(stdout,"STATENAME\t\t%s\n",names->statefile);
    fprintf(stdout,"STATEYEAR\t\t%d\n",global_param.stateyear);
    fprintf(stdout,"STATEMONTH\t\t%d\n",global_param.statemonth);
    fprintf(stdout,"STATEDAY\t\t%d\n",global_param.stateday);
    if (options.STATE_FORMAT == StateOutputFormat::BINARY_STATEFILE)
      fprintf(stdout,"STATE_FORMAT\tBINARY_STATEFILE\n");
    else if (options.STATE_FORMAT == StateOutputFormat::ASCII_STATEFILE)
      fprintf(stdout,"STATE_FORMAT\tASCII\n");
    else
      fprintf(stdout,"STATE_FORMAT\tNETCDF_STATEFILE");
  }
  else {
    fprintf(stdout,"SAVE_STATE\t\tFALSE\n");
  }

  fprintf(stdout,"\n");
  fprintf(stdout,"Output Data:\n");
  fprintf(stdout,"Result dir:\t\t%s\n",names->result_dir);
  fprintf(stdout,"OUT_STEP\t\t%d\n",global_param.out_dt);
  if (options.ALMA_OUTPUT)
    fprintf(stdout,"ALMA_OUTPUT\t\tTRUE\n");
  else
    fprintf(stdout,"ALMA_OUTPUT\t\tFALSE\n");

  WriteOutputContext context(this);
  fprintf(stdout, "OUTPUT_FORMAT\t\t%s\n", context.outputFormat->getDescriptionOfOutputType());

  if (options.OUTPUT_FORCE)
  	fprintf(stdout, "OUTPUT_FORCE\t\tTRUE\n");
  else
  	fprintf(stdout, "OUTPUT_FORCE\t\tFALSE\n");

  fprintf(stdout, "PARALLEL_THREADS\t%d\n", global_param.num_threads);

  if (options.COMPRESS)
    fprintf(stdout,"COMPRESS\t\tTRUE\n");
  else
    fprintf(stdout,"COMPRESS\t\tFALSE\n");
  if (options.MOISTFRACT)
    fprintf(stdout,"MOISTFRACT\t\tTRUE\n");
  else
    fprintf(stdout,"MOISTFRACT\t\tFALSE\n");
  if (options.PRT_HEADER)
    fprintf(stdout,"PRT_HEADER\t\tTRUE\n");
  else
    fprintf(stdout,"PRT_HEADER\t\tFALSE\n");
  if (options.PRT_SNOW_BAND)
    fprintf(stdout,"PRT_SNOW_BAND\t\tTRUE\n");
  else
    fprintf(stdout,"PRT_SNOW_BAND\t\tFALSE\n");
  fprintf(stdout,"SKIPYEAR\t\t%d\n",global_param.skipyear);
  fprintf(stdout,"\n");

}
