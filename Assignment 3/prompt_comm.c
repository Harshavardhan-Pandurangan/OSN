#include "Headers.h"

extern char homeDir[size];

// Directory value to be printed
void setDirectory(char *directory){
    char currDir[size];
    if(getcwd(currDir, size) == NULL){
        fprintf(stderr, "Shell: Cannot get directory details\n");
        exit(0);
    }

    if(strcmp(currDir, homeDir) == 0){
        strcpy(directory, "~");
    }else if(strlen(homeDir) < strlen(currDir) && strncmp(homeDir, currDir, strlen(homeDir)) == 0 && currDir[strlen(homeDir)] == '/'){
        sprintf(directory, "~%s", currDir + strlen(homeDir));
    }else{
        strcpy(directory, currDir);
    }
    
    return;
}

// 3 components to the shell display
// username, system deatils and directory
void promptText(time_t time){
    char user[size], system[size], directory[size];
    
    uid_t uid = getuid();
    struct passwd *pw;
    pw = getpwuid(uid);
    if(!pw || !pw->pw_name){
        fprintf(stderr, "Shell: Cannot get user details\n");
        exit(0);
    }
    strcpy(user, pw->pw_name);
    
    if(gethostname(system, size) != 0){
        fprintf(stderr, "Shell: Cannot get system details\n");
        exit(0);
    }

    setDirectory(directory);
    char timechar[size];
    if(time > 1){
        sprintf(timechar, " took %lds", time);
    }else{
        strcpy(timechar, "");
    }

    printf("%s<%s@%s%s:%s%s%s%s> %s", "\033[1m\033[34m", user, system, "\033[1m\033[0m", 
           "\033[1m\033[32m", directory, timechar, "\033[1m\033[34m", "\033[0m\033[0m");
    return;
}