#include "Headers.h"
#include <time.h>

extern int spaceGet(char *path, char *dest);

int printChildren(char *input, int d_flag, int f_flag, char *dest){
    
    int pw_res = 0;

    struct dirent *dt;
    DIR *dir;
    if(strcmp(dest, "")){
        
        dir = opendir(input);
        if(dir == NULL){
            return 1;
        }

        dt = readdir(dir);
        while(dt != NULL){

            struct stat sb;
            char temp[size];
            sprintf(temp, "%s/%s", input, dt->d_name);
            if(stat(temp, &sb) != 0){
                // does not exist (proly)
            }else{
                if(S_ISDIR(sb.st_mode) == 0){

                    if(f_flag && !strcmp(dest, dt->d_name)){
                        printf("%s\n", temp);
                    }

                }else{

                    if(!strcmp(dt->d_name, "..") || !strcmp(dt->d_name, ".")){
                        // don't loop
                    }else{

                        if(d_flag && !strcmp(dest, dt->d_name)){
                            printf("%s\n", temp);
                        }

                        pw_res += printChildren(temp, d_flag, f_flag, dest);
                    }
                }
            }

            dt = readdir(dir);
        }

        closedir(dir);
    }else{

        int d_temp = 0, f_temp = 0;
        if(!d_flag && !f_flag){
            d_temp = 1;
            f_temp = 1;
        }else{
            d_temp = d_flag;
            f_temp = f_flag;
        }
        
        dir = opendir(input);
        if(dir == NULL){
            return 1;
        }

        dt = readdir(dir);

        while(dt != NULL){

            if(dt->d_type == DT_DIR){
                if(strcmp(dt->d_name, "..") && strcmp(dt->d_name, ".")){
                    if(d_temp){
                        printf("%s/%s\n", input, dt->d_name);
                    }
                
                    char temp[size];
                    sprintf(temp, "%s/%s", input, dt->d_name);
                    pw_res += printChildren(temp, d_flag, f_flag, dest);
                }
            
            }else{
                
                if(f_temp){
                    printf("%s/%s\n", input, dt->d_name);
                }
            }

            dt = readdir(dir);
        }
    }

    return pw_res;
}

int discoverComm(char *input){

    int d_flag = 0, f_flag = 0;
    char search_where[size] = "", search_what[size] = "";
    while(strcmp(input, "") != 0){

        char dest[size];
        spaceGet(input, dest);

        if(dest[0] == '-'){
            for(int i = 1 ; i < strlen(dest) ; i++){
                if(dest[i] == 'd'){
                    d_flag = 1;
                }else if(dest[i] == 'f'){
                    f_flag = 1;
                }else{
                    return 2;
                }
            }
        }else if(dest[0] == '\'' || dest[0] == '\"'){
            if((dest[0] == '\'' && dest[strlen(dest)-1] == '\'') ||
               (dest[0] == '\"' && dest[strlen(dest)-1] == '\"')){
                   dest[strlen(dest) - 1] = '\0';
                   strcpy(search_what, dest + 1);
            }else{
                return 3;
            }
        }else{
            strcpy(search_where, dest);
        }
    }

    if(strcmp(search_where, "") == 0){
        strcpy(search_where, ".");
    }else if(strncmp(search_where, "./", 2) == 0){
        // relative path in cwd
    }else if(search_where[0] == '/'){
        // absolute path
    }else if(search_where[0] == '.'){
        // cwd
    }else{
        char temp[2 * size];
        sprintf(temp, "./%s", search_where);
        strcpy(search_where, temp);
    }

    struct stat sb;
    if(stat(search_where, &sb) != 0){
        return 3;
    }else{
        if(S_ISDIR(sb.st_mode)){
            // directory
        }else{
            return 4;
        }
    }

    char path[size];
    strcpy(path, ".");

    int pw_res = printChildren(search_where, d_flag, f_flag, search_what);

    return 0;
}
