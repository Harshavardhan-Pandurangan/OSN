#include "Headers.h"

extern void removeSpaces(char *input, int type);
int in_file, out_file, err_file;

int ioRedirect(char *input){
    
    char in[size], out[size];
    in[0] = '\0';
    out[0] = '\0';
    int in_n = 0, out_n = 0, out_type = 0;

    in_file = dup(STDIN_FILENO);
    out_file = dup(STDOUT_FILENO);
    err_file = dup(STDERR_FILENO);
    
    for(int i = 0 ; i < strlen(input) ; i++){

        if(input[i] == '<'){

            int j, k;
            for(j = i+1 ; j < strlen(input) ; j++){
                if(input[j] != ' '){
                    break;
                }
            }
            for(k = j+1 ; k < strlen(input) ; k++){
                if(input[k] == ' ' || input[k] == '>' || input[k] == '<'){
                    break;
                }
            }

            strncpy(in, input + j, k - j);
            in[k-j] = '\0';

            char temp[size];
            strncpy(temp, input, i);
            temp[i] = '\0';
            strcat(temp, input + k);
            strcpy(input, temp);

            in_n++;

        }else if(input[i] == '>'){

            if(input[i+1] == '>'){
                i++;
                out_type = 1;
            }

            int j, k;
            for(j = i+1 ; j < strlen(input) ; j++){
                if(input[j] != ' '){
                    break;
                }
            }
            for(k = j+1 ; k < strlen(input) ; k++){
                if(input[k] == ' ' || input[k] == '>' || input[k] == '<'){
                    break;
                }
            }

            strncpy(out, input + j , k - j);
            out[k-j] = '\0';

            char temp[size];
            strncpy(temp, input, i);
            temp[i] = '\0';
            strcat(temp, input + k);
            strcpy(input, temp);

            if(out_type == 1){
                input[strlen(input) - 1] = '\0';
            }

            out_n++;
        }
    }

    if(in_n > 1){
        return 1;
    }else if(out_n > 1){
        return 2;
    }

    if(in_n == 1){
        
        removeSpaces(in, 0);
        struct stat sb;
        if(stat(in, &sb) != 0){
            return 3;
        }else{
            
            if(S_ISDIR(sb.st_mode)){
                return 4;
            }else{
                int in_file_n = open(in, O_RDONLY);
                dup2(in_file_n, STDIN_FILENO);
                close(in_file_n);
            }
        }
    }

    if(out_n == 1){
        
        removeSpaces(out, 0);
        struct stat sb;
        int out_file_n;
        
        stat(out, &sb);
        if(out_type == 0){
            out_file_n = open(out, O_WRONLY | O_TRUNC | O_CREAT, 0644);
        }else{
            out_file_n = open(out, O_WRONLY | O_APPEND | O_CREAT, 0644);
        }
        
        dup2(out_file_n, STDOUT_FILENO);
        close(out_file_n);
    }

    return 0;
}

void resetRedirect(){
    
    dup2(in_file, STDIN_FILENO);
    dup2(out_file, STDOUT_FILENO);
    dup2(err_file, STDERR_FILENO);
}
