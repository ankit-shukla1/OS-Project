#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include<sys/types.h>
#include<unistd.h>

#define MIN_PID 100
#define MAX_PID 1000

#define size MAX_PID-MIN_PID

// integer array used as the data structure for keeping record of which pid is available.
// if pid_count[100] has value 0 that means pid 100 is  available and if it has value 1 then it has already been allocated.
int pid_count[size];


// function for initialising the integer array which will keep account of the available process ids.
int allocate_map(void)
{
	int i=0;
	while(i<size)
	{
	//initialising all the values to 0 to indicate that at the start all the process ids are available.	
		pid_count[i++]=0;
	}

//checking if all the values have been inititalised to 0.	
	if(i==size)
	{
		// if everything has been initialised properly then returning 1.
		return 1;
	}
	
	else
	{
	// if some error has occured during the initialisation then returning -1 to indicate the error.
		return -1;	
	}
}


int allocate_pid(void)
{
	int i;
	for(i=0;i<size;i++)
	{
		if(pid_count[i]==0)
		{
			pid_count[i]=1;
			return i;
		}
	}
	
	if(i==size)
	return -1;		
}


void release_pid(int tid)
{
	pid_count[tid]=0;
}



void *make_thread(void *t_position)
{
	int thread_n = *((int *)t_position);
	int t_id  = allocate_pid();
	t_id+=MIN_PID;
	int wait_time  = rand()%15 + 1;
	if(t_id != -1)
	{
		printf("PId %d allocated to thread %d\n\n",t_id,thread_n);
		sleep(wait_time);
		printf("PId %d released by thread %d after %d seconds\n\n",t_id,thread_n,wait_time);
		release_pid(t_id);
		
	}
	else
	printf("All the pids have been allocated, the thread will have to wait\n\n");

	pthread_exit(NULL);
}


int main()
{
	
	int total_threads=102;
	
	pthread_t thread_count[total_threads];
	
	
	// calling the function to intialise all values of the integer array to 0.
	int check=allocate_map();
	
	if(check == 1)
	{
	// printing the succesful execution message for initialisation of the data structure 
		printf("The initialisation of the pid counter is successful.\n\n");
	}
	
	else
	{
	// printing the error message for initialisation of the data structure.
		printf("There was a error in initialising the pid counter.\n\n");
	}
	
	
	int i;   //loop variable
	
	for(i=0; i<total_threads; i++)
	{
		int test=pthread_create(&thread_count[i],NULL,*make_thread,(void*)&i);
		
		if(test!=0)
		{
		 	printf("Some error occured during creation of the thread\n\n");			
		}
	}

	for(i=0; i<total_threads; i++)
        pthread_join(thread_count[i],NULL);
	
	
}
