#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define NUM_THREADS 5

struct thread_dat{
	int thread_id;
	int row;
	int col;
	int block_size;
	int num_elements;
	double **A;
	double **B;
	double **C;
};

double** mat_multiply(int row, int col, int block_size, int n, double **A, double **B, double **C){
	int i,j,k;
	for (i=row; i< row+block_size; i++){
		for(j=col; j< col+block_size; j++){
			for(k=0; k< n; k++){
				C[i][j] += A[i][k]*B[k][j];
			}
		}
	}
}

void *sub_matrix_multiply(void *thread_args){
	struct thread_dat * data = (struct thread_dat *) thread_args;
	int i,j,k;
	for (i=data->row; i< data->row+data->block_size; i++){
		for(j=data->col; j< data->col+data->block_size; j++){
			for(k=0; k< data->num_elements; k++){
				data->C[i][j] += data->A[i][k]*data->B[k][j];
			}
		}
	}
	pthread_exit(NULL);
}

int main(int argc, char* argv[]){

		// printf("hi\n");
	 //    printf("Num arguments %i\n", argc);
	 //    printf("argument is %i\n", atoi(argv[1]));
		//struct thread_dat thread_data_list[4094*4094];

		int i, j, k;
		struct timespec start, stop; 
		double time;
		long int n = 4096; // matrix size is n*n
		//int b = 128; //block size

		pthread_t *threads;

		
		int b = atoi(argv[1]);

		long int num_threads = (n/b)*(n/b);
		threads = malloc(sizeof(pthread_t)*num_threads);

		
		double **A = (double**) malloc (sizeof(double*)*n);
		double **B = (double**) malloc (sizeof(double*)*n);
		double **C = (double**) malloc (sizeof(double*)*n);
		for (i=0; i<n; i++) {
			A[i] = (double*) malloc(sizeof(double)*n);
			B[i] = (double*) malloc(sizeof(double)*n);
			C[i] = (double*) malloc(sizeof(double)*n);
		}
		
		for (i=0; i<n; i++){
			for(j=0; j< n; j++){
				A[i][j]=i;
				B[i][j]=i+j;
				C[i][j]=0;			
			}
		}
				
		if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
		
		// Your code goes here //
		// Matrix C = Matrix A * Matrix B //	
		//*******************************//
		
		for (i=0; i<n/b; i++){
			for(j=0; j< n/b; j++){
				mat_multiply(i*b, j*b, b, n, A, B, C);
			}
		}

		// for(i=0; i<n; i ++){
		// 	for(j=0; j<n; j++){
		// 		for(k = 0; k<n; k++){
		// 			C[i][j] += A[i][k]*B[k][j];
		// 		}
		// 	}
		// }
		//*******************************//

		
		if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
		time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
		
		printf("Number of FLOPs = %lu, Execution time = %f sec,\n%lf MFLOPs per sec\n", 2*n*n*n, time, 1/time/1e6*2*n*n*n);		
		printf("C[100][100]=%f\n", C[100][100]);
		
		// release memory
		for (i=0; i<n; i++) {
			free(A[i]);
			free(B[i]);
			free(C[i]);
		}
		free(A);
		free(B);
		free(C);
		return 0;
}