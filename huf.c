#include <stdio.h>
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

struct node
{
    struct node* right;
    struct node* left;
    int data;
    char lettre;
};
typedef struct node node;

node* new_node(int iData, char lettre){
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node) {
        new_node->data = iData;
        new_node->lettre = lettre;
        new_node->right = NULL;
        new_node->left = NULL;
    }
    return new_node;
}

char* intAtocharA(int* k,int n){
	char *z = calloc(n, sizeof(char));
	for (int i = 0; i < n; ++i)
	{
		z[i]=(char)k[i];
	}
	return z;
}
int serachIndex(kodlar** K,int taille, char c){
    for (int i = 0; i < taille; ++i)
    {
        if (K[i]->karakter == c)
            return i;
    }
}
int isLeaf(node* root)
{ 
    return !(root->left) && !(root->right); 
} 

void printArr(int arr[], int n) 
{ 
    int i; 
    for (i = 0; i < n; ++i) 
        printf("%d", arr[i]); 
  
    printf("\n"); 
} 

void printCodes(node* root, int arr[], int top,int* i,kodlar** K) /*root = notre arbe arr = notre code correspondant notre charactere top = ou nous somme dans arr i = index et K est la tableau des kodlar*/
{  
        if (root->left) { 
            arr[top] = 0; 
            printCodes(root->left, arr, top + 1,i,K);
            //printf("%c\n",'l'); 
        }
        if (root->right) {
            arr[top] = 1; 
            printCodes(root->right, arr, top + 1,i,K); 
            //printf("%c\n",'r');
        }
        if (isLeaf(root)) { 
            //printArr(arr,top);
            //printf("%d\n",top);          
            K[*i]=yenikod((root->lettre),arr,top);
            *i = *i + 1;
            //printf("%c\n",root->lettre);
            //printArr(K[*i]->code,K[*i]->codesize);
            //printf("%i en son if te i \n",*i );
        }
}

void EkleveSirala(node* tmp,node** A,int* taille){
    node* tmp1 = (node*)malloc(sizeof( node));
    A[*taille]=tmp;
    *taille = *taille + 1;
    int swapped;
    for (int i = 0; i < *taille-1; i++) 
    { 
        swapped = 0; 
        for (int j = 0; j < *taille-i-1; j++) 
        { 
            if (A[j]->data > A[j+1]->data) 
            { 
                tmp1 = A[j];
                A[j]=A[j+1];
                A[j+1]=tmp1;
                swapped = 1; 
            } 
        } 
     if (swapped == 0) 
        break; 
   }
}


node* Huff( node** A,int taille){
    node* tmp = (node*)malloc(sizeof(node));
    char a = ' ';
    while(taille != 1){
        tmp = new_node((A[0]->data+A[1]->data),a);
        //printf("%i\n", taille);
        tmp->right = A[0];
        tmp->left = A[1];
        A[0]=A[taille-1];
        A[1]=A[taille-2];
        taille = taille - 2;
        EkleveSirala(tmp,A,&taille);
        //printf("%i\n",taille);
    }
    return tmp;
}


int kactane(char *chemin,int compte[]){ //combien
    FILE* fic = fopen(chemin, "r");
    if (!fic)
    {
        fprintf(stderr,"impossible d ouvrir le fichier %s !", chemin);
        return 1;
    }
    
    int c;
    while (EOF != (c=fgetc(fic))){
        compte[c]++;
    }
    fclose(fic);
    
    return 0;
}

void trier(int T[],int size, char chr[]){
    int temp;
    char tempc;
    for (int i = 0; i < size; ++i)
    {
        for(int j= i+1;j < size; ++j)
        {
            if (T[j]<T[i])
            {
                temp=T[i];
                T[i]= T[j];
                T[j]=temp;

                tempc = chr[i];
                chr[i] = chr[j];
                chr[j] = tempc;
            }
        }
    }
}

void creeCharTab(char Tchar[]){
    for (int i = 0; i < 256; ++i)
    {
        Tchar[i] = i;
    }
}

int calculProba(char *chemin, float* proba){
    FILE* fic=fopen(chemin,"r");
    if (!fic){
        fprintf(stderr,"Impossible d’ouvrir le fichier %s !",chemin);
        return 1; 
    }
    int compte[256]={0};
    int taille=0;
    int c;
    while(EOF!=(c=fgetc(fic))){
        compte[c]++;
        taille++;
    }
    fclose(fic);
    if (taille==0)
        return 0;  
    for(int i=0;i<256;i++){
        proba[i]=compte[i]/(float)taille; 
    }
}

int main(int argc, char *argv[], char *env[]) {
    if (argc != 4)
    {
        printf("syntaxe: %s <Fichier qu'on veut compresser> <Fichier des code Huffman> <Fichier des cles pour la programme dehuf>  \n", argv[0]);
        return 1;
    }
    char Tchar[256];
    creeCharTab(Tchar);
    int T[256] = {0};
    kactane(argv[1], T);
    trier(T,256, Tchar);
    int taille = 0;
    node* A[256];
    for (int i = 0; i < 256; ++i)
    {
        if (T[i]!=0)
        {
            A[taille] = new_node(T[i], Tchar[i]);
            taille++;
        }
    }

    node *HuffTree = Huff(A,taille);
    int arr[MAX_TREE_HT], top = 0; 
    int p = 0;
    //printf("%i fonksiyona girmeden önce:\n",taille );
    kodlar* K[taille];
    printCodes(HuffTree,arr,top,&p,K);
    for (int iter = 0; iter < taille; ++iter)
    {
        printf("%c ", K[iter]->karakter);
        printArr(K[iter]->code,K[iter]->codesize);
    }
    float proba[256];
    if (!calculProba(argv[1],proba)){ 
        for(int i=0;i<256;i++){
            if(proba[i]!=0){
                printf("car : %c de code : %x de proba : %f\n",i,i,proba[i]);
            }
        } 
    }
    FILE* fcle = fopen(argv[3],"w");
    for (int i = 0; i < taille; ++i)
    {
    	fprintf(fcle, "%d",K[i]->karakter);
        fprintf(fcle, ":");
        for (int c = 0; c < K[i]->codesize; ++c)
        {
        	fprintf(fcle, "%d", K[i]->code[c]);
        }
        fprintf(fcle, "\n");
    }
    FILE* fic = fopen(argv[1], "r");
    FILE* fout = fopen(argv[2],"w"); 
    char c;
    while (EOF != (c=fgetc(fic))){   
        int b = serachIndex(K,taille,c); 
        for (int i = 0; i < K[b]->codesize; ++i)
        {
        	fprintf(fout,"%d",K[b]->code[i]);     	
        } 
        fprintf(fout," ");
    } 
    	fclose(fic);
    	fclose(fout);
    	fclose(fcle);
    return 0;
}



