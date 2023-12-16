#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
typedef struct Node{//Struct of each User
	char userName[12];//12 char because of range of unsigned long long int(outcome of horner's rule for 12 chars string is 20 digits)
	int deleted;//deleted variable is 0 by default and can be 1 if data is deleted
}User;
void insertIntoHash(User* , char* , int , int);//the function to insert new user to the Hash Table
void deleteFromHash(User*, char* ,int , int);//the function to delete the user from the Hash Table
void searchInHash(User* , char* ,int, int);//the function to search the user in the Hash Table
User* reHashing(User* , int, int);//the function to reHash the HashTable except deleted users
int findPrimeGreater(int);//the function to find the smallest prime number greater than x
int* getIndex(User* , char* , int, int, char* );//the function to get info about hashIndex and finding result
int main(){
	int mode,modeChanged=0;//modeChanged=0 if mode will be changed, modeChanged=1 if mode changed
	int i,N,m,selection;//N: number of users || m:size of hashTable
	float lf;//loadfactor
	int finished=0;//to finish the program
	char name[12];
	User *HashTable;
	printf("\nEnter the number of users,please!\t");
	scanf("%d",&N);
	printf("\nEnter the load factor(0 to 1 || i.e:0.75 ): ");
	scanf("%f",&lf);
	m=findPrimeGreater(N/lf);
	HashTable = (struct Node*)calloc(m,sizeof(struct Node));
	if (HashTable == NULL) {
    	printf("Memory not allocated!\n");
  	  	exit(1);
  	}
	while(!finished){
		if(!modeChanged){
			printf("\nChoose your mode(1:NORMAL\t2:DEBUG): ");
			scanf("%d",&mode);
			mode--;//mode=0:NORMAL || mode=1:DEBUG
			modeChanged=1;		
		}
		do{
			printf("\n1:Insertion\t2:Searching\t3:Deletion\t4:reHashing\t5:Change the mode\t6:Exit\nEnter your choice between 1 and 6:\t");
			scanf("%d",&selection);
		}while(selection<1 || selection>6);
		switch(selection){
			case 1://Inserting a user to HashTable
				printf("\nEnter the name of new user[max:12char]:\t");
				scanf("%s",name);
				insertIntoHash(HashTable,name,m,mode);
				break;
			case 2://Searching a user in HashTable
				printf("\nEnter the name of new user[max:12char]:\t");
				scanf("%s",name);
				searchInHash(HashTable,name,m,mode);				
				break;
			case 3://Deleting a user from HashTable
				printf("\nEnter the name of new user[max:12char]:\t");
				scanf("%s",name);
				deleteFromHash(HashTable,name,m,mode);
				break;
			case 4://rehashing the HashTable with undeleted users
				HashTable=reHashing(HashTable,m,mode);
				break;
			case 5://changing the mode
				modeChanged=0;
				break;
			case 6:
				finished=1;//to finish the loop
				break;												
		}
	}
	return 0;
}
//this function returns hashIndex and 'found' value indicating whether the name is already in the table
int* getIndex(User *HashTable, char *name, int m, int mode,char *process){
	unsigned long long int key;
	int infoArr[2];
	int i,h1,h2,nameSize,found,hashIndex;
	int *ptr;
	ptr=infoArr;
	nameSize= strlen(name);
	key=0;
	for(i=0;i<nameSize;i++){
		key+=name[nameSize-i-1]*pow(31,i);
	}
	h1=key%m;
	h2=1+key%(m-2);
	if(mode){
		printf("\nh1(\"%s\")=%d\nh2(\"%s\")=%d",name,h1,name,h2);
	}
	i=0;
	found=0;
	do{
		hashIndex = (h1 + i*h2)%m;//double hashing
		if(!strcmp(HashTable[hashIndex].userName,name)){//checking if it is a value already in the table
			found=1;
		}
		if(mode && strlen(HashTable[hashIndex].userName) && !found){//if mode is DEBUG then printing some information about collusion
			printf("\n%s elemani %d. %s",name,hashIndex,process);
		}
		i++;
	}while(strlen(HashTable[hashIndex].userName)  && i<m && !found);//'i<m' check in case you want to add more elements despite occupancy
	infoArr[0]=hashIndex;
	infoArr[1]=found;
	return ptr;
}
void insertIntoHash(User *HashTable, char *name, int m, int mode){
	int *getInfo;//pointer to get the info from getIndex function
	int found,hashIndex;
	getInfo=getIndex(HashTable,name,m,mode,"adrese eklenemedi");
	hashIndex=*(getInfo+0);
	found=*(getInfo+1);
	if(!strlen(HashTable[hashIndex].userName)){//checking if it is a NULL address at the table, if so, then inserting the user to the address
		strcpy(HashTable[hashIndex].userName,name);
		HashTable[hashIndex].deleted=0;
		printf("\nElemaniniz %d. adrese yerlestirildi.\n",hashIndex);
	}
	else if(found){//checks if the user has already been inserted to the table
		if(HashTable[hashIndex].deleted==1){//changes the state of 'deleted' if the user is in the table but was previously deleted
			HashTable[hashIndex].deleted=0;
		}
		else{//if the user is in the table and not deleted, then it exists
			printf("\nEleman tabloda mevcut oldugu icin ekleme islemi yapilmadi!\n");
		}		
	}
	else{//If the user could not be inserted to the table and is not in the table at the same time, the table is full
		printf("\nTablo dolu oldugu icin ekleme islemi yapilamadi!\n");
	}
}
void deleteFromHash(User *HashTable, char *name, int m, int mode){
	int *getInfo;
	int found,hashIndex;
	getInfo=getIndex(HashTable,name,m,mode,"adreste bulunamadi");
	hashIndex=*(getInfo+0);
	found=*(getInfo+1);
	if(found == 1 && HashTable[hashIndex].deleted==0){//checks if the user exists and has not already been deleted before, then deletes
		HashTable[hashIndex].deleted=1;
		printf("\n%d adresindeki %s elemani silindi.\n",hashIndex,name);
	}
	else{//does not exist if the user is not present in the table or has already been deleted.
		printf("\nBu isimde kullanici bulunmamaktadir.\n");
	}
}
void searchInHash(User *HashTable, char *name, int m, int mode){
	int *getInfo;
	int found,hashIndex;
	getInfo=getIndex(HashTable,name,m,mode,"adreste bulunamadi");
	hashIndex=*(getInfo+0);
	found=*(getInfo+1);	
	if(found == 1 && HashTable[hashIndex].deleted==0){//checks if the user exists and has not already been deleted before, then prints
		printf("\n%s elemani %d adresinde bulunmaktadir.\n",name,hashIndex);
	}
	else{
		printf("\n%s elemani tabloda bulunamadi.\n",name);
	}
}
User* reHashing(User *HashTable, int m, int mode){
	User *newTable;
	int *getInfo;
	int i,hashIndex;
	newTable = (struct Node*)calloc(m,sizeof(struct Node));
	if (newTable == NULL) {
    	printf("Memory not allocated!\n");
  	  	exit(1);
  	}
	for(i=0;i<m;i++){
		if(strlen(HashTable[i].userName)){
			if(mode){
				printf("\n%s elemani eski tabloda %d.adreste bulunmaktaydi.",HashTable[i].userName,i);
				if(HashTable[i].deleted==1){
					printf("\n%s elemani eski tabloda silindigi icin yeni tabloya eklenmedi.",HashTable[i].userName);	
				}
			}
			if(HashTable[i].deleted==0){
				getInfo=getIndex(newTable,HashTable[i].userName,m,0,"");
				hashIndex=*(getInfo+0);
				strcpy(newTable[hashIndex].userName,HashTable[i].userName);			
				printf("\n%s elemani yeni tabloda %d. adrese yerlestirildi.",newTable[hashIndex].userName,hashIndex);					
			}
		}
	}
	free(HashTable);
	return newTable;
}
int findPrimeGreater(int x){
	int i=2;
	int prime=0;
	while(!prime){
		if(x%i==0){
			x++;
			i=2;
		}
		else if(i==x/2){
			prime=1;
		}
		else{
			i++;
		}
	}
	return x;
}
