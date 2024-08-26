#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char init_symbol[2];

int has_non_terminal=0;
int go_next=0;
int is_last_symbol=0;
int epsilon_found=0;

int len_visited_arr=0;

int num_term=6;
int num_non_term=4;
char term[][6]={"a\0","b\0","d\0","g\0","h\0","$\0"};
char non_term[][4]={"S\0","A\0","B\0","C\0"};

int num_rules=9;
char production_rules[][2][100]={
    {"S\0","ACB\0"},
    {"S\0","CbB\0"},
    {"S\0","Ba\0"},
    {"A\0","da\0"},
    {"A\0","BC\0"},
    {"B\0","g\0"},
    {"B\0","$\0"},
    {"C\0","h\0"},
    {"C\0","$\0"},
};

int in_visited_arr(char** visited_arr,char* symbol){
    for (int i=0;i<len_visited_arr;i++){
        if(!strcmp(visited_arr[i],symbol)){
            return 1;
        }
    }
    return 0;
}

void add_to_visited_arr(char** visited_arr,char* symbol){
    if(!in_visited_arr(visited_arr,symbol)){
        char* tempstr=(char*)malloc(10*sizeof(char));
        strcpy(tempstr,symbol);
        visited_arr[len_visited_arr]=tempstr;
        len_visited_arr++;
    }
    // printf("\n");
    // for(int i=0;i<len_visited_arr;i++){
    //     printf("%s\n",visited_arr[i]);
    // }
    // printf("\n");
}

int non_terminalCheck(char* symbol){
    for(int i=0;i<num_non_term;i++){
        if(!strcmp(symbol,non_term[i])){
            return 1;
        }
    }
    return 0;
}

int terminalCheck(char* symbol){
    for(int i=0;i<num_term;i++){
        if(!strcmp(symbol,term[i])){
            return 1;
        }
    }
    return 0;
}

char** production(int* len_symbol_production,char* symbol){
    char** result=(char**)malloc(100*sizeof(char*));
    for(int i=0;i<num_non_term;i++){
        if(!strcmp(symbol,non_term[i])){
            for(int i=0;i<num_rules;i++){
                if(!strcmp(symbol,production_rules[i][0])){
                    char* tempsymbol=(char*)malloc(10*sizeof(char));
                    tempsymbol=production_rules[i][1];
                    result[*len_symbol_production]=tempsymbol;
                    (*len_symbol_production)++;
                }
            }
        }
    }
    char* tempsymbol=(char*)malloc(1*sizeof(char));
    tempsymbol="\0";
    result[*len_symbol_production]=tempsymbol;
    return result;
}


void first(char** visited_arr,char* previous_symbol,char* symbol){
    // has_non_terminal=0;
    char** symbol_production;
    int len_symbol_production=0;
    if(terminalCheck(symbol)){
        if(symbol[0]!='$' || is_last_symbol){
            if(!in_visited_arr(visited_arr,symbol)){
                add_to_visited_arr(visited_arr,symbol);
                printf("%c ",symbol[0]);   
            }
        }
        else{
            if(!strcmp(previous_symbol,init_symbol)){
                printf("%c ",symbol[0]);
            }
            epsilon_found=1;
        }
        return;
    }
    else{
        symbol_production=production(&len_symbol_production,symbol);
    }
    // printf("%d\n",len_symbol_production);
    // printf("%s\n",symbol_production[0]);
    if(len_symbol_production==0){
        printf("Invalid symbol\n");
        exit(0);
        return;
    }
    else if(in_visited_arr(visited_arr,symbol) && !is_last_symbol){
        // printf("Already Visited");
        return;
    }
    else{
        add_to_visited_arr(visited_arr,symbol);
        for (int i=0;i<len_symbol_production;i++){
            has_non_terminal=0;
            go_next=0;
            if(previous_symbol[0]=='\0'){
                is_last_symbol=0;
            }
            for(int j=0;symbol_production[i][j]!='\0';j++){
                epsilon_found=0;
                char tempsymbol[2]={symbol_production[i][j]+'\0'};
                if(non_terminalCheck(tempsymbol)){
                    if(previous_symbol[0]=='\0' && symbol_production[i][j+1]=='\0'){
                        is_last_symbol=1;
                    }
                }
                first(visited_arr,symbol,tempsymbol);
                if(go_next){
                    break;
                }
                // if(is_last_symbol){
                //     return;
                // }

                // if(epsilon_found){
                //     return;
                // }
                if(non_terminalCheck(tempsymbol)){
                    has_non_terminal=1;
                }

                if(terminalCheck(tempsymbol) && tempsymbol[0]!='$'){
                    if(!has_non_terminal && symbol_production[i][j+1]=='\0'){
                        go_next=1;
                    }
                    break;
                }
            }
        }
    }
}
    // else if((!is_last_symbol ||terminalCheck(symbol)) && in_visited_arr(visited_arr,symbol)){
    //     // printf("Already Visited\n");
    //     return;
    // }
    // else{
    //     if(!is_last_symbol && strcmp(symbol,"$")){
    //         add_to_visited_arr(visited_arr,symbol);
    //     }
    //     if(len_symbol_production==1 && terminalCheck(symbol)){
    //         if(symbol[0]=='$'){
    //             epsilon_found=1;
    //             if(!(is_last_symbol || !strcmp(previous_symbol,init_symbol))){
    //                 return;
    //             }
    //             add_to_visited_arr(visited_arr,"$\0");
    //         }
    //         printf("%c ",symbol[0]);
    //     }
    //     else{
    //         for(int i=0;i<len_symbol_production;i++){
    //             for(int j=0;j>=0;j++){
    //                 if(symbol_production[i][j]=='\0'){
    //                     break;
    //                 }
    //                 is_last_symbol=0;
    //                 char tempsymbol[2]={symbol_production[i][j],'\0'};
    //                 if(terminalCheck(tempsymbol)){
    //                     j=-1;
    //                     i++;
    //                 }
    //                 if(symbol_production[i][j+1]=='\0' && non_terminalCheck(tempsymbol)){
    //                     is_last_symbol=1;
    //                 }
    //                 first(visited_arr,symbol,tempsymbol);
    //                 if(!strcmp(previous_symbol,"\0")){
    //                     go_back=0;
    //                 }
    //                 if(go_back){
    //                     break;
    //                 }
    //                 if(!epsilon_found && symbol_production[i][j+1]=='\0' && !non_terminalCheck(symbol)){
    //                     go_back=1;
    //                     i=len_symbol_production;
    //                     break;
    //                 }
    //             }
    //             epsilon_found=0;
    //         }
    //         // for(int i=0;i<strlen(symbol_production);i++){
    //         //     char tempsymbol[2]={symbol_production[i],'\0'};
    //         //     first(visited_arr,tempsymbol);
    //         // }
    //     }
    // }

int main(){
    scanf("%s",init_symbol);
    printf("First(%s) = { ",init_symbol);
    char** visited_arr=(char**)malloc(100*sizeof(char*));
    // char* first_arr=(char*)malloc(100*sizeof(char));
    first(visited_arr,"\0",init_symbol);
    printf("}");

    // for(int i=0;i<num_term;i++){
    //     printf("%s",term[i]);
    // }
    // printf("\n");

    // for(int i=0;i<num_non_term;i++){
    //     printf("%s",non_term[i]);
    // }
    // printf("\n");

    // for(int i=0;i<2;i++){
    //     for(int j=0;j<2;j++){
    //         printf("%s ",production_rules[i][j]);
    //     }
    //     printf("\n");        
    // }

    return 0;
}
