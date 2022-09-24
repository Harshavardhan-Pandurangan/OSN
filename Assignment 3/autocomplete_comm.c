#include "Headers.h"

extern void promptText(time_t time);

extern char homeDir[size];
extern char prevDir[size];

void tab(char *input, int *count, time_t time){

    int quote_flag = 0;
    int iter;
    for(iter = strlen(input)-1 ; iter >= 0 ; iter--){
        
        if(input[iter] == '\''){
            if(quote_flag == 0){
                quote_flag = 1;
            }else if(quote_flag == 1){
                quote_flag = 0;
            }
        }else if(input[iter] == '\"'){
            if(quote_flag == 0){
                quote_flag = 2;
            }else if(quote_flag == 2){
                quote_flag = 0;
            }
        }

        if(quote_flag == 0 && input[iter] == ' '){
            break;
        }
    }

    char temp[size];
    strcpy(temp, input + iter + 1);

    char path[size] = "", search[size] = "";
    strcpy(path, temp);
    for(int i = 0 ; i < strlen(temp) ; i++){
        if(temp[i] == '/'){
            strncpy(path, temp, i + 1);
            path[i+1] = '\0';
            strcpy(search, temp + i + 1);
        }
    }

    if(path[strlen(path)-1] != '/'){
        strcpy(search, path);
        strcpy(path, "");
    }

    if(path[0] == '~'){
        char temp_path[size];
        strcpy(temp_path, path);
        if(path[1] == '/'){
            sprintf(path, "%s%s", homeDir, temp_path+1);
        }else if(path[1] == '\0'){
            sprintf(path, "%s", homeDir);
        }
    }else{
        if(path[0] == '/'){
            //
        }else{
            char temp_path[size], cur_dir[size];
            strcpy(temp_path, path);
            getcwd(cur_dir, size);
            sprintf(path, "%s/%s", cur_dir, temp_path);
        }
    }

    struct dirent *dt;
    DIR *dir = opendir(path);

    if(dir != NULL){
        dt = readdir(dir);
        char match[size];
        int match_count = 0;
        while(dt != NULL){
            if(strncmp(search, dt->d_name, strlen(search)) == 0 && strlen(search) <= strlen(dt->d_name)){
                match_count++;
                strcpy(match, dt->d_name);
            }
            dt = readdir(dir);
        }

        closedir(dir);

        if(match_count == 0){

            // do not change anything
        
        }else if(match_count == 1){
            
            struct stat sb;
            char name[size];
            sprintf(name, "%s%s", path, match);
            stat(name, &sb);
            if(S_ISDIR(sb.st_mode)){
                
                for(int i = strlen(search) ; i < strlen(match) ; i++){
                    input[*count] = match[i];
                    printf("%c", match[i]);
                    *count += 1;
                }
                input[*count] = '/';
                printf("/");
                *count += 1;

            }else{
                
                for(int i = strlen(search) ; i < strlen(match) ; i++){
                    input[*count] = match[i];
                    printf("%c", match[i]);
                    *count += 1;
                }
                input[*count] = ' ';
                printf(" ");
                *count += 1;
            }
        }else{
            
            char prefix[size] = "";
            int i_count = 0;
            dir = opendir(path);

            dt = readdir(dir);
            while(dt != NULL){
                if(strncmp(search, dt->d_name, strlen(search)) == 0 && strlen(search) <= strlen(dt->d_name)){
                    if(strlen(prefix) == 0 && i_count == 0){
                        strcpy(prefix, dt->d_name);
                        i_count++;
                        printf("\n%s", dt->d_name);
                    }else{
                        char temp_prefix[size];
                        for(int i = 0 ; i < strlen(prefix) ; i++){
                            if(dt->d_name[i] != prefix[i]){
                                strncpy(temp_prefix, prefix, i);
                                temp_prefix[i] = '\0';
                                break;
                            }
                        }
                        strcpy(prefix, temp_prefix);
                        printf("\n%s", dt->d_name);
                    }
                }
                dt = readdir(dir);
            }

            for(int i = strlen(search) ; i < strlen(prefix) ; i++){
                input[*count] = prefix[i];
                *count += 1;
            }

            printf("\n");
            promptText(time);
            printf("%s", input);
        }
    }

    return;
}

void inputProcess(char *input, time_t time){
    
    for(int i = 0 ; i < size ; i++){
        input[i] = '\0';
    }

    struct termios old, new;
    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new);
    setbuf(stdout, NULL);

    int count = 0;
    
    char in;
    while(read(STDIN_FILENO, &in, 1) == 1){
        
        if(iscntrl(in)){

            if(in == 10){
                printf("\n");
                break;
            }else if(in == 127){
                if(count > 0){
                    input[--count] = '\0';
                    printf("\b \b");
                }
            }else if(in == 9){
                tab(input, &count, time);
            }
        }else{

            printf("%c", in);
            input[count++] = in;
            input[count] = '\0';
        }
    }
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &old);

    return;
}
