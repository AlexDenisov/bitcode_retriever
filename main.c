#include <stdio.h>
#include "macho_reader.h"

#include <stdlib.h>

void retrieve_bitcode(FILE *stream, const int offset, const int swap_bytes) {
  struct mach_header *header = load_mach_header(stream, offset, swap_bytes);
  printf("%s\n", get_cpu_type_name(header));
  struct segment_command *segment = load_llvm_segment_command(stream, header, offset, swap_bytes);

  free(segment);
  free(header);
}

void retrieve_bitcode_64(FILE *stream, const int offset, const int swap_bytes) {
  struct mach_header_64 *header = load_mach_header_64(stream, offset, swap_bytes);
  printf("%s\n", get_cpu_type_name_64(header));
  struct segment_command_64 *segment = load_llvm_segment_command_64(stream, header, offset, swap_bytes);

  free(segment);
  free(header);
}

int main(int argc, char *argv[]) {
  const char *filename = argv[1];
  FILE *stream = fopen(filename, "rb");

  uint32_t magic = get_magic(stream, 0);
  int is64 = is_magic_64(magic);
  int swap_bytes = is_should_swap_bytes(magic);

  if (is64) {
    retrieve_bitcode_64(stream, 0, swap_bytes);
  } else {
    retrieve_bitcode(stream, 0, swap_bytes);
  }

  fclose(stream);
  return 0;
}

