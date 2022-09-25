#include "Headers.h"

extern int spaceGet(char *path, char *dest);
pid_t sample;

void sysComm(char *input, int type){
    
    char func[size];
    spaceGet(input, func);

    char temp_arg[200][size];
    strcpy(temp_arg[0], func);
    int arg_count = 1;
    while(strcmp(input, "") != 0){
        spaceGet(input, temp_arg[arg_count]);
        arg_count++;
    }
    
    char *arguments[arg_count+1];
    for(int i = 0 ; i < arg_count ; i++){
        arguments[i] = strdup(temp_arg[i]);
    }
    arguments[arg_count] = NULL;

    pid_t new_proc = fork();

    if(new_proc < 0){
        
        fprintf(stderr, "Shell: fork: cannot create fork process\n");

    }else if(new_proc == 0){

        if(type){
            setpgid(0, 0);
        }

        if(execvp(func, arguments) < 0){

            fprintf(stderr, "Shell: Command not found\n");
            exit(1);

        }

    }else{

        sample = getpid();
        
        if(type == 0){
            
            signal(SIGTTIN, SIG_IGN);
            signal(SIGTTOU, SIG_IGN);

            waitpid(new_proc, NULL, 0);

            signal(SIGTTIN, SIG_DFL);
            signal(SIGTTOU, SIG_DFL);

        }else{

            fprintf(stdout, "%s with pid [%d] started successfully\n", func, new_proc);

        }

        sample = -1;

    }

    return;
}