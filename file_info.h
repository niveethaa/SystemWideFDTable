#ifndef FILE_INFO_H
#define FILE_INFO_H

typedef struct{
    int pid;
    int fd;
    long inode;
    char name[260];
} FileInfo;

#endif