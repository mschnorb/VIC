/* RCS Id String
 * $Id$
 */
/************************************************************************
  Modifications:
  2006-Sep-23 Implemented flexible output configuration; uses the new
              out_data, out_data_files, and save_data structures.	TJB
	      Removed the following functions:
		conv_force_vic2alma
		conv_results_vic2alma
	      Added the following new functions:
		create_output_list
		free_out_data_files
		init_output_list
		parse_output_info
		set_output_defaults
		set_output_var
		zero_output_list
  2006-Oct-16 Merged infiles and outfiles structs into filep_struct.	TJB
  2006-Nov-07 Removed LAKE_MODEL option.				TJB
  2007-Jan-15 Added PRT_HEADER option.					TJB
  2007-Apr-03 Modified the data types of the following functions for
	      CONTINUE_ON_ERROR:					KAC/GTC
	      CalcAerodynamic
	      dist_prec
	      distribute_node_moisture_properties
	      full_energy
	      initialize_new_storm
	      redistribute_during_storm
	      runoff
	      snow_intercept
	      snow_melt
	      solve_T_profile
	      surface_fluxes
  2007-Apr-24 Added Ming Pan's new functions for IMPLICIT option.       JCA
              fda_heat_eqn
              newt_raph
              tridiag
              fdjac3
              solve_T_profile_implicit
  2007-Apr-21 Added functions:						TJB
	      free_dmy
	      free_out_data
	      free_veglib
  2007-Aug-09 Added features for EXCESS_ICE option.			JCA
  2007-Aug-22 Made calc_water_balance_error  type double.		JCA
  2007-Nov-06 Moved get_dist() from LAKE.h to this file.		JCA
  2008-Feb-17 Changed argument list for snow_density().			KMA via TJB
  2008-Apr-21 Added snow depth and albedo to snow_albedo() argument
	      list.							KAC via TJB
  2008-Oct-23 Modified put_data() to be type int, so that it can
	      return an error status.					TJB
  2009-Jan-16 Added avgJulyAirTemp to argument list of
	      compute_treeline().					TJB
  2009-Feb-09 Removed dz_node from several functions.			KAC via TJB
  2009-Mar-16 Added resid_moist to argument list of
	      estimate_layer_ice_content().				TJB
  2009-May-17 Added asat to argument list of surface_fluxes(),
	      full_energy(), and wetland_energy().			TJB
  2009-Jun-09 Modified argument lists of some functions that were
	      modified to use extension of veg_lib structure to contain
	      bare soil information.					TJB
  2009-Jun-09 Added compute_pot_evap().					TJB
  2009-Jun-09 Removed unnecessary functions quick_penman() and
	      compute_penman_constants().				TJB
  2009-Jun-19 Added T flag to indicate whether TFALLBACK occurred.	TJB
  2009-Jun-26 Simplified argument list of runoff() by passing all cell_data
	      variables via a single reference to the cell data structure.	TJB
  2009-Jul-07 Added soil_con.BandElev[] to read_snowband() arg list.	TJB
  2009-Jul-31 Removed unused layer_node_fract array from
	      estimate_layer_ice_content().				TJB 
  2009-Sep-19 Added T fbcount to count TFALLBACK occurrences.		TJB
  2009-Sep-28 Added collect_wb_terms() and collect_eb_terms(). Changed
	      argument list of read_snowband().				TJB
  2009-Oct-08 Extended T fallback scheme to snow and ice T.		TJB
  2009-Nov-15 Removed ice0 and moist0 from argument list of solve_snow,
	      since they are never used.				TJB
  2009-Dec-11 Removed save_data structure from argument list of 
	      initialize_model_state().					TJB
  2009-Dec-11 Removed min_liq and options.MIN_LIQ.			TJB
  2010-Mar-31 Added cell_area to initialize_atmos().			TJB
  2010-Apr-26 Simplified argument lists for solve_snow() and
	      snow_intercept().						TJB
  2010-Apr-28 Removed net_short, displacement, roughness, and ref_height
	      from arg list of arno_evap() as they are no longer used.	TJB
  2010-Apr-28 Removed individual soil_con variables from argument list
	      of initialize_atmos() and replaced with *soil_con.	TJB
  2010-Nov-11 Added lakefactor to collect_wb_terms() and collect_eb_terms()
	      so that these functions could handle changes in how lake
	      and wetland cell/soil/snow/energy fluxes are represented.	TJB
  2010-Dec-01 Added compute_zwt().					TJB
  2011-Jan-04 Made read_soilparam_arc() a sub-function of
	      read_soilparam().						TJB
  2011-Mar-01 Added wrap_compute_zwt().  Added compute_runoff_and_asat().
	      Changed the argument list of initialize_soil().		TJB
  2011-Mar-31 Added frost_fract to collect_wb_terms() arglist.		TJB
  2011-May-24 Replaced finish_frozen_soil_calcs() with
	      calc_layer_average_thermal_props().  Added
	      estimate_layer_ice_content_quick_flux().			TJB
  2011-Jun-03 Added options.ORGANIC_FRACT.  Soil properties now take
	      organic fraction into account.				TJB
  2011-Jun-10 Added bulk_dens_min and soil_dens_min to arglist of
	      soil_conductivity() to fix bug in commputation of kappa.	TJB
  2011-Nov-04 Updated mtclim functions to MTCLIM 4.3.                   TJB
************************************************************************/

