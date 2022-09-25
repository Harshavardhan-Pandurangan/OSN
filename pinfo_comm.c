#include "Headers.h"

int pinfoComm(char *input){

    pid_t pid_val;
    if(strcmp(input, "") == 0){
        pid_val = getpid();
    }else{
        
        for(int i = 0 ; i < strlen(input) ; i++){
            if(input[i] == ' '){
                return 1;
            }else if(input[i] < 48 || input[i] > 57){
                return 2;
            }
        }

        pid_val = atoi(input);
    }

    char filepath[size];
    sprintf(filepath, "/proc/%d/stat", pid_val);

    FILE *file = fopen(filepath, "r");
    if (file == NULL){
        return 3;
    }
    
    int pid;
    char exename[size], status;
    fscanf(file, "%d %s %c", &pid, exename, &status);
    sprintf(filepath, "/proc/%d/exe", pid_val);
    
    char exepath[size];
    int ret = readlink(filepath, exepath, size);
    
    char execomplete[2*size];
    if (ret != -1){
        exepath[ret] = '\0';
        strcpy(execomplete, exepath);
    }else{
        strcpy(execomplete, exename);
    }
    fclose(file);

    sprintf(filepath, "/proc/%d/status", pid_val);
    file = fopen(filepath, "r");
    if (file == NULL){
        return 3;
    }

    char mem[size], tracer[size];
    while (fscanf(file, " %s", tracer) == 1){
        if (strcmp(tracer, "VmSize:") == 0){
            fscanf(file, " %s", mem);
            break;
        }
    }
    fclose(file);
    
    printf("pid : %d\n", pid);
    printf("process Status : %c\n", status);
    printf("memory : %s\n", mem);
    printf("executable Path : %s\n", execomplete);
    printf("\n");

    return 0;
}
