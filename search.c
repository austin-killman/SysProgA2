#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
int binarySearch(Offset arr[],int l,int r,char * searchName);

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

int main(int a, char** args) {
    FILE * file = fopen("index.bin","rb");
    u_int32_t lengthBuffer;
    long offsetBuffer;
    Offset * nameOffsetArray = malloc(0);
    int i =1;
    do{
        nameOffsetArray =realloc(nameOffsetArray, sizeof(Offset)*i);
        nameOffsetArray[i-1].name = malloc(1);
        nameOffsetArray[i-1].name[0]='\0';
        fread(&lengthBuffer,sizeof(lengthBuffer),1,file);
        nameOffsetArray[i-1].nameLength = lengthBuffer;
        char c = 'a';
        int j=1;
        for(int k=0;k<lengthBuffer;k++)
        {
            fread(&c,sizeof(c),1,file);
            nameOffsetArray[i-1].name = realloc(nameOffsetArray[i-1].name,++j);
            strncat(nameOffsetArray[i-1].name,&c,sizeof(char));
        }

        fread(&offsetBuffer,sizeof(offsetBuffer),1,file);
        nameOffsetArray[i-1].offset = offsetBuffer;
        i++;
    }while(offsetBuffer!=0);
    int offsetArrayLength = i-1;
    // IF we want to make ful proof find largest size of inputted length
    char * searchName;
    char c;
    int j=1;
    fclose(file);

    while(true)
    {
        searchName = malloc(1);
        searchName[0]='\0';

        while (( c = fgetc(stdin))!='\n')
        {
            if(!isatty(fileno(stdin))){
                // printf("%c",c);
            }
            searchName = realloc(searchName,++j);
            strncat(searchName,&c,sizeof(char));
        }
        printf(">> %s\n", searchName);
        if(strcmp(searchName,"q")==0)
        {
            free(searchName);
            break;
        }
        int index = binarySearch(nameOffsetArray,0,offsetArrayLength-1,searchName);
        if(index<0)
        {
            printf("./search: \'%s\' not found!\n",searchName);
        }else
        {
            FILE * indexFile = fopen("cards.bin","rb");
            fseek(indexFile,nameOffsetArray[index].offset,SEEK_SET);
            Card displayCard;

            displayCard.name = malloc(strlen(nameOffsetArray[index].name)+ 1);
            strcpy(displayCard.name,nameOffsetArray[index].name);

            u_int32_t a = 0;
            fread(&a,sizeof(u_int32_t),1,indexFile);
            displayCard.id = a;

            fread(&a,sizeof(u_int32_t),1,indexFile);
            displayCard.cost=malloc(1);
            displayCard.cost[0] = '\0';
            char character;
            for(int i =0;i<a;i++){
                fread(&character,1,1,indexFile);
                displayCard.cost = realloc(displayCard.cost,i+2);
                strncat(displayCard.cost,&character,sizeof(char));
            }

            fread(&displayCard.converted_cost,sizeof(u_int32_t),1,indexFile);

            fread(&a,sizeof(u_int32_t),1,indexFile);
            displayCard.type=malloc(1);
            displayCard.type[0] = '\0';
            for(int i =0;i<a;i++){
                fread(&character,1,1,indexFile);
                displayCard.type = realloc(displayCard.type,i+2);
                strncat(displayCard.type,&character,sizeof(char));
            }

            fread(&a,sizeof(u_int32_t),1,indexFile);
            displayCard.text=malloc(1);
            displayCard.text[0] = '\0';
            for(int i =0;i<a;i++){
                fread(&character,1,1,indexFile);
                displayCard.text = realloc(displayCard.text,i+2);
                strncat(displayCard.text,&character,sizeof(char));
            }

            fread(&a,sizeof(u_int32_t),1,indexFile);
            displayCard.stats=malloc(1);
            displayCard.stats[0] = '\0';
            for(int i =0;i<a;i++){
                fread(&character,1,1,indexFile);
                displayCard.stats = realloc(displayCard.stats,i+2);
                strncat(displayCard.stats,&character,sizeof(char));
            }

            fread(&displayCard.rarity, sizeof(enum rarity), 1,indexFile);
            fclose(indexFile);
            printf("%s%*s\n",displayCard.name, (52-(int)strlen(displayCard.name)),displayCard.cost);
            printf("%s%*s\n",displayCard.type, (52-(int)strlen(displayCard.type)),getRarity(displayCard.rarity));
            printLine();
            printf("%s\n",displayCard.text);
            printLine();
            printf("%*s\n\n",52, displayCard.stats);

            free(displayCard.name);
            free(displayCard.stats);
            free(displayCard.text);
            free(displayCard.type);
            free(displayCard.cost);
        }

        free(searchName);
    }
    for(int i=0;i<offsetArrayLength;i++)
    {
        free(nameOffsetArray[i].name);
    }
    free(nameOffsetArray);
}

int binarySearch(Offset arr[],int l,int r,char * searchName){
    if(r>=l)
    {
        int mid  = l+(r-1)/2;
        if(strcmp(arr[mid].name,searchName)==0)
        {
            return mid;
        }
        if(strcmp(arr[mid].name,searchName)>0)
        {
            return binarySearch(arr,l,mid-1,searchName);
        }
        return binarySearch(arr,mid+1,r,searchName);
    }

    return -1;
}