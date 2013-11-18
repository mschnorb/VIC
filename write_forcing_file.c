#include <stdio.h>
#include <stdlib.h>
#include "vicNl.h"

static char vcid[] = "$Id$";

void write_forcing_file(cell_info_struct *cell,
			int                nrecs,
			WriteOutputFormat *outputFormat,
			out_data_struct   *out_data,
			const ProgramState* state)
/**********************************************************************
  write_forcing_file          Keith Cherkauer           July 19, 2000

  This routine writes the complete forcing data files for use in 
  future simulations.

  Modifications:
  xx-xx-01 Modified to output pressures, which are handled internally
           in kPa, as Pa for backward compatability.			KAC
  2005-Mar-24 Added support for ALMA variables.				TJB
  2006-08-23 Changed order of fread/fwrite statements from ...1, sizeof...
             to ...sizeof, 1,...					GCT
  2006-Sep-23 Implemented flexible output configuration; uses the new
              out_data and out_data_files structures.			TJB
  2006-Nov-30 Convert pressure and vapor pressure to kPa for output.	TJB
  2008-Jun-10 Fixed typo in QAIR and REL_HUMID eqns.			TJB
  2009-Feb-22 Added OUT_VPD.						TJB
  2011-Nov-04 Added OUT_TSKC.						TJB

**********************************************************************/
{
  int                 rec = 0, i = 0, j = 0, v = 0;
  short int          *tmp_siptr = NULL;
  unsigned short int *tmp_usiptr = NULL;
  dmy_struct         *dummy_dmy = NULL;
  int                 dummy_dt = 0;
  int                 dt_sec = 0;

  dt_sec = state->global_param.dt*SECPHOUR;

  for ( rec = 0; rec < nrecs; rec++ ) {
    for ( j = 0; j < state->NF; j++ ) {

      out_data[OUT_AIR_TEMP].data[0]  = cell->atmos[rec].air_temp[j];
      out_data[OUT_DENSITY].data[0]   = cell->atmos[rec].density[j];
      out_data[OUT_LONGWAVE].data[0]  = cell->atmos[rec].longwave[j];
      out_data[OUT_PREC].data[0]      = cell->atmos[rec].prec[j];
      out_data[OUT_PRESSURE].data[0]  = cell->atmos[rec].pressure[j]/kPa2Pa;
      out_data[OUT_QAIR].data[0]      = EPS * cell->atmos[rec].vp[j]/cell->atmos[rec].pressure[j];
      out_data[OUT_REL_HUMID].data[0] = 100.*cell->atmos[rec].vp[j]/(cell->atmos[rec].vp[j]+cell->atmos[rec].vpd[j]);
      out_data[OUT_SHORTWAVE].data[0] = cell->atmos[rec].shortwave[j];
      out_data[OUT_TSKC].data[0]      = cell->atmos[rec].tskc[j];
      out_data[OUT_VP].data[0]        = cell->atmos[rec].vp[j]/kPa2Pa;
      out_data[OUT_VPD].data[0]       = cell->atmos[rec].vpd[j]/kPa2Pa;
      out_data[OUT_WIND].data[0]      = cell->atmos[rec].wind[j];
      if (out_data[OUT_AIR_TEMP].data[0] >= cell->soil_con.MAX_SNOW_TEMP) {
        out_data[OUT_RAINF].data[0] = out_data[OUT_PREC].data[0];
        out_data[OUT_SNOWF].data[0] = 0;
      }
      else if (out_data[OUT_AIR_TEMP].data[0] <= cell->soil_con.MIN_RAIN_TEMP) {
        out_data[OUT_RAINF].data[0] = 0;
        out_data[OUT_SNOWF].data[0] = out_data[OUT_PREC].data[0];
      }
      else {
        out_data[OUT_RAINF].data[0] = ((out_data[OUT_AIR_TEMP].data[0]-cell->soil_con.MIN_RAIN_TEMP)/(cell->soil_con.MAX_SNOW_TEMP-cell->soil_con.MIN_RAIN_TEMP))*out_data[OUT_PREC].data[0];
        out_data[OUT_SNOWF].data[0] = out_data[OUT_PREC].data[0]-out_data[OUT_RAINF].data[0];
      }

      for (v=0; v<N_OUTVAR_TYPES; v++) {
        for (i=0; i<out_data[v].nelem; i++) {
          out_data[v].aggdata[i] = out_data[v].data[i];
        }
      }

      if (state->options.ALMA_OUTPUT) {
        out_data[OUT_PREC].aggdata[0] /= dt_sec;
        out_data[OUT_RAINF].aggdata[0] /= dt_sec;
        out_data[OUT_SNOWF].aggdata[0] /= dt_sec;
        out_data[OUT_AIR_TEMP].aggdata[0] += KELVIN;
        out_data[OUT_PRESSURE].aggdata[0] *= 1000;
        out_data[OUT_VP].aggdata[0] *= 1000;
        out_data[OUT_VPD].aggdata[0] *= 1000;
      }

      outputFormat->write_data(out_data, dummy_dmy, dummy_dt, state);
    }
  }

}
