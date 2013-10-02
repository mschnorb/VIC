// $Id$
/**********************************************************************
  This file contains "#define" statements and "typedef" statements.
  It also contains "extern" declarations for global variables.  For such
  variables, a single declaration/definition of the global variable, not
  containing the word "extern", must exist in global.h.  This is because
  global.h is only included by one file (vicNl.c), while vicNl_def.h is
  included multiple times (all *.c files) via vicNl.h.

  Modifications:
  2005-Mar-24 Added data structures to accomodate ALMA variables.	TJB
  2005-Apr-23 Changed ARNO_PARAMS to NIJSSEN2001_BASEFLOW.		TJB
  2005-Apr-23 Added out_data.aero_cond.					TJB
  2005-May-01 Added the ALMA vars CRainf, CSnowf, LSRainf, and LSSnowf.	TJB
  2005-May-02 Added the ALMA vars Wind_E, Wind_N.			TJB
  2005-Dec-21 Removed Trad.                                             GCT
  2006-Sep-23 Implemented flexible output configuration.
              Added output variable types.  Added binary output format
              types.  Removed all output files except the state file from
              the outfiles_struct and the filenames_struct.  Added
              Noutfiles to the option_struct.  Created new out_data_struct
              and out_data_files_struct.  Added new save_data structure.
              Organized the physical constants into one section; got rid
              of redundant Stefan-Boltzmann constant.  Implemented
              aggregation of output variables; added AGG_TYPE definitions.  TJB
  2006-Oct-10 Shortened the names of output variables whose names were
	      too long; fixed typos in others; created new OUT_IN_LONG
	      variable.							TJB
  2006-Oct-16 Merged infiles and outfiles structs into filep_struct;
	      This included merging global->statename to filenames->statefile. TJB
  2006-Nov-07 Added OUT_SOIL_TNODE.					TJB
  2006-Nov-07 Removed LAKE_MODEL option.				TJB
  2006-Nov-07 Organized model constants a bit more.			TJB
  2006-Dec-20 All atmos_data arrays are always dynamically allocated now.	TJB
  2006-Dec-29 Added REL_HUMID to list of supported met input variables.	TJB
  2007-Jan-02 Added ALMA_INPUT option; removed TAIR and PSURF from list
	      of supported met input variables.				TJB
  2007-Jan-15 Added PRT_HEADER option.					TJB
  2007-Apr-03 Added CONTINUEONERROR option.				GCT
  2007-Apr-03 Added ERROR value						KAC
  2007-Apr-24 Added IMPLICIT option.					JCA
  2007-Apr-24 Added EXP_TRANS option.					JCA
  2007-Apr-24 Added Zsum_node to soil_con structure.			JCA
  2007-Aug-08 Added features for EXCESS_ICE option.			JCA
  2007-Aug-22 Added OUTPUT_WATER_ERROR as output variable.		JCA
  2007-Sep-19 Added MAX_SUBSIDENCE parameter to EXCESS_ICE option.	JCA
  2007-Oct-24 Added surf_water to lake_var structure.			KAC via TJB
  2007-Nov-06 Updated lake_var structure with new variables.		LCB via TJB
  2008-Apr-21 Added snow surf_temp, pack_temp, and coldcontent to lake_var
	      structure.						LCB via TJB
  2008-Apr-21 Added SNOW_ALBEDO option.					KAC via TJB
  2008-Apr-21 Added SNOW_DENSITY option.				TJB
  2008-Sep-09 Added SOIL_TNODE_WL as an output variable, the soil
	      temperature in the wetland fraction of the grid cell.	LCB via TJB
  2009-Jan-12 Added COMPUTE_TREELINE and JULY_TAVG_SUPPLIED options.	TJB
  2009-Jan-16 Modified aero_resist_used and Ra_used to become arrays of
	      two elements (surface and overstory); added
	      options.AERO_RESIST_CANSNOW.				TJB
  2009-Jan-16 Added AERO_COND1&2 and AERO_RESIST1&2 to track
	      surface and overstory values; changed AERO_COND
	      and AERO_RESIST to track "scene" values.			TJB
  2009-Feb-09 Updated description of PRT_SNOW_BAND option.		TJB
  2009-Feb-22 Added OUT_VPD.						TJB
  2009-Mar-16 Added min_liq to the layer_data_struct.			TJB
  2009-May-17 Added OUT_ASAT.						TJB
  2009-May-17 Added AR_406_LS to options.AERO_RESIST_CANSNOW.		TJB
  2009-May-17 Added options.MIN_LIQ.					TJB
  2009-May-18 Added options.PLAPSE and Rd, the gas constant for dry
	      air.							TJB
  2009-May-20 Added options.GRND_FLUX_TYPE.				TJB
  2009-May-22 Added TFALLBACK value to options.CONTINUEONERROR.		TJB
  2009-Jun-09 Modified to use extension of veg_lib structure to contain
	      bare soil information.					TJB
  2009-Jun-09 Added OUT_PET_*, potential evap for various reference
	      land cover types.						TJB
  2009-Jun-09 Cell_data structure now only stores final aero_resist
	      values (called "aero_resist").  Preliminary uncorrected
	      aerodynamic resistances for current vegetation and various
	      reference land cover types for use in potential evap
	      calculations is stored in temporary array aero_resist.	TJB
  2009-Jun-19 Added T flag to indicate whether TFALLBACK occurred.	TJB
  2009-Jul-07 Added BandElev[] to soil_con_struct.			TJB
  2009-Jul-31 Added lake_idx to lake_con struct and LAKE to veg_con
	      struct.							TJB
  2009-Aug-28 OUT_LAKE_ICE_TEMP and OUT_LAKE_SURF_TEMP are [C].		TJB
  2009-Sep-19 Added T fbcount to count TFALLBACK occurrences.		TJB
  2009-Sep-19 Changed Cp to be 1013, the value for moist air.		TJB
  2009-Sep-19 Made TFALLBACK a separate option from CONTINUEONERROR.	TJB
  2009-Sep-28 Added snow and energy structures to lake_var_struct.	TJB
  2009-Sep-30 Miscellaneous fixes for lake model.			TJB
  2009-Oct-08 Extended T fallback scheme to snow and ice T.		TJB
  2009-Nov-09 Changed definition of sarea to include ice extent.	LCB via TJB
  2009-Dec-11 Removed min_liq and options.MIN_LIQ.			TJB
  2010-Feb-14 Added OUT_LAKE_AREA_FRAC.					TJB
  2010-Mar-31 Added RUNOFF_IN and OUT_RUNOFF_IN.			TJB
  2010-Apr-28 Replaced GLOBAL_LAI with VEGPARAM_LAI and LAI_SRC.	TJB
  2010-Sep-24 Renamed RUNOFF_IN and OUT_RUNOFF_IN to CHANNEL_IN and
	      OUT_LAKE_CHAN_IN, respectively.  Renamed OUT_EVAP_LAKE
	      to OUT_LAKE_EVAP.  Added other lake water balance terms
	      to set of output variables.  Added volumetric versions 
	      of these too.						TJB
  2010-Nov-02 Added OUT_LAKE_RO_IN, OUT_LAKE_RO_IN_V, OUT_LAKE_RCHRG,
	      OUT_LAKE_RCHRG_V, OUT_LAKE_VAPFLX, and OUT_LAKE_VAPFLX_V.	TJB
  2010-Nov-02 Changed units of lake_var moisture fluxes to volume (m3).	TJB
  2010-Nov-21 Added OUT_LAKE_DSTOR, OUT_LAKE_DSTOR_V, OUT_LAKE_DSWE,
	      OUT_LAKE_DSWE_V, OUT_LAKE_SWE, and OUT_LAKE_SWE_V.	TJB
  2010-Nov-26 Added lake->ice_throughfall, lake->swe_save, and
	      lake->volume_save.					TJB
  2010-Dec-01 Added cell->zwt and OUT_ZWT.				TJB
  2011-Jan-04 Added zwtvmoist_zwt and zwtvmoist_moist to soil_con_struct
	      for storing relationship between soil moisture and water
	      water table position based on soil water retention curve.	TJB
  2011-Mar-01 Added cell->zwt2, cell->zwt3, OUT_ZWT2, OUT_ZWT3, and
	      OUT_ZWTL.							TJB
  2011-May-31 Removed options.GRND_FLUX.				TJB
  2011-May-31 Increased length of zwtvmoist_* arrays.			TJB
  2011-Jun-03 Added options.ORGANIC_FRACT.  Soil properties now take
	      organic fraction into account.				TJB
  2011-Nov-04 Added options to handle new forcing estimation features.	TJB
  2012-Jan-02 Removed wetland_veg_class from lake_con_struct.		TJB
*********************************************************************/

#include <user_def.h>
#include <snow.h>
#include <cmath>
#include <limits>
#include <climits>
#include <vector>

/***** Model Constants *****/
#define MAXSTRING    2048
#define MINSTRING    20
#define HUGE_RESIST  1.e20	/* largest allowable double number */
#define SPVAL        1.e20	/* largest allowable double number - used to signify missing data */
#define SMALL        1.e-12	/* smallest allowable double number */
#define LITTLE 1		/* little-endian flag */
#define BIG 2			/* big-endian flag */
#define ERROR -999              /* Error Flag returned by subroutines */

//Using the C style "Not A Number" conventions, this will *not* work if you compile with -ffast-math
//You cannot check for equality (x == INVALID) as this always returns false for any NAN. Use the IS_VALID() or IS_INVALID() functions below.
#define INVALID NAN
#define INVALID_INT INT_MIN

//The compiler will automatically call the correct function based on the (overloaded) input type
inline bool IS_INVALID(float a) { return isnan(a); }
inline bool IS_VALID(float a) { return !isnan(a); }
inline bool IS_INVALID(double a) { return isnan(a); }
inline bool IS_VALID(double a) { return !isnan(a); }
inline bool IS_INVALID(int a) { return a == INVALID_INT; }
inline bool IS_VALID(int a) { return (!IS_INVALID(a)); }

/***** Met file formats *****/
#define ASCII 1
#define BINARY 2
#define NETCDF 3

/***** Snow Albedo parametrizations *****/
#define USACE   0
#define SUN1999 1

/***** Snow Density parametrizations *****/
#define DENS_BRAS   0
#define DENS_SNTHRM 1

/***** Baseflow parametrizations *****/
#define ARNO        0
#define NIJSSEN2001 1

/***** Aerodynamic Resistance options *****/
#define AR_406      0
#define AR_406_LS   1
#define AR_406_FULL 2
#define AR_410      3
#define AR_COMBO    4

