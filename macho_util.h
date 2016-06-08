#ifndef MACHO_UTIL_H
#define MACHO_UTIL_H

#include "macho_retriever.h"

char *fname(const char *name, const char *ext);
char *write_to_xar(struct bitcode_t *bitcode) ;

void extract_xar(const char *path, const char *dest, char *files[], int *count);
void write_to_bitcode(struct bitcode_t *bitcode, char *files[], int *count);

#endif
