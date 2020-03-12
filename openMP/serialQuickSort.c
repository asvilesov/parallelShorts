#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <omp.h>

#define		size	  16*1024*1024
 
void quickSort(int *array, int start, int end){
   // you quick sort function goes here 
	int seed = 5;
   if(start < end){ //recursive stop condition 
		int rand_pivot = rand_r(&seed)%(end-start+1) + start;
		//printf("piv %i \n", rand_pivot);
		int temp = array[rand_pivot];
		array[rand_pivot] = array[end];
		array[end] = temp;
		int i = 0;
		int j = start-1;
		int pivot = array[end];
		int k = 0;
		for(i = start; i < end; i++){ //partition
			if(array[i] < pivot){
				j++;
				temp = array[j]; // switch two elements
				array[j] = array[i];
				array[i] = temp; 
			}
		}
		j++;
		array[end] = array[j];
		array[j] = pivot;

		// for(k=0;k<16;k++) printf("%d ", array[k]);
		// printf("\n");

		// printf("%i\n", j);

		quickSort(array, start, j-1);
		quickSort(array, j+1, end);
	}
   
}
int main(void){
	int i, j, tmp;
	struct timespec start, stop; 
	double exe_time;
	srand(time(NULL)); 
	int * m = (int *) malloc (sizeof(int)*size);
	for(i=0; i<size; i++){
		m[i]=size-i;
	}
	
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	////////**********Your code goes here***************//
	for(i=0;i<16;i++) printf("%d ", m[i]);
	printf("\n");
	
	quickSort(m, 0, size-1);
			
	///////******************************////
	
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
	exe_time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
	
	for(i=0;i<16;i++) printf("%d ", m[i]);		
	printf("\nExecution time = %f sec\n",  exe_time);		
}	