#include <math.h>
#include <vicNl_def.h>
#include <LAKE.h>
#include "root_brent.h"
#include "atmos_energy_bal.h"
#include "surf_energy_bal.h"
#include "soil_thermal_eqn.h"
#include "IceEnergyBalance.h"
#include "canopy_energy_bal.h"
#include "SnowPackEnergyBalance.h"

/*** SubRoutine Prototypes ***/

double advected_sensible_heat(double, double, double, double, double);
atmos_data_struct * alloc_atmos(int, int);
double arno_evap(layer_data_struct *, layer_data_struct *, double, double, 
		 double, double, double, double, double, double, double, double, double, const double *, const ProgramState*);

unsigned char average_moisture_for_storm(double *, double *, double, double);

int   CalcAerodynamic(char, double, double, double, double, double,
	  	       double *, double *, double *, double *, double *);
void   calc_cloud_cover_fraction(atmos_data_struct *, dmy_struct *, int,
				 int, int, double *);
void   calc_energy_balance_error(int, double, double, double, double, double, int, CellBalanceErrors*);
void   calc_forcing_stats(int, atmos_data_struct *, const int);
void   calc_longwave(double *, double, double, double, const ProgramState*);
void   calc_netlongwave(double *, double, double, double);
double calc_netshort(double, int, double, double *);
double calc_rainonly(double,double,double,double,double);
double calc_rc(double,double,float,double,double,double,double,char);
void   calc_root_fractions(veg_con_struct *, soil_con_struct *, const ProgramState*);
double calc_snow_coverage(int *, double, double, double, double, double, 
                          double, double, double *, double *, double *, 
                          double *, double *);
double calc_snow_ground_flux(int, int, int, int, double, double, double, 
			     double, double, double *, double *, double *, 
			     double *, energy_bal_struct *, 
			     snow_data_struct *, layer_data_struct *,
                             layer_data_struct *, soil_con_struct *, char *);

int    calc_soil_thermal_fluxes(int, double *, double *, char *, int *, double *, const double *,
				double *, const double *, const double *, const double *,
				const double *, double *, double *,
				double *, double *, double *, double **const*,
        double *, double *, int, int, int, int, const ProgramState*);

double CalcBlowingSnow(double, double, int, double, double, double, double, 
                       double, double, double, double, double, float, 
                       float, double, int, int, float, double, double, double *); 
double calc_atmos_energy_bal(double, double, double, double, double, double, 
                             double, double, double, double, double, double, 
                             double, double, double, double, 
                             double *, double *, double *, double *, 
                             double *, double *, double *, double *, char *, int *, const ProgramState*);
