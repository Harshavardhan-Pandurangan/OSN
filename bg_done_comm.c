#include "Headers.h"

void handleBgProcess(){
    
    int status;
    int pid = waitpid(-1, &status, WNOHANG);

    if(WIFSTOPPED(status)){
        
        return;
    
    }else if(pid >= 0){
        
        char exit_stat[size];
        if(WIFEXITED(status)){
            strcpy(exit_stat, "normally");
        }else{
            strcpy(exit_stat, "abnormally");
        }
        
        fprintf(stderr, "\nBackground process with pid [%d] done. Exited %s\n", pid, exit_stat);
        
        return;
    }
}