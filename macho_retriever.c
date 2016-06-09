#include "macho_reader.h"
#include "macho_retriever.h"

#include <mach-o/fat.h>
#include <mach-o/loader.h>

#include <string.h>

int max_number_of_archives() { return get_cpu_type_count(); }

struct bitcode_archive *make_bitcode(FILE *stream, const char *cpuname, const uint64_t offset, const uint64_t size) {
  struct bitcode_archive *bitcode = malloc(sizeof(struct bitcode_archive));
  bitcode->size = size;

  bitcode->buffer = malloc(sizeof(char) * size);
  fseek(stream, offset, SEEK_SET);
  fread(bitcode->buffer, sizeof(char), size, stream);

  bitcode->cpu = cpuname;
  return bitcode;
}

struct bitcode_archive *extract_bitcode(FILE *stream, const int offset, const int swap_bytes) {
  struct mach_header *header = load_mach_header(stream, offset, swap_bytes);
  const char *cpu_name = get_cpu_type_name(header);
  struct segment_command *segment = load_llvm_segment_command(stream, header, offset, swap_bytes);
  if (!segment) {
    free(header);
    return NULL;
  }

  struct bitcode_archive *bitcode = make_bitcode(stream, cpu_name, offset + segment->fileoff, segment->filesize);
  free(segment);
  free(header);
  return bitcode;
}

struct bitcode_archive *extract_bitcode_64(FILE *stream, const int offset, const int swap_bytes) {
  struct mach_header_64 *header = load_mach_header_64(stream, offset, swap_bytes);
  const char *cpu_name = get_cpu_type_name_64(header);
  struct segment_command_64 *segment = load_llvm_segment_command_64(stream, header, offset, swap_bytes);
  if (!segment) {
    free(header);
    return NULL;
  }

  struct bitcode_archive *bitcode = make_bitcode(stream, cpu_name, offset + segment->fileoff, segment->filesize);
  free(segment);
  free(header);
  return bitcode;
}

struct bitcode_archive *retrieve_bitcode_from_nonfat(FILE *stream, const uint32_t offset) {
  uint32_t magic = get_magic(stream, offset);
  int is64 = is_magic_64(magic);
  int swap_bytes = is_should_swap_bytes(magic);

  if (is64) {
    return extract_bitcode_64(stream, offset, swap_bytes);
  } else {
    return extract_bitcode(stream, offset, swap_bytes);
  }
}

int is_macho(FILE *stream) {
  uint32_t magic = get_magic(stream, 0);
  return is_magic_macho(magic);
}

void retrieve_bitcode(FILE *stream, struct bitcode_archive *bitcodes[], int *count) {
  uint32_t magic = get_magic(stream, 0);
  if (is_fat(magic)) {
    int swap_bytes = is_should_swap_bytes(magic);
    struct fat_header *header = load_fat_header(stream, swap_bytes);
    *count = header->nfat_arch;
    for (int i = 0; i < *count; i++) {
      uint32_t offset = offset_for_arch(stream, i, swap_bytes);
      bitcodes[i] = retrieve_bitcode_from_nonfat(stream, offset);
    }
    free(header);
  } else {
    bitcodes[0] = retrieve_bitcode_from_nonfat(stream, 0);
    *count = 1;
  }
}
