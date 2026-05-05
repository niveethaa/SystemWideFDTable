#include "display_FDTables.h"

void print_perProcess(FileInfo *data, int count){
    printf("\t PID\t FD\n"); 
    printf("\t============\n"); 
    for(int i = 0; i < count; i++){ // Loop through all entries
        printf("\t %d\t %d\n", data[i].pid, data[i].fd); // Print PID and FD
    }
    printf("\t ============\n"); 
}

void print_systemWide(FileInfo *data, int count){
    printf("\n\t PID\t FD\t Filename\n"); 
    printf("\t ============\n"); 
    for(int i = 0; i < count; i++){ // Loop through all entries
        printf("\t %d\t %d\t %s\n", data[i].pid, data[i].fd, data[i].name); // Print PID, FD and filename
    }
    printf("\t ============\n"); 
}

void print_vnodes(FileInfo *data, int count){
    printf("\n\t FD\t Inode\n"); 
    printf("\t ============\n"); 
    for(int i = 0; i < count; i++){ // Loop through all entries
        printf("\t %d\t %ld\n", data[i].fd, data[i].inode); // Print FD and inode
    }
    printf("\t ============\n"); 
}

void print_composite(FileInfo *data, int count){
    printf("\n\t PID\t FD\t Filename\t\t Inode\n"); 
    printf("\t ============\n"); 
    for(int i = 0; i < count; i++){ // Loop through all entries
        printf("\t %d\t %d\t %s\t\t %ld\n", data[i].pid, data[i].fd, data[i].name, data[i].inode); // Print pid, FD, filename and inode
    }
    printf("\t ============\n");
}

void print_summary(FileInfo *data, int count){
    printf("\n\t PID\t FD Count\n");
    printf("\t ============\n"); 
    for(int i = 0; i < count; i++){ // Loop through all entries
        int already_printed = 0; // Flag to check if PID already printed
        for(int j = 0; j < i; j++){ // Check all previous entries
            if(data[j].pid == data[i].pid){ // If PID already seen
                already_printed = 1; // Mark as already printed
                break;
            }
        }
        if(already_printed == 0){ // If PID not printed yet
            int fd_count = 0; 
            for(int k = 0; k < count; k++){ // Count all FDs for this PID
                if(data[k].pid == data[i].pid){
                    fd_count++; // Increment FD count
                }
            }
            printf("\t %d\t (%d)\n", data[i].pid, fd_count); // Print PID and FD count
        }
    }
    printf("\t ============\n"); 
}

void print_threshold(FileInfo *data, int count, int value){
    printf("\n## Offending processes:\n"); 
    for(int i = 0; i < count; i++){ // Loop through all entries
        int already_printed = 0; // Flag to check if PID already printed
        for(int j = 0; j < i; j++){ // Check all previous entries
            if(data[j].pid == data[i].pid){ // If PID already seen
                already_printed = 1; // Mark as already printed
                break;
            }
        }
        if(already_printed == 0){ // If PID not printed yet
            int fd_count = 0;
            for(int k = 0; k < count; k++){ // Count all FDs for this PID
                if(data[k].pid == data[i].pid){
                    fd_count++; // Increment FD count
                }
            }
            if(fd_count > value){ // Only print if FD count exceeds threshold
                printf("\t %d\t (%d)\n", data[i].pid, fd_count); // Print PID and FD count
            }
        }
    }
    printf("\n");
}