/***** Ground Flux options *****/
#define GF_406  0
#define GF_410  1
#define GF_FULL 2

/***** VP algorithm options *****/
#define VP_ITER_NONE     0
#define VP_ITER_ALWAYS   1
#define VP_ITER_ANNUAL   2
#define VP_ITER_CONVERGE 3

/***** Longwave Clear-Sky Algorithm options *****/
#define LW_TVA        0
#define LW_ANDERSON   1
#define LW_BRUTSAERT  2
#define LW_SATTERLUND 3
#define LW_IDSO       4
#define LW_PRATA      5

/***** Longwave Cloud Algorithm options *****/
#define LW_CLOUD_BRAS       0
#define LW_CLOUD_DEARDORFF  1

/***** Potential Evap types *****/
#define N_PET_TYPES 6
#define N_PET_TYPES_NON_NAT 4
#define PET_SATSOIL 0
#define PET_H2OSURF 1
#define PET_SHORT   2
#define PET_TALL    3
#define N_PET_TYPES_NAT 2
#define PET_NATVEG  4
#define PET_VEGNOCR 5

/***** LAI source types *****/
#define LAI_FROM_VEGLIB     0
#define LAI_FROM_VEGPARAM   1

/***** Hard-coded veg class parameters (mainly for pot_evap) *****/
#define BARE_SOIL_ALBEDO 0.2	    /* albedo for bare soil */
#define H2O_SURF_ALBEDO 0.08	    /* albedo for deep water surface */
extern const char   ref_veg_over[];
extern const double ref_veg_rarc[];
extern const double ref_veg_rmin[];
extern const double ref_veg_lai[];
extern const double ref_veg_albedo[];
extern const double ref_veg_rough[];
extern const double ref_veg_displ[];
extern const double ref_veg_wind_h[];
extern const double ref_veg_RGL[];
extern const double ref_veg_rad_atten[];
extern const double ref_veg_wind_atten[];
extern const double ref_veg_trunk_ratio[];
extern const char ref_veg_ref_crop[];

/***** Time Constants *****/
#define DAYS_PER_YEAR 365.
#define HOURSPERDAY   24        /* number of hours per day */
#define HOURSPERYEAR  24*365    /* number of hours per year */
#define SECPHOUR     3600	/* seconds per hour */
#define SEC_PER_DAY 86400.	/* seconds per day */

/***** Physical Constants *****/
#define RESID_MOIST      0.0        /* define residual moisture content 
				       of soil column */
#define MAX_ICE_INIT      0.95        /* define maximum volumetric ice fraction
				       of soil column, for EXCESS_ICE option */
#define ICE_AT_SUBSIDENCE 0.8        /* minimum ice/porosity fraction before
					subsidence occurs, for EXCESS_ICE option */
#define MAX_SUBSIDENCE    1.0        /* maximum depth of subsidence per layer per
					time-step (mm) */
#define ice_density      917.	    /* density of ice (kg/m^3) */
#define T_lapse          6.5        /* temperature lapse rate of US Std 
				       Atmos in C/km */
#define von_K        0.40	/* Von Karman constant for evapotranspiration */
#define KELVIN       273.15	/* conversion factor C to K */
#define STEFAN_B     5.6696e-8	/* stefan-boltzmann const in unit W/m^2/K^4 */
#define Lf           3.337e5	/* Latent heat of freezing (J/kg) at 0C */
#define RHO_W        999.842594	/* Density of water (kg/m^3) at 0C */
#define Cp           1013.0	/* Specific heat at constant pressure of moist air 
				   (J/deg/K) (H.B.H. p.4.13)*/
#define CH_ICE       2100.0e3	/* Volumetric heat capacity (J/(m3*C)) of ice */
#define CH_WATER     4186.8e3   /* volumetric heat capacity of water */
#define K_SNOW       2.9302e-6  /* conductivity of snow (W/mK) */
#define SOLAR_CONSTANT 1400.0	/* Solar constant in W/m^2 */
#define EPS          0.62196351 /* Ratio of molecular weights: M_water_vapor/M_dry_air */
#define G            9.81       /* gravity */
#define Rd           287        /* Gas constant of dry air (J/degC*kg) */
#define JOULESPCAL   4.1868     /* Joules per calorie */
#define GRAMSPKG     1000.      /* convert grams to kilograms */
#define kPa2Pa 1000.            /* converts kPa to Pa */
#define DtoR 0.017453293	/* degrees to radians */
#ifndef PI
#define PI 3.1415927
#endif

/* define constants for saturated vapor pressure curve (kPa) */
#define A_SVP 0.61078
#define B_SVP 17.269
#define C_SVP 237.3

/* define constants for penman evaporation */
#define CP_PM 1013		/* specific heat of moist air at constant pressure (J/kg/C)
				   (Handbook of Hydrology) */
#define PS_PM 101300		/* sea level air pressure in Pa */
#define LAPSE_PM -0.006		/* environmental lapse rate in C/m */

/***** Physical Constraints *****/
#define MINSOILDEPTH 0.001	/* minimum layer depth with which model can
					work (m) */
#define STORM_THRES  0.001      /* thresehold at which a new storm is 
				   decalred */
#define SNOW_DT       5.0	/* Used to bracket snow surface temperatures
				   while computing the snow surface energy 
				   balance (C) */
#define SURF_DT       1.0	/* Used to bracket soil surface temperatures 
                                   while computing energy balance (C) */
#define SOIL_DT       0.25      /* Used to bracket soil temperatures while
                                   solving the soil thermal flux (C) */
#define CANOPY_DT    1.0	/* Used to bracket canopy air temperatures 
                                   while computing energy balance (C) */
#define CANOPY_VP    25.0	/* Used to bracket canopy vapor pressures 
                                   while computing moisture balance (Pa) */

/***** Define Boolean Values *****/
#ifndef FALSE
#define FALSE 0
#define TRUE !FALSE
#endif

#ifndef WET
#define WET 0
#define DRY 1
#endif

#ifndef SNOW
#define RAIN 0
#define SNOW 1
#endif

#define min(a,b) (a < b) ? a : b
#define max(a,b) (a > b) ? a : b


/***** Forcing Variable Types *****/
#define N_FORCING_TYPES 24
#define AIR_TEMP   0 /* air temperature per time step [C] (ALMA_INPUT: [K]) */
#define ALBEDO     1 /* surface albedo [fraction] */
#define CHANNEL_IN 2 /* incoming channel flow [m3] (ALMA_INPUT: [m3/s]) */
#define CRAINF     3 /* convective rainfall [mm] (ALMA_INPUT: [mm/s]) */
#define CSNOWF     4 /* convective snowfall [mm] (ALMA_INPUT: [mm/s]) */
#define DENSITY    5 /* atmospheric density [kg/m3] */
#define LONGWAVE   6 /* incoming longwave radiation [W/m2] */
#define LSRAINF    7 /* large-scale rainfall [mm] (ALMA_INPUT: [mm/s]) */
#define LSSNOWF    8 /* large-scale snowfall [mm] (ALMA_INPUT: [mm/s]) */
#define PREC       9 /* total precipitation (rain and snow) [mm] (ALMA_INPUT: [mm/s]) */
#define PRESSURE  10 /* atmospheric pressure [kPa] (ALMA_INPUT: [Pa]) */
#define QAIR      11 /* specific humidity [kg/kg] */
#define RAINF     12 /* rainfall (convective and large-scale) [mm] (ALMA_INPUT: [mm/s]) */
#define REL_HUMID 13 /* relative humidity [fraction] */
#define SHORTWAVE 14 /* incoming shortwave [W/m2] */
#define SNOWF     15 /* snowfall (convective and large-scale) [mm] (ALMA_INPUT: [mm/s]) */
#define TMAX      16 /* maximum daily temperature [C] (ALMA_INPUT: [K]) */
#define TMIN      17 /* minimum daily temperature [C] (ALMA_INPUT: [K]) */
#define TSKC      18 /* cloud cover fraction [fraction] */
#define VP        19 /* vapor pressure [kPa] (ALMA_INPUT: [Pa]) */
#define WIND      20 /* wind speed [m/s] */
#define WIND_E    21 /* zonal component of wind speed [m/s] */
#define WIND_N    22 /* meridional component of wind speed [m/s] */
#define SKIP      23 /* place holder for unused data columns */

