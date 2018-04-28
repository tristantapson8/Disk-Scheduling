#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <string.h>
#include <fcntl.h>

#define REQUEST_SIZE 8
#define CYLINDER_MIN 0 
#define CYLINDER_MAX 199

// requests hard coded in, just for testing purposes; array of any size can be used however
int requests[REQUEST_SIZE] = {98, 183, 37, 122, 14, 124, 65, 67};


// bubblesort sorting algorithm
static * bubblesort (int arr[], int size){

	int i;
	int j;
	int k;
	int temp;

	for(i = 0; i < (size-1); i++){
		for(j = 0; j < (size-1-i); j++){
			if(arr[j] > arr[j+1]){
				temp = arr[j+1];
				arr[j+1] = arr[j];
				arr[j] = temp;
			}
		}
	}
	
	// print statement for testing purposes
	for(k = 0; k < size; k++){
		//printf("bubblesort: %d\n", arr[k]);
	}

	return arr;
}

// First come first serve (FCFS) disk scheduling
int FCFS(int arr[], int head){

	int i;
	int hm = abs(arr[0] - head);
	printf("FCFS DISK SCHEDULING ALGORITHM\n\n");

	for(i = 0; i < REQUEST_SIZE; i++){
		printf("FCFS - servicing request: %d\n", arr[i]);
		
		if( i < REQUEST_SIZE-1){
			hm = hm + abs(arr[i+1] - arr[i]);
		}

	}

	return hm;
}

// Shortest seek time first (SSTF) disk scheduling
int SSTF(int arr[], int head){

	printf("SSTF DISK SCHEDULING ALGORITHM\n\n");

	int i;
	int j;
	int k;
	int l;
	int hm = 0;

	int by_Dist[REQUEST_SIZE];
	int done_Dist[REQUEST_SIZE];

	for(i = 0; i < REQUEST_SIZE; i++){
		by_Dist[i] = abs(arr[i] - head);
	}

	bubblesort(by_Dist, REQUEST_SIZE);

	for(j = 0; j < REQUEST_SIZE; j++){
		for(k = 0; k < REQUEST_SIZE; k++){
			if(by_Dist[j] + head == requests[k] ){
				printf("SSTF - servicing request: %d\n", requests[k]);
				done_Dist[j] = requests[k];
				break;
			}

			else if( head - by_Dist[j] == requests[k]){
				printf("SSTF - servicing request: %d\n", requests[k]);
				done_Dist[j] = requests[k];
				break;
			}
		}

	}

	hm = hm + abs(head - done_Dist[0]);
	for(l = 0; l < REQUEST_SIZE-1; l++){
		hm = hm + abs(done_Dist[l+1] - done_Dist[l]);
	}

	return hm;
}

// LEFT Scan Disk Scheduling
int SCAN_L(int arr[], int head, int look){

	if(look == 0){
		printf("SCAN DISK SCHEDULING ALGORITHM\n\n");
	}

	else if(look == 1){
		printf("LOOK DISK SCHEDULING ALGORITHM\n\n");
	}
	
	int i;
	int count_down = 0;
	int j;
	int k;
	int hm = 0; // counter for head movement

	bubblesort(arr, REQUEST_SIZE);

	int done_Scan[REQUEST_SIZE+1];

	for(i = REQUEST_SIZE-1; i > -1; i--){
		if(arr[i] < head){
			done_Scan[count_down] = arr[i];
			if(look == 0){
				printf("SCAN - servicing request: %d\n", done_Scan[count_down]);
			}
			else if(look == 1){
				printf("LOOK - servicing request: %d\n", done_Scan[count_down]);
			}
			
			count_down++;
		}
	}
	
	if(look == 0){
		done_Scan[count_down] = CYLINDER_MIN; // 0
		count_down++;
	}

	for(j = 0; j < REQUEST_SIZE; j++){
		if(arr[j] > head){
			done_Scan[count_down] = arr[j];
			if(look == 0){
				printf("SCAN - servicing request: %d\n", done_Scan[count_down]);
			}
			else if(look == 1){
				printf("LOOK - servicing request: %d\n", done_Scan[count_down]);
			}

			count_down++;
		}
	}

	hm = hm + abs(head - done_Scan[0]);

	if(look == 0){
		for(k = 0; k < REQUEST_SIZE; k++){
			hm = hm + abs(done_Scan[k+1] - done_Scan[k]);
		}
	}

	if(look == 1){
		for(k = 0; k < REQUEST_SIZE-1; k++){
			hm = hm + abs(done_Scan[k+1] - done_Scan[k]);
		}
	}

	return hm;
}

