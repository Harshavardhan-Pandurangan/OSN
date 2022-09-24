
#include "Headers.h"

extern char homeDir[size];

int spaceGet(char *path, char *dest){

    int quote_flag = 0;
    for(int i = 0 ; i <= strlen(path) ; i++){
        
        if(path[i] == '\''){
            if(quote_flag == 0){
                quote_flag = 1;
            }else if(quote_flag == 1){
                quote_flag = 0;
            }
        }else if(path[i] == '\"'){
            if(quote_flag == 0){
                quote_flag = 2;
            }else if(quote_flag == 2){
                quote_flag = 0;
            }
        }else if(path[i] == ' '){
            if(quote_flag != 0){
                continue;
            }
            strncpy(dest, path, i);
            dest[i] = '\0';
            char temp[size];
            strcpy(temp, path + i + 1);
            strcpy(path, temp);
            return quote_flag;
        }else if(path[i] == '\0'){
            strncpy(dest, path, i);
            dest[i] = '\0';
            char temp[size];
            strcpy(temp, path + i);
            strcpy(path, temp);
            return quote_flag;
        }
    }
}

void sort(char file_dir[200][size], int file_dir_count){

    for(int i = 0 ; i < file_dir_count ; i++){
        //printf("%s\n", file_dir[i]);
    }

    for(int i = file_dir_count-1 ; i > 0 ; i--){
        for(int j = 0 ; j < i ; j++){
            
            int n1 = 0, n2 = 0;
            if(file_dir[j][0] == '.'){
                n1 = 1;
            }
            if(file_dir[j+1][0] == '.'){
                n2 = 1;
            }
            int compare_res = strcasecmp(file_dir[j] + n1, file_dir[j+1] + n2);
            if(compare_res > 0){
                char temp[size];
                strcpy(temp, file_dir[j]);
                strcpy(file_dir[j], file_dir[j+1]);
                strcpy(file_dir[j+1], temp);
            }
        }
    }

    for(int i = 0 ; i < file_dir_count ; i++){
        //printf("%s\n", file_dir[i]);
    }

    return;
}

void getColor(char *color, char *path, char *file){

    struct stat sb;
    stat(path, &sb);

    if(S_ISDIR(sb.st_mode)){
        strcpy(color, "\033[1m\033[34m");
    }else if((sb.st_mode & S_IXUSR) || (sb.st_mode & S_IXGRP) || (sb.st_mode & S_IXOTH)){
        strcpy(color, "\033[1m\033[32m");
    }else{
        strcpy(color, "\033[0m\033[0m");
    }

}

long int sprintfData(char *output, char *path, char *file){

    struct stat sb;
    stat(path, &sb);

    char permissions[11];
    permissions[0] = (S_ISDIR(sb.st_mode)) ? 'd' : '-';
    permissions[1] = (sb.st_mode & S_IRUSR) ? 'r' : '-';
    permissions[2] = (sb.st_mode & S_IWUSR) ? 'w' : '-';
    permissions[3] = (sb.st_mode & S_IXUSR) ? 'x' : '-';
    permissions[4] = (sb.st_mode & S_IRGRP) ? 'r' : '-';
    permissions[5] = (sb.st_mode & S_IWGRP) ? 'w' : '-';
    permissions[6] = (sb.st_mode & S_IXGRP) ? 'x' : '-';
    permissions[7] = (sb.st_mode & S_IROTH) ? 'r' : '-';
    permissions[8] = (sb.st_mode & S_IWOTH) ? 'w' : '-';
    permissions[9] = (sb.st_mode & S_IXOTH) ? 'x' : '-';

    char pwd_uid_name[size], pwd_gid_name[size];
    if(getpwuid(sb.st_uid) != NULL){
        strcpy(pwd_uid_name, getpwuid(sb.st_uid)->pw_name);
    }else{
        sprintf(pwd_uid_name, "%d", sb.st_uid);
    }
    if(getpwuid(sb.st_gid) != NULL){
        strcpy(pwd_gid_name, getpwuid(sb.st_gid)->pw_name);
    }else{
        sprintf(pwd_gid_name, "%d", sb.st_gid);
    }

    long int file_size = sb.st_size;

    char date_time[20];
    strftime(date_time, 20, "%b %2d %H:%M", localtime(&(sb.st_mtim)));

    char color[30];
    getColor(color, path, file);

    sprintf(output, " %s %3ld %15s %15s %8ld %s %s%s%s\n", permissions, sb.st_nlink, 
            pwd_uid_name, pwd_gid_name, file_size, date_time, color, file, "\033[0m\033[0m");
    
    return sb.st_blocks;
}

