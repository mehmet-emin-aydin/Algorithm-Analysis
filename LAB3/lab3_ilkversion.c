#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//Struct of each User
typedef struct Node{
	char userName[30];
	int deleted;
}User;



//double hashing function
int h(unsigned long long int key, int i, int m);
//first hashing function of double hashing
int h1(unsigned long long int key, int m);
//second hashing function of double hashing
int h2(unsigned long long int key,int m);
//the function to insert new user to the Hash Table
void insertIntoHash(User *HashTable, char *name, int m);
//the function to delete the user from the Hash Table
void deleteFromHash(User *HashTable, char *name,int m);
//the function to search the user in the Hash Table
void searchInHash(User *HashTable, char *name,int m);
//the function to find the smallest prime number greater than x
int findPrimeGreater(int x);
int* getIndex(User *HashTable, char *name, int m);
//int findIndex(char *name);

int main(){
	int i;
	int N,m,choice;
	float lf;
	int finished=0;
	char name[30];
	int deleted;
	User *HashTable;
	printf("Enter the number of users,please!\t");
	scanf("%d",&N);
	printf("Enter the load factor(0 to 1 || i.e:0.75 ):\t");
	scanf("%f",&lf);
	m=findPrimeGreater(N/lf);
	HashTable = (struct Node*)calloc(m,sizeof(struct Node));
//	for(i=0;i<m;i++){
//		HashTable[i].userName=NULL;
//		HashTable[i].deleted=0;
//	}

	while(!finished){
		do{
		printf("1:Insertion\t2:Searching\t3:Deletion\t4:Exit\nEnter your choice between 1 and 4:\t");
		scanf("%d",&choice);
		}while(choice<1 || choice>4);
		switch(choice){
			case 1://Inserting a user to HashTable
				printf("Enter the name of new user:\t");
				scanf("%s",name);
				insertIntoHash(HashTable,name,m);
				break;
			case 2://Searching a user in HashTable

				break;
			case 3://Deleting a user from HashTable
				printf("Enter the name of new user:\t");
				scanf("%s",name);
				deleteFromHash(HashTable,name,m);
				break;
			case 4:
				finished=1;//to finish the loop
				break;												
		}
	}
	
	return 0;
}



int h(unsigned long long int key, int i, int m){
	return (h1(key,m) + i*h2(key,m))%m;
}

int h1(unsigned long long int key, int m){
	return key%m;
}

int h2(unsigned long long int key,int m){
	return 1+ key%(m-2);
}

