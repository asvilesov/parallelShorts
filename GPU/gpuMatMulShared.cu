#include <stdlib.h>
#include <stdio.h>
#include <cublas.h>
#include <time.h>

#define n 1024

__global__ void mat_mul(int *a, int *b, int *c){
	int my_x = blockIdx.x*blockDim.x + threadIdx.x;
	int my_y = blockIdx.y*blockDim.y + threadIdx.y;
	int i = 0;
	int j = 0;
	__shared__ int s_A[32][32];
	__shared__ int s_B[32][32];
	int loc_c = 0;


	for(i = 0; i < n/blockDim.x; i++){ //can be either block dim since they are the same
		s_A[threadIdx.x][threadIdx.y] = a[1024*my_x + i*blockDim.y + threadIdx.y];
		s_B[threadIdx.x][threadIdx.y] = b[1024*(threadIdx.x + i*blockDim.x) + my_y];

		__syncthreads();
		
		for(j = 0; j < blockDim.x; j++){
			loc_c += s_A[threadIdx.x][j]*s_B[j][threadIdx.y];
		}

		__syncthreads();
		
	}

	c[my_x*n + my_y] = loc_c;  
}

int main(){	
	int i;

	cudaEvent_t start, stop;
	float time_execute = 0;
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
	
	int *A = (int*) malloc (sizeof(int)*n*n);
	int *B = (int*) malloc (sizeof(int)*n*n);
	int *C = (int*) malloc (sizeof(int)*n*n);
	for (i=0; i<n*n; i++) {
		A[i] = 1;
		B[i] = 2;
		C[i] = 0;
	}

	int *gpu_a, *gpu_b, *gpu_c;
	cudaMalloc((void**)&gpu_a, sizeof(int)*n*n); 
	cudaMalloc((void**)&gpu_b, sizeof(int)*n*n);
	cudaMalloc((void**)&gpu_c, sizeof(int)*n*n);

	cudaMemcpy(gpu_a, A, sizeof(int)*n*n, cudaMemcpyHostToDevice);
	cudaMemcpy(gpu_b, B, sizeof(int)*n*n, cudaMemcpyHostToDevice);
	
	dim3 dimGrid(32,32);
	dim3 dimBlock(32,32);

	cudaEventRecord(start, 0);

	mat_mul<<< dimGrid, dimBlock >>> (gpu_a, gpu_b, gpu_c);

	cudaEventRecord(stop, 0);

	cudaEventSynchronize(stop);
	cudaEventElapsedTime(&time_execute, start, stop);
	cudaEventDestroy(start);
	cudaEventDestroy(stop);

	cudaMemcpy(C, gpu_c, sizeof(int)*n*n, cudaMemcpyDeviceToHost);

	printf("C[451][451] = %i\n", C[451*1024+451]);
	//printf("Total Time: %f\n", time_execute);
    

	free(A);
	free(B);
	free(C);
	cudaFree(gpu_a);
	cudaFree(gpu_b);
	cudaFree(gpu_c);
	return 0;
	
}	