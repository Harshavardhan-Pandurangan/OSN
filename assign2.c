#include "Headers.h"

extern void promptText(time_t time);
extern void clearScreen();
extern void runCommands(char *input);
extern void handleBgProcess();
extern void removeSpaces(char *input, int type);
extern void inputProcess(char *input, time_t time);
extern pid_t sample;

char homeDir[size];
int waitInput = 0;

void cntrlC(int signo){
    if(waitInput){
        printf("^C\n");
        promptText(0);
    }else if (sample != -1){
        printf("\n");
        kill(sample, SIGINT);
    }
    sample = -1;
    
}

void cntrlZ(int signo){
    if(waitInput){
        printf("^Z\n");
        promptText(0);
    }else if (sample != -1){
        printf("\n");
        kill(sample, SIGTSTP);
    }
    sample = -1;
    
}

// main driver function for the shell
int main(){
    clearScreen();

    if(getcwd(homeDir, size) == NULL){
        fprintf(stderr, "Shell: Cannot get home directory path\n");
        exit(0);
    }

    signal(SIGCHLD, handleBgProcess);
    signal(SIGINT, cntrlC);
    signal(SIGTSTP, cntrlZ);


    time_t proc_time = 0;
    char prompt[size];
    while(1){
        waitInput = 1;
        sample = -1;
        
        promptText(proc_time);
        char input[size];
        inputProcess(input, proc_time);
        removeSpaces(input, 0);
        
        if(*input && input[strlen(input)-1] == '\n'){
            input[strlen(input)-1] = '\0';
        }

        time_t start = time(NULL);
        waitInput = 0;
        runCommands(input);
        time_t end = time(NULL);
        
        proc_time = end - start;
    }
}
