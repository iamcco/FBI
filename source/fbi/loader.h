// reference:
// https://github.com/devkitPro/3ds-hbmenu/blob/master/source/loaders/rosalina.c

#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENTRY_ARGBUFSIZE 0x400

typedef struct {
  char *dst;
  u32 buf[ENTRY_ARGBUFSIZE / sizeof(u32)];
} argData_s;

Result loader_launch_file(const char *path, const char *input_args);