//int findIndex(char *name){
//	int i;
//	int nameSize= strlen(name);
//	for(i=0;i<nameSize;i++){
//		key+=name[nameSize-i-1]*pow(31,i);
//	}
//	return h(key);
//}
int* getIndex(User *HashTable, char *name, int m){
	int infoArr[2];
	int *ptr;
	ptr=infoArr;
	int i,nameSize;
	int found;
	int hashIndex;
	unsigned long long int key;
	nameSize= strlen(name);
	key=0;
	for(i=0;i<nameSize;i++){
		key+=name[nameSize-i-1]*pow(31,i);
		printf("\nkey%d:%llu\n",i,key);
	}
	printf("\nkey:%llu\n",key);
	i=0;
	found=0;
	do{
		hashIndex = h(key,i,m);
		printf("\nhashindex:%d\n",hashIndex);
		if(!strcmp(HashTable[hashIndex].userName,name)){
			found=1;
		}
		i++;
	}while(strlen(HashTable[hashIndex].userName)  && i<m && !found);//doluluk oranina ragmen fazla eleman eklemek isterse diye i<m kontrolü
	infoArr[0]=hashIndex;
	printf("\ninsidehash:%d\tptrindex:%d\n",hashIndex,*(infoArr+0));
	
	infoArr[1]=found;
	return ptr;
}
void insertIntoHash(User *HashTable, char *name, int m){
	int *getInfo;
	int found;
	int hashIndex;
//	int i,nameSize;
//	unsigned long long int key;
//	nameSize= strlen(name);
//	for(i=0;i<nameSize;i++){
//		key+=name[nameSize-i-1]*pow(31,i);
//		printf("\nkey%d:%llu\n",i,key);
//	}
//	printf("\nkey:%llu\n",key);
//	i=0;
//	oldRecord=0;
//	do{
//		hashIndex = h(key,i,m);
//		printf("\nhashindex:%d\n",hashIndex);
//		if(!strcmp(HashTable[hashIndex].userName,name)){
//			oldRecord=1;
//		}
//		i++;
//	}while(strlen(HashTable[hashIndex].userName)  && i<m && !oldRecord);//doluluk oranina ragmen fazla eleman eklemek isterse diye i<m kontrolü
//	

	getInfo=getIndex(HashTable,name,m);
	hashIndex=*(getInfo+0);
	found=*(getInfo+1);

	if(!strlen(HashTable[hashIndex].userName)){
		strcpy(HashTable[hashIndex].userName,name);
		HashTable[hashIndex].deleted=0;
		printf("\nElemaniniz %d. adrese yerlestirildi.\n",hashIndex);
	}
	else if(found){
		if(HashTable[hashIndex].deleted==1){
			HashTable[hashIndex].deleted=0;
		}
		else{
			printf("\nEleman tabloda mevcut oldugu icin ekleme islemi yapilmadi!\n");
		}		
	}
	else{
		printf("\nTablo dolu oldugu icin ekleme islemi yapilamadi!\n");
	}
}
void deleteFromHash(User *HashTable, char *name, int m){
	int *getInfo;
	int found;
	int hashIndex;
//	int i,nameSize;
//	unsigned long long int key;
//	nameSize= strlen(name);
//	for(i=0;i<nameSize;i++){
//		key+=name[nameSize-i-1]*pow(31,i);
//		printf("\nkey%d:%llu\n",i,key);
//	}
//	printf("\nkey:%llu\n",key);
//	i=0;
//	found=0;
//	do{
//		hashIndex = h(key,i,m);
//		printf("\nhashindex:%d\n",hashIndex);
//		if(!strcmp(HashTable[hashIndex].userName,name)){
//			found=1;
//		}
//		i++;
//	}while(strlen(HashTable[hashIndex].userName)  && i<m && !found);//doluluk orani 1 ise bos olmayacagindan i<m kontrolu


	getInfo=getIndex(HashTable,name,m);
	hashIndex=*(getInfo+0);
	found=*(getInfo);

	if(found = 1 && HashTable[hashIndex].deleted==0){
		HashTable[hashIndex].deleted=1;
		printf("\n%d adresindeki %s elemani silindi.\n",hashIndex,name);
	}
	else{
		printf("\nBu isimde kullanici bulunmamaktadir.\n");
	}
}

void searchInHash(User *HashTable, char *name, int m){
	int *getInfo;
	int found;
	int hashIndex;
	
//	int i,nameSize;
//	unsigned long long int key;
//	nameSize= strlen(name);
//	for(i=0;i<nameSize;i++){
//		key+=name[nameSize-i-1]*pow(31,i);
//		printf("\nkey%d:%llu\n",i,key);
//	}
//	printf("\nkey:%llu\n",key);
//	i=0;
//	found=0;
//	do{
//		hashIndex = h(key,i,m);
//		printf("\nhashindex:%d\n",hashIndex);
//		if(!strcmp(HashTable[hashIndex].userName,name)){
//			found=1;
//		}
//		i++;
//	}while(strlen(HashTable[hashIndex].userName)  && i<m && !found);//doluluk orani 1 ise bos olmayacagindan i<m kontrolu

	getInfo=getIndex(HashTable,name,m);
	hashIndex=*(getInfo+0);
	found=*(getInfo+1);
		
	if(found = 1 && HashTable[hashIndex].deleted==0){
		printf("\n%s elemani %d adresinde bulunmaktadir.”\n",name,hashIndex);
	}
	else{
		printf("\n%s elemani tabloda bulunamadi.\n",name);
	}
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
	
