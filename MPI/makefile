all: ring sum1 sum2 sum3

ring: ringPass.c
	mpicc ringPass.c -o ring

sum1: scatter.c
	mpicc scatter.c -o sum1

sum2: reduction.c
	mpicc reduction.c -o sum2

sum3: gather.c
	mpicc gather.c -o sum3

