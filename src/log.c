#ifndef BIN_OPT_LOADER_FUNCTIONS
#define BIN_OPT_LOADER_FUNCTIONS

#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void binOptLogger32(int32_t val, int32_t arg0, int32_t arg1, char* opName, char* funcName, uint64_t valID){
  FILE* logfile = fopen("log.txt", "a");
  fprintf(logfile, "%ld %d = %d %s %d\n", valID, val, arg0, opName, arg1);
  fclose(logfile);
}

void binOptLogger64(int32_t val, int32_t arg0, int32_t arg1, char* opName, char* funcName, uint64_t valID){
  FILE* logfile = fopen("log.txt", "a");
  fprintf(logfile, "%ld %d = %d %s %d\n", valID, val, arg0, opName, arg1);
  fclose(logfile);
}

#endif
