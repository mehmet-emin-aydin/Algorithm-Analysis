#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 5
//function to display the array
void printArr(int arr[], int size){
	int i;
	for (i=0;i < size;i++){
		printf("%d ",arr[i]);
	}
	printf("\n");
}
// function to swap elements
void swap(int *a, int *b) {
  int t = *a;
  *a = *b;
  *b = t;
}
// function to find the partition position
int partition(int array[], int low, int high,int pivot) {
  int i = (low - 1); // left pointer
  int j;//right pointer
  // traverse each element of the array
  // compare them with the pivot
  for ( j = low; j < high; j++) {
    if (array[j] < pivot) {
      // if element smaller than pivot is found
      // swap it with the greater element pointed by i
      i++;
      
      // swap element at i with element at j
      swap(&array[i], &array[j]);
    }
    else if(array[j]==pivot){
    	swap(&array[j],&array[high]);
    	j--;
	}
  }
  swap(&array[i + 1], &array[high]);  // swap the pivot element with the greater element at i
  return (i + 1);  // return the partition point
}
void matchPairs(int lock[], int key[], int low, int high){
    if(low<high){
        int random = low + rand() % (high - low);   	
        int pivot = partition(lock, low, high, key[random]);  // select a random element as pivot
        partition(key, low, high, lock[pivot]);
        matchPairs(lock, key, low, pivot-1);
        matchPairs(lock, key, pivot+1, high);
    }
}
int main() {
    srand(time(NULL));
    int i;
//    int lock[N]={13,4,8,12,7,6,21,3,1,5};//Sample for N=10
//    int key[N]={6,4,8,21,5,3,7,1,13,12};
//    int lock[N]={1,2,3,4,5,6,7,8,9,10};//Sample of worst case for N=10
//    int key[N]={10,9,8,7,6,5,4,3,2,1};
    int lock[N]={8,5,3,7,2};//Sample for N=5
    int key[N]={3,2,7,5,8};
//    int lock[N]={13,4,8,12,22,15,7,6,18,21,3,34,11,1,5};//Sample for N=15
//    int key[N]={6,4,11,22,15,34,18,8,21,5,3,7,1,13,12};	
    printf("Locks: ");
	printArr(lock,N);
    printf("Keys: ");
	printArr(key,N);
	printf("\n");
    matchPairs(lock, key, 0, N-1);
    printf("Locks: ");
	printArr(lock,N);
    printf("\nKeys: ");
	printArr(key,N);
    return 0;
}
