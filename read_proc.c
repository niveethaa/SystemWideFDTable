#include "read_proc.h"


long get_inode_number(int pid, int fd){ //Helper function that reads /proc/<pid>/fdinfo/<fd> and then extracts the inode number

    char fd_info_path[4096]; //Storing path of file
    snprintf(fd_info_path, sizeof(fd_info_path), "/proc/%d/fdinfo/%d", pid, fd); // Creates the file path for the fd_info file of this PID and file descriptor

    FILE *fp = fopen(fd_info_path, "r"); //Open the file and read it
    if(fp == NULL) {
        fprintf(stderr, "Something went wrong with reading the file\n. "); 
        return -1;
    }

    char line[256];
    long inode = -1; 
    while(fgets(line, sizeof(line), fp) != NULL){ //Read the lines in the file until finding the line that starts with ino
        if(strncmp(line, "ino:", 4) == 0){
            sscanf(line, "ino:\t%ld", &inode); //Get the inode number
            break;
        }
    }

    fclose(fp); //Close file and return the inode number 
    return inode;
}

void read_one_pid(int pid, FileInfo **data, int *count, int *max){ //Helper function: Reads all the file descriptors for a specific PID and stores it 

    char fd_pathway[4096]; 
    snprintf(fd_pathway, sizeof(fd_pathway), "/proc/%d/fd", pid); //Getting the file path with the specific pid 

    DIR *fd_directory = opendir(fd_pathway); //Opens the directory that has the file pathway
    if(fd_directory == NULL){ //If the directory open is NULL then exit program
        return;
    } 

    struct dirent *fd_data;  // Pointer that reads the data returned from readdir 

    while((fd_data = readdir(fd_directory)) != NULL){  //While data is not NULL loop through all the data 
        if(fd_data->d_name[0] != '.'){ //Ignore any . 

            char full_pathway[8192]; 
            snprintf(full_pathway, sizeof(full_pathway), "%s/%s", fd_pathway, fd_data->d_name); //Building the full file path all the way to the file name

            char file_name[4096]; 
            ssize_t length = readlink(full_pathway, file_name, sizeof(file_name)-1); //Reads the symbolic link to get the file name and gets the size of it. 

            if(length != -1){ //If the readlink succeeded then code continues
                file_name[length] = '\0';

                if(*count == *max){ //Checks if the array is full 
                    *max *= 2; //Doubles the size of the array 
                    FileInfo *temp = realloc(*data, *max * sizeof(FileInfo)); //Getting more entries to fit more file data
                   
                    if(temp == NULL){
                        closedir(fd_directory);
                        return;
                    }
                    *data = temp; //Updates the pointer to point at the bigger memory data
                }

                long inode = get_inode_number(pid, atoi(fd_data->d_name)); //Calling helper function to get inode number and converting to int

                FileInfo *data_entry = *data + *count; //Get the pointer of the next empty data line
                data_entry->pid   = pid; //Stores pid in data_entry
                data_entry->fd    = atoi(fd_data->d_name); //Converts fd name into an int and stores in data_entry
                data_entry->inode = inode;              
                strncpy(data_entry->name, file_name, 259); //Copy the filename into data_entry
                data_entry->name[259] = '\0'; //Makes sure the filename string has a null terminator
                *count = *count + 1; //Increase the total number of data entry stored by 1 
            }
        }
    }
    closedir(fd_directory); //Closes the directory of the file descriptor pathway 
}

void readProc_info(int user_pid, FileInfo **data, int *count, int *max){ //Reads the /proc and gets all the file descriptor info for the user processes
    if(user_pid != 0){ //If a specific PID was given by user
        read_one_pid(user_pid, data, count, max); //Reads only the file data entries for the specific PID
        return;
    }

    DIR *proc_directory = opendir("/proc"); //Open the /proc directory and scan all the info
    if(proc_directory == NULL){
        fprintf(stderr, "Cannot open /proc\n");
        return;
    }

    struct dirent *proc_data; //Pointer used to store each data entry
    while((proc_data = readdir(proc_directory)) != NULL){ //Looping through all data entry as long as it's not NULL

        if(proc_data->d_name[0] >= '0' && proc_data->d_name[0] <= '9'){ // heck if entry name starts with a number meaning it is a PID
            int current_pid = atoi(proc_data->d_name);  //Convert the PID from string to integer

            char proc_pathway[4096];  //String to store the path to this PID in /proc
            snprintf(proc_pathway, sizeof(proc_pathway), "/proc/%d", current_pid); //Build the path /proc/<pid>

            struct stat proc_status; //Struct to store file information about this PID directory
            if(stat(proc_pathway, &proc_status) == 0){ //Get file info for this PID directory
                if(proc_status.st_uid == getuid()){ //Check if this process belongs to the current user
                    read_one_pid(current_pid, data, count, max); //Read all FDs for this PID
                }
            }
        }
    }

    closedir(proc_directory); //Close /proc directory when done
}
