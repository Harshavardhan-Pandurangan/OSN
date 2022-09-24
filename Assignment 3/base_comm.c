#include "Headers.h"

extern int cdCommand(char *path);
extern int echoCommand(char *text);
extern int pwdCommand();
extern int lsComm(char *input, char **too_big);
extern int historyComm(char *input, int type);
extern int discoverComm(char *input);
extern void sysComm(char *input, int type);
extern int pinfoComm(char *input);

// Clear shell
void clearScreen(){

    printf("\e[1;1H\e[2J");

    return;
}

// Remove additional spaces
void removeSpaces(char *input, int type){
    
    if(type == 0){
        
        char temp[size];
        while(input[0] == ' '){
            strcpy(temp, input + 1);
            strcpy(input, temp);
        }
        while(input[strlen(input) - 1] == ' '){
            input[strlen(input) - 1] = '\0';
        }

    }else if(type == 1){
        
        char temp[size];
        int t = 0, quote_flag = 0, i = 0;

        while(input[i] == ' '){
            i++;
        }
        for(; i < strlen(input) ; i++){
            
            if(input[i] == '\''){
                if(quote_flag == 0){
                    quote_flag = 1;
                }else if(quote_flag == 1){
                    quote_flag = 0;
                }
            }else if(input[i] == '\"'){
                if(quote_flag == 0){
                    quote_flag = 2;
                }else if(quote_flag == 2){
                    quote_flag = 0;
                }
            }

            if(input[i] == ' ' && quote_flag == 0){
                temp[t++] = ' ';
                while(input[i] == ' '){
                    i++;
                }
                i--;
            }else{
                temp[t++] = input[i];
            }
        }
        temp[t] = '\0';

        strcpy(input, temp);

    }

    return;
}

// Run each separate command
void runCommand(char *input, int type){

    removeSpaces(input, 0);

    //
    
    if(strncmp(input, "cd", 2) == 0){

        removeSpaces(input, 0);
        if(input[2] != '\0' && input[2] != ' '){
            fprintf(stderr, "Shell: command not found\n");
        }else{
        
            char temp[size];
            strcpy(temp, input + 2);
            strcpy(input, temp);
            removeSpaces(input, 0);
        
            int res = cdCommand(input);
            if(res == 0){
                // proper execution
            }else if(res == 1){
                fprintf(stderr, "cd: too many arguments\n");
            }else if(res == 2){
                fprintf(stderr, "cd: no such file or directory\n");
            }else{
                fprintf(stderr, "cd: unknown error\n");
            }
        }

    }else if(strncmp(input, "echo ", 5) == 0){
        
        char temp[size];
        strcpy(temp, input + 5);
        strcpy(input, temp);
        
        int res = echoCommand(input);
        if(res == 1){
            fprintf(stderr, "echo: unknown error\n");
        }

    }else if(strncmp(input, "pwd", 3) == 0){
        
        if(input[3] != '\0' && input[3] != ' '){
            fprintf(stderr, "Shell: command not found\n");
        }else{

            int res = pwdCommand();
            if(res == 0){
                //proper execution
            }else{
                fprintf(stderr, "pwd: could not get directory path\n");
            }
        }

    }else if(strncmp(input, "ls", 2) == 0){
        
        if(input[2] != '\0' && input[2] != ' '){
            fprintf(stderr, "Shell: command not found\n");
        }else{

            char temp[size];
            strcpy(temp, input + 3);
            strcpy(input, temp);
            removeSpaces(input, 1);
            
            char *too_big[200];
            for(int i = 0 ; i < 200 ; i++){
                too_big[i] = (char*) malloc (sizeof(char) * 5000);
                strcpy(too_big[i], "");
            }
            int res = lsComm(input, too_big);
            if(res == 0){
                //proper execution
            }else if(res == 1){
                fprintf(stderr, "ls: invalid flags\n");
            }else if(res == 2){
                int i = 0;
                while(strcmp(too_big[i], "") != 0){
                    fprintf(stderr, "ls: cannot access %s: directory has too many files to display\n", too_big[i]);
                    free(too_big[i]);
                    i++;
                }
            }
        }

    }else if(strncmp(input, "history", 7) == 0){

        if(input[7] != ' ' && input[7] != '\0'){
            fprintf(stderr, "Shell: command not found\n");
        }else{

            char temp[size];
            strcpy(temp, input + 7);
            strcpy(input, temp);
            removeSpaces(input, 1);
            int res = historyComm(input, 1);

            if(res == 1){
                fprintf(stderr, "history: too many arguments\n");
            }else if(res == 2){
                fprintf(stderr, "history: proper numeric value required\n");
            }
        }

    }else if(strncmp(input, "discover", 8) == 0){

        if(input[8] != ' ' && input[8] != '\0'){
            fprintf(stderr, "Shell: command not found\n");
        }else{

            char temp[size];
            strcpy(temp, input + 8);
            strcpy(input, temp);
            removeSpaces(input, 1);

            int res = discoverComm(input);
            if(res == 1){
                fprintf(stderr, "discover: too many arguments\n");
            }else if(res == 2){
                fprintf(stderr, "discover: invalid flags\n");
            }else if(res == 3){
                fprintf(stderr, "discover: search path directory not found\n");
            }else if(res == 4){
                fprintf(stderr, "discover: search path is not a directory\n");
            }
        }

    }else if(strncmp(input, "pinfo", 5) == 0){
        
        if(input[5] != ' ' && input[5] != '\0'){
            fprintf(stderr, "Shell: command not found\n");
        }else{

            char temp[size];
            strcpy(temp, input + 5);
            strcpy(input, temp);
            removeSpaces(input, 1);

            int res = pinfoComm(input);
            if(res == 0){
                // proper execution
            }else if(res == 1){
                fprintf(stderr, "pinfo: too many arguments\n");
            }else if(res == 2){
                fprintf(stderr, "pinfo: proper numeric value required\n");
            }else if(res == 3){
                fprintf(stderr, "pinfo: could not access process\n");
            }

        }

    }else if(strcmp(input, "clear") == 0){
   
        clearScreen();

    }else if(strcmp(input, "exit") == 0){
        
        clearScreen();
        exit(0);

    }else{
        
        sysComm(input, type);

    }
}

// Separate all commands from single input string and call runCommand() function
void runCommands(char *input){
    
    historyComm(input, 0);

    int sizel = strlen(input);
    int i, start = 0;

    for(i = 0; i < sizel; i++){
        if(input[i] == ';'){
            input[i] = '\0';
            runCommand(input + start, 0);
            start = i + 1;
        }
        if(input[i] == '&'){
            input[i] = '\0';
            runCommand(input + start, 1);
            start = i + 1;
        }
    }
    if(start < i){
        runCommand(input + start, 0);
    }

    return;
}
