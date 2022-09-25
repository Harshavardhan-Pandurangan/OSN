#include "Headers.h"

extern char homeDir[size];

int historyComm(char *input, int type){
    
    FILE *file;
    char history_file[2*size];
    sprintf(history_file, "%s/.history_temp.txt", homeDir);

    file = fopen(history_file, "r");
    
    if(file == NULL){
        file = fopen(history_file, "w");
        fclose(file);
        file = fopen(history_file, "r");
    }


    if(type == 0){          // add command line
        
        char line[size];
        
        char curr_history[200][size];
        int curr_history_count = 0;
        
        while(fscanf(file, "%[^\n]\n", line) != EOF){
            strcpy(curr_history[curr_history_count], line);
            curr_history_count++;
        }

        fclose(file);

        if(strcmp(input, curr_history[curr_history_count-1]) == 0){
            return 0;
        }

        if(curr_history_count < 20){
            
            file = fopen(history_file, "a");
            fprintf(file, "%s\n", input);
        
        }else{

            file = fopen(history_file, "w");
            for(int i = 1 ; i < curr_history_count ; i++){
                fprintf(file, "%s\n", curr_history[i]);
            }
            fprintf(file, "%s\n", input);

        }

        fclose(file);

    }else if(type == 1){    // display history
        
        char line[size];
        //printf("1\n");
        //printf("ok\n");
        for(int i = 0 ; i < strlen(input) ; i++){
            if(input[i] == ' '){
                return 1;
            }else if(input[i] < 48 || input[i] > 57){
                return 2;
            }
        }

        char curr_history[200][size];
        int curr_history_count = 0;
        
        while(fscanf(file, "%[^\n]\n", line) != EOF){
            strcpy(curr_history[curr_history_count], line);
            curr_history_count++;
        }

        long long int iter = (curr_history_count == 20) ? 10 : curr_history_count % 10;
        if(strlen(input) > 0){
            iter = atoll(input);
        }

        if(iter > 20){
            fprintf(stderr, "history: maximum of 20 previous commands stored: asked for %lld\n\n", iter);
        }

        for(int i = (iter > curr_history_count) ? 0 : curr_history_count - iter ; i < curr_history_count ; i++){
            printf("%s\n", curr_history[i]);
        }
        //printf("2\n");

        fclose(file);

    }

    return 0;
}