// RIGHT Scan Disk Scheduling
int SCAN_R(int arr[], int head, int look){
	
	if(look == 0){
		printf("SCAN DISK SCHEDULING ALGORITHM\n\n");
	}

	else if(look == 1){
		printf("LOOK DISK SCHEDULING ALGORITHM\n\n");
	}

	int i;
	int count_up = 0;
	int j;
	int k;
	int hm = 0; // counter for head movement

	bubblesort(arr, REQUEST_SIZE);

	int done_Scan[REQUEST_SIZE+1];

	for(i = 0; i < REQUEST_SIZE; i++){
		if(arr[i] > head){

			done_Scan[count_up] = arr[i];

			if(look == 0){
				printf("SCAN - servicing request: %d\n", done_Scan[count_up]);
			}

			else if(look == 1){
				printf("LOOK - servicing request: %d\n", done_Scan[count_up]);
			}

			count_up++;
		}
	}
	
	if(look == 0){
		done_Scan[count_up] = CYLINDER_MAX; // 199
		count_up++;
	}

	for(j = REQUEST_SIZE-1; j > -1; j--){
		if(arr[j] < head){
			done_Scan[count_up] = arr[j];
			if(look == 0){
				printf("SCAN - servicing request: %d\n", done_Scan[count_up]);
			}
			else if(look == 1){
				printf("LOOK - servicing request: %d\n", done_Scan[count_up]);
			}
			count_up++;
		}
	}

	
	hm = hm + abs(head - done_Scan[0]);

	if(look == 0){
		for(k = 0; k < REQUEST_SIZE; k++){
			hm = hm + abs(done_Scan[k+1] - done_Scan[k]);
		}
	}

	else if(look == 1){
		for(k = 0; k < REQUEST_SIZE-1; k++){
			hm = hm + abs(done_Scan[k+1] - done_Scan[k]);
		}
	}

	return hm;
}

// main function 
int main(int argc, char* argv[]){

	int h = atoi(argv[1]);
	
	char * l = "LEFT";
	char * r = "RIGHT";

	int look_false = 0;
	int look_true = 1;
	
	char * direction;
	direction = malloc(strlen(argv[2])+1);
	strcpy(direction, argv[2]);
	printf("\n");
	
	// left direction check
	if(strcmp(direction, l) == 0){
		
		printf("FCFS - Total head movements = %d\n\n", FCFS(requests, h));
		printf("SSTF - Total head movements = %d\n\n", SSTF(requests, h));
		printf("SCAN - Total head movements = %d\n\n", SCAN_L(requests, h, look_false));
		printf("LOOK - Total head movements = %d\n\n", SCAN_L(requests, h, look_true));
	}

	// right direction check
	else if(strcmp(direction, r) == 0){
		
		printf("FCFS - Total head movements = %d\n\n", FCFS(requests, h));
		printf("SSTF - Total head movements = %d\n\n", SSTF(requests, h));
		printf("SCAN - Total head movements = %d\n\n", SCAN_R(requests, h, look_false));
		printf("LOOK - Total head movements = %d\n\n", SCAN_R(requests, h, look_true));
		
	}

	return 0;
}