int lsComm(char *input, char **too_big){
    
    int a_flag = 0, l_flag = 0, file_dir_count = 0, do_not_exist_count = 0;
    char file_dir[200][size];
    char do_not_exist[200][size];
    
    while(strcmp(input, "") != 0){
        
        char part[size];
        int check = spaceGet(input, part);
        if(check != 0){
            strcpy(do_not_exist[do_not_exist_count], part + 1);
            do_not_exist_count++;
        }
        
        if(part[0] == '-'){
            for(int i = 1 ; i < strlen(part) ; i++){
                if(part[i] == 'a'){
                    a_flag = 1;
                }else if(part[i] == 'l'){
                    l_flag = 1;
                }else{
                    return 1;
                }
            }
        }else{
            if(part[0] == '\'' || part[0] == '\"'){
                strcpy(file_dir[file_dir_count], part+1);
                file_dir[file_dir_count][strlen(part) - 2] = '\0';
            }else{
                strcpy(file_dir[file_dir_count], part);
            }
            file_dir_count++;
        }
    }

    sort(file_dir, file_dir_count);

    if(file_dir_count == 0 && do_not_exist_count == 0){
        strcpy(file_dir[0], ".");
        file_dir_count++;
    }
    
    char files[file_dir_count][size], directories[file_dir_count][size];
    int file_count = 0, directory_count = 0;
    struct stat sb;

    for(int i = 0 ; i < file_dir_count ; i++){

        if(strcmp(file_dir[i], "~") == 0){
            strcpy(file_dir[i], homeDir);
        }

        if(stat(file_dir[i], &sb) != 0){

            strcpy(do_not_exist[do_not_exist_count], file_dir[i]);
            do_not_exist_count++;

        }else{

            if(S_ISDIR(sb.st_mode)){
                strcpy(directories[directory_count], file_dir[i]);
                directory_count++;
            }else{
                strcpy(files[file_count], file_dir[i]);
                file_count++;
            }
        }
    }

    for(int i = 0 ; i < do_not_exist_count ; i++){
        fprintf(stderr, "ls: cannot access '%s': No such file or directory\n", do_not_exist[i]);
    }
    if(do_not_exist_count > 0){
        printf("\n");
    }

    for(int i = 0 ; i < file_count ; i++){

        char output[4*size];
        if(l_flag == 1){
            sprintfData(output, files[i], files[i]);
            printf("%s\n", output);
        }else{
            char color[30];
            getColor(color, files[i], files[i]);
            printf(" %s%s%s\n", color, files[i], "\033[0m\033[0m");
        }
    }
    if(file_count > 0){
        printf("\n");
    }

    int too_big_count = 0;
    for(int i = 0 ; i < directory_count ; i++){

        char dir_contents[200][size];
        int dir_contents_count = 0;

        struct dirent *dt;
        DIR *dir = opendir(directories[i]);

        int count = 0;
        dt = readdir(dir);
        while(dt != NULL){
            if(dt->d_type == DT_REG || dt->d_type == DT_DIR){
                count++;
            }
            dt = readdir(dir);
        }
        closedir(dir);

        if(count > 200){
            strcpy(too_big[too_big_count], directories[i]);
            too_big_count++;
            continue;
        }

        dir = opendir(directories[i]);
        dt = readdir(dir);
        while(dt != NULL){
            if(a_flag == 1 || (a_flag == 0 && dt->d_name[0] != '.')){
                strcpy(dir_contents[dir_contents_count], dt->d_name);
                dir_contents_count++;
            }
            dt = readdir(dir);
        }
        closedir(dir);

        sort(dir_contents, dir_contents_count);

        char outputs[dir_contents_count][2*size];
        long int total = 0;

        for(int j = 0 ; j < dir_contents_count ; j++){
            if(l_flag == 1){
                char temp[2*size];
                sprintf(temp, "%s/%s", directories[i], dir_contents[j]);
                total += sprintfData(outputs[j], temp, dir_contents[j]);
            }else{
                char temp[2*size];
                sprintf(temp, "%s/%s", directories[i], dir_contents[j]);
                char color[30];
                getColor(color, temp, dir_contents[j]);
                sprintf(outputs[j], " %s%s%s\n", color, dir_contents[j], "\033[0m\033[0m");
            }
        }


        if(file_dir_count > 1){
            printf("%s:\n", directories[i]);
        }
        
        if(l_flag == 1){
            printf("total %ld\n", total / 2);
        }

        for(int j = 0 ; j < dir_contents_count ; j++){
            printf("%s", outputs[j]);
        }

        if(i + 1 != directory_count){
            printf("\n");
        }
    }

    if(too_big_count > 0){
        return 2;
    }

    return 0;
}
