#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 7

struct node {
	int key;
	char name[40];
	struct node *next;
};

struct hash {
	struct node *head;
	int count;
};


struct hash *hashTable = NULL;
//
//
//
struct node *createNode(int key,char *name);
void insertToHash(int key ,char name[]);
void deleteFromHash(int key);
void searchInHash(int key);
void display();
int main(){
	
	int key,opt;
	char name[40];
	
	//okuma islemleri...
	//
	//
	
	hashTable = (struct hash*)calloc(M,sizeof(struct hash));
	if(hashTable == NULL){
		printf("Yer acilamadi");
		exit(0);
	}

	while(1){
		printf("\nPress your choose->\t Insert:1\tDisplay:2\tSearch:3\tDelete:4\texit:5\n");
		scanf("%d",&opt);
		printf("Enter the key:");
		scanf("%d",&key);
		printf("Enter the name");
		scanf("%s",name);		
	    switch(opt)
	    {
	        case 1:
	            insertToHash(key,name);
	            break;
	        case 2:
	            display();
	            break;
	        case 3:
	            searchInHash(key);
	            break;
	        case 4:
	        	deleteFromHash(key);
	        	break;
	        case 5:exit(0);
	                                       
	    }		
	}
	

	
}
struct node *createNode(int key,char *name){
	struct node *newNode;
	newNode =(struct node*)malloc(sizeof(struct node));
	newNode->key=key;
	//newNode->age=age
	strcpy(newNode->name, name);
	newNode->next=NULL;
	return newNode;
}


void insertToHash(int key, char *name){
	int hashIndex;
	hashIndex =key%M;
	struct node *newNode = createNode(key,name);
	if(!hashTable[hashIndex].head){
		hashTable[hashIndex].head = newNode;
		hashTable[hashIndex].count = 1;
		return;
	}
	newNode->next =hashTable[hashIndex].head;
	hashTable[hashIndex].head = newNode;
	hashTable[hashIndex].count++;
	return;
}

void deleteFromHash(int key){
	int hashIndex=key%M;
	int flag=0;
	struct node *tmp, *myNode;
	myNode = hashTable[hashIndex].head;
	if(!myNode){
		printf("Given data is not present in hash Table!!\n");
		return ;
	}
	tmp = myNode;
	while(myNode!=NULL &&!flag){
		if(myNode->key==key){
			flag =1;
			if(myNode==hashTable[hashIndex].head){
				hashTable[hashIndex].head= myNode->next;
			}
			else{
				tmp->next=myNode->next;
			}
			hashTable[hashIndex].count--;
			free(myNode);
		}
		tmp=myNode;
		myNode = myNode->next;
	}
	if(flag){
		printf("Data deleted\n");
	}
	else{
		printf("data is not present\n");
	}
	return;
}

void searchInHash(int key){
	int hashIndex = key %M;
	int flag =0;
	struct node *myNode;
	myNode = hashTable[hashIndex].head;
	if(!myNode){
		printf("eleman yok!\n");
		return ;
	}
	
	while(myNode !=NULL && !flag){
		if(myNode->key==key){
            printf("VoterID  : %d\n", myNode->key);
            printf("Name     : %s\n", myNode->name);
			flag=1;		
		}
		myNode=myNode->next;
	}
	if(!flag){
		printf("eleman yok\n");
	}
	return;
}

void display() {
    struct node *myNode;
    int i;
    for (i = 0; i < M; i++) {
        if (hashTable[i].count == 0)
            continue;
        myNode = hashTable[i].head;
        if (!myNode)
            continue;
        printf("\nData at index %d in Hash Table:\n", i);
        printf("VoterID     Name          Age   \n");
        printf("--------------------------------\n");
        while (myNode != NULL) {
            printf("%-12d", myNode->key);
            printf("%-15s", myNode->name);
            myNode = myNode->next;
        }
    }
    return;
}


