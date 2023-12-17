#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// En uzun ortak alt dizi uzunluğunu ve alt dizileri tutan yapı
typedef struct {
    int index;
    int length;
    char **subsequences;
}LCSResult;
void printCost(int** matrix,int N, int M);
void printB(int** matrix,int N, int M);
void LCS(int** cost, int** b, char** str);

void findLCS(int** b, char** str,LCSResult* Subseq, int i, int j,int k);


int main(){
    char **str, c;//**str:strings that they are entered by user. c:each char that entered by user to enter the strings
    int i,j,k;
    int lcs_size;//longest size=cost[N][M]
    int N,M;//N:row , M:column
    int **cost,**b;//if b[i][j]==1 it means addxy,else if b[i][j]==2 it means skipx ,else if b[i][j]==3 it means skipy,
    //else if b[i][j]==4 it means that b[i-1][j] and b[i][j-1] are same

    LCSResult* Subseq = NULL;
    Subseq = (LCSResult*)malloc(sizeof(LCSResult));
    Subseq->index=0;
    Subseq->subsequences=NULL;
    // Subseq.subsequences=(char**)calloc(sizeof(char*),1);
    str = (char **)malloc(sizeof(char *)*2);  // here 2 indicates 2 (char*)
    str[0]=(char *)malloc(sizeof(char))  ; // here 10 indicates 10 (char)
    str[1]=(char *)malloc(sizeof(char));
    for (i=0;i<2;i++){
        c= ' ';
        printf("Enter String [%d] : ",i);
        j=0;
        while (c != '\n') {
            c = getc(stdin);    // read the input from keyboard standard input
            str[i] = (char*)realloc(str[i], j+1 * sizeof(char));    // re-allocate (resize) memory for character read to be stored
            str[i][j] = c;    // store read character by making pointer point to c
            j++;
        }
        str[i][j-1] = '\0'; // at the end append null character to mark end of string
        printf("\nThe entered string is : %s\n", str[i]);  
    }
    N=strlen(str[0])+1;//+1 for 0 row
    M=strlen(str[1])+1;//+1 for 0 row
    printf("N:%d M:%d\n",N,M);
    cost = (int**)malloc(sizeof(int*)*(N));
    b = (int**)malloc(sizeof(int*)*(N));
    if(cost==NULL && b==NULL){
        printf("Memory not allocated");
        exit(1);
    }
    for(i=0;i<N;i++){
        cost[i] = (int*) malloc(sizeof(int)*(M));
        b[i] = (int*) malloc(sizeof(int)*(M));
        if(cost[i]==NULL && b[i]==NULL){
            printf("Memory not allocated");
            exit(1);
        }
        for(j=0;j<M;j++){
            cost[i][j]=0;
            b[i][j]=0;
        }
    }
    LCS(cost,b,str);

    Subseq->length=cost[N-1][M-1];
    findLCS(b,str,Subseq, N-1, M-1,Subseq->length);

    for(i=0;i<(Subseq->index);i++){
        free(Subseq->subsequences[i]);
    }
    free(Subseq->subsequences);
    free(Subseq);
    for(i=0;i<N;i++){
        free(cost[i]);
    }
    free(cost);
    for(i=0;i<N;i++){
        free(b[i]);
    }
    free(b);
        
    free(str[0]); // important step the pointer declared must be made free   
    free(str[1]); // important step the pointer declared must be made free   
    free(str); // important step the pointer declared must be made free
    return 0;
}

void findLCS(int** b, char** str,LCSResult* Subseq, int i, int j,int k){
    /*This function is used to trace the longest common substring.*/
    int index=(Subseq->index);
    int tmp;
    if((Subseq->subsequences)==NULL){
        Subseq->subsequences=(char**)calloc(sizeof(char*),1);
        Subseq->subsequences[index]=(char*)malloc(sizeof(char)*(Subseq->length));//first lcs
        Subseq->subsequences[index][Subseq->length]='\0';
    }
    
    while(i!=0 && j!=0){
        switch(b[i][j]){
            case 0://daha onceki elemanlarda ayni olan kalmamistir.
                i=0;
                j=0;
                break;
            case 1://bu char lcs de vardir
                
                Subseq->subsequences[Subseq->index][k-1]=str[0][i-1];
                i--;
                j--;
                k--;
                break;
            case 2://en uzun hali y den gelmis
                i--;
                break;
            case 3://en uzun hali x den gelmis
                j--;
                break;
            case 4:
                Subseq->index++;
                Subseq->subsequences=(char**)realloc(Subseq->subsequences,sizeof(char*)*(index+1));
                Subseq->subsequences[Subseq->index]=(char*)malloc(sizeof(char)*(Subseq->length));
                strcpy(Subseq->subsequences[Subseq->index],Subseq->subsequences[index]);
                findLCS(b,str,Subseq,i-1,j,k);
                Subseq->index++;
                Subseq->subsequences=(char**)realloc(Subseq->subsequences,sizeof(char*)*(index+1));
                Subseq->subsequences[Subseq->index]=(char*)malloc(sizeof(char)*(Subseq->length));
                strcpy(Subseq->subsequences[Subseq->index],Subseq->subsequences[index]);            
                findLCS(b,str,Subseq,i,j-1,k);
                i=-1;
                j=-1;
                break;
        }
    } 
    if(i==0 || j==0){
        printf("Longest Common Subsequence : %s\n",Subseq->subsequences[index]);
    }   
    free(Subseq->subsequences[index]);
}


void LCS(int** cost, int** b, char** str){
    int i,j;
    int N,M;
    N=strlen(str[0])+1;
    M=strlen(str[1])+1;
    for(i=1;i<N;i++){
        for(j=1;j<M;j++){
            if(str[0][i-1]==str[1][j-1]){//ADDXY
                cost[i][j]=cost[i-1][j-1]+1;
                b[i][j]=1;
            }
            else if(cost[i-1][j]>cost[i][j-1]){//SKIPY
                cost[i][j]=cost[i-1][j];
                b[i][j]=2;
            }
            else if(cost[i-1][j]<cost[i][j-1]){//SKIPX
                cost[i][j]=cost[i][j-1];
                b[i][j]=3;
            }
            else{//SAME LONG
                cost[i][j]=cost[i][j-1];//it does not matter
                b[i][j]=4;                
            }            
        }
        printf("\t\tCOST MATRIX\n");
        printCost(cost,N,M);
        printf("\t\tWAY MATRIX\n");  
        printB(b,N,M);
        printf("\n");
    }
}




void printCost(int** matrix,int N, int M){
    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<M;j++){
            printf("  %d   ",matrix[i][j]);
        }
        printf("\n");
    }
}
void printB(int** matrix,int N, int M){
    int i,j;
    for(i=0;i<N;i++){
        for(j=0;j<M;j++){
            if(matrix[i][j]==1){
                printf("ADDXY ");
            }
            else if(matrix[i][j]==2){
                printf("SKIPX ");
            }            
            else{
                printf("SKIPY ");
            }
        }
        printf("\n");
    }
}