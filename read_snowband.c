#include <stdio.h>
#include <stdlib.h>
#include "vicNl.h"
#include <string.h>

static char vcid[] = "$Id$";

void read_snowband(FILE    *snowband,
		   soil_con_struct *soil_con,
		   const int num_elevation_snow_bands)
/**********************************************************************
  read_snowband		Keith Cherkauer		July 9, 1998

  This routine reads snow band median elevation, and
  precipitation fraction for use with the snow model.

  04-25-03 Modified to allocate treeline variable.            KAC
  2007-Sep-18 Modified to output a warning and set snow bands to 1
	      if no snowband information is present in the snowband
	      file for the current grid cell.				KAC via TJB
  2009-Jan-12 Changed wording of error messages from "snow elevation band"
	      to "snow band".						TJB
  2009-Jul-07 Added code to store band elevations in soil_con.BandElev[].
	      Added logic to make sure grid cell average elevation equals
	      the average of the band elevations.			TJB
  2009-Sep-28 Moved initialization of snow band parameters to the
	      read_soilparam* functions.				TJB
**********************************************************************/
{
  char    ErrStr[MAXSTRING];
  int     band;
  int     cell;
  double  total;
  double  area_fract;
  float   band_elev;
  float   avg_elev;

  if ( num_elevation_snow_bands > 1 ) {

    /** Find Current Grid Cell in SnowBand File **/
#if !NO_REWIND
    rewind(snowband);
#endif

    fscanf(snowband, "%d", &cell);
    while ( cell != soil_con->gridcel && !feof(snowband) ) {
      fgets(ErrStr,MAXSTRING,snowband);
      fscanf(snowband, "%d", &cell);
    }

    if ( feof(snowband) ) {
      fprintf(stderr, "WARNING: Cannot find current gridcell (%i) in snow band file; setting cell to have one elevation band.\n",
              soil_con->gridcel);
      /** 1 band is the default; no action necessary **/
      return;
    }

    /** Read Area Fraction **/
    total = 0.;
    for( band = 0; band < num_elevation_snow_bands; band++ ) {
      fscanf(snowband, "%lf", &area_fract);
      if(area_fract < 0) {
      	sprintf(ErrStr,"Negative snow band area fraction (%f) read from file", area_fract);
      	nrerror(ErrStr);
      }
      soil_con->AreaFract[band]  = area_fract;
      total              += area_fract;
    }
    if ( total != 1. ) {
      fprintf(stderr,"WARNING: Sum of the snow band area fractions (%f) does not equal 1; dividing each fraction by the sum\n",
	      total);
      for ( band = 0; band < num_elevation_snow_bands; band++ )
      	soil_con->AreaFract[band] /= total;
    }

    /** Read Band Elevation **/
    avg_elev = 0;
    for ( band = 0; band < num_elevation_snow_bands; band++ ) {
      fscanf(snowband, "%f", &band_elev);
      if ( band_elev < 0 ) {
      	fprintf(stderr,"Negative snow band elevation (%f) read from file\n", band_elev);
      }
      soil_con->BandElev[band] = band_elev;
      avg_elev += soil_con->BandElev[band]*soil_con->AreaFract[band];
    }
    if (fabs(avg_elev-soil_con->elevation) > 1.0) {
      fprintf(stderr,"WARNING: average band elevation %f not equal to grid_cell average elevation %f; setting grid cell elevation to average band elevation.\n", avg_elev, soil_con->elevation);
      soil_con->elevation = (float)avg_elev;
    }
    for ( band = 0; band < num_elevation_snow_bands; band++ ) {
      soil_con->Tfactor[band] = ( soil_con->elevation - soil_con->BandElev[band] ) / 1000. * soil_con->T_LAPSE;
    }

   /** Calculate Precipitation Fraction **/
   total = 0.;
    for ( band = 0; band < num_elevation_snow_bands; band++ ) {
      soil_con->Pfactor[band] = (1.0 + soil_con->PGRAD * (soil_con->BandElev[band] - soil_con->elevation)) * soil_con->AreaFract[band];
      if (soil_con->Pfactor[band] < 0) {
        sprintf(ErrStr, "Snow band precipitation factor (%f) must be between 0 and 1", soil_con->Pfactor[band]);
      nrerror(ErrStr);
     }
     total += soil_con->Pfactor[band];
   }
    if ( total != 1. ) {
      fprintf(stderr,"WARNING: Sum of the snow band precipitation fractions (%f) does not equal 1; dividing each fraction by the sum\n",
	      total);
      for(band = 0; band < num_elevation_snow_bands; band++)
	    soil_con->Pfactor[band] /= total;
   }
    for ( band = 0; band < num_elevation_snow_bands; band++ ) {
      if (soil_con->AreaFract[band] > 0)
	    soil_con->Pfactor[band] /= soil_con->AreaFract[band];
      else 
	    soil_con->Pfactor[band]  = 0.;
   }

  }

} 