/***** Output Variable Types *****/
#define N_OUTVAR_TYPES 160
// Water Balance Terms - state variables
#define OUT_ASAT             0  /* Saturated Area Fraction */
#define OUT_LAKE_AREA_FRAC   1  /* lake surface area as fraction of the grid cell area [fraction] */
#define OUT_LAKE_DEPTH       2  /* lake depth (distance between surface and deepest point) [m] */
#define OUT_LAKE_ICE         3  /* moisture stored as lake ice [mm over lake ice area] */
#define OUT_LAKE_ICE_FRACT   4  /* fractional coverage of lake ice [fraction] */
#define OUT_LAKE_ICE_HEIGHT  5  /* thickness of lake ice [cm] */
#define OUT_LAKE_MOIST       6  /* liquid water and ice stored in lake [mm over grid cell] */
#define OUT_LAKE_SURF_AREA   7  /* lake surface area [m2] */
#define OUT_LAKE_SWE         8  /* liquid water equivalent of snow on top of lake ice [m over lake ice area] */
#define OUT_LAKE_SWE_V       9  /* volumetric liquid water equivalent of snow on top of lake ice [m3] */
#define OUT_LAKE_VOLUME     10  /* lake volume [m3] */
#define OUT_ROOTMOIST       11  /* root zone soil moisture  [mm] */
#define OUT_SMFROZFRAC      12  /* fraction of soil moisture (by mass) that is ice, for each soil layer */
#define OUT_SMLIQFRAC       13  /* fraction of soil moisture (by mass) that is liquid, for each soil layer */
#define OUT_SNOW_CANOPY     14  /* snow interception storage in canopy  [mm] */
#define OUT_SNOW_COVER      15  /* fractional area of snow cover [fraction] */
#define OUT_SNOW_DEPTH      16  /* depth of snow pack [cm] */
#define OUT_SOIL_ICE        17  /* soil ice content  [mm] for each soil layer */
#define OUT_SOIL_LIQ        18  /* soil liquid content  [mm] for each soil layer */
#define OUT_SOIL_MOIST      19  /* soil total moisture content  [mm] for each soil layer */
#define OUT_SOIL_WET        20  /* vertical average of (soil moisture - wilting point)/(maximum soil moisture - wilting point) [mm/mm] */
#define OUT_SURFSTOR        21  /* storage of liquid water and ice (not snow) on surface (ponding) [mm] */
#define OUT_SURF_FROST_FRAC 22  /* fraction of soil surface that is frozen [fraction] */
#define OUT_SWE             23  /* snow water equivalent in snow pack (including vegetation-intercepted snow)  [mm] */
#define OUT_WDEW            24  /* total moisture interception storage in canopy [mm] */
#define OUT_ZWT             25  /* water table position [cm] - method 1 (zwt within lowest unsaturated layer) */
#define OUT_ZWT2            26  /* water table position [cm] - method 2 (zwt of total moisture across top-most N-1 layers, lumped together) */
#define OUT_ZWT3            27  /* water table position [cm] - method 3 (zwt of total moisture across all layers, lumped together) */
#define OUT_ZWTL            28  /* per-layer water table positions [cm] (one per soil layer) */
// Water Balance Terms - fluxes
#define OUT_BASEFLOW        29  /* baseflow out of the bottom layer  [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_DELINTERCEPT    30  /* change in canopy interception storage  [mm] */
#define OUT_DELSOILMOIST    31  /* change in soil water content  [mm] */
#define OUT_DELSURFSTOR     32  /* change in surface liquid water storage  [mm] */
#define OUT_DELSWE          33  /* change in snow water equivalent  [mm] */
#define OUT_EVAP            34  /* total net evaporation [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_EVAP_BARE       35  /* net evaporation from bare soil [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_EVAP_CANOP      36  /* net evaporation from canopy interception [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_INFLOW          37  /* moisture that reaches top of soil column [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_LAKE_BF_IN      38  /* incoming baseflow from lake catchment [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_LAKE_BF_IN_V    39  /* incoming volumetric baseflow from lake catchment [m3] (ALMA_OUTPUT: [m3/s]) */
#define OUT_LAKE_BF_OUT     40  /* outgoing baseflow from lake [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_LAKE_BF_OUT_V   41  /* outgoing volumetric baseflow from lake [m3] (ALMA_OUTPUT: [m3/s]) */
#define OUT_LAKE_CHAN_IN    42  /* channel inflow into lake [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_LAKE_CHAN_IN_V  43  /* volumetric channel inflow into lake [m3] (ALMA_OUTPUT: [m3/s]) */
#define OUT_LAKE_CHAN_OUT   44  /* channel outflow from lake [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_LAKE_CHAN_OUT_V 45  /* volumetric channel outflow from lake [m3] (ALMA_OUTPUT: [m3/s]) */
#define OUT_LAKE_DSTOR      46  /* change in lake moisture storage (liquid plus ice cover) [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_LAKE_DSTOR_V    47  /* volumetric change in lake moisture storage (liquid plus ice cover) [m3] (ALMA_OUTPUT: [m3/s]) */
#define OUT_LAKE_DSWE       48  /* change in swe on top of lake ice [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_LAKE_DSWE_V     49  /* volumetric change in swe on top of lake ice [m3] (ALMA_OUTPUT: [m3/s]) */
#define OUT_LAKE_EVAP       50  /* net evaporation from lake surface [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_LAKE_EVAP_V     51  /* net volumetric evaporation from lake surface [m3] (ALMA_OUTPUT: [m3/s]) */
#define OUT_LAKE_PREC_V     52  /* volumetric precipitation over lake surface [m3] (ALMA_OUTPUT: [m3/s]) */
#define OUT_LAKE_RCHRG      53  /* recharge from lake to surrounding wetland [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_LAKE_RCHRG_V    54  /* volumetric recharge from lake to surrounding wetland [m3] (ALMA_OUTPUT: [m3/s]) */
#define OUT_LAKE_RO_IN      55  /* incoming runoff from lake catchment [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_LAKE_RO_IN_V    56  /* incoming volumetric runoff from lake catchment [m3] (ALMA_OUTPUT: [m3/s]) */
#define OUT_LAKE_VAPFLX     57  /* outgoing sublimation from snow on top of lake ice [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_LAKE_VAPFLX_V   58  /* outgoing volumetric sublimation from snow on top of lake ice [m3] (ALMA_OUTPUT: [m3/s]) */
#define OUT_PET_SATSOIL     59  /* potential evap from saturated bare soil [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_PET_H2OSURF     60  /* potential evap from open water [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_PET_SHORT       61  /* potential evap (transpiration only) from short reference crop (grass) [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_PET_TALL        62  /* potential evap (transpiration only) from tall reference crop (alfalfa) [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_PET_NATVEG      63  /* potential evap (transpiration only) from current vegetation and current canopy resistance [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_PET_VEGNOCR     64  /* potential evap (transpiration only) from current vegetation and 0 canopy resistance [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_PREC            65  /* incoming precipitation [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_RAINF           66  /* rainfall  [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_REFREEZE        67  /* refreezing of water in the snow  [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_RUNOFF          68  /* surface runoff [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_SNOW_MELT       69  /* snow melt  [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_SNOWF           70  /* snowfall  [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_SUB_BLOWING     71  /* net sublimation of blowing snow [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_SUB_CANOP       72  /* net sublimation from snow stored in canopy [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_SUB_SNOW        73  /* total net sublimation from snow pack (surface and blowing) [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_SUB_SURFACE     74  /* net sublimation from snow pack surface [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_TRANSP_VEG      75  /* net transpiration from vegetation [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_WATER_ERROR     76  /* water budget error [mm] */
// Energy Balance Terms - state variables
#define OUT_ALBEDO          77  /* average surface albedo [fraction] */
#define OUT_BARESOILT       78  /* bare soil surface temperature [C] (ALMA_OUTPUT: [K]) */
#define OUT_FDEPTH          79  /* depth of freezing fronts [cm] (ALMA_OUTPUT: [m]) for each freezing front */
#define OUT_LAKE_ICE_TEMP   80  /* temperature of lake ice [C] (ALMA_OUTPUT: [K]) */
#define OUT_LAKE_SURF_TEMP  81  /* lake surface temperature [C] (ALMA_OUTPUT: [K]) */
#define OUT_RAD_TEMP        82  /* average radiative surface temperature [K] */
#define OUT_SALBEDO         83  /* snow pack albedo [fraction] */
#define OUT_SNOW_PACK_TEMP  84  /* snow pack temperature [C] (ALMA_OUTPUT: [K]) */
#define OUT_SNOW_SURF_TEMP  85  /* snow surface temperature [C] (ALMA_OUTPUT: [K]) */
#define OUT_SNOWT_FBFLAG    86  /* snow surface temperature flag */
#define OUT_SOIL_TEMP       87  /* soil temperature [C] (ALMA_OUTPUT: [K]) for each soil layer */
#define OUT_SOIL_TNODE      88  /* soil temperature [C] (ALMA_OUTPUT: [K]) for each soil thermal node */
#define OUT_SOIL_TNODE_WL   89  /* soil temperature [C] (ALMA_OUTPUT: [K]) for each soil thermal node in the wetland */
#define OUT_SOILT_FBFLAG    90  /* soil temperature flag for each soil thermal node */
#define OUT_SURF_TEMP       91  /* average surface temperature [C] (ALMA_OUTPUT: [K]) */
#define OUT_SURFT_FBFLAG    92  /* surface temperature flag */
#define OUT_TCAN_FBFLAG     93  /* Tcanopy flag */
#define OUT_TDEPTH          94  /* depth of thawing fronts [cm] (ALMA_OUTPUT: [m]) for each thawing front */
#define OUT_TFOL_FBFLAG     95  /* Tfoliage flag */
#define OUT_VEGT            96  /* average vegetation canopy temperature [C] (ALMA_OUTPUT: [K]) */
// Energy Balance Terms - fluxes
#define OUT_ADV_SENS        97  /* net sensible flux advected to snow pack [W/m2] */
#define OUT_ADVECTION       98  /* advected energy [W/m2] */
#define OUT_DELTACC         99  /* rate of change in cold content in snow pack [W/m2] (ALMA_OUTPUT: [J/m2]) */
#define OUT_DELTAH         100  /* rate of change in heat storage [W/m2] (ALMA_OUTPUT: [J/m2]) */
#define OUT_ENERGY_ERROR   101  /* energy budget error [W/m2] */
#define OUT_FUSION         102  /* net energy used to melt/freeze soil moisture [W/m2] */
#define OUT_GRND_FLUX      103  /* net heat flux into ground [W/m2] */
#define OUT_IN_LONG        104  /* incoming longwave at ground surface (under veg) [W/m2] */
#define OUT_LATENT         105  /* net upward latent heat flux [W/m2] */
#define OUT_LATENT_SUB     106  /* net upward latent heat flux from sublimation [W/m2] */
#define OUT_MELT_ENERGY    107  /* energy of fusion (melting) in snowpack [W/m2] */
#define OUT_NET_LONG       108  /* net downward longwave flux [W/m2] */
#define OUT_NET_SHORT      109  /* net downward shortwave flux [W/m2] */
#define OUT_R_NET          110  /* net downward radiation flux [W/m2] */
#define OUT_RFRZ_ENERGY    111  /* net energy used to refreeze liquid water in snowpack [W/m2] */
#define OUT_SENSIBLE       112  /* net upward sensible heat flux [W/m2] */
#define OUT_SNOW_FLUX      113  /* energy flux through snow pack [W/m2] */
// Miscellaneous Terms
#define OUT_AERO_COND      114  /* "scene" aerodynamic conductance [m/s] (tiles with overstory contribute overstory conductance; others contribute surface conductance) */
#define OUT_AERO_COND1     115  /* surface aerodynamic conductance [m/s] */
#define OUT_AERO_COND2     116  /* overstory aerodynamic conductance [m/s] */
#define OUT_AERO_RESIST    117  /* "scene"canopy aerodynamic resistance [s/m]  (tiles with overstory contribute overstory resistance; others contribute surface resistance)*/
#define OUT_AERO_RESIST1   118  /* surface aerodynamic resistance [s/m] */
#define OUT_AERO_RESIST2   119  /* overstory aerodynamic resistance [s/m] */
#define OUT_AIR_TEMP       120  /* air temperature [C] (ALMA_OUTPUT: [K])*/
#define OUT_DENSITY        121  /* near-surface atmospheric density [kg/m3]*/
#define OUT_LONGWAVE       122  /* incoming longwave [W/m2] */
#define OUT_PRESSURE       123  /* near surface atmospheric pressure [kPa] (ALMA_OUTPUT: [Pa])*/
#define OUT_QAIR           124  /* specific humidity [kg/kg] */
#define OUT_REL_HUMID      125  /* relative humidity [fraction]*/
#define OUT_SHORTWAVE      126  /* incoming shortwave [W/m2] */
#define OUT_SURF_COND      127  /* surface conductance [m/s] */
#define OUT_TSKC           128  /* cloud cover fraction [fraction] */
#define OUT_VP             129  /* near surface vapor pressure [kPa] (ALMA_OUTPUT: [Pa]) */
#define OUT_VPD            130  /* near surface vapor pressure deficit [kPa] (ALMA_OUTPUT: [Pa]) */
#define OUT_WIND           131  /* near surface wind speed [m/s] */
// Band-specific quantities
#define OUT_ADV_SENS_BAND       132  /* net sensible heat flux advected to snow pack [W/m2] */
#define OUT_ADVECTION_BAND      133  /* advected energy [W/m2] */
#define OUT_ALBEDO_BAND         134  /* average surface albedo [fraction] */
#define OUT_DELTACC_BAND        135  /* change in cold content in snow pack [W/m2] */
#define OUT_GRND_FLUX_BAND      136  /* net heat flux into ground [W/m2] */
#define OUT_IN_LONG_BAND        137  /* incoming longwave at ground surface (under veg) [W/m2] */
#define OUT_LATENT_BAND         138  /* net upward latent heat flux [W/m2] */
#define OUT_LATENT_SUB_BAND     139  /* net upward latent heat flux due to sublimation [W/m2] */
#define OUT_MELT_ENERGY_BAND    140  /* energy of fusion (melting) in snowpack [W/m2] */
#define OUT_NET_LONG_BAND       141  /* net downward longwave flux [W/m2] */
#define OUT_NET_SHORT_BAND      142  /* net downward shortwave flux [W/m2] */
#define OUT_RFRZ_ENERGY_BAND    143  /* net energy used to refreeze liquid water in snowpack [W/m2] */
#define OUT_SENSIBLE_BAND       144  /* net upward sensible heat flux [W/m2] */
#define OUT_SNOW_CANOPY_BAND    145  /* snow interception storage in canopy [mm] */
#define OUT_SNOW_COVER_BAND     146  /* fractional area of snow cover [fraction] */
#define OUT_SNOW_DEPTH_BAND     147  /* depth of snow pack [cm] */
#define OUT_SNOW_FLUX_BAND      148  /* energy flux through snow pack [W/m2] */
#define OUT_SNOW_MELT_BAND      149  /* snow melt [mm] (ALMA_OUTPUT: [mm/s]) */
#define OUT_SNOW_PACKT_BAND     150  /* snow pack temperature [C] (ALMA_OUTPUT: [K]) */
#define OUT_SNOW_SURFT_BAND     151  /* snow surface temperature [C] (ALMA_OUTPUT: [K]) */
#define OUT_SWE_BAND            152  /* snow water equivalent in snow pack [mm] */
// Dynamic Soil Property Terms - EXCESS_ICE option
#if EXCESS_ICE
#define OUT_SOIL_DEPTH          153  /* soil moisture layer depths [m] */
#define OUT_SUBSIDENCE          154  /* subsidence of soil layer [mm] */
#define OUT_POROSITY            155  /* porosity [mm/mm] */
#define OUT_ZSUM_NODE           156  /* depths of thermal nodes [m] */
#endif // EXCESS_ICE

