#include <stdio.h>
#include <stdlib.h>

#define SIZE (int) pow(2,22) // size of the memory
// since we generate process in size of > 10 and < 100 kb, make 1 kb block size
#define BLOCK (int)pow(2,10) // 1 KB block 
#define ITERATIONS (int) pow(2,20)

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

}

void add_to_memory(int batch_num, int size) {
	int *index = (int*)(best_fit(size));
	int left = index[0];
	int hole = index[1];
	int i;
	for(i = left+1; i < left+size-1; i++) {
		process_memory[i] = 1;
	}
}

int available_hole_size() {
	int hole_size = 0;
	// TODO
	return hole_size;
}

void generate_processes(int N) {
	// TODO Create process with different sizes and check for available hole sizes
	int process[N][1];	
	int i;
	for (i = 0; i < N; i++) {
		add_to_memory(batch_num, process[i][0]);
	}
	batch_num++;
}

int main (int argc, char *argv[]) {

	int N = atoi(argv[1]);
	long long int block_nums = SIZE/BLOCK;
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
