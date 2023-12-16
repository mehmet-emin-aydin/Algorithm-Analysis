#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 15
//int partition(int arr[], int low, int high, int pivot){
//    int i = low;
//    int temp;
//    int j;
//    for ( j = low; j < high; ++j) {
//        if(arr[j]<pivot) {
//            temp = arr[j];
//            arr[j] = arr[i];
//            arr[i] = temp;
//            i++;
//        }
//        else if(arr[j]==pivot){
//            temp = arr[j];
//            arr[j] = arr[high];
//            arr[high] = temp;
//            j--;
//        }
//    }
//    temp = arr[i];
//    arr[i] = arr[high];
//    arr[high] = temp;
//    return i;
//}




//int partition(int arr[], int low, int high, int pivot){
//	int l,r;
//	l=low;
//	r=high;
//	while (l<r){
//		if(arr[l]<pivot ) l++;
//		else if(arr[r]>pivot) r--;
////		else if(arr[r]==pivot){
////			arr[r]+=arr[high];
////			arr[high]=arr[r]-arr[high];
////			arr[r]=arr[r]-arr[high];
////			r--;			
////		}
//		else if(arr[l]==pivot){
//			arr[l]+=arr[high];
//			arr[high]=arr[l]-arr[high];
//			arr[l]=arr[l]-arr[high];
//			l++;			
//		}
//		else if(arr[r]==pivot){
//			arr[r]+=arr[high];
//			arr[high]=arr[r]-arr[high];
//			arr[r]=arr[r]-arr[high];
//			r--;			
//		}
//		else{
//			arr[l]+=arr[r];
//			arr[r]=arr[l]-arr[r];
//			arr[l]=arr[l]-arr[r];
//			l++;
//			r--;
//		}
//		
//	}
// 	
//	return r;
//}

//void swap(int arr[], int i, int j){
//	arr[i]+=arr[j];
//	arr[j]=arr[i]-arr[j];
//	arr[i]=arr[i]-arr[j];	
//}



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
  
  // select the rightmost element as pivot
//  int pivot = array[high];
  
  // pointer for greater element
  int i = (low - 1);

  // traverse each element of the array
  // compare them with the pivot
  int j;
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

  // swap the pivot element with the greater element at i
  swap(&array[i + 1], &array[high]);
  
  // return the partition point
  return (i + 1);
}
//int partition(int arr[], int low, int high, int pivot){
//	int l,r;
//	l=low;
//	r=high;
//	while(l<=r){
//		if(arr[l]==pivot){
//			swap(arr,l,low);
//			l++;
//		}
//		else if(arr[r]==pivot){
//			swap(arr,r,low);
//			r--;
//		}		
//		else if (arr[l]<pivot) l++;
//		else
//	}
//	swap(arr,r,low);
//    for ( l = 0; l < high; l++) {
//        printf("%d ", arr[l]);
//    }
//    printf("\n%d\n\n",pivot);
//	return r;
//}


void matchPairs(int lock[], int key[], int low, int high){
    if(low<high){
        int random = low + rand() % (high - low);   	
        int pivot = partition(lock, low, high, key[random]);
        partition(key, low, high, lock[pivot]);

        matchPairs(lock, key, low, pivot-1);
        matchPairs(lock, key, pivot+1, high);
    }
}
//bu fonksiyon kullanilmiyor ancak siz testlerinizde kullanabilirsiniz
void generateRandomArray(int arr[], int n){
    int random;
    int i,j;
    for ( i = 0; i < n; ++i) {
        random = 1 + rand() % n;
        for ( j = 0; j < i; ++j) {
            if(arr[j]==random){
                random = 1 + rand() % n;
                j=-1;
            }
        }
        arr[i] = random;
    }
}
int main() {
    srand(time(NULL));
    int i;
    int lock[N]={5,2,3,15,1,6,7,13,4,10,11,8,14,12,9};
    int key[N]={1,7,8,11,10,13,12,9,2,14,15,5,3,6,4};
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