int    calc_layer_average_thermal_props(energy_bal_struct *, layer_data_struct *,
					layer_data_struct *, layer_data_struct *,
					const soil_con_struct *, int, int, double *, const ProgramState*);
double calc_surf_energy_bal(double, double, double, double, double, double,
                            double, double, double, double, double, double,
                            double, double, double, double, double, double,
                            double, double, double, double, double, double,
                            double, double, double,
                            double *, double *, double *, double *, double *,
                            double *, double *, double *, double *, double *,
                            const float *, int, int,
                            int, int, int, int, int, int, int, int, int, int,
                            atmos_data_struct *, const dmy_struct *,
                            energy_bal_struct *, layer_data_struct *,
                            layer_data_struct *,
                            snow_data_struct *, const soil_con_struct *,
                            veg_var_struct *, veg_var_struct *, int, const ProgramState*);
double calc_trans(double, double);
double calc_veg_displacement(double);
double calc_veg_height(double);
double calc_veg_roughness(double);
double calc_water_balance_error(int, double, double, double, int, CellBalanceErrors*);
double canopy_evap(layer_data_struct *, layer_data_struct *, veg_var_struct *,
    veg_var_struct *, char, int, int, double, double *, double, double, double,
    double, double, double, double, double, double, double, double *, const double *,
    const double *, const double *, const double *, const float *, const ProgramState*);

filep_struct   get_files(const filenames_struct *, const ProgramState*);
FILE  *check_state_file(char *, ProgramState*);
void   close_files(const filep_struct *, out_data_file_struct *, filenames_struct *, bool, const ProgramState*);
void   cmd_proc(int argc, char *argv[], char* global_file_name, ProgramState*);
void   collect_eb_terms(const energy_bal_struct&, const snow_data_struct&, const cell_data_struct&,
                        FallBackStats*, double, double, double,
                        int, int, double, int, int, double *, double *,
                        double *, double,
                        out_data_struct *, const ProgramState*);
void   collect_wb_terms(const cell_data_struct&, const veg_var_struct&, const snow_data_struct&, const lake_var_struct&,
                        double, double, double, double, int, int, double, int, double *,
                        double *, out_data_struct *, const ProgramState*);
void   compress_files(char string[]);
void   compute_dz(double *, double *, int, double);
void   correct_precip(double *, double, double, double, double);
void   compute_pot_evap(int, const dmy_struct *, int, int, double, double , double, double, double, double **, double *, const ProgramState*);
void   compute_runoff_and_asat(const soil_con_struct *, double *, double, double *, double *, const ProgramState*);
void   compute_soil_layer_thermal_properties(layer_data_struct *, const soil_con_struct*, int);
void   compute_treeline(atmos_data_struct *, const dmy_struct *, double, double *, char *, const ProgramState*);
double compute_zwt(const soil_con_struct *, int, double);
out_data_struct* copy_output_data(out_data_struct* out_data, const ProgramState* state);
out_data_struct *create_output_list(const ProgramState*);

int dist_prec(cell_info_struct*, const dmy_struct *, filep_struct *, out_data_file_struct *,
    out_data_struct *, int, int, char, const ProgramState*);

int distribute_node_moisture_properties(energy_bal_struct*,
    const soil_con_struct*, double *, const ProgramState*);


void   distribute_soil_property(double *,double,double,
				double **l_param,
				int, int, double *, double *);

double error_print_atmos_energy_bal(double Tcanopy, double LatentHeat,
    double NetRadiation, double Ra, double Tair, double atmos_density,
    double InSensible, double *SensibleHeat, char *ErrorString);
double error_print_atmos_moist_bal(double VPcanopy, double InLatent, double Lv,
    double Ra, double atmos_density, double gamma, double vp,
    double *AtmosLatent, char *ErrorString);
