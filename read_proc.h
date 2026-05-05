#ifndef READ_PROC_H
#define READ_PROC_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "file_info.h" 

long get_inode_number(int pid, int fd);
void read_one_pid(int pid, FileInfo **data, int *count, int *max);
void readProc_info(int user_pid, FileInfo **data, int *count, int *max);

#endif