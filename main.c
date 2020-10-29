#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "main.h"

int getFileLineNumber(char fileName[]);
void getData(char fileName[]);
void printLine();
const char* getRarity(enum rarity rar);

int compFunc(const void * a, const void  * b){
    return (strcmp((*(Card *)a).name, (*(Card *)b).name));
}

int compFunc2(const void * a, const void  * b){
    return (strcmp((*(Offset *)a).name, (*(Offset *)b).name));
}

int defineRarirty(char * rarity) {

    if((strcmp(rarity, "common"))==0){
        return common;
    }else if((strcmp(rarity, "uncommon"))==0){
        return uncommon;
    }else if(strcmp(rarity, "rare")==0){
        return rare;
    }else if((strcmp(rarity, "mythic"))==0){
        return mythic;
    }else {
        return 4;
    }
}

int main(int a, char** args) {
    
    char file[1000] = "./";
    char * input = args[1];
    strcat(file, input);
  
    if((open(file,O_RDONLY))!=-1 ){
        getData(file);
    }else{
        fprintf(stderr,"./parser: cannot open(\"does-not-exist.csv\"): No such file or directory");
        // printf("./parser: cannot open(\"%s\"): No such file or directory",input);
    }
    
}

int getFileSize(char fileName[]) {
    FILE * filePtr = fopen(fileName, "r"); 
    int lines=0;
    int character;
    while(!feof(filePtr)){
        //while for nav line
        character = getc(filePtr);
        if(character == '\n'){
            ++lines;
        }
    }
    fclose(filePtr);
    return lines;
}

