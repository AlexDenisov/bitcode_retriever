#include "macho_reader.h"
#include "macho_retriever.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

FILE *create_xar_file(const char *cpuname) {
  const char *ext = ".xar";
  int length = strlen(cpuname) + strlen(ext);
  char *filename = calloc(sizeof(char), length);
  strcpy(filename, cpuname);
  strcat(filename, ext);

  FILE *output = fopen(filename, "wb");
  free(filename);

  if (!output) {
    printf("Cannot open '%s' for writing\n", filename);
    exit(1);
  }

  return output;
}

void write_to_xar(struct bitcode_t *bitcode) {
  FILE *output = create_xar_file(bitcode->arch);
  fwrite(bitcode->buffer, sizeof(char), bitcode->size, output);
  fclose(output);
}

int main(int argc, char *argv[]) {
  const char *filename = argv[1];
  FILE *stream = fopen(filename, "rb");

  struct bitcode_t *bitcodes[4];
  int count;
  retrieve_bitcode(stream, bitcodes, &count);

  for (int i = 0; i < count; i++) {
    if (bitcodes[i]) {
      write_to_xar(bitcodes[i]);
    }
  }

  fclose(stream);
  return 0;
}
