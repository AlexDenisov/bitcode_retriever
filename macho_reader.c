#include "macho_reader.h"

#include <stdlib.h>
#include <string.h>

#include <mach-o/loader.h>
#include <mach-o/swap.h>
#include <mach/machine.h>

static int uint32_size = sizeof(uint32_t);

static int mach_header_size = sizeof(struct mach_header);
static int mach_header_64_size = sizeof(struct mach_header_64);

static int load_command_size = sizeof(struct load_command);

static int segment_command_size = sizeof(struct segment_command);
static int segment_command_64_size = sizeof(struct segment_command_64);

struct _cpu_type_names {
  cpu_type_t cputype;
  const char *cpu_name;
};

static struct _cpu_type_names cpu_type_names[] = {
  { CPU_TYPE_I386, "i386" },
  { CPU_TYPE_X86_64, "x86_64" },
  { CPU_TYPE_ARM, "arm" },
  { CPU_TYPE_ARM64, "arm64" }
};

static const char *cpu_type_name(cpu_type_t cpu_type) {
  static int cpu_type_names_size = sizeof(cpu_type_names) / sizeof(struct _cpu_type_names);
  for (int i = 0; i < cpu_type_names_size; i++ ) {
    if (cpu_type == cpu_type_names[i].cputype) {
      return cpu_type_names[i].cpu_name;
    }
  }

  return "unknown";
}

const char *get_cpu_type_name(struct mach_header *header) {
  return cpu_type_name(header->cputype);
}

const char *get_cpu_type_name_64(struct mach_header_64 *header) {
  return cpu_type_name(header->cputype);
}

uint32_t get_magic(FILE *stream, int offset) {
  uint32_t magic = 0;
  fseek(stream, offset, SEEK_SET);
  fread(&magic, uint32_size, 1, stream);
  rewind(stream);
  return magic;
}

int is_magic_64(uint32_t magic) {
  return magic == MH_MAGIC_64 || magic == MH_CIGAM_64;
}

int is_should_swap_bytes(uint32_t magic) {
  return magic == MH_CIGAM || magic == MH_CIGAM_64;
}

struct mach_header *load_mach_header(FILE *stream, const int offset, const int swap_bytes) {
  struct mach_header *header = calloc(mach_header_size, 1);
  fseek(stream, offset, SEEK_SET);
  fread(header, mach_header_size, 1, stream);
  rewind(stream);

  if (swap_bytes) {
    swap_mach_header(header, 0);
  }

  return header;
}

struct mach_header_64 *load_mach_header_64(FILE *stream, const int offset, const int swap_bytes) {
  struct mach_header_64 *header = calloc(mach_header_64_size, 1);
  fseek(stream, offset, SEEK_SET);
  fread(header, mach_header_64_size, 1, stream);
  rewind(stream);

  if (swap_bytes) {
    swap_mach_header_64(header, 0);
  }

  return header;
}

struct load_command *load_load_command(FILE *stream, const int offset, const int swap_bytes) {
  struct load_command *command = calloc(load_command_size, 1);
  fseek(stream, offset, SEEK_SET);
  fread(command, load_command_size, 1, stream);
  rewind(stream);

  if (swap_bytes) {
    swap_load_command(command, 0);
  }

  return command;
}

struct segment_command *load_segment_command(FILE *stream, const int offset, const int swap_bytes) {
  struct segment_command *command = calloc(segment_command_size, 1);
  fseek(stream, offset, SEEK_SET);
  fread(command, segment_command_size, 1, stream);
  rewind(stream);

  if (swap_bytes) {
    swap_segment_command(command, 0);
  }

  return command;
}

struct segment_command_64 *load_segment_command_64(FILE *stream, const int offset, const int swap_bytes) {
  struct segment_command_64 *command = calloc(segment_command_64_size, 1);
  fseek(stream, offset, SEEK_SET);
  fread(command, segment_command_64_size, 1, stream);
  rewind(stream);

  if (swap_bytes) {
    swap_segment_command_64(command, 0);
  }

  return command;
}

struct segment_command *load_llvm_segment_command(FILE *stream, struct mach_header *header, const int offset, const int swap_bytes) {
  int cmd_offset = offset + mach_header_size;
  for (int i = 0; i < header->ncmds; i++) {
    struct load_command *cmd = load_load_command(stream, cmd_offset, swap_bytes);
    if (cmd->cmd == LC_SEGMENT) {
      struct segment_command *segment = load_segment_command(stream, cmd_offset, swap_bytes);
      if (!strncmp("__LLVM", segment->segname, 7)) {
        return segment;
      }
      free(segment);
    }
    cmd_offset += cmd->cmdsize;
    free(cmd);
  }

  return 0;
}

struct segment_command_64 *load_llvm_segment_command_64(FILE *stream, struct mach_header_64 *header, const int offset, const int swap_bytes) {

  int cmd_offset = offset + mach_header_64_size;
  for (int i = 0; i < header->ncmds; i++) {
    struct load_command *cmd = load_load_command(stream, cmd_offset, swap_bytes);
    if (cmd->cmd == LC_SEGMENT_64) {
      struct segment_command_64 *segment = load_segment_command_64(stream, cmd_offset, swap_bytes);
      if (!strncmp("__LLVM", segment->segname, 7)) {
        return segment;
      }
      free(segment);
    }
    cmd_offset += cmd->cmdsize;
    free(cmd);
  }

  return 0;
}

