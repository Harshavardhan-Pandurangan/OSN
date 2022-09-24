#include "Headers.h"

extern int removeSpaces(char *input, int type);
extern char homeDir[size];

char prevDir[size] = "";

int spaceCheck(char *input){

    int quote_flag = 0;
    int len = strlen(input);

    for(int i = 0 ; i < len ; i++){
        
        if(input[i] == 34){
            if(quote_flag == 0){
                quote_flag = 1;
            }else if(quote_flag == 1){
                quote_flag = 0;
            }
        }else if(input[i] == 39){
            if(quote_flag == 0){
                quote_flag = 2;
            }else if(quote_flag == 2){
                quote_flag = 0;
            }
        }else{
            if(input[i] == 32){
                if(quote_flag != 0){
                    continue;
                }else{
                    return 1;
                }
            }else{
                continue;
            }
        }
    }

    return 0;
}

void slashGet(char *path, char *dest){
    
    for(int i = 0 ; i <= strlen(path) ; i++){
        
        if(path[i] == '/'){
            strncpy(dest, path, i);
            dest[i] = '\0';
            char temp[size];
            strcpy(temp, path + i + 1);
            strcpy(path, temp);
            strcat(dest, "/");
            return;
        }else if(path[i] == '\0'){
            strncpy(dest, path, i);
            dest[i] = '\0';
            char temp[size];
            strcpy(temp, path + i);
            strcpy(path , temp);
            return;
        }
    }
}

void stepBack(char *path){
    
    for(int i = strlen(path) - 1 ; i >= 0 ; i--){
        
        if(path[i] == '/' && i == 0){
            if(strlen(path) > 1){
                strcpy(path, "/");
            }
            return;
        }if(path[i] == '/'){
            char t_path[size];
            strncpy(t_path, path, i);
            t_path[i] = '\0';
            strcpy(path, t_path);
            return;
        }
    }
}

int cdCommand(char *path){
    
    if(spaceCheck(path) == 1){
        return 1;
    }

    char cdDir[size];
    if(getcwd(cdDir, size) == NULL){
        if(getcwd(cdDir, size) == NULL){
            return 3;
        }
    }

    char temp[size];
    strcpy(temp, cdDir);

    if(path[0] == '\0'){
        chdir(homeDir);
    }

    while(path[0] != '\0'){
        char stepDir[size];
        slashGet(path, stepDir);
        
        if(strcmp(stepDir, ".") == 0){
            continue;
        }else if(strcmp(stepDir, "..") == 0){
            stepBack(cdDir);
        }else if(strcmp(stepDir, "-") == 0){
            if(strcmp(prevDir, "") == 0){
                fprintf(stderr, "cd: OLDPWD not set\n");
                continue;
            }
            strcpy(cdDir, prevDir);
            printf("%s\n", prevDir);
        }else if(strcmp(stepDir, "~") == 0){
            strcpy(cdDir, homeDir);
        }else if(strcmp(stepDir, "/") == 0){
            strcpy(cdDir, "/");
        }else{
            char newDir[2*size+1];
            if((stepDir[0] == '\'' && stepDir[strlen(stepDir) - 1] == '\'') ||
               (stepDir[0] == '\"' && stepDir[strlen(stepDir) - 1] == '\"')){
                stepDir[strlen(stepDir) - 1] = '\0';
                char temp[size];
                strcpy(temp, stepDir + 1);
                strcpy(stepDir, temp);
            }
            sprintf(newDir, "%s/%s", cdDir, stepDir);
            strcpy(cdDir, newDir);
        }

        if(chdir(cdDir) != 0){
            return 2;
        }
    }

    strcpy(prevDir, temp);

    return 0;
}

int echoCommand(char *text){
    
    removeSpaces(text, 0);

    int space_flag = 0, quote_flag = 0;
    int text_size = strlen(text);

    for(int i = 0 ; i < text_size ; i++){
        
        if(text[i] == 34){
            if(quote_flag == 0){
                quote_flag = 1;
                space_flag = 1;
            }else if(quote_flag == 1){
                quote_flag = 0;
                space_flag = 0;
            }else{
                printf("%c", text[i]);
            }
        }else if(text[i] == 39){
            if(quote_flag == 0){
                quote_flag = 2;
                space_flag = 1;
            }else if(quote_flag == 2){
                quote_flag = 0;
                space_flag = 0;
            }else{
                printf("%c", text[i]);
            }
        }else{
            if(text[i] == 32){
                if(quote_flag != 0){
                    printf("%c", text[i]);
                }else if(space_flag == 0){
                    space_flag = 1;
                    printf("%c", text[i]);
                }else{
                    continue;
                }
            }else{
                if(quote_flag == 0 && space_flag == 1){
                    space_flag = 0;
                }

                if(text[i] == 92){
                    if(text[i+1] == 92){
                        printf("%c", 92);
                        i++;
                    }
                }else{
                    printf("%c", text[i]);
                }
            }
        }
    }

    printf("\n");

    return 0;
}

int pwdCommand(){
    
    char curDir[size];
    if(getcwd(curDir, size) == NULL){
        return 1;
    }else{
        printf("%s\n", curDir);
        return 0;
    }
}
