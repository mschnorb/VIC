#include <stdio.h>
#include <stdlib.h>
#include "vicNl.h"
#include <math.h>

static char vcid[] = "$Id$";

int  redistribute_during_storm(HRU& hru,
			       int                 rec,
			       double              Wdmax,
			       double              new_mu,
			       double             *max_moist,
			       const ProgramState* state) {
/**********************************************************************
  redistribute_during_storm.c     Keith Cherkauer     January 13, 1998

  This subroutine redistributes soil moisture when the current storm 
  changes intensity.

  Modified:
  06-24-98 Changed to run on only one vegetation type at a time    KAC
  07-13-98 modified to redistribute Wdew within all defined
           elevation bands                                         KAC
  08-19-99 simplified logic, and added check to make sure soil
           moisture does not exceed maximum soil moisture content  Bart
  6-8-2000 modified to work with spatially distribute frozen soils KAC
  4-6-2007 modified to return errors to calling routine rather than
           ending program.                                         KAC

**********************************************************************/
 
  double old_mu = hru.mu;
  /** Redistribute Soil Moisture **/

  for (int layer = 0; layer < state->options.Nlayer; layer++) {

    hru_data_struct& cellWet = hru.cell[WET];
    hru_data_struct& cellDry = hru.cell[DRY];

    double temp_wet = cellWet.layer[layer].moist;
    double temp_dry = cellDry.layer[layer].moist;
    unsigned char error = redistribute_moisture_for_storm(&temp_wet, &temp_dry,
        max_moist[layer], old_mu, new_mu);
    if (error) {
      fprintf(stderr, "%s: Error in moist accounting %f -> %f record %i\n",
          __FILE__,
          cellWet.layer[layer].moist * old_mu
              + cellDry.layer[layer].moist * (1. - old_mu),
          temp_wet * new_mu + temp_dry * (1. - new_mu), rec);
      return (ERROR);
    }
    cellWet.layer[layer].moist = temp_wet;
    cellDry.layer[layer].moist = temp_dry;

#if SPATIAL_FROST
    for (int frost_area = 0; frost_area < FROST_SUBAREAS; frost_area++ ) {
      temp_wet = cellWet.layer[layer].soil_ice[frost_area];
      temp_dry = cellDry.layer[layer].soil_ice[frost_area];
#else
    temp_wet = cellWet.layer[layer].soil_ice;
    temp_dry = cellDry.layer[layer].soil_ice;
#endif
    error = redistribute_moisture_for_storm(&temp_wet, &temp_dry,
        max_moist[layer], old_mu, new_mu);
    if (error) {
#if SPATIAL_FROST
      fprintf(stderr,"%s: Error in ice accounting %f -> %f record %i\n",
          __FILE__,cellWet.layer[layer].soil_ice[frost_area]
          *old_mu + cellDry.layer[layer].soil_ice[frost_area]
          *(1.-old_mu), temp_wet*new_mu+temp_dry*(1.-new_mu),rec);
#else
      fprintf(stderr, "%s: Error in ice accounting %f -> %f record %i\n",
          __FILE__,
          cellWet.layer[layer].soil_ice * old_mu
              + cellDry.layer[layer].soil_ice * (1. - old_mu),
          temp_wet * new_mu + temp_dry * (1. - new_mu), rec);
#endif
      return (ERROR);
    }
#if SPATIAL_FROST
    cellWet.layer[layer].soil_ice[frost_area] = temp_wet;
    cellDry.layer[layer].soil_ice[frost_area] = temp_dry;
  }
#else
    cellWet.layer[layer].soil_ice = temp_wet;
    cellDry.layer[layer].soil_ice = temp_dry;
#endif
  }

  /****************************************
   Redistribute Stored Water in Vegetation
   ****************************************/
  if (hru.isArtificialBareSoil == false) {
    double temp_wet = hru.veg_var[WET].Wdew;
    double temp_dry = hru.veg_var[DRY].Wdew;
    unsigned char error = redistribute_moisture_for_storm(&temp_wet, &temp_dry,
        Wdmax, old_mu, new_mu);
    if (error) {
      fprintf(stderr, "%s: Error in Wdew accounting %f -> %f record %i\n",
          __FILE__,
          hru.veg_var[WET].Wdew * old_mu
              + hru.veg_var[DRY].Wdew * (1. - old_mu),
          temp_wet * new_mu + temp_dry * (1. - new_mu), rec);
      return (ERROR);
    }
    hru.veg_var[WET].Wdew = temp_wet;
    hru.veg_var[DRY].Wdew = temp_dry;
  }

  return(0);
}

unsigned char redistribute_moisture_for_storm(double *wet_value,
					      double *dry_value,
					      double  max_moist,
					      double  old_mu,
					      double  new_mu) {
/**********************************************************************
  This subroutine redistributes the given parameter between wet and
  dry cell fractions when the precipitation changes in intensity.

  Modified 08-19-99 to add check that maximum soil moisture is not
    exceeded.                                              Bart
**********************************************************************/

  unsigned char error;
  double old_wet;
  double old_dry;
  double diff1 = 0.;
  double diff2 = 0.;
  double diff3 = 0.;

  if (fabs(*wet_value - *dry_value) < SMALL)
    return FALSE;

  old_wet = *wet_value;
  old_dry = *dry_value;

  if (old_mu > new_mu && (1.-new_mu) > SMALL && new_mu > SMALL) {
    *dry_value = (old_mu-new_mu) * old_wet + (1.-old_mu) * old_dry;
    *dry_value /= (1.-new_mu);
  }
  else if ((1.-new_mu) > SMALL && new_mu > SMALL) {
    *wet_value = (new_mu-old_mu) * old_dry + old_mu * old_wet;
    *wet_value /= new_mu;
  }
  else {
    *wet_value = (1.-old_mu) * old_dry + old_mu * old_wet;
    *dry_value = *wet_value;
  }
  diff1 = fabs((old_mu * old_wet + (1.-old_mu) * old_dry) -
    (new_mu * *wet_value + (1.-new_mu) * *dry_value));
  if (*dry_value > max_moist) {
    diff2 = fabs(*dry_value - max_moist);
    *dry_value = max_moist;
  }
  if (*wet_value > max_moist) {
    diff3 = fabs(*wet_value - max_moist);
    *wet_value = max_moist;
  }
  if(diff1 > SMALL || diff2 > SMALL || diff3 > SMALL) 
    error = TRUE;
  else 
    error = FALSE;
  
  return (error);
}