double error_print_canopy_energy_bal(double Tfoliage, int band, int month,
    int rec, double delta_t, const double elevation, const double *Wcr, const double *Wpwp,
    const double *depth, const double *frost_fract, double AirDens, double EactAir,
    double Press, double latent_heat_Le, double Tcanopy, double Vpd, double mu,
    double *Evap, double *Ra, double *Ra_used, double *Rainfall, double *Wind,
    int UnderStory, int iveg, int veg_class, double *displacement,
    double *ref_height, double *roughness, const float *root, double IntRain,
    double IntSnow, double *Wdew, layer_data_struct *layer_wet,
    layer_data_struct *layer_dry, veg_var_struct *veg_var_wet,
    veg_var_struct *veg_var_dry, double LongOverIn, double LongUnderOut,
    double NetShortOver, double *AdvectedEnergy, double *LatentHeat,
    double *LatentHeatSub, double *LongOverOut, double *NetLongOver,
    double *NetRadiation, double *RefreezeEnergy, double *SensibleHeat,
    double *VaporMassFlux, char *ErrorString, const ProgramState* state);
double ErrorPrintSnowPackEnergyBalance(double TSurf, int rec, int iveg,
    int band, double Dt, double Ra, double* RaUsed, double Displacement,
    double Z, double* Z0, double AirDens, double EactAir, double LongSnowIn,
    double Lv, double Press, double Rain, double ShortRad, double Vpd,
    double Wind, double OldTSurf, double SnowCoverFract, double SnowDensity,
    double SurfaceLiquidWater, double SweSurfaceLayer, double Tair,
    double TGrnd, double *AdvectedEnergy, double *AdvectedSensibleHeat,
    double *DeltaColdContent, double *GroundFlux,
    double *LatentHeat, double *LatentHeatSub, double *NetLongSnow,
    double *RefreezeEnergy, double *SensibleHeat, double *VaporMassFlux,
    double *BlowingMassFlux, double *SurfaceMassFlux, char *ErrorString);
double error_print_solve_T_profile(double T, double TL, double TU, double T0,
    double moist, double max_moist, double bubble, double expt, double ice0,
    double gamma, double A, double B, double C, double D, double E,
    char *ErrorString);

double error_print_surf_energy_bal(double Ts, int year, int month, int day,
    int hour, int iveg, int VEG, int veg_class, double delta_t, double Cs1,
    double Cs2, double D1, double D2, double T1_old, double T2, double Ts_old,
    double bubble, double dp, double expt, double ice0, double kappa1,
    double kappa2, double max_moist, double moist,const float *root, int UnderStory,
    int overstory, double NetShortBare, double NetShortGrnd,
    double NetShortSnow, double Tair, double atmos_density,
    double atmos_pressure, double emissivity, double LongBareIn,
    double LongSnowIn, double precipitation_mu, double surf_atten, double vp,
    double vpd, double *Wdew, double *displacement, double *ra, double *ra_used,
    double *rainfall, double *ref_height, double *roughness, double *wind,
    double Le, double Advection, double OldTSurf, double TPack,
    double Tsnow_surf, double kappa_snow, double melt_energy,
    double snow_coverage, double snow_density, double snow_swq,
    double snow_water, double *deltaCC, double *refreeze_energy,
    double *VaporMassFlux, int Nnodes, double *Cs_node, double *T_node,
    double *Tnew_node, double *ice_node, double *kappa_node, double *moist_node,
    layer_data_struct *layer_wet, layer_data_struct *layer_dry,
    veg_var_struct *veg_var_wet, veg_var_struct *veg_var_dry, int INCLUDE_SNOW,
    int NOFLUX, int EXP_TRANS, int SNOWING, int *FIRST_SOLN,
    double *NetLongBare, double *NetLongSnow, double *T1, double *deltaH,
    double *fusion, double *grnd_flux, double *latent_heat,
    double *latent_heat_sub, double *sensible_heat, double *snow_flux,
    double *store_error, char *ErrorString, const soil_con_struct* soil_con,
    const ProgramState* state);

double estimate_dew_point(double, double, double, double, double);

