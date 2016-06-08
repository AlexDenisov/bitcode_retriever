#ifndef MACHO_RETRIEVER_H
#define MACHO_RETRIEVER_H

#include <stdio.h>
#include <stdlib.h>

struct bitcode_t {
  uint64_t size;
  char* buffer;
  char* arch;
};

struct bitcode_t* make_bitcode(FILE* stream, const char* cpuname, const uint64_t offset, const uint64_t size);
struct bitcode_t* extract_bitcode(FILE* stream, const int offset, const int swap_bytes);
struct bitcode_t* extract_bitcode_64(FILE* stream, const int offset, const int swap_bytes);

struct bitcode_t* retrieve_bitcode_from_nonfat(FILE* stream, const uint32_t offset);

void retrieve_bitcode(FILE* stream, struct bitcode_t* bitcodes[], int* count);

#endif
