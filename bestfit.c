#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#define SIZE (int) pow(2,22) // size of the memory
// since we generate process in size of > 10 and < 100 kb, make 1 kb block size
#define BLOCK (int)pow(2,10) // 1 KB block 
#define ITERATIONS (int) pow(2,20)

long long int block_nums = SIZE/BLOCK;
int *process_memory;
int batch_num = 0;
int oldest_batch = 0;
int num_holes;
int process_added = 0;
int *holes;

int *best_fit(int size) {
	// TODO
}

void delete_oldest_batch (void) {
	bool success = FALSE;
	int i, j;
	for (i = 0; i < block_nums-1; i++) {
		if(process_memory[i] == oldest_batch && process_memory[i+1] >= 10) {
			j = i;
			while (process_memory[j] != -1) {
				process_memory[j] = 0;
				j++;
			}
			process_memory[j] = 0;
			success = TRUE;
		}
	}
	if (success) {
		oldest_batch++;
	} else {
		exit(-1);
	}
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

int available_hole_size() {
	int hole_size = 0;
	// TODO
	int i, j;
	for(i = 0; i < block_nums-1; i++) {
		j = i + 1;
	}
	
	return hole_size;
}

void generate_processes(int N) {
	// TODO Create process with different sizes and check for available hole sizes
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
		hole_size = (int) available_hole_size;
	}

	for (i = 0; i < N; i++) {
		add_to_memory(batch_num, process[i][0]);
	}
	batch_num++;
}

int main (int argc, char *argv[]) {

	int N = atoi(argv[1]);
	process_memory = (int*) malloc((sizeof(int) * block_nums));
	srand(time(NULL));

	int i;
	for (i = 0; i < block_nums; i++) {
		process_memory[i] = 0;
	}

	// TODO
	while(process_added < ITERATIONS) {
		generate_processes(N);
	}

	return 0;
}
