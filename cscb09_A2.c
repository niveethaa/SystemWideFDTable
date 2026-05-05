#include "read_proc.h"
#include "display_FDTables.h"

int main(int argc, char ** argv){

    //Flag Arguments
    int process = 0; 
    int system_wide = 0; 
    int vnodes = 0; 
    int composite = 0; 
    int summary = 0; 
    int threshold = 0;
    int value = 0; //Stores the X from --threshold=X
    int user_pid = 0; //Positional Argument

    //Parsing through flags 
    for (int i = 1; i < argc; i++){
        if(strcmp(argv[i], "--per-process")==0){
            process = 1; 
        }

       else if(strcmp(argv[i], "--systemWide")==0){
            system_wide = 1; 
        }

        else if(strcmp(argv[i], "--Vnodes")==0){
            vnodes = 1; 
        }

        else if(strcmp(argv[i], "--composite")==0){
            composite = 1; 
        }

       else if(strcmp(argv[i], "--summary")==0){
            summary = 1; 
        }

        else if(strncmp(argv[i], "--threshold=", 12)==0){ //12 is the number of characters the compared string has
            threshold = 1;
            value = atoi(argv[i] + 12); //We add 12 to access the value of X and converted into the string to int

            if(value <0){
                fprintf(stderr, "There's an error X needs to be a positive number\n"); //Ensures X is positive integer 
                return -1; 
            }
        
        }else if(argv[i][0] != '-' ){ //Checks for any arguments that don't start with a - then get the maximum number of processes 
            user_pid = atoi(argv[i]); 

            if(user_pid <=0){
                fprintf(stderr, "There's an error pid needs to be a positive number\n"); //Ensures X is positive integer 
                return -1; 
            }
        }else{
            fprintf(stderr, "There's an error, arguments might not be typed right.");    //Check if the arguments are being typed right 
            return -1; 
        }
    }

    //If no arguments are given then the composite flag is updated and composite table will be displayed for it
    if(!process && !system_wide && !vnodes && !composite && !summary && !threshold){
        composite = 1; 
    }

    //Reading /proc to get the information about FD
    int count = 0;
    int max = 256; 
    FileInfo *data = malloc(max * sizeof(FileInfo)); //Stores the file data in dynamic array

    if(data == NULL){
        fprintf(stderr, "There's not enough memory for getting the data.");
        return -1;
    }

    readProc_info(user_pid, &data, &count, &max); //Function to read the /proc file information 
    
    //Printing the FD tables based on the matches of the flags
    if(process){
        print_perProcess(data, count);
    }

    if(system_wide){
        print_systemWide(data, count);
    }
    if(vnodes){
        print_vnodes(data, count);
    }

    if(composite){
        print_composite(data, count);  
    }

    if(summary){
        print_summary(data, count);
    }

    if(threshold){
        print_threshold(data, count, value);
    }   
    
    free(data); //Release the data 

    return 0;





}