int estimate_layer_ice_content(layer_data_struct *layer, double *T, int Nnodes,
    int Nlayers, const soil_con_struct* soil_con, const ProgramState* state);

int estimate_layer_ice_content_quick_flux(layer_data_struct *layer,
    double Tsurf, double T1, const soil_con_struct* soil_con,
    const ProgramState* state);

double estimate_T1(double, double, double, double, double, double, double, 
		   double, double, double, double);
double exp_interp(double,double,double,double,double);

double f(double, double, double, double, double, double, double, double,
         double, double, int, double *, double, double, double, double *,
         double *, double *, double *, double *, double *);
void   find_0_degree_fronts(energy_bal_struct *, const double *, double *, int);
layer_data_struct find_average_layer(layer_data_struct *, layer_data_struct *,
				     double, double, const ProgramState*);
void   find_sublayer_temperatures(layer_data_struct *, double *, double *,
				  double *, double, double, int, int);
void   free_atmos(int nrecs, atmos_data_struct **atmos);
void   free_dist_prcp(dist_prcp_struct *, int);
void   free_dmy(dmy_struct **dmy);
void   free_vegcon(veg_con_struct **);
void   free_veglib(veg_lib_struct **);
void   free_out_data_files(out_data_file_struct *, const ProgramState*);
void   free_out_data(out_data_struct **);
int    full_energy(char, int, int, atmos_data_struct *, dist_prcp_struct *,
		     const dmy_struct *, lake_con_struct *, const soil_con_struct *,
		     const veg_con_struct *, WriteDebug*, const ProgramState*);
double func_aero_resist(double,double,double,double,double);
double func_atmos_moist_bal(double VPcanopy, double InLatentHeat, double Lv,
    double Ra, double atmos_density, double gamma, double atmospheric_vp);
double get_avg_temp(double, double, double *, double *, int);
double get_dist(double, double, double, double);
void   get_force_type(char *, int, int *, ProgramState*);
void   get_next_time_step(int *, int *, int *, int *, int *, int);

double hermint(double, int, double *, double *, double *, double *, double *);
void   hermite(int, double *, double *, double *, double *, double *);
void   HourlyT(int, int, int *, double *, int *, double *, double *);

out_data_file_struct* copy_data_file_format(const out_data_file_struct* out_template, const ProgramState* state);
void   init_output_list(out_data_struct *, int, const char *, int, float);
void   initialize_atmos(atmos_data_struct *, const dmy_struct *, FILE **, int *ncids, soil_con_struct *, const ProgramState*);

int initialize_model_state(cell_info_struct*, dmy_struct,
    filep_struct, int, const ProgramState *);

int    initialize_new_storm(std::vector<HRUElement>&,
			    int, int, int, double, double, const ProgramState *);
void   initialize_snow(std::vector<HRUElement>&);
void   initialize_soil(std::vector<HRUElement>&, int, soil_con_struct *, veg_con_struct *, int, const ProgramState*);
void initialize_veg(std::vector<HRUElement>&, int);

void   latent_heat_from_snow(double, double, double, double, double, 
                             double, double, double *, double *, 
                             double *, double *, double *);
double linear_interp(double,double,double,double,double);

dist_prcp_struct make_dist_prcp(int, const int, const int);
dmy_struct *make_dmy(global_param_struct *, const ProgramState*);
void make_in_files(filep_struct *, filenames_struct *, soil_con_struct *, const ProgramState*);
void make_out_files(filep_struct *, filenames_struct *, soil_con_struct *, out_data_file_struct *, const ProgramState*);
void   MassRelease(double *,double *,double *,double *);
double maximum_unfrozen_water(double, double, double, double, double, double);
double maximum_unfrozen_water_quick(double, double, double **);
double modify_Ksat(double, const ProgramState*);
void mtclim_wrapper(int, int, double, double, double, double,
                      double, double, double, double,
                    int, /* MPN: separate instance; should probably be const */ dmy_struct *, double *,
                      double *, double *, double *, double *, double *, const ProgramState*);

