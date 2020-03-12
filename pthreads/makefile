all: mm kmeans pkmeans pmkmeans 


pmm: parMatMul.c
	gcc parMatMul.c -o pmm -lpthread -lm

kmeans: kmeansSerial.c
	gcc kmeansSerial.c -o kmeans

pkmeans: parKmeans.c
	gcc parKmeans.c -o pkmeans -lpthread

pmkmeans: parMutexKmeans.c
	gcc parMutexKmeans.c -o pmkmeans -lpthread 

