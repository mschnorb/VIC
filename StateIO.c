#include "StateIO.h"

StateIO::StateIO(std::string filename, IOType type, const ProgramState* state) : filename(filename), state(state), ioType(type) {
}

StateIO::~StateIO() {
}

int StateIO::processNewline() {
  return 0;
}

int StateIO::process(int* data, int numValues, const StateVariableMetaData* meta) {
  if (ioType == StateIO::Reader) {
    return read(data, numValues, meta);
  } else {
    return write(data, numValues, meta);
  }
}

int StateIO::process(double* data, int numValues, const StateVariableMetaData* meta) {
  if (ioType == StateIO::Reader) {
    return read(data, numValues, meta);
  } else {
    return write(data, numValues, meta);
  }
}

int StateIO::process(char* data, int numValues, const StateVariableMetaData* meta) {
  if (ioType == StateIO::Reader) {
    return read(data, numValues, meta);
  } else {
    return write(data, numValues, meta);
  }
}

inline int StateIO::process(const int* data, int numValues, const StateVariableMetaData* meta) {
  if (ioType == StateIO::Reader) {
    throw new VICException("Error in StateIO::process. Can't read into a const array!\n");
    //return read(data, numValues, meta);
  } else {
    return write(data, numValues, meta);
  }
}

inline int StateIO::process(const double* data, int numValues, const StateVariableMetaData* meta) {
  if (ioType == StateIO::Reader) {
    throw new VICException("Error in StateIO::process. Can't read into a const array!\n");
    //return read(data, numValues, meta);
  } else {
    return write(data, numValues, meta);
  }
}

inline int StateIO::process(const char* data, int numValues, const StateVariableMetaData* meta) {
  if (ioType == StateIO::Reader) {
    throw new VICException("Error in StateIO::process. Can't read into a const array!\n");
    //return read(data, numValues, meta);
  } else {
    return write(data, numValues, meta);
  }
}


