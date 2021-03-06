#ifndef WRITEOUTPUTBINARY_H_
#define WRITEOUTPUTBINARY_H_

#include "WriteOutputFormat.h"

class WriteOutputBinary: public WriteOutputFormat {
public:
  WriteOutputBinary(const ProgramState* state) : WriteOutputFormat(state) {}
  const char* getDescriptionOfOutputType();
  void openFile();
  void compressFiles();
  void write_data(OutputData *out_data, const dmy_struct *dmy, int dt, const ProgramState* state);
//  void write_data(OutputData *out_data, const dmy_struct *dmy, int dt, ProgramState* state);
  void write_header(OutputData *out_data, const dmy_struct *dmy, const ProgramState* state);

private:
  void prepareDataForWriting(OutputData* out_data);
};

#endif /* WRITEOUTPUTBINARY_H_ */
