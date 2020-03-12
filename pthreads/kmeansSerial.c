#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>

#define h  800 
#define w  800

#define input_file  "input.raw"
#define output_file "output.raw"

int main(int argc, char** argv){
    int i;
    FILE *fp;

  	unsigned char *a = (unsigned char*) malloc (sizeof(unsigned char)*h*w);
    
	// the matrix is stored in a linear array in row major fashion
	if (!(fp=fopen(input_file, "rb"))) {
		printf("can not opern file\n");
		return 1;
	}
	fread(a, sizeof(unsigned char), w*h, fp);
	fclose(fp);
    
	// measure the start time here
	struct timespec start, stop; 
	double time;
	if( clock_gettime(CLOCK_REALTIME, &start) == -1) { perror("clock gettime");}
	

	int num_clusters = 4;
	double kmeans[] = {0.0,85.0,170.0,255.0};
	double kmeans_sum[num_clusters];
	double kmeans_count[num_clusters];
	int iterations = 30;
	int max_dist, cluster_assignement;

	for(int i = 0; i < iterations; i++){ //number of iterations of kmeans
		for(int j = 0; j < h*w; j++){ //compute kmeans distances
			max_dist = 255 - 0;
			cluster_assignement = 0;
			for(int k = 0; k < num_clusters; k++){ //find which cluster the value is closest too
				if (abs(a[j]-kmeans[k]) < max_dist){
					max_dist = abs(a[j]-kmeans[k]);
					cluster_assignement = k;
				}
			}
			kmeans_sum[cluster_assignement] += a[j];
			kmeans_count[cluster_assignement]++;

			if (i+1 == iterations){ //do this only on the last iteration
				a[j] = 85*cluster_assignement;
			}
		}

		for(int k = 0; k < num_clusters; k++){ //recompute averages of clusters
			kmeans[k] = kmeans_sum[k]/kmeans_count[k];
			kmeans_sum[k] = 0;
			kmeans_count[k] = 0;
		}
	}
	
	// measure the end time here
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		

	time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
	printf("Execution time = %f sec\n",time);
	
	
	if (!(fp=fopen(output_file,"wb"))) {
		printf("can not opern file\n");
		return 1;
	}	
	fwrite(a, sizeof(unsigned char),w*h, fp);
    fclose(fp);
    
    return 0;
}