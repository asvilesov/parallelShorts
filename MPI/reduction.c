#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define length 64

int main(int argc, char** argv){
    FILE *file;
    int x;
    file =fopen("number.txt", "r");


    int size, rank;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int arr[length];

    int sum = 0;
    int temp = 0;
    int total_sum = 0;
    int i = 0;

    if(rank  == 0){
        char buffer[10];
        for(i = 0; i < length; i++){
            fscanf(file, "%s", buffer);
            arr[i] = atoi(buffer);
        }
    }

    MPI_Bcast(arr, length, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0){
        for(i = 0; i < 16; i++){
            sum+=arr[i];
        }
    }
    else if(rank  == 1){
        for(i = 16; i < 33; i++){
            sum+=arr[i];
        }
    }
	else if(rank  == 2){
        for(i = 33; i < 49; i++){
            sum+=arr[i];
        }
    }
    else if(rank  == 3){
        for(i = 49; i < 64; i++){
            sum+=arr[i];
        }
    }
    //printf("My sum is %i\n", sum);
    MPI_Reduce(&sum, &total_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0){
        printf("I am rank %i and my value is %i\n", rank, total_sum);
    }

    MPI_Finalize();
    return 0;
}