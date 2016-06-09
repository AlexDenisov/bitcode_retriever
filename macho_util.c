#include "macho_reader.h"
#include "macho_retriever.h"
#include "macho_util.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xar/xar.h>

char *fname(const char *name, const char *ext) {
  int length = strlen(name) + 1 + strlen(ext);
  char *filename = calloc(sizeof(char), length);
  strcpy(filename, name);
  strcat(filename, ".");
  strcat(filename, ext);
  return filename;
}

char *write_to_xar(struct bitcode_archive *bitcode) {
  char *filename = fname(bitcode->cpu, "xar");
  FILE *output = fopen(filename, "wb");

  if (!output) {
    fprintf(stderr, "Cannot open '%s' for writing\n", filename);
    free(filename);
    return NULL;
  }

  fwrite(bitcode->buffer, sizeof(char), bitcode->size, output);
  fclose(output);

  return filename;
}

int extract_xar(const char *path, const char *cpu, char *files[], int *count) {
  xar_t x;
  xar_iter_t xi;
  xar_file_t xf;
  xar_stream xs;
  char buffer[8192];

  x = xar_open(path, READ);
  if (!x) {
    fprintf(stderr, "Error opening archive\n");
    return 1;
  }

  xi = xar_iter_new();
  if (!xi) {
    fprintf(stderr, "Error creating xar iterator\n");
    return 2;
  }

  *count = 0;
  for (xf = xar_file_first(x, xi); xf; xf = xar_file_next(xi)) {
    char *path = xar_get_path(xf);
    const char *type;
    xar_prop_get(xf, "type", &type);
    if (!type) {
      fprintf(stderr, "File has no type %s\n", path);
      free(path);
      continue;
    }
    if (strcmp(type, "file") != 0) {
      fprintf(stderr, "Skipping %s\n", path);
      free(path);
      continue;
    }

    if (xar_extract_tostream_init(x, xf, &xs) != XAR_STREAM_OK) {
      fprintf(stderr, "Error initializing stream %s\n", path);
      free(path);
      continue;
    }

    char *prefix = fname(cpu, path);
    char *output_path = fname(prefix, "bc");
    free(path);
    free(prefix);

    FILE *output = fopen(output_path, "wb");
    if (!output) {
      fprintf(stderr, "Error opening output file %s\n", output_path);
      free(output_path);
      continue;
    }

    xs.avail_out = sizeof(buffer);
    xs.next_out = buffer;

    int32_t ret;
    while ((ret = xar_extract_tostream(&xs)) != XAR_STREAM_END) {
      if (ret == XAR_STREAM_ERR) {
        fprintf(stderr, "Error extracting stream %s\n", output_path);
        free(output_path);
        return 3;
      }

      fwrite(buffer, sizeof(char), sizeof(buffer) - xs.avail_out, output);

      xs.avail_out = sizeof(buffer);
      xs.next_out = buffer;
    }

    if (xar_extract_tostream_end(&xs) != XAR_STREAM_OK) {
      fprintf(stderr, "Error ending stream %s\n", output_path);
    }

    fclose(output);
    files[(*count)++] = output_path;
  }
  return 0;
}

int write_to_bitcode(struct bitcode_archive *bitcode, char *files[], int *count) {
  char *xar_file = write_to_xar(bitcode);
  int extracted = extract_xar(xar_file, bitcode->cpu, files, count);
  if (extracted != 0) {
    fprintf(stderr, "Error extracting xar file %s\n", xar_file);
    free(xar_file);
    return 1;
  }

  int removed = remove(xar_file);
  if (removed != 0) {
    fprintf(stderr, "Error removing xar file %s\n", xar_file);
    free(xar_file);
    return 2;
  }

  free(xar_file);
  return 0;
}