double new_snow_density(double, const ProgramState*);
void   nrerror(const char *);

FILE  *open_file(const char *string, const char *type);
FILE  *open_state_file(global_param_struct *, filenames_struct, int, int, char);

void parse_output_info(const char*, out_data_file_struct *, out_data_struct *, ProgramState*);
double penman(double, double, double, double, double, double, double);
void   prepare_full_energy(int, int, int, dist_prcp_struct *, 
			   const soil_con_struct *, double *, double *, const ProgramState*);
double priestley(double, double);
int put_data(cell_info_struct*, out_data_file_struct *, out_data_struct *,
    const dmy_struct *, int, const ProgramState*);

double read_arcinfo_value(char *, double, double);
int    read_arcinfo_info(char *, double **, double **, int **);
void   read_atmos_data(FILE *, int ncid, int, int, double **, soil_con_struct *, const ProgramState*);
double **read_forcing_data(FILE **, int *ncids, global_param_struct, soil_con_struct *, const ProgramState*);
void read_initial_model_state(FILE *, dist_prcp_struct *, int, int, int,
    soil_con_struct *, int, char *, int *, lake_con_struct,
    const ProgramState*);
void   read_snowband(FILE *, soil_con_struct *, const int);
void   read_snowmodel(atmos_data_struct *, FILE *, int, int, int, int);
soil_con_struct read_soilparam(FILE *, char *, char *, char *, ProgramState*);
soil_con_struct read_soilparam_arc(FILE *, char *, int *, char *, int,
    double *lat, double *lng, int *cellnum, ProgramState*);
veg_lib_struct *read_veglib(FILE *, int *, char);
veg_con_struct *read_vegparam(FILE *, int, int, const ProgramState*);
int redistribute_during_storm(std::vector<HRUElement>&, int,
      int, int, double, double, double, double *, const ProgramState*);
void   redistribute_moisture(layer_data_struct *, double *, double *,
			     double *, double *, double *, int);
unsigned char redistribute_moisture_for_storm(double *, double *, double, 
					      double, double);
int    runoff(cell_data_struct *, cell_data_struct *,
              energy_bal_struct *, const soil_con_struct *, double *,
              int, double, int, int, int, const ProgramState*);

void set_max_min_hour(double *, int, int *, int *);
void set_node_parameters(double *, double *, double *, double *, double *,
    double *, double *, double *, double *, double *, double *, double *,
    double *, double ***, double *, double *, double *, double *, int, int,
    char, const ProgramState*);
out_data_file_struct *set_output_defaults(out_data_struct *, ProgramState* state);
int set_output_var(out_data_file_struct *, int, int, out_data_struct *, const char *, int, const char *, int, float);
double snow_albedo(double, double, double, double, double, double, int, char, const ProgramState*);
double snow_density(snow_data_struct *, double, double, double, double, double, const ProgramState*);
int    snow_intercept(double, double, double, double, double, double,
                      double, double, double, double, double, double, 
                      double *, double *, double *, double *, double *, 
                      double *, double *, double *, double *, double *, 
                      double *, double *, double *, double *, double *, 
                      double *, char *, int *, double *, double *, double *, 
                      double *, double *, double *, const float *,
                      int, int, int, int, int, int, int, int,
                      const atmos_data_struct &, layer_data_struct *,
                      layer_data_struct *, const soil_con_struct *,
                      veg_var_struct *, veg_var_struct *, const ProgramState*);
int    snow_melt(double, double, double, double, double *, double, double *, double, 
		 double, double, double, double, double, double, double, 
                 double, double, double, double, double, double, 
                 double *, double *, double *, double *, double *, double *, 
                 double *, double *, double *, double *, double *, double *, 
                 int, int, int, int, snow_data_struct *, const soil_con_struct *, const ProgramState*);
double soil_conductivity(double, double, double, double, double, double, double, double);
void   soil_thermal_calc(soil_con_struct *, layer_data_struct *,
			 energy_bal_struct, double *, double *, double *,
			 int, int);
