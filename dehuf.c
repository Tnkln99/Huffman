#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TREE_HT 50

struct kodlar{
    char karakter;
    int codesize;
    int code[MAX_TREE_HT];
};
typedef struct kodlar kodlar;

kodlar* yenikod(char karakter, int* code,int codesize){
    kodlar* yenikod = (kodlar*)malloc(sizeof(kodlar));
    if(yenikod){
        yenikod->karakter = karakter;
        yenikod->codesize = codesize;
        for (int i = 0; i < codesize; ++i)
        {
            yenikod->code[i] = code[i];
        }
    }
    return yenikod;
}

int serachIndex(kodlar* K,int taille,int codesize, int* c){
    int result = 1;
    for (int i = 0; i < taille; ++i)
    {
        if (K[i].codesize == codesize){
            result = 1;
            for (int j = 0; j < codesize; ++j)
            {
                if (K[i].code[j]!=c[j])
                {
                    result = 0;
                    break;
                }
            }
            if (result == 1){
                return i;
            }
        }
    }
    return -1;
}

int main(int argc, char *argv[], char *env[])
{
    if (argc != 3)
    {
        printf("syntaxe: %s <Fichier des CodesEtCle de la programme Huf> <Fichier pour decompresser de la programme Huf>  \n", argv[0]);
        return 1;
    }
    kodlar K[256];
    FILE* fic = fopen(argv[1], "r");
    FILE* fcom = fopen(argv[2], "r");
    char tmp[256];
    int i = 0;
    while (fgets (tmp, 256, fic)!=NULL){
        int ascii;
        char code[256];
        //sscanf(tmp,"%d:%50[^\n]",ascii,code);
        //printf("%d:%s",ascii,code);
        //printf("%s",tmp );
        if (sscanf(tmp,"%d:%s",&ascii,code))
        {
            //printf("%d:%s\n",ascii,code);
            K[i].karakter = (char)ascii;
            K[i].codesize = 0;
            for (int j = 0; j < 256; ++j)
            {
                if (code[j]=='1' || code[j]=='0')
                {
                    if (code[j]=='1')
                    {
                        K[i].code[K[i].codesize] = 1;
                    }
                    else{ 
                        K[i].code[K[i].codesize] = 0;
                    }
                    K[i].codesize++;
                    //printf("%c",code[j]);
                }
            }
            //printf("-%d\n",K[i].codesize );
            i++;
        }
    } 
    int x[256];
    char z;
    int y = 0;
    while (EOF != (z=fgetc(fcom))){
        //printf("%s\n", "as");
        if (z==32){
            int f = serachIndex(K,i,y,x);
            if (f !=-1){
                printf("%c",(K[f].karakter));
            }
            //printf("%s\n","sa" );
            //printf(" %d, %d \n",f,y);
            y = 0;
            continue;
        } 
        x[y]=atoi(&z);  
        y++;    
    }
    fclose(fic);
    fclose(fcom);
    return 0;
}

