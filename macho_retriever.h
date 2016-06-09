#ifndef MACHO_RETRIEVER_H
#define MACHO_RETRIEVER_H

#include <stdio.h>
#include <stdlib.h>

struct bitcode_archive {
  uint64_t size;
  char* buffer;
  char* cpu;
};

struct bitcode_archive* make_bitcode(FILE* stream, const char* cpuname, const uint64_t offset, const uint64_t size);
struct bitcode_archive* extract_bitcode(FILE* stream, const int offset, const int swap_bytes);
struct bitcode_archive* extract_bitcode_64(FILE* stream, const int offset, const int swap_bytes);

struct bitcode_archive* retrieve_bitcode_from_nonfat(FILE* stream, const uint32_t offset);

int max_number_of_archives();
void retrieve_bitcode(FILE* stream, struct bitcode_archive* bitcodes[], int* count);

#endif
