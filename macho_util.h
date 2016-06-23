#ifndef MACHO_UTIL_H
#define MACHO_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libxml/tree.h>
#include "macho_retriever.h"

char *fname(const char *name, const char *ext);
char *write_to_xar(struct bitcode_archive *bitcode);

int extract_xar(const char *path, const char *cpu, char *files[], int *count);
int write_to_bitcode(struct bitcode_archive *bitcode, char *files[], int *count);

int get_options(xmlNode *option_parent, char *options[], int *size);
int get_linker_options(xmlNode *a_node, char *options[], int *size);
int retrieve_toc(const char *xar_path, const char *toc_path);
int retrieve_linker_options(const char *xar_path, char *options[], int *size);

#ifdef __cplusplus
}
#endif
#endif
