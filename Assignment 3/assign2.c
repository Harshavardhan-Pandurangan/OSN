#include "Headers.h"

extern void promptText(time_t time);
extern void clearScreen();
extern void runCommands(char *input);
extern void handleBgProcess();
extern void removeSpaces(char *input, int type);
extern void inputProcess(char *input, time_t time);

char homeDir[size];

// main driver function for the shell
int main(){
    clearScreen();

    if(getcwd(homeDir, size) == NULL){
        fprintf(stderr, "Shell: Cannot get home directory path\n");
        exit(0);
    }

    signal(SIGCHLD, &handleBgProcess);

    time_t proc_time = 0;
    char prompt[size];
    while(1){

        promptText(proc_time);
        char input[size];
        inputProcess(input, proc_time);
        removeSpaces(input, 0);
        if(*input && input[strlen(input)-1] == '\n'){
            input[strlen(input)-1] = '\0';
        }

        time_t start = time(NULL);
        runCommands(input);
        time_t end = time(NULL);
        proc_time = end - start;
    }
}