/***** Output BINARY format types *****/
#define OUT_TYPE_DEFAULT 0 /* Default data type */
#define OUT_TYPE_CHAR    1 /* char */
#define OUT_TYPE_SINT    2 /* short int */
#define OUT_TYPE_USINT   3 /* unsigned short int */
#define OUT_TYPE_INT     4 /* int */
#define OUT_TYPE_FLOAT   5 /* single-precision floating point */
#define OUT_TYPE_DOUBLE  6 /* double-precision floating point */

/***** Output aggregation method types *****/
#define AGG_TYPE_AVG     0 /* average over agg interval */
#define AGG_TYPE_BEG     1 /* value at beginning of agg interval */
#define AGG_TYPE_END     2 /* value at end of agg interval */
#define AGG_TYPE_MAX     3 /* maximum value over agg interval */
#define AGG_TYPE_MIN     4 /* minimum value over agg interval */
#define AGG_TYPE_SUM     5 /* sum over agg interval */

/***** Codes for displaying version information *****/
#define DISP_VERSION 1
#define DISP_COMPILE_TIME 2
#define DISP_ALL 3

/***** Data Structures *****/

/** file structures **/
typedef struct {
  FILE *forcing[2];     /* atmospheric forcing data files */
  int forcing_ncid[2];
  FILE *globalparam;    /* global parameters file */
  FILE *init_state;     /* initial model state file */
  FILE *lakeparam;      /* lake parameter file */
  FILE *snowband;       /* snow elevation band data file */
  FILE *soilparam;      /* soil parameters for all grid cells */
  FILE *statefile;      /* output model state file */
  FILE *veglib;         /* vegetation parameters for all vege types */
  FILE *vegparam;       /* fractional coverage info for grid cell */
} filep_struct;

typedef struct {
  char  forcing[2][MAXSTRING];  /* atmospheric forcing data file names */
  char  f_path_pfx[2][MAXSTRING];  /* path and prefix for atmospheric forcing data file names */
  char  global[MAXSTRING];      /* global control file name */
  char  init_state[MAXSTRING];  /* initial model state file name */
  char  lakeparam[MAXSTRING];   /* lake model constants file */
  char  result_dir[MAXSTRING];  /* directory where results will be written */
  char  snowband[MAXSTRING];    /* snow band parameter file name */
  char  soil[MAXSTRING];        /* soil parameter file name, or name of 
				   file that has a list of all aoil 
				   ARC/INFO files */
  char  soil_dir[MAXSTRING];    /* directory from which to read ARC/INFO 
				   soil files */
  char  statefile[MAXSTRING];   /* name of file in which to store model state */
  char  veg[MAXSTRING];         /* vegetation grid coverage file */
  char  veglib[MAXSTRING];      /* vegetation parameter library file */
} filenames_struct;

typedef struct {

  // simulation modes
  int    AboveTreelineVeg; /* Default veg type to use above treeline;
			      Negative number indicates bare soil. */
  char   AERO_RESIST_CANSNOW; /* "AR_406" = multiply aerodynamic resistance
					    by 10 for latent heat but not
					    for sensible heat (as in
					    VIC 4.0.6); do NOT apply stability
					    correction; use surface aero_resist
					    for ET when no snow in canopy.
				 "AR_406_LS" = multiply aerodynamic resistance
					    by 10 for BOTH latent heat AND
					    sensible heat; do NOT apply
					    stability correction;
					    use surface aero_resist
					    for ET when no snow in canopy.
				 "AR_406_FULL" = multiply aerodynamic resistance
					    by 10 for BOTH latent heat AND
					    sensible heat; do NOT apply
					    stability correction;
					    always use canopy aero_resist
					    for ET.
				 "AR_410" = do not multiply aerodynamic
					    resistance by 10 in snow-filled
					    canopy (as in VIC 4.1.0);
					    DO apply stability correction;
					    always use canopy aero_resist
					    for ET.
				 "AR_COMBO" = multiply aerodynamic resistance
					    by 10 in snow-filled canopy for
					    BOTH latent AND sensible heat
					    computations AND apply stability
					    correction AND always use canopy
					    aero_resist for ET;
					    i.e. 406_FULL AND 410 */
  char   BLOWING;        /* TRUE = calculate sublimation from blowing snow */
  char   COMPUTE_TREELINE; /* TRUE = Determine treeline and exclude overstory
			      vegetation from higher elevations */
  char   CONTINUEONERROR;/* TRUE = VIC will continue to run after a cell has an error */
  char   CORRPREC;       /* TRUE = correct precipitation for gage undercatch */
  char   DIST_PRCP;      /* TRUE = Use distributed precipitation model */
  char   EQUAL_AREA;     /* TRUE = RESOLUTION stores grid cell area in km^2;
			    FALSE = RESOLUTION stores grid cell side length in degrees */
  char   EXP_TRANS;      /* TRUE = Uses grid transform for exponential node 
			    distribution for soil heat flux calculations*/
  char   FROZEN_SOIL;    /* TRUE = Use frozen soils code */
  char   FULL_ENERGY;    /* TRUE = Use full energy code */
  char   GRND_FLUX_TYPE; /* "GF_406"  = use (flawed) formulas for ground flux, deltaH, and fusion
                                        from VIC 4.0.6 and earlier
                            "GF_410"  = use formulas from VIC 4.1.0
                            "GF_FULL" = (use of this option is discouraged due to double-counting of surf_atten;
                                        this option has only been kept for backwards- compatibility) use ground
                                        flux formula from VIC 4.1.0 and also take surf_atten into account in
                                        deltaH and fusion
                            default = GF_410 */
  char   IMPLICIT;       /* TRUE = Use implicit solution when computing 
			    soil thermal fluxes */
  char   JULY_TAVG_SUPPLIED; /* If TRUE and COMPUTE_TREELINE is also true,
			        then average July air temperature will be read
			        from soil file and used in calculating treeline */
  char   LAKES;          /* TRUE = use lake energy code */
  char   LW_CLOUD;       /* Longwave cloud formulation; "LW_CLOUD_x" = code for LW cloud formulation - see LW_CLOUD codes above */
  char   LW_TYPE;        /* Longwave clear sky algorithm; "LW_x" = code for LW algorithm - see LW codes above */
  float  MIN_WIND_SPEED; /* Minimum wind speed in m/s that can be used by 
			    the model. **/
  char   MTCLIM_SWE_CORR;/* TRUE = correct MTCLIM's downward shortwave radiation estimate in presence of snow */
  int    Nlayer;         /* Number of layers in model */
  int    Nnode;          /* Number of soil thermal nodes in the model */
  char   NOFLUX;         /* TRUE = Use no flux lower bondary when computing 
			    soil thermal fluxes */
  char   PLAPSE;         /* TRUE = If air pressure not supplied as an
			    input forcing, compute it by lapsing sea-level
			    pressure by grid cell average elevation;
			    FALSE = air pressure set to constant 95.5 kPa */
  float  PREC_EXPT;      /* Exponential that controls the fraction of a
			    grid cell that receives rain during a storm
			    of given intensity */
  int    ROOT_ZONES;     /* Number of root zones used in simulation */
  char   QUICK_FLUX;     /* TRUE = Use Liang et al., 1999 formulation for
			    ground heat flux, if FALSE use explicit finite
			    difference method */
  char   QUICK_SOLVE;    /* TRUE = Use Liang et al., 1999 formulation for 
			    iteration, but explicit finite difference
			    method for final step. */
  char   SNOW_ALBEDO;    /* USACE: Use algorithm of US Army Corps of Engineers, 1956; SUN1999: Use algorithm of Sun et al., JGR, 1999 */
  char   SNOW_DENSITY;   /* DENS_BRAS: Use algorithm of Bras, 1990; DENS_SNTHRM: Use algorithm of SNTHRM89 adapted for 1-layer pack */
  int    SNOW_BAND;      /* Number of elevation bands over which to solve the 
			    snow model */
  int    SNOW_STEP;      /* Time step in hours to use when solving the 
			    snow model */
  float  SW_PREC_THRESH; /* Minimum daily precipitation [mm] that can cause "dimming" of incoming shortwave radiation */
  char   TFALLBACK;      /* TRUE = when any temperature iterations fail to converge,
                                   use temperature from previous time step; the number
                                   of instances when this occurs will be logged and
                                   reported at the end of the cell's simulation
                            FALSE = when iterations fail to converge, report an error
                                    and abort simulation for current grid cell
                            Default = TRUE */
  char   VP_INTERP;      /* How to disaggregate VP from daily to sub-daily;
                            TRUE = linearly interpolate between daily VP values, assuming they occur at the times of Tmin;
                            FALSE = hold VP constant at the daily value */
  char   VP_ITER;        /* VP_ITER_NONE = never iterate with SW
                            VP_ITER_ALWAYS = always iterate with SW
                            VP_ITER_ANNUAL = use annual Epot/PRCP criterion
                            VP_ITER_CONVERGE = always iterate until convergence */
  int    Nlakenode;      /* Number of lake thermal nodes in the model. */

  // input options
  char   ALMA_INPUT;     /* TRUE = input variables are in ALMA-compliant units; FALSE = standard VIC units */
  char   ARC_SOIL;       /* TRUE = use ARC/INFO gridded ASCII files for soil 
			    parameters*/
  char   BASEFLOW;       /* ARNO: read Ds, Dm, Ws, c; NIJSSEN2001: read d1, d2, d3, d4 */
  int    GRID_DECIMAL;   /* Number of decimal places in grid file extensions */
  char   VEGPARAM_LAI;   /* TRUE = veg param file contains monthly LAI values */
  char   LAI_SRC;        /* LAI_FROM_VEGLIB = read LAI values from veg library file
                            LAI_FROM_VEGPARAM = read LAI values from the veg param file */
  char   LAKE_PROFILE;   /* TRUE = user-specified lake/area profile */
  char   ORGANIC_FRACT;  /* TRUE = organic matter fraction of each layer is read from the soil parameter file; otherwise set to 0.0. */

  // state options
  char   BINARY_STATE_FILE; /* TRUE = model state file is binary (default) */
  char   INIT_STATE;     /* TRUE = initialize model state from file */
  char   SAVE_STATE;     /* TRUE = save state file */       

  // output options
  char   ALMA_OUTPUT;    /* TRUE = output variables are in ALMA-compliant units; FALSE = standard VIC units */
  char   BINARY_OUTPUT;  /* TRUE = output files are in binary, not ASCII */
  char   COMPRESS;       /* TRUE = Compress all output files */
  char   MOISTFRACT;     /* TRUE = output soil moisture as fractional moisture content */
  int    Noutfiles;      /* Number of output files (not including state files) */
  char   PRT_HEADER;     /* TRUE = insert header at beginning of output file; FALSE = no header */
  char   PRT_SNOW_BAND;  /* TRUE = print snow parameters for each snow band. This is only used when default
				   output files are used (for backwards-compatibility); if outfiles and
				   variables are explicitly mentioned in global parameter file, this option
				   is ignored. */

} option_struct;

