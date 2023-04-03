#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

uint64_t fact(uint64_t arg) {
  uint64_t res = 0;
  if (arg < 2) {
    res = 1;
  } else {
    uint64_t next = fact(arg - 1);
    res = arg * next;
  }
  return res;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    printf("Usage: 1 argument - factorial len\n");
    return 1;
  }
  uint64_t arg = atoi(argv[1]);
  if (errno == 0) {
    printf("Fact(%lu) = %lu\n", arg, fact(arg));
  } else {
    printf("Usage: 1 argument - factorial len\n");
    return 1;
  }
  return 0;
}

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
