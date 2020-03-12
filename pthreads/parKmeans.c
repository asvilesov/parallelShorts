#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <time.h>
#include <pthread.h>

#define h  800 
#define w  800

#define input_file  "input.raw"
#define output_file "output.raw"

#define iterations 50

pthread_mutex_t mutex;
//pthread_mutex_init(mutex_array, NULL);

struct thread_dat{
	int thread_id;
	int i;
	int j;
	int p;
	int num_clusters;
	double *kmeans; //only access within thread
	double *kmeans_sum;
	double *kmeans_count;
	unsigned char *a;
	pthread_mutex_t *mutex_array;
};

void *sub_kmeans(void *thread_args);



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
	

	//  Your code goes here
	//init all kmeans and pthreads vars
	int num_clusters = 6;
	double kmeans[] = {0.0,65.0,100.0,125.0,190.0,255.0};
	double kmeans_sum[] = {0,0,0,0,0,0};
	double kmeans_count[] = {0,0,0,0,0,0};
	int max_dist, cluster_assignement;

	int p = atoi(argv[1]);
	//pthread_mutex_t mutex_array[p];
	pthread_t *threads;
	threads = malloc(sizeof(pthread_t)*p);
	struct thread_dat* thread_data_list = malloc(p * sizeof(struct thread_dat));
	int check;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	double **kmeans_sum_matrix = (double**) malloc (sizeof(double*)*p);
	double **kmeans_count_matrix = (double**) malloc (sizeof(double*)*p);
	for (i=0; i<p; i++) {
		kmeans_sum_matrix[i] = (double*) malloc(sizeof(double)*num_clusters);
		kmeans_count_matrix[i] = (double*) malloc(sizeof(double)*num_clusters);
	}

	//pthread_mutex_init(&mutex, NULL);
	//pthread_mutex_init(mutex_array, NULL);
	// for(j = 0; j < p; j++){
	// 	pthread_mutex_init(&(mutex_array[j]), NULL);
	// }

	for(int i = 0; i < iterations; i++){ //number of iterations of kmeans
		for(int j = 0; j < p; j++){ //compute kmeans distances
			thread_data_list[j].thread_id = j;
			thread_data_list[j].j = j;
			thread_data_list[j].i = i;
			thread_data_list[j].p = p;
			thread_data_list[j].num_clusters = num_clusters;
			thread_data_list[j].kmeans = kmeans;
			thread_data_list[j].kmeans_sum = kmeans_sum_matrix[j];
			thread_data_list[j].kmeans_count = kmeans_count_matrix[j];
			thread_data_list[j].a = a;
			//thread_data_list[j].mutex_array = mutex_array;

			//printf("iteration: %i and thread: %i\n", i, j);

			check = pthread_create(&threads[j], &attr, sub_kmeans, (void *) &thread_data_list[j]);
			if (check) { printf("ERROR; return code from pthread_create() is %d\n", check); exit(-1);}
		}

		pthread_attr_destroy(&attr);

		int l = 0;

		for(l=0; l<p; l++) {
				if(pthread_join(threads[l],NULL)!=0){
					printf("joining failed\n");
				}
			}

		for(int k = 0; k < p; k++){
			for(int j = 0; j < num_clusters; j++){
				kmeans_sum[j] += kmeans_sum_matrix[k][j];
				kmeans_count[j] += kmeans_count_matrix[k][j];
			}
		}

		for(int k = 0; k < num_clusters; k++){ //recompute averages of clusters
			//printf("Cluster: %i has a sum of %f and count of %f\n", k, kmeans_sum[k], kmeans_count[k]);
			kmeans[k] = kmeans_sum[k]/kmeans_count[k];
			kmeans_sum[k] = 0;
			kmeans_count[k] = 0;
		}
	}



	// for(int k = 0; k < num_clusters; k++){
	// 	printf("%f\n", kmeans[k]);
	// }


	//
	
	// measure the end time here
	if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) { perror("clock gettime");}		
	
	// print out the execution time here
	time = (stop.tv_sec - start.tv_sec)+ (double)(stop.tv_nsec - start.tv_nsec)/1e9;
	printf("Execution time = %f sec\n",time);
	
	
	if (!(fp=fopen(output_file,"wb"))) {
		printf("can not opern file\n");
		return 1;
	}	
	fwrite(a, sizeof(unsigned char),w*h, fp);
    fclose(fp);

    pthread_exit(NULL);
    
    return 0;
}

void *sub_kmeans(void *thread_args){
	int cluster_assignement;
	int max_dist;
	struct thread_dat * data = (struct thread_dat *) thread_args;
	int thread_id = data->thread_id;
	int i = data->i;
	int j = data->j;
	int p = data->p;
	int num_clusters = data->num_clusters;
	double *kmeans = data->kmeans;
	double *kmeans_sum = data->kmeans_sum;
	double *kmeans_count = data->kmeans_count;
	unsigned char *a = data->a;
	pthread_mutex_t* mutex_array = data->mutex_array;

	int iter;

	for(iter = j*h*w/p; iter<(j+1)*h*w/p; ++iter){
		max_dist = 255;
		cluster_assignement = 0;
		for(int k = 0; k < num_clusters; k++){ //find which cluster the value is closest too
			if (abs(a[iter]-kmeans[k]) < max_dist){
				max_dist = abs(a[iter]-kmeans[k]);
				cluster_assignement = k;
			}
		}
		//pthread_mutex_lock(&mutex);
		kmeans_sum[cluster_assignement] += a[iter];
		kmeans_count[cluster_assignement]++;
		//pthread_mutex_unlock(&mutex);

		if (i+1 == iterations){ //do this only on the last iteration
				a[iter] = 51*cluster_assignement;
			}
	}
	//printf("%f\n", kmeans_sum[3]);

	pthread_exit(NULL);

}