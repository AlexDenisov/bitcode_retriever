#ifndef MACHO_RETRIEVER_H
#define MACHO_RETRIEVER_H

#include <stdio.h>
#include <stdlib.h>

FILE *create_output_file(const char *cpuname);
void extract_bitcode(FILE *stream, const char *cpuname, const uint32_t offset, const uint32_t size);
void extract_bitcode_64(FILE *stream, const char *cpuname, const uint64_t offset, const uint64_t size);
void retrieve_bitcode(FILE *stream, const int offset, const int swap_bytes);
void retrieve_bitcode_64(FILE *stream, const int offset, const int swap_bytes);
void retrieve_bitcode_from_nonfat(FILE *stream, const uint32_t offset);
void retrieve_bitcode_from_fat(FILE *stream, const int swap_bytes);

#endif
