#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

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

// double** mat_multiply(int row, int col, int block_size, int n, double **A, double **B, double **C){
// 	int i,j,k;
// 	for (i=row; i< row+block_size; i++){
// 		for(j=col; j< col+block_size; j++){
// 			for(k=0; k< n; k++){
// 				C[i][j] += A[i][k]*B[k][j];
// 			}
// 		}
// 	}
// }

void *sub_matrix_multiply(void *thread_args){
	struct thread_dat * data = (struct thread_dat *) thread_args;
	//printf("Created thread: %i", data->thread_id);
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

		int i, j, k;
		struct timespec start, stop; 
		double time;
		long int n = 4096; // matrix size is n*n
		//int b = 128; //block size
		int p = atoi(argv[1]);
		p =sqrt(p);

		int b = n/p;

		long int num_threads = (n/b)*(n/b);

		//pthread_t  threads[num_threads];
		pthread_t *threads;
		threads = malloc(sizeof(pthread_t)*num_threads);
		int check;
		pthread_attr_t attr;
		pthread_attr_init(&attr);
		// pthread_attr_setstacksize(&attr, 1024);
		pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

		struct thread_dat* thread_data_list = malloc(num_threads * sizeof(struct thread_dat));
		//struct thread_dat thread_data_list[num_threads];

		
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
				thread_data_list[i*(n/b)+j].thread_id = i*(n/b)+j;
				thread_data_list[i*(n/b)+j].row = i*b;
				thread_data_list[i*(n/b)+j].col = j*b;
				thread_data_list[i*(n/b)+j].block_size = b;
				thread_data_list[i*(n/b)+j].num_elements = n;
				thread_data_list[i*(n/b)+j].A = A;
				thread_data_list[i*(n/b)+j].B = B;
				thread_data_list[i*(n/b)+j].C = C;
				//mat_multiply(i*b, j*b, b, n, A, B, C);
				check = pthread_create(&threads[i*(n/b)+j], &attr, sub_matrix_multiply, (void *) &thread_data_list[i*(n/b)+j]);
				if (check) { printf("ERROR; return code from pthread_create() is %d\n", check); exit(-1);}
			}
		}

		pthread_attr_destroy(&attr);

		for (i=0; i<n/b; i++){
			for(j=0; j< n/b; j++){
				if(pthread_join(threads[i*(n/b)+j],NULL)!=0){
					printf("Joining has failed\n");
				}
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

		pthread_exit(NULL);

		return 0;
}



// Number of FLOPs = 137438953472, Execution time = 645.218258 sec,
// 213.011569 MFLOPs per sec
// C[100][100]=879616000.000000
// alexander@hp-alex:~/Desktop/Fall2019/EE451/lab2$ ./bin 4
// Number of FLOPs = 137438953472, Execution time = 553.554480 sec,
// 248.284421 MFLOPs per sec
// C[100][100]=879616000.000000
// alexander@hp-alex:~/Desktop/Fall2019/EE451/lab2$ ./bin 16
// Number of FLOPs = 137438953472, Execution time = 588.075576 sec,
// 233.709678 MFLOPs per sec
// C[100][100]=879616000.000000
// alexander@hp-alex:~/Desktop/Fall2019/EE451/lab2$ ./bin 64
// Number of FLOPs = 137438953472, Execution time = 637.090866 sec,
// 215.728965 MFLOPs per sec
// C[100][100]=879616000.000000
// alexander@hp-alex:~/Desktop/Fall2019/EE451/lab2$ ./bin 1
// Number of FLOPs = 137438953472, Execution time = 1519.449613 sec,
// 90.453117 MFLOPs per sec
// C[100][100]=879616000.000000