#if LINK_DEBUG

typedef struct {
  FILE    *fg_balance;
  FILE    *fg_energy;
  FILE    *fg_grid;
  FILE    *fg_kappa;
  FILE    *fg_lake;
  FILE    *fg_modelstep_atmos;
  FILE    *fg_moist;
  FILE    *fg_snow;
  FILE    *fg_snowstep_atmos;
  FILE    *fg_temp;
  char     DEBUG;
  char     PRT_ATMOS;
  char     PRT_BALANCE;
  char     PRT_FLUX;
  char     PRT_GLOBAL;
  char     PRT_GRID;
  char     PRT_KAPPA;
  char     PRT_LAKE;
  char     PRT_MOIST;
  char     PRT_SNOW;
  char     PRT_SOIL;
  char     PRT_TEMP;
  char     PRT_VAR;
  char     PRT_VEGE;
  char     debug_dir[512];
  double **inflow[2];
  double **outflow[2];
  double **store_moist[2];
} debug_struct;

#endif

/*******************************************************
  Stores forcing file input information.
*******************************************************/
typedef struct {
  char    SIGNED;
  int     SUPPLIED;
  double  multiplier;
} force_type_struct;

/******************************************************************
  This structure records the parameters set by the forcing file
  input routines.  Those filled, are used to estimate the paramters
  needed for the model run in initialize_atmos.c.
  ******************************************************************/
typedef struct {
  force_type_struct TYPE[N_FORCING_TYPES];
  int  FORCE_DT[2];     /* forcing file time step */
  int  FORCE_ENDIAN[2]; /* endian-ness of input file, used for
			   DAILY_BINARY format */
  int  FORCE_FORMAT[2]; /* NETCDF or ASCII or BINARY */
  int  FORCE_INDEX[2][N_FORCING_TYPES];
  int  N_TYPES[2];
} param_set_struct;

/*******************************************************
  This structure stores all model run global parameters.
  *******************************************************/
typedef struct {
  double MAX_SNOW_TEMP; /* maximum temperature at which snow can fall (C) */
  double MIN_RAIN_TEMP; /* minimum temperature at which rain can fall (C) */
  double measure_h;  /* height of measurements (m) */
  double wind_h;     /* height of wind measurements (m) */ 
  float  resolution; /* Model resolution (degrees) */
  int    dt;         /* Time step in hours (24/dt must be an integer) */
  int    out_dt;     /* Output time step in hours (24/out_dt must be an integer) */
  int    endday;     /* Last day of model simulation */
  int    endmonth;   /* Last month of model simulation */
  int    endyear;    /* Last year of model simulation */
  int    forceday[2];   /* day forcing files starts */
  int    forcehour[2];  /* hour forcing files starts */
  int    forcemonth[2]; /* month forcing files starts */
  int    forceskip[2];  /* number of model time steps to skip at the start of
			the forcing file */
  int    forceyear[2];  /* year forcing files start */
  int    nrecs;      /* Number of time steps simulated */
  int    skipyear;   /* Number of years to skip before writing output data */
  int    startday;   /* Starting day of the simulation */
  int    starthour;  /* Starting hour of the simulation */
  int    startmonth; /* Starting month of the simulation */
  int    startyear;  /* Starting year of the simulation */
  int    stateday;   /* Day of the simulation at which to save model state */
  int    statemonth; /* Month of the simulation at which to save model state */
  int    stateyear;  /* Year of the simulation at which to save model state */
} global_param_struct;

/***********************************************************
  This structure stores the soil parameters for a grid cell.
  ***********************************************************/
