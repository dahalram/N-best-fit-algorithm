#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#define MEM_SIZE 4194304 // size of the memory 4MB
// since we generate process in size of > 10 and < 100 kb, make 1 kb block size
#define BLOCK 1024 // 1 KB block 
#define ITERATIONS  1048576

long long block_nums = 4096; // MEM_SIZE/BLOCK
int *process_memory;
int batch_num = 0;
int oldest_batch = 0;
int process_added = 0;
int *holes;

int available_hole_size(void) {
	int hole_size = 0;
	int i;
	for(i = 0; i < block_nums-1; i++) {
		if(process_memory[i] == 0 && process_memory[i+1] == 0) {
			while((i+1)< block_nums && process_memory[i+1] == 0) {
				hole_size++;
				i++;
			}
			i = i+1;
		}
	}
	
	return hole_size;
}

void delete_oldest_batch () {
	bool success = false;
	int i, j;
	for (i = 0; i < block_nums-1; i++) {
		if(process_memory[i] == oldest_batch && process_memory[i+1] >= 10) {
			j = i;
			while (process_memory[j] != -1) {
				process_memory[j] = 0;
				j++;
			}
			process_memory[j] = 0;
			success = true;
		}
	}
	if (success) {
		oldest_batch++;
	} else {
		exit(-1);
	}
}

int *best_fit(int size) {
	bool success = false;
	int curr_hole = 0, best_place = 0;
	int *index = (int*)malloc(2*sizeof(int));
	int available = 1048576;//(int) pow(2,20);

	while(oldest_batch < batch_num && !success) {
		int i, j = 1;
		for(i = 0; i < block_nums-1 && j < block_nums; i++) {
			j = i +1;
			curr_hole = 0;
			if(process_memory[i] == 0 && process_memory[j] == 0) {
				j++;
				while(j < block_nums && process_memory[j] == 0) {
					j++;
				}
				curr_hole = j - i + 1;
				if (curr_hole > size && curr_hole < available) {
					available = curr_hole;
					best_place = i;
					if (available_hole_size() > size) {
						success = true;
					}
				}
				i = j -1;
			}
		}
		if (!success) {
			delete_oldest_batch();
		}
	} 
	
	index[0] = best_place;
	index[1] = available;
	return index;
}


void add_to_memory(int batch_number, int size) {
	int *index = (int*)(best_fit(size));
	int left = index[0];
	int hole = index[1];
	int i;
	if (hole < size) {
		printf("Hole size is smaller than process size\n");
		exit(-1);
	}
	for(i = left+2; i < left+size-1; i++) {
		process_memory[i] = 1;
	}
	process_memory[left] = batch_number;
	process_memory[left+1] = size;
	process_memory[left+size-1] = -1;
	process_added++;
}


void generate_processes(int N) {
	// Create process with different sizes and check for available hole sizes
	int process[N][1];
	int size, i;
	int total = 0;
	for (i = 0; i < N; i++) {
		size = rand()%91 + 10;
		total += size;
		process[i][0] = size;
	}
	
	int hole_size = (int) available_hole_size();
	while(hole_size < total)  {
		delete_oldest_batch();
		hole_size = (int) available_hole_size();
	}

	for (i = 0; i < N; i++) {
		add_to_memory(batch_num, process[i][0]);
	}
	batch_num++;
}

void print_stats(int N) {
	long long mean, std_dev, var, sum = 0;
	int i;
	for (i = 0; i <= batch_num; i++) {
		sum += holes[i];
	}
	mean = sum/(batch_num+1);
	long tmp = 0;
	for(i = 0; i <= batch_num; i++) {
		tmp += (long long) pow((holes[i] - mean), 2);
	}
	var = (long long) tmp/batch_num;
	std_dev = (long long) pow((long long) var, 0.5);
	printf("\nAverage hole sizes of batch size %d : %ld \n", N, mean);
	printf("Standard Deviation: %ld\n", std_dev);
	printf("Average plus 4 sd: %ld\n", mean + 4 * std_dev);
}

int main (int argc, char *argv[]) {

	int N = atoi(argv[1]);
	process_memory = (int*) malloc((sizeof(int) * block_nums));
	srand(time(NULL));

	int i;
	for (i = 0; i < block_nums; i++) {
		process_memory[i] = 0;
	}

	holes = (int*) malloc(sizeof(int)*(ITERATIONS/N+N));
	for(i = 0; i < ITERATIONS/N+N; i++) {
		holes[i] = 0;
	}

	while(process_added < ITERATIONS) {
		generate_processes(N);
		holes[batch_num-1] = available_hole_size();
	}

	print_stats(N);
	return 0;
}
