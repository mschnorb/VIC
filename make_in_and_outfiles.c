#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vicNl.h>
#include <netcdf.h>

static char vcid[] = "$Id$";

/********************************
  Input Forcing Files
********************************/
void make_in_files(filep_struct         *filep,
			  filenames_struct     *filenames,
			  soil_con_struct      *soil,
			  const ProgramState   *state)
/**********************************************************************
	make_in_and_outfile	Dag Lohman	January 1996

  This program builds the files names for input and output of grided
  data files.

  Modifications:
  5/20/96	The routine was modified to accept a variable
		number of layers, as well as to work with 
		frozen soils					KAC
  11-18-02 Modified to print notification that the output fluxes file
           will be in a binary format.                          LCB
  29-Oct-03 Distinguishing between input lakeparam file and output
	    lake file.							TJB
  2005-Mar-24 Modified to handle ALMA output files.			TJB
  2006-Sep-23 Implemented flexible output configuration; uses the new
              out_data and out_data_files structures; removed the
              OPTIMIZE and LDAS_OUTPUT options.				TJB
  2006-Oct-16 Merged infiles and outfiles structs into filep_struct;
	      Merged builtnames into filenames->			TJB
  2007-Oct-31 Append "/" to result_dir so that this need not be done
	      in global parameter file.					TJB
  2011-May-25 Expanded latchar, lngchar, and junk allocations to handle
	      GRID_DECIMAL > 4.						TJB

**********************************************************************/
{
  char   latchar[20], lngchar[20], junk[6];

  sprintf(junk, "%%.%if", state->options.GRID_DECIMAL);
  sprintf(latchar, junk, soil->lat);
  sprintf(lngchar, junk, soil->lng);
 


  strcpy(filenames->forcing[0], filenames->f_path_pfx[0]);
  /* Append lat/lon for non-NetCDF files */
  if(state->param_set.FORCE_FORMAT[0] != NETCDF) {
    strcat(filenames->forcing[0], latchar);
    strcat(filenames->forcing[0], "_");
    strcat(filenames->forcing[0], lngchar);
  }

  filep->forcing[0] = NULL;
  if (state->param_set.FORCE_FORMAT[0] == NETCDF)
    assert(nc_open(filenames->forcing[0], NC_NOWRITE, &filep->forcing_ncid[0]) == NC_NOERR); /* TODO proper error handling */
  else if(state->param_set.FORCE_FORMAT[0] == BINARY)
    filep->forcing[0] = open_file(filenames->forcing[0], "rb");
  else
    filep->forcing[0] = open_file(filenames->forcing[0], "r");

  filep->forcing[1] = NULL;
  if(strcasecmp(filenames->f_path_pfx[1],"MISSING")!=0) {
    strcpy(filenames->forcing[1], filenames->f_path_pfx[1]);
    if(state->param_set.FORCE_FORMAT[1] != NETCDF) {
      strcat(filenames->forcing[1], latchar);
      strcat(filenames->forcing[1], "_");
      strcat(filenames->forcing[1], lngchar);
    }
    if(state->param_set.FORCE_FORMAT[1] == NETCDF)
      assert(nc_open(filenames->forcing[1], NC_NOWRITE, &filep->forcing_ncid[1]) == NC_NOERR); /* TODO proper error handling */
    else if(state->param_set.FORCE_FORMAT[1] == BINARY) /* MPN: Changed this to [1]; It's used elsewhere so I presume it's actually set. */
      filep->forcing[1] = open_file(filenames->forcing[1], "rb");
    else 
      filep->forcing[1] = open_file(filenames->forcing[1], "r");
  }

}

/********************************
Output Files
********************************/
void make_out_files(filep_struct         *filep,
    filenames_struct     *filenames,
    soil_con_struct      *soil,
    out_data_file_struct *out_data_files,
    const ProgramState   *state) {

  char   latchar[20], lngchar[20], junk[6];

  sprintf(junk, "%%.%if", state->options.GRID_DECIMAL);
  sprintf(latchar, junk, soil->lat);
  sprintf(lngchar, junk, soil->lng);

  WriteOutputContext context(state->options.OUTPUT_FORMAT);

  for (int filenum=0; filenum < state->options.Noutfiles; filenum++) {
    strcpy(out_data_files[filenum].filename, filenames->result_dir);
    strcat(out_data_files[filenum].filename, "/");
    strcat(out_data_files[filenum].filename, out_data_files[filenum].prefix);
    strcat(out_data_files[filenum].filename, "_");
    strcat(out_data_files[filenum].filename, latchar);
    strcat(out_data_files[filenum].filename, "_");
    strcat(out_data_files[filenum].filename, lngchar);

    out_data_files[filenum].fh = context.outputFormat->openFile(out_data_files[filenum].filename);
  }
}