typedef struct {
  int      FS_ACTIVE;                 /* if TRUE frozen soil algorithm is 
					 active in current grid cell */
  double   Ds;                        /* fraction of maximum subsurface flow 
					 rate */
  double   Dsmax;                     /* maximum subsurface flow rate 
					 (mm/day) */
  double   Ksat[MAX_LAYERS];          /* saturated hydraulic  conductivity 
					 (mm/day) */
  double   Wcr[MAX_LAYERS];           /* critical moisture level for soil 
					 layer, evaporation is no longer 
					 affected moisture stress in the 
					 soil (mm) */
  double   Wpwp[MAX_LAYERS];          /* soil moisture content at permanent 
					 wilting point (mm) */
  double   Ws;                        /* fraction of maximum soil moisture */
#if EXCESS_ICE
  double   Ds_orig;                   /* fraction of maximum subsurface flow 
					 rate */
  double   Dsmax_orig;                /* maximum subsurface flow rate 
					 (mm/day) */
  double   Ws_orig;                   /* fraction of maximum soil moisture */
#endif  
  double   alpha[MAX_NODES];          /* thermal solution constant */
  double   annual_prec;               /* annual average precipitation (mm) */
  double   avg_temp;                  /* average soil temperature (C) */
  double   avgJulyAirTemp;            /* Average July air temperature (C) */
  double   b_infilt;                  /* infiltration parameter */
  double   beta[MAX_NODES];           /* thermal solution constant */
  double   bubble[MAX_LAYERS];        /* bubbling pressure, HBH 5.15 (cm) */
  double   bubble_node[MAX_NODES];    /* bubbling pressure (cm) */
  double   bulk_density[MAX_LAYERS];  /* soil bulk density (kg/m^3) */
  double   bulk_dens_min[MAX_LAYERS]; /* bulk density of mineral soil (kg/m^3) */
  double   bulk_dens_org[MAX_LAYERS]; /* bulk density of organic soil (kg/m^3) */
  double   c;                         /* exponent in ARNO baseflow scheme */
  double   depth[MAX_LAYERS];         /* thickness of each soil moisture layer (m).  In the case of EXCESS_ICE, this is the effective (dynamic) depth. */
#if SPATIAL_SNOW
  double   depth_full_snow_cover;     // minimum depth for full snow cover
#endif // SPATIAL_SNOW
  double   dp;                        /* soil thermal damping depth (m) */
  double   dz_node[MAX_NODES];        /* thermal node thickness (m) */
  double   Zsum_node[MAX_NODES];      /* thermal node depth (m) */
  double   expt[MAX_LAYERS];          /* layer-specific exponent n (=3+2/lambda) in Campbell's eqn for hydraulic conductivity, HBH 5.6 */
  double   expt_node[MAX_NODES];      /* node-specific exponent n (=3+2/lambda) in Campbell's eqn for hydraulic conductivity, HBH 5.6 */

  double   frost_fract[FROST_SUBAREAS]; /* spatially distributed frost coverage fractions */
  double   frost_slope;               // slope of frost distribution

  double   gamma[MAX_NODES];          /* thermal solution constant */
  double   init_moist[MAX_LAYERS];    /* initial layer moisture level (mm) */
  double   max_infil;                 /* maximum infiltration rate */
  double   max_moist[MAX_LAYERS];     /* maximum moisture content (mm) per layer */
  double   max_moist_node[MAX_NODES]; /* maximum moisture content (mm/mm) per node */
  double   phi_s[MAX_LAYERS];         /* soil moisture diffusion parameter (mm/mm) */
  double   porosity[MAX_LAYERS];      /* porosity (fraction) */
  double   quartz[MAX_LAYERS];        /* quartz content of soil (fraction of mineral soil volume) */
  double   organic[MAX_LAYERS];       /* organic content of soil (fraction of total soil volume) */
  double   resid_moist[MAX_LAYERS];   /* residual moisture content of soil layer */
  double   rough;                     /* soil surface roughness (m) */
  double   snow_rough;                /* snow surface roughness (m) */
  double   soil_density[MAX_LAYERS];  /* soil particle density (kg/m^3) */
  double   soil_dens_min[MAX_LAYERS]; /* particle density of mineral soil (kg/m^3) */
  double   soil_dens_org[MAX_LAYERS]; /* particle density of organic soil (kg/m^3) */
  float   *BandElev;                  /* Elevation of each snow elevation band */
  double  *AreaFract;                 /* Fraction of grid cell included in each snow elevation band */
  double  *Pfactor;                   /* Change in Precipitation due to elevation (fract) in each snow elevation band */
  double  *Tfactor;                   /* Change in temperature due to elevation (C) in each snow elevation band */
  char    *AboveTreeLine;             /* Flag to indicate if band is above the treeline */

  double **ufwc_table_layer[MAX_LAYERS];
  double **ufwc_table_node[MAX_NODES]; 

  float    elevation;                 /* grid cell elevation (m) */
  float    lat;                       /* grid cell central latitude */
  float    lng;                       /* grid cell central longitude */
  double   cell_area;                 /* Area of grid cell (m^2) */
  float    time_zone_lng;             /* central meridian of the time zone */
  float  **layer_node_fract;          /* fraction of all nodes within each layer */
  int      gridcel;                   /* grid cell number */
  double   zwtvmoist_zwt[MAX_LAYERS+2][MAX_ZWTVMOIST]; /* zwt values in the zwt-v-moist curve for each layer */
  double   zwtvmoist_moist[MAX_LAYERS+2][MAX_ZWTVMOIST]; /* moist values in the zwt-v-moist curve for each layer */
  double   slope;
  double   aspect;
  double   ehoriz;
  double   whoriz;
  //Excess ice variables
  double   min_depth[MAX_LAYERS];     /* soil layer depth as given in the soil file (m).  The effective depth will always be >= this value. */
  double   porosity_node[MAX_NODES];  /* porosity for each thermal node */
  double   effective_porosity[MAX_LAYERS]; /* effective soil porosity (fraction) when soil pores are expanded due to excess ground ice */
  double   effective_porosity_node[MAX_NODES]; /* effective soil porosity (fraction) when soil pores are expanded due to excess ground ice */
  double   Wcr_FRACT[MAX_LAYERS];
  double   Wpwp_FRACT[MAX_LAYERS];
  double   subsidence[MAX_LAYERS];      /* subsidence of soil layer, mm*/
} soil_con_struct;

/*****************************************************************
  This structure stores the dynamic soil properties for a grid cell
  *****************************************************************/
#if EXCESS_ICE
typedef struct {
  double soil_depth[MAX_LAYERS];             /* soil moisture layer depths [m] */
  double subsidence[MAX_LAYERS];             /* subsidence of soil layer [mm] */
  double porosity[MAX_LAYERS];               /* porosity [mm/mm] */
  double zsum_node[MAX_NODES];               /* depths of thermal nodes [m] */
} dynamic_soil_struct;
#endif // EXCESS_ICE

/*******************************************************************
  This structure stores information about the vegetation coverage of
  the current grid cell.
  *******************************************************************/
typedef struct {
  double  Cv;               /* fraction of vegetation coverage */ 
  double  Cv_sum;           /* total fraction of vegetation coverage */
  float   root[MAX_LAYERS]; /* percent of roots in each soil layer (fraction) */
  float  *zone_depth;       /* depth of root zone */
  float  *zone_fract;       /* fraction of roots within root zone */
  int     veg_class;        /* vegetation class reference number */
  int     vegetat_type_num; /* number of vegetation types in the grid cell */
  float   sigma_slope;      /* Std. deviation of terrain slope for each vegetation class. */
  float   lag_one;          /* Lag one gradient autocorrelation of terrain slope */
  float   fetch;            /* Average fetch length for each vegetation class. */
  int     LAKE;             /* TRUE = this tile is a lake/wetland tile */
} veg_con_struct;

/******************************************************************
  This structure stores parameters for individual vegetation types.
  ******************************************************************/
typedef struct {
  char   overstory;        /* TRUE = overstory present, important for snow 
			      accumulation in canopy */
  double LAI[12];          /* monthly leaf area index */
  double Wdmax[12];        /* maximum monthly dew holding capacity (mm) */
  double albedo[12];       /* vegetation albedo (added for full energy) 
			      (fraction) */
  double displacement[12]; /* vegetation displacement height (m) */
  double emissivity[12];   /* vegetation emissivity (fraction) */
  int    NVegLibTypes;     /* number of vegetation classes defined in library */
  double rad_atten;        /* radiation attenuation due to canopy, 
			      default = 0.5 (N/A) */
  double rarc;             /* architectural resistance (s/m) */
  double rmin;             /* minimum stomatal resistance (s/m) */
  double roughness[12];    /* vegetation roughness length (m) */
  double trunk_ratio;      /* ratio of trunk height to tree height, 
			      default = 0.2 (fraction) */
  double wind_atten;       /* wind attenuation through canopy, 
			      default = 0.5 (N/A) */
  double wind_h;           /* height at which wind is measured (m) */
  float  RGL;              /* Value of solar radiation below which there 
			      will be no transpiration (ranges from 
			      ~30 W/m^2 for trees to ~100 W/m^2 for crops) */
  int    veg_class;        /* vegetation class reference number */
} veg_lib_struct;

/***************************************************************************
   This structure stores the atmospheric forcing data for each model time 
   step for a single grid cell.  Each array stores the values for the 
   SNOW_STEPs during the current model step and the value for the entire model
   step.  The latter is referred to by array[NR].  Looping over the SNOW_STEPs
   is done by for (i = 0; i < NF; i++) 
***************************************************************************/
typedef struct {
  double *air_temp;  /* air temperature (C) */
  double *channel_in;/* incoming channel inflow for time step (mm) */
  double *density;   /* atmospheric density (kg/m^3) */
  double *longwave;  /* incoming longwave radiation (W/m^2) (net incoming
                        longwave for water balance model) */
  double out_prec;   /* Total precipitation for time step - accounts
                        for corrected precipitation totals */
  double out_rain;   /* Rainfall for time step (mm) */
  double out_snow;   /* Snowfall for time step (mm) */
  double *prec;      /* average precipitation in grid cell (mm) */
  double *pressure;  /* atmospheric pressure (kPa) */
  double *shortwave; /* incoming shortwave radiation (W/m^2) */
  char   *snowflag;  /* TRUE if there is snowfall in any of the snow
                        bands during the timestep, FALSE otherwise*/
  double *tskc;      /* cloud cover fraction (fraction) */
  double *vp;        /* atmospheric vapor pressure (kPa) */
  double *vpd;       /* atmospheric vapor pressure deficit (kPa) */
  double *wind;      /* wind speed (m/s) */
} atmos_data_struct;

/*************************************************************************
  This structure stores information about the time and date of the current
  time step.
  *************************************************************************/
typedef struct {
  int day;                      /* current day */
  int day_in_year;              /* julian day in year */
  int hour;                     /* beginning of current hour */
  int month;                    /* current month */
  int year;                     /* current year */
} dmy_struct;			/* array of length nrec created */

/***************************************************************
  This structure stores all soil variables for each layer in the
  soil column.
  ***************************************************************/
typedef struct {
  double Cs;                /* average volumetric heat capacity of the 
			       current layer (J/m^3/K) */
  double T;                 /* temperature of the unfrozen sublayer (C) */
  double evap;              /* evapotranspiration from soil layer (mm) */
#if SPATIAL_FROST
  double soil_ice[FROST_SUBAREAS]; /* ice content of the frozen sublayer (mm) */
#else
  double soil_ice;               /* ice content of the frozen sublayer (mm) */
#endif
  double kappa;             /* average thermal conductivity of the current 
			       layer (W/m/K) */
  double moist;             /* moisture content of the unfrozen sublayer 
			       (mm) */
  double phi;               /* moisture diffusion parameter */
  double zwt;               /* water table position relative to soil surface within the layer (cm) */
} layer_data_struct;

/******************************************************************
  This structure stores soil variables for the complete soil column 
  for each grid cell.
  ******************************************************************/
typedef struct {
  double aero_resist[2];               /* The (stability-corrected) aerodynamic
                                          resistance (s/m) that was actually used
                                          in flux calculations.
					  [0] = surface (bare soil, non-overstory veg, or snow pack)
					  [1] = overstory */
  double asat;                         /* saturated area fraction */
  double baseflow;                     /* baseflow from current cell (mm/TS) */
  double inflow;                       /* moisture that reaches the top of 
					  the soil column (mm) */
  double pot_evap[N_PET_TYPES];        /* array of different types of potential evaporation (mm) */
  double runoff;                       /* runoff from current cell (mm/TS) */
  layer_data_struct layer[MAX_LAYERS]; /* structure containing soil variables 
					  for each layer (see above) */
  double rootmoist;                    /* total of layer.moist over all layers
                                          in the root zone (mm) */
  double wetness;                      /* average of
                                          (layer.moist - Wpwp)/(porosity*depth - Wpwp)
                                          over all layers (fraction) */
  double zwt;                          /* average water table position [cm] - method 1 */
  double zwt2;                         /* average water table position [cm] - method 2 */
  double zwt3;                         /* average water table position [cm] - method 3 */
} cell_data_struct;

/***********************************************************************
  This structure stores energy balance components, and variables used to
  solve the thermal fluxes through the soil column.
  ***********************************************************************/
