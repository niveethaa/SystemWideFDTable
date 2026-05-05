#ifndef DISPLAY_FDTABLES_H
#define DISPLAY_FDTABLES_H

#include <stdio.h>
#include "file_info.h"

void print_perProcess(FileInfo *data, int count);
void print_systemWide(FileInfo *data, int count);
void print_vnodes(FileInfo *data, int count);
void print_composite(FileInfo *data, int count);
void print_summary(FileInfo *data, int count);
void print_threshold(FileInfo *data, int count, int value);

#endif