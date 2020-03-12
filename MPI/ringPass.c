#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char** argv){
    int size, rank;
    MPI_Init(&argc,&argv);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    int a;
    if(rank  == 0){
        a=451;
        printf("Process %i: Initially Msg = %i\n", rank, a);
        MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Recv(&a, 1, MPI_INT, 3, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    else if( rank  == 1){
  		MPI_Recv(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        a++;
        MPI_Send(&a, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
    }
	else if( rank  == 2){
 		MPI_Recv(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        a++;
        MPI_Send(&a, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
    }
    else if( rank  == 3){
		MPI_Recv(&a, 1, MPI_INT, 2, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        a++;
        MPI_Send(&a, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    if(rank == 0){
        printf("Process %i: Msg = %i. Done! \n", rank, a);
    }
    else{
        printf("Process %i: Msg = %i\n", rank, a);
    }
    MPI_Finalize();
    return 0;
}