typedef struct {
  // State variables
  double  AlbedoLake;            /* albedo of lake surface (fract) */
  double  AlbedoOver;            /* albedo of intercepted snow (fract) */
  double  AlbedoUnder;           /* surface albedo (fraction) */
  double  Cs[2];                 /* heat capacity for top two layers (J/m^3/K) */
  double  Cs_node[MAX_NODES];    /* heat capacity of the soil thermal nodes (J/m^3/K) */
  double  fdepth[MAX_FRONTS];    /* all simulated freezing front depths */
  char    frozen;                /* TRUE = frozen soil present */
  double  ice_content[MAX_NODES];/* thermal node ice content */
  double  kappa[2];              /* soil thermal conductivity for top two layers (W/m/K) */
  double  kappa_node[MAX_NODES]; /* thermal conductivity of the soil thermal nodes (W/m/K) */
  double  moist[MAX_NODES];      /* thermal node moisture content */
  int     Nfrost;                /* number of simulated freezing fronts */
  int     Nthaw;                 /* number of simulated thawing fronts */
  double  T[MAX_NODES];          /* thermal node temperatures (C) */
  char    T_fbflag[MAX_NODES];   /* flag indicating if previous step's temperature was used */
  int     T_fbcount[MAX_NODES];  /* running total number of times that previous step's temperature was used */
  int     T1_index;              /* soil node at the bottom of the top layer */
  double  Tcanopy;               /* temperature of the canopy air */
  char    Tcanopy_fbflag;        /* flag indicating if previous step's temperature was used */
  int     Tcanopy_fbcount;       /* running total number of times that previous step's temperature was used */
  double  tdepth[MAX_FRONTS];    /* all simulated thawing front depths */
  double  Tfoliage;              /* temperature of the overstory vegetation */
  char    Tfoliage_fbflag;       /* flag indicating if previous step's temperature was used */
  int     Tfoliage_fbcount;      /* running total number of times that previous step's temperature was used */
  double  Tsurf;                 /* temperature of the understory */
  char    Tsurf_fbflag;          /* flag indicating if previous step's temperature was used */
  int     Tsurf_fbcount;         /* running total number of times that previous step's temperature was used */
  double  unfrozen;              /* frozen layer water content that is unfrozen */
  // Fluxes
  double  advected_sensible;     /* net sensible heat flux advected to snowpack (Wm-2) */
  double  advection;             /* advective flux (Wm-2) */
  double  AtmosError;
  double  AtmosLatent;           /* latent heat exchange with atmosphere */
  double  AtmosLatentSub;        /* latent sub heat exchange with atmosphere */
  double  AtmosSensible;         /* sensible heat exchange with atmosphere */
  double  canopy_advection;      /* advection heat flux from the canopy (W/m^2) */
  double  canopy_latent;         /* latent heat flux from the canopy (W/m^2) */
  double  canopy_latent_sub;     /* latent heat flux of sublimation from the canopy (W/m^2) */
  double  canopy_refreeze;       /* energy used to refreeze/melt canopy intercepted snow (W/m^2) */
  double  canopy_sensible;       /* sensible heat flux from canopy interception (W/m^2) */
  double  deltaCC;               /* change in snow heat storage (Wm-2) */
  double  deltaH;                /* change in soil heat storage (Wm-2) */
  double  error;                 /* energy balance error (W/m^2) */
  double  fusion;                /* energy used to freeze/thaw soil water */
  double  grnd_flux;             /* ground heat flux (Wm-2) */
  double  latent;                /* net latent heat flux (Wm-2) */
  double  latent_sub;            /* net latent heat flux from snow (Wm-2) */
  double  longwave;              /* net longwave flux (Wm-2) */
  double  LongOverIn;            /* incoming longwave to overstory */
  double  LongUnderIn;           /* incoming longwave to understory */
  double  LongUnderOut;          /* outgoing longwave from understory */
  double  melt_energy;           /* energy used to reduce snow cover fraction (Wm-2) */
  double  NetLongAtmos;          /* net longwave radiation to the atmosphere (W/m^2) */
  double  NetLongOver;           /* net longwave radiation from the overstory (W/m^2) */
  double  NetLongUnder;          /* net longwave radiation from the understory (W/m^2) */
  double  NetShortAtmos;         /* net shortwave to the atmosphere */
  double  NetShortGrnd;          /* net shortwave penetrating snowpack */
  double  NetShortOver;          /* net shortwave radiation from the overstory (W/m^2) */
  double  NetShortUnder;         /* net shortwave radiation from the understory (W/m^2) */
  double  out_long_canopy;       /* outgoing longwave to canopy */
  double  out_long_surface;      /* outgoing longwave to surface */
  double  refreeze_energy;       /* energy used to refreeze the snowpack (Wm-2) */
  double  sensible;              /* net sensible heat flux (Wm-2) */
  double  shortwave;             /* net shortwave radiation (Wm-2) */
  double  ShortOverIn;           /* incoming shortwave to overstory */
  double  ShortUnderIn;          /* incoming shortwave to understory */
  double  snow_flux;             /* thermal flux through the snow pack (Wm-2) */
} energy_bal_struct;

/***********************************************************************
  This structure stores vegetation variables for each vegetation type in 
  a grid cell.
  ***********************************************************************/
typedef struct {
  double canopyevap;		/* evaporation from canopy (mm/TS) */
  double throughfall;		/* water that reaches the ground through the canopy (mm/TS) */
  double Wdew;			/* dew trapped on vegetation (mm) */
} veg_var_struct;

/************************************************************************
  This structure stores snow pack variables needed to run the snow model.
  ************************************************************************/
typedef struct {
  // State variables
  double albedo;            /* snow surface albedo (fraction) */
  double canopy_albedo;     /* albedo of the canopy (fract) */
  double coldcontent;       /* cold content of snow pack */
  double coverage;          /* fraction of snow band that is covered with snow */
  double density;           /* snow density (kg/m^3) */
  double depth;             /* snow depth (m) */
  int    last_snow;         /* time steps since last snowfall */
  double max_swq;           /* last maximum swq - used to determine coverage
			       fraction during current melt period (m) */
  char   MELTING;           /* flag indicating that snowpack melted 
			       previously */
  double pack_temp;         /* depth averaged temperature of the snowpack (C) */
  double pack_water;        /* liquid water content of the snow pack (m) */
  int    snow;              /* TRUE = snow, FALSE = no snow */
  double snow_canopy;       /* amount of snow on canopy (m) */
  double store_coverage;    /* stores coverage fraction covered by new snow (m) */
  int    store_snow;        /* flag indicating whether or not new accumulation
			       is stored on top of an existing distribution */
  double store_swq;         /* stores newly accumulated snow over an 
			       established snowpack melt distribution (m) */
  double surf_temp;         /* depth averaged temperature of the snow pack surface layer (C) */
  double surf_temp_fbcount; /* running total number of times that previous step's temperature was used */
  double surf_temp_fbflag;  /* flag indicating if previous step's temperature was used */
  double surf_water;        /* liquid water content of the surface layer (m) */
  double swq;               /* snow water equivalent of the entire pack (m) */
  double swq_slope;         /* slope of uniform snow distribution (m/fract) */
  double tmp_int_storage;   /* temporary canopy storage, used in snow_canopy */
  // Fluxes
  double blowing_flux;      /* depth of sublimation from blowing snow (m) */
  double canopy_vapor_flux; /* depth of water evaporation, sublimation, or 
			       condensation from intercepted snow (m) */
  double mass_error;        /* snow mass balance error */
  double melt;              /* snowpack melt (mm) */
  double Qnet;              /* Residual of energy balance at snowpack surface */
  double surface_flux;      /* depth of sublimation from blowing snow (m) */
  double transport;	    /* flux of snow (potentially) transported from veg type */
  double vapor_flux;        /* depth of water evaporation, sublimation, or 
			       condensation from snow pack (m) */
} snow_data_struct;

/******************************************************************
  This structure stores the lake/wetland parameters for a grid cell
  ******************************************************************/
typedef struct {
  // Lake basin dimensions
  int    numnod;                  /* Maximum number of lake nodes for this grid cell */
  double z[MAX_LAKE_NODES+1];     /* Elevation of each lake node (when lake storage is at maximum), relative to lake's deepest point (m) */  
  double basin[MAX_LAKE_NODES+1]; /* Area of lake basin at each lake node (when lake storage is at maximum) (m^2) */
  double Cl[MAX_LAKE_NODES+1];    /* Fractional coverage of lake basin at each node (when lake storage is at maximum) (fraction of grid cell area) */
  double b;                       /* Exponent in default lake depth-area profile (y=Ax^b) */
  double maxdepth;                /* Maximum allowable depth of liquid portion of lake (m) */
  double mindepth;                /* Minimum allowable depth of liquid portion of lake (m) */
  double maxvolume;               /* Lake volume when lake depth is at maximum (m^3) */
  double minvolume;               /* Lake volume when lake depth is at minimum (m^3) */
  // Hydrological properties
  float  bpercent;                /* Fraction of wetland baseflow (subsurface runoff) that flows into lake */
  float  rpercent;                /* Fraction of wetland surface runoff that flows into lake */
  double eta_a;                   /* Decline of solar radiation w/ depth (m^-1) */ /* not currently used */
  double wfrac;                   /* Width of lake outlet, expressed as fraction of lake perimeter */
  // Initial conditions
  double depth_in;                /* Initial lake depth (distance from surface to deepest point) (m) */
  int    lake_idx;                /* index number of the lake/wetland veg tile */
} lake_con_struct;

/*****************************************************************
  This structure stores the lake/wetland variables for a grid cell
  *****************************************************************/
