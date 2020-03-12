all: pis pi1 pi2 psort ssort

pis: estimatePiSerial.c
	gcc estimatePiSerial.c -o pis

pi1: estimatePi1.c
	gcc estimatePi1.c -o pi1 -lrt -fopenmp

pi2: estimatePi2.c
	gcc estimatePi2.c -o pi2 -lrt -fopenmp

psort: parQuickSort.c
	gcc parQuickSort.c -o psort -lrt -fopenmp

ssort: serialQuickSort.c
	gcc serialQuickSort.c -o ssort