double solve_snow(char, double, double, double, double, double, double,
                  double, double, double, double, double, double,
                  double *, double *, double *, double *, double *,
                  double *, double *, double *, double *, double *,
                  double *, double *, double *, double *, double *, double *,
                  double *, double *, double *, double *, double *, double *,
                  double *, double *, double *, double *, double *, double *,
                  const float *, int, int, int, int, int, int, int, int, int, int *,
                  const dmy_struct *, const atmos_data_struct &, energy_bal_struct *,
                  layer_data_struct *, layer_data_struct *,
                  snow_data_struct *, const soil_con_struct *,
                  veg_var_struct *, veg_var_struct *, const ProgramState*);

int solve_T_profile(double *T, double *T0, char *Tfbflag, int *Tfbcount,
    double *kappa, double *Cs, double *moist, double deltat, double *ice,
    double Dp, double **const* ufwc_table_node, double *porosity,
    double *effective_porosity, int Nnodes, int *FIRST_SOLN, int NOFLUX,
    int EXP_TRANS, int veg_class, const soil_con_struct* soil_con,
    const ProgramState* state);

int solve_T_profile_implicit(double *, double *, double *, double *,
    double *, double, double *, double, int, int *, int, int, int,
    const soil_con_struct*, const ProgramState*);

double StabilityCorrection(double, double, double, double, double, double);
void   store_moisture_for_debug(int,int,double *, std::vector<HRUElement>&,
				const soil_con_struct *, const ProgramState*);
int surface_fluxes(char, double, double, double, double, int, double *,
        double *, double, double, double *, double *, double **, double *, double *,
        double *, double *, double *, double *, double *, double *, double *,
        const float *, int, int, int, int, int, int, int, int, int, atmos_data_struct *,
        const dmy_struct *, energy_bal_struct *, cell_data_struct *,
        cell_data_struct *, snow_data_struct *, const soil_con_struct *, veg_var_struct *,
        veg_var_struct *, float, float, float, const ProgramState*);
double svp(double);
double svp_slope(double);

void transpiration(layer_data_struct *, int, int, double, double, double,
    double, double, double, double, double, double, double, const double *, const double *,
    const double *, double *, double *, double *, const double *, const float *, const ProgramState*);
void tridag(double *,double *,double *,double *,double *,int);
void tridiag(double *, double *, double *, double *, int);
int update_thermal_nodes(dist_prcp_struct *, 
			  int, int, int, soil_con_struct *, veg_con_struct *, const ProgramState*);
void usage(char *);

void   vicerror(const char *);
double volumetric_heat_capacity(double,double,double,double);

void wrap_compute_zwt(const soil_con_struct *, cell_data_struct *, const ProgramState*);
void write_atmosdata(atmos_data_struct *, int, const ProgramState*);
void write_data(out_data_file_struct *, out_data_struct *, const dmy_struct *, int, const ProgramState*);
void write_debug(atmos_data_struct *, soil_con_struct *, cell_data_struct *,
                 energy_bal_struct *, snow_data_struct *, veg_var_struct *,
                 const dmy_struct *, double, double, int, int, int, int, int, char, const ProgramState*);
void write_dist_prcp(dist_prcp_struct *);
void write_forcing_file(atmos_data_struct *, int, out_data_file_struct *, out_data_struct *, const ProgramState*);
void write_header(out_data_file_struct *, out_data_struct *, const dmy_struct *, const ProgramState*);
void write_layer(layer_data_struct *, int, int, const double*);
void write_model_state(dist_prcp_struct *, int, int, filep_struct *,
      soil_con_struct *, char *, int *, lake_con_struct, const ProgramState*);
void write_snow_data(snow_data_struct, int, int);
void write_soilparam(soil_con_struct *, const ProgramState*);
void write_vegparam(veg_con_struct *, const ProgramState*);
void write_vegvar(veg_var_struct *, int);

void zero_output_list(out_data_struct *);
