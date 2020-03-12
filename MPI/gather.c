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
    int arr_rec[length/4];

    int sum = 0;
    int temp = 0;
    int total_sum = 0;
    int sum_arr[4];
    int i = 0;

    for(int i = 0; i < length/4; i++){
        arr_rec[i] = 0;
    }

    if(rank  == 0){
        char buffer[10];
        for(i = 0; i < length; i++){
            fscanf(file, "%s", buffer);
            arr[i] = atoi(buffer);
        }
    }

    MPI_Scatter(arr, length/4, MPI_INT, arr_rec, length/4, MPI_INT, 0, MPI_COMM_WORLD);

    for(i = 0; i < 16; i++){
        sum+=arr_rec[i];
    }
    MPI_Gather(&sum, 1, MPI_INT, sum_arr, 1, MPI_INT, 0, MPI_COMM_WORLD);

    if(rank == 0){
        for(i = 0; i < 4; i++){
            total_sum+=sum_arr[i];
        }
        printf("I am rank %i and my value is %i\n", rank, total_sum);
    }

    MPI_Finalize();
    return 0;
}