typedef struct {
  // Current lake dimensions and liquid water state variables
  int    activenod;               /* Number of nodes whose corresponding layers currently contain water */
  double dz;                      /* Vertical thickness of all horizontal water layers below the surface layer (m) */
  double surfdz;                  /* Vertical thickness of surface (top) water layer (m) */
  double ldepth;                  /* Current depth of liquid water in lake (distance from surface to deepest point) (m) */
  double surface[MAX_LAKE_NODES+1];/* Area of horizontal cross-section of liquid water in lake at each node (m^2) */
  double sarea;                   /* Current surface area of ice+liquid water on lake surface (m^2) */
  double sarea_save;              /* Surface area of ice+liquid water on lake surface (m^2) at beginning of time step */
  double volume;                  /* Current lake water volume, including liquid water equivalent of lake ice (m^3) */
  double volume_save;             /* Lake water volume, including liquid water equivalent of lake ice (m^3) at beginning of time step */
  double temp[MAX_LAKE_NODES];    /* Lake water temperature at each node (C) */
  double tempavg;                 /* Average liquid water temperature of entire lake (C) */
  // Current properties (state variables) specific to lake ice/snow
  double areai;                   /* Area of ice coverage (at beginning of time step) (m^2) */
  double new_ice_area;            /* Area of ice coverage (at end of time step) (m^2) */
  double ice_water_eq;            /* Liquid water equivalent volume of lake ice (m^3) */
  double hice;                    /* Height of lake ice at thickest point (m) */ 
  double tempi;                   /* Lake ice temperature (C) */
  double swe;                     /* Water equivalence of lake snow cover - end of step (m^3) */
  double swe_save;                /* Water equivalence of lake snow cover - beginning of step (m^3) */
  double surf_temp;               /* Temperature of surface snow layer (C) */
  double pack_temp;               /* Temperature of pack snow layer (C) */
  double coldcontent;             /* cold content of snow pack */
  double surf_water;              /* Water content of surface snow layer (m^3) */
  double pack_water;              /* Water content of pack snow layer (m^3) */
  double SAlbedo;                 /* Albedo of lake snow (fraction) */
  double sdepth;                  /* Depth of snow on top of ice (m^3) */
  // Other current lake properties (derived from state variables and forcings)
  double aero_resist;	          /* Aerodynamic resistance (s/m) after stability correction */
  double density[MAX_LAKE_NODES]; /* Lake water density profile (kg/m^3) */
  // Moisture fluxes
  double baseflow_in;             /* Volume of baseflow into lake from the rest of the grid cell (m3) */
  double baseflow_out;            /* Volume of baseflow out of lake to channel network (m3) */
  double channel_in;              /* Volume of channel inflow into lake (m3) */
  double evapw;                   /* Volume of evaporative flux from lake (and ice/snow) surface (m3) */
  double ice_throughfall;         /* Volume of precipitation reaching lake water surface, i.e. total precip minus accumulation of snow on ice (m3) */
  double prec;                    /* Volume of precipitation falling on lake (and ice/snow) surface (m3) */
  double recharge;                /* Volume of recharge from lake to wetland (m3) */
  double runoff_in;               /* Volume of surface runoff into lake from the rest of the grid cell (m3) */
  double runoff_out;              /* Volume of surface runoff out of lake to channel network (m3) */
  double snowmlt;                 /* Volume of moisture released by melting of lake snow (m3) */
  double vapor_flux;              /* Volume of moisture sublimated from lake snow (m3) */
  // Structures compatible with other land cover types
  // Some of this information is currently redundant with other variables in the lake_var structure
  snow_data_struct  snow;         /* Snow pack on top of lake ice */
  energy_bal_struct energy;       /* Energy fluxes and soil temperatures */
  cell_data_struct  soil;         /* Soil column below lake */
} lake_var_struct;

/*****************************************************************
  This structure joins together data which was accessed in the
  same way (as a 2d array [veg][band]). Since the data is specific
  to a certain veg/band index, it has been joined together in a unified
  structure. For compatibility, vegIndex, and bandIndex are also stored.
*****************************************************************/
struct HRUElement {
  cell_data_struct cell[2]; /* Stores soil layer variables (wet and dry) */
  energy_bal_struct energy; /* Stores energy balance variables */
  snow_data_struct snow; /* Stores snow variables */
  veg_var_struct veg_var[2]; /* Stores vegetation variables (wet and dry) */
  int vegIndex;
  int bandIndex;
};

/*****************************************************************
  This structure stores all variables needed to solve, or save 
  solututions for all versions of this model.  Vegetation and soil
  variables are created for both wet and dry fractions of the grid
  cell (for use with the distributed precipitation model).
*****************************************************************/
struct dist_prcp_struct{
  dist_prcp_struct(int nBands) : NUM_BANDS(nBands) {}
  double             *mu;         /* fraction of grid cell that receives precipitation */
  lake_var_struct     lake_var;   /* Stores lake/wetland variables */
  std::vector<HRUElement> hruElements;

  HRUElement* getHRUElement(int veg, int band) {
    int index = (veg * NUM_BANDS) + band;
    return &hruElements[index];
  }
private:
  int NUM_BANDS;
};

/*******************************************************
  This structure stores moisture state information for
  differencing with next time step.
  *******************************************************/
typedef struct {
  double	total_soil_moist; /* total column soil moisture [mm] */
  double	surfstor;         /* surface water storage [mm] */
  double	swe;              /* snow water equivalent [mm] */
  double	wdew;             /* canopy interception [mm] */
} save_data_struct;

/*******************************************************
  This structure stores output information for one variable.
  *******************************************************/
typedef struct {
  char		varname[20]; /* name of variable */
  int		write;       /* FALSE = don't write; TRUE = write */
  char		format[10];  /* format, when written to an ascii file;
		                should match the desired fprintf format specifier, e.g. %.4f */
  int		type;        /* type, when written to a binary file;
		                OUT_TYPE_USINT  = unsigned short int
		                OUT_TYPE_SINT   = short int
		                OUT_TYPE_FLOAT  = single precision floating point
		                OUT_TYPE_DOUBLE = double precision floating point */
  float		mult;        /* multiplier, when written to a binary file */
  int		aggtype;     /* type of aggregation to use;
				AGG_TYPE_AVG    = take average value over agg interval
				AGG_TYPE_BEG    = take value at beginning of agg interval
				AGG_TYPE_END    = take value at end of agg interval
				AGG_TYPE_MAX    = take maximum value over agg interval
				AGG_TYPE_MIN    = take minimum value over agg interval
				AGG_TYPE_SUM    = take sum over agg interval */
  int		nelem;       /* number of data values */
  double	*data;       /* array of data values */
  double	*aggdata;    /* array of aggregated data values */
} out_data_struct;

/*******************************************************
  This structure stores output information for one output file.
  *******************************************************/
#define OUT_DATA_FILE_STRUCT_PREFIX_LENGTH 20
typedef struct {
  char		prefix[OUT_DATA_FILE_STRUCT_PREFIX_LENGTH];  /* prefix of the file name, e.g. "fluxes" */
  char		filename[MAXSTRING]; /* complete file name */
  FILE		*fh;         /* filehandle */
  int		nvars;       /* number of variables to store in the file */
  int		*varid;      /* id numbers of the variables to store in the file
		                (a variable's id number is its index in the out_data array).
		                The order of the id numbers in the varid array
		                is the order in which the variables will be written. */
} out_data_file_struct;

/********************************************************
  This structure holds all variables needed for the error
  handling routines.
  ********************************************************/
typedef struct {
  atmos_data_struct *atmos;
  double             dt;
  energy_bal_struct *energy;
  filep_struct       filep;
  int                rec;
  out_data_struct   *out_data;
  out_data_file_struct    *out_data_files;
  snow_data_struct  *snow;
  soil_con_struct    soil_con;
  veg_con_struct    *veg_con;
  veg_var_struct    *veg_var;
} Error_struct;

/***********************************************************
  This struct stores the per-cell error data which is relevant
  for the water and energy balance functions.
  ***********************************************************/
struct CellBalanceErrors {
  CellBalanceErrors() :
      water_last_storage(0), water_cum_error(0), water_max_error(0),
      energy_cum_error(0), energy_max_error(0) {
  }
  double water_last_storage;
  double water_cum_error;
  double water_max_error;
  double energy_cum_error;
  double energy_max_error;
};

/***********************************************************
  This struct stores the per-cell information about fall backs
  which occured, and is only really used for printing/debugging.
  See put_data.c for its use.
  ***********************************************************/
struct FallBackStats {
  FallBackStats() : step_count(0), Tfoliage_fbcount_total(0),
      Tcanopy_fbcount_total(0), Tsnowsurf_fbcount_total(0),
      Tsurf_fbcount_total(0), Tsoil_fbcount_total(0) {}
  int step_count;
  int Tfoliage_fbcount_total;
  int Tcanopy_fbcount_total;
  int Tsnowsurf_fbcount_total;
  int Tsurf_fbcount_total;
  int Tsoil_fbcount_total;
};

class ProgramState;

struct WriteDebug {
  WriteDebug();
  void write_debug(atmos_data_struct *atmos, soil_con_struct *soil_con,
      cell_data_struct *cell, energy_bal_struct *energy, snow_data_struct *snow,
      veg_var_struct *veg_var, const dmy_struct *dmy, double out_short,
      double precipitation_mu, int Nveg, int veg, int rec, const int gridcell,
      int dist, char NEWCELL, const ProgramState *state);
private:
  bool        FIRST;
  double    **MOIST_ERROR;
  double     *INIT_MOIST;
  double     *ENERGY_ERROR;
  double     *ENERGY_ERROR_CALC;
  double     *INFLOW;
  double     *RUNOFF;
  double     *BASEFLOW;
  double     *EVAP;
  double     *INSHORT;
  double     *OUTSHORT;
  double     *INLONG;
  double     *OUTLONG;
  double     *SENSIBLE;
  double     *LATENT;
  double     *GRND_FLUX;
  double     *ADVECTION;
  double     *DELTA_CC;
  double     *SNOW_FLUX;
  double     *REFREEZEENERGY;
  double     *DELTA_H;
};

/***********************************************************
  This structure stores the per-cell data that must exist
  before or after running the model for that cell; also
  eventually must contain state to be affected by glacier
  dynamics model. Previously "cell_data_struct" in vicNL.c
  ***********************************************************/
typedef struct {
  soil_con_struct  soil_con;
  char            *init_STILL_STORM;
  int             *init_DRY_TIME;
  char             ErrStr[MAXSTRING];
  WriteDebug      writeDebug;
  dist_prcp_struct prcp;
  veg_con_struct  *veg_con;
  lake_con_struct  lake_con;
  save_data_struct save_data;
  atmos_data_struct *atmos;
  CellBalanceErrors cellErrors;
  FallBackStats fallBackStats;
} cell_info_struct;

/********************************************************
  This structure holds all the meta state of the program.
  This is a cleaner alternative to using global variables.
  ********************************************************/
class ProgramState {
public:
  ProgramState() { veg_lib = NULL; }
  global_param_struct global_param;
  veg_lib_struct *veg_lib;
  option_struct options;
#if LINK_DEBUG
  debug_struct debug;
#endif
  Error_struct Error;
  param_set_struct param_set;
  int NR;  /* array index for atmos struct that indicates the model step avarage or sum */
  int NF;  /* array index loop counter limit for atmos struct that indicates the SNOW_STEP values */
  void initialize_global();
  void init_global_param(filenames_struct *, const char* global_file_name);
  void display_current_settings(int, filenames_struct *);
  void open_debug();
};