void getData(char fileName[]){
    int fileSize = getFileSize(fileName);
    fileSize--;
    Card * allCards = malloc(0);
    FILE * filePtr = fopen(fileName, "r"); 
    int cardIndex =0;
    char ** fields = calloc(8,sizeof(char*));

    while(getc(filePtr) != '\n' && getc(filePtr)!= EOF) {}

    for(int lineNum=0; lineNum<=fileSize; lineNum++){
        char * line = malloc(1);
        line[0] = '\0';
        int lineLength = 1;
        char character='a';
        bool quoteCheck = false;
        bool doubleQuoteCheck = false;

        while(character != '\n' && character != EOF) {
            character = getc(filePtr);
            line = realloc(line, ++lineLength);
            strncat(line, &character, 1);
        }

        char c2;
        int j=0;
        
        
        for (int k=0; k<8; k++){
            fields[k] = malloc(1);
            fields[k][0] = '\0';
        }

        int fieldNum = 0;
        int length = 0;
        bool slashCheck = false;
        bool ndouble = false;
        while((c2 = line[j]) != '\0' && c2 != '\n'){
            if(!quoteCheck || c2 != '"' || length !=1 ){
                
                if(doubleQuoteCheck == true && c2 == '"'){
                    fields[fieldNum] = realloc(fields[fieldNum], ++length*sizeof(char));
                    strncat(fields[fieldNum], &c2, 1);
                    ndouble = true;
                }
                doubleQuoteCheck = false;
                if(c2 == '"'){
                    slashCheck = false;
                    quoteCheck = !quoteCheck;
                    if(!ndouble){
                        doubleQuoteCheck = true;
                        ndouble = false;
                    }
                }else{
                    ndouble = false;
                    if(quoteCheck == false && c2 == ',' ){
                        length = 1;
                        fieldNum++;
                    }else{ // add anything else that needs to be checked for string stuff
                        // if(slashCheck && c2 == 'n'){
                        //     fields[fieldNum] = realloc(fields[fieldNum], ++length*sizeof(char*));
                        //     strncat(fields[fieldNum], (char *)'\n', 1);
                        //     slashCheck =false;
                            
                        // }
                        if(c2 == '\\') {
                            
                            slashCheck = true;
                        }
                        else if(slashCheck && c2 == 'n'){
                            char ok='\n';
                            length = length+1;
                            fields[fieldNum] = realloc(fields[fieldNum], length*sizeof(char*));
                            strncat(fields[fieldNum],&ok,1);
                            slashCheck=false;
                        }
                        else{
                            fields[fieldNum] = realloc(fields[fieldNum], ++length*sizeof(char*));
                            strncat(fields[fieldNum], &c2, 1);
                            slashCheck=false;
                        }        
                    }
                }
            }else{
                // length = 1;
                // fieldNum++;
                quoteCheck = !quoteCheck;
            }
            ++j;
        }
        bool addNewCard = true;
        
        for(int i =0; i < cardIndex; i++){
            if(strcmp(fields[1], allCards[i].name)==0) {
                if(atoi(fields[0]) > allCards[i].id) {
                    //allCards = realloc(allCards, ++cardIndex * sizeof(Card));
                    free(allCards[i].name);
                    free(allCards[i].cost);
                    free(allCards[i].type );
                    free(allCards[i].text);
                    free(allCards[i].stats);
                    allCards[i].id = atoi(fields[0]); 

                    allCards[i].name = calloc(strlen(fields[1])+1,1);
                    strcpy(allCards[i].name,fields[1]);


                    allCards[i].cost = calloc(strlen(fields[2])+1,1);
                    strcpy(allCards[i].cost,fields[2]);

                    allCards[i].converted_cost = atoi(fields[3]); 

                    allCards[i].type = calloc(strlen(fields[4])+1,1);
                    strcpy(allCards[i].type,fields[4]);


                    allCards[i].text = calloc(strlen(fields[5])+1,1);
                    strcpy(allCards[i].text,fields[5]);


                    allCards[i].stats = calloc(strlen(fields[6])+1,1);
                    strcpy(allCards[i].stats,fields[6]);
                
                    allCards[i].rarity = defineRarirty(fields[7]); 
                }
                addNewCard = false;
                break;
            }
            addNewCard = true;
        }
        
        if(addNewCard == true){
            allCards = realloc(allCards, ++cardIndex * sizeof(Card));

            allCards[cardIndex-1].id = atoi(fields[0]); 

            allCards[cardIndex-1].name = calloc(strlen(fields[1])+1,1);
            strcpy(allCards[cardIndex-1].name,fields[1]);


            allCards[cardIndex-1].cost = calloc(strlen(fields[2])+1,1);
            strcpy(allCards[cardIndex-1].cost,fields[2]);

            allCards[cardIndex-1].converted_cost = atoi(fields[3]); 

            allCards[cardIndex-1].type = calloc(strlen(fields[4])+1,1);
            strcpy(allCards[cardIndex-1].type,fields[4]);


            allCards[cardIndex-1].text = calloc(strlen(fields[5])+1,1);
            strcpy(allCards[cardIndex-1].text,fields[5]);


            allCards[cardIndex-1].stats = calloc(strlen(fields[6])+1,1);
            strcpy(allCards[cardIndex-1].stats,fields[6]);
        
            allCards[cardIndex-1].rarity = defineRarirty(fields[7]); 
        }
        free(fields[0]);
        free(fields[1]);
        free(fields[2]);
        free(fields[3]);
        free(fields[4]);
        free(fields[5]);
        free(fields[6]);
        free(fields[7]);
        free(line);
    }
    fclose(filePtr);


    FILE * theRest = fopen("cards.bin","wb");
    u_int32_t len =0;
    Offset locationArray[cardIndex-1];

    for(int i =0; i < cardIndex-1; i++){
       
        locationArray[i].offset = ftell(theRest);
        locationArray[i].name = calloc(strlen(allCards[i].name)+1,1);
        strcpy(locationArray[i].name,allCards[i].name);
        printf("%s ||",allCards[i].name);
        printf("%s || %s ||",allCards[i].cost,allCards[i].type);
        printf("%s\n\n----------------------------------------------------\n",allCards[i].text);


        fwrite(&allCards[i].id,sizeof(allCards[i].id),1,theRest);

        len = (u_int32_t)strlen(allCards[i].cost);
        fwrite(&len,sizeof(u_int32_t), 1,theRest);
        int j=0;
        while(allCards[i].cost[j]!='\0'){
            fwrite(&allCards[i].cost[j],sizeof(char), 1,theRest);
            j++;
        }


        fwrite(&allCards[i].converted_cost,sizeof(allCards[i].converted_cost), 1,theRest);

        len = strlen(allCards[i].type);
        fwrite(&len,sizeof(u_int32_t), 1,theRest);       
        j=0;
        while(allCards[i].type[j]!='\0'){
            fwrite(&allCards[i].type[j],sizeof(char), 1,theRest);
            j++;
        }


        len = strlen(allCards[i].text);
        fwrite(&len,sizeof(u_int32_t), 1,theRest);       
        j=0;
        while(allCards[i].text[j]!='\0'){
            fwrite(&allCards[i].text[j],sizeof(char), 1,theRest);
            j++;
        }


        len = strlen(allCards[i].stats);
        fwrite(&len,sizeof(u_int32_t), 1,theRest);       
        j=0;
        while(allCards[i].stats[j]!='\0'){
            fwrite(&allCards[i].stats[j],sizeof(char), 1,theRest);
            j++;
        }

        fwrite(&allCards[i].rarity,sizeof(enum rarity), 1,theRest);

    }
    fclose(theRest);
    qsort(allCards, cardIndex, sizeof(Card), compFunc);
    qsort(locationArray,cardIndex-1,sizeof(Offset), compFunc2);
    FILE * write = fopen("index.bin","wb");

    for(int i =1; i < cardIndex; i++){
//---------------------------------------------------------------------------
        locationArray[i].nameLength = strlen(allCards[i].name);
        fwrite(&locationArray[i].nameLength,sizeof(locationArray[i].nameLength), 1,write);
        int j=0;
        while(allCards[i].name[j]!='\0'){
            fwrite(&allCards[i].name[j],sizeof(char), 1,write);
            j++;
        }
    // printf("%ld",locationArray[i-1].offset);
        if((i)<cardIndex){
            fwrite(&locationArray[i-1].offset,sizeof(long),1,write);
            free(locationArray[i-1].name);
        }else{
            long a =0;
            fwrite(&a,sizeof(a),1,write);
        }
        
        // if(strcmp(locationArray[i].name,"Orzhov Enforcer")==0){
        //     printf("%s|\n%s|\n%d|\n",allCards[i+1].name,allCards[i+1].cost,allCards[1+i].converted_cost);
        //     printf("%s|\n%s|\n%s|\n%d|\n",allCards[i+1].type,allCards[i+1].text,allCards[1+i].stats,allCards[i+1].rarity);
        
        // }


        free(allCards[i-1].name);
        free(allCards[i-1].cost);
        free(allCards[i-1].type );
        free(allCards[i-1].text);
        free(allCards[i-1].stats);
    }
    fclose(write);
    free(allCards[cardIndex-1].name);
    free(allCards[cardIndex-1].cost);
    free(allCards[cardIndex-1].type );
    free(allCards[cardIndex-1].text);
    free(allCards[cardIndex-1].stats);
    free(allCards);
    free(fields);
}

const char* getRarity(enum rarity rar) 
{
    switch (rar) 
    {
        case common: return "common";
        case uncommon: return "uncommon";
        case rare: return "rare";
        case mythic: return "mythic";
        default: return "wRONG";
    }
}

void printLine() {
    printf("----------------------------------------------------\n");
}