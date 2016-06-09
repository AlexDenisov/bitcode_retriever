#ifndef MACHO_UTIL_H
#define MACHO_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include "macho_retriever.h"

char *fname(const char *name, const char *ext);
char *write_to_xar(struct bitcode_archive *bitcode);

int extract_xar(const char *path, const char *cpu, char *files[], int *count);
int write_to_bitcode(struct bitcode_archive *bitcode, char *files[], int *count);

#ifdef __cplusplus
}
#endif
#endif
