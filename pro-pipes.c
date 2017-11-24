#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/types.h>

#define M_READ     0
#define M_WRITE    1
#define A_READ     2
#define A_WRITE    3

#define NUM_PIPES   2
int main()

{
	
	pid_t pid,pid1,pid2;
	int status,n;

	int ray[n];	
	
	 int fd[2*NUM_PIPES];
	//whiel val[0] is the maximum and val[1] is the average 
   	 float val[2]={0.0,0.0};
	int  len, i;

	  for (i=0; i<NUM_PIPES; ++i)
    {
	//creating two pipes one for max and one for min
        if (pipe(fd+(i*2)) < 0)
        {
            perror("Failed to allocate pipes");
            exit(EXIT_FAILURE);
        }
    }

	printf("Enter size of your Arrays : ");
	scanf("%d", &n);
int t;
	for(i = 0; i < n; i++)
		{
			printf("Enter Your Data : ");
			
			scanf("%d", &t);
			ray[i]=t;
			
		}





	pid1 = fork();

	if(pid1 < 0)
	{
		fprintf(stderr, "Fork Failed" );
		return 1; 
	}
	else if (pid1 == 0)//child process
	{
	int max =ray[0];
		
		printf("In the Child Process # 1\n");

		   close(fd[A_READ]);
        	  close(fd[A_WRITE]);
        	  close(fd[M_READ]);

		for(i=1;i<n;i++)
		{
			if(ray[i] > max)
				max = ray[i];
		}
		
		val[0]= (float)max;

		if (write(fd[M_WRITE], &val[0], sizeof(val[0])) != sizeof(val[0]))
            {
                perror("Child: Failed to write  value");
                exit(EXIT_FAILURE);
            }
	
		close(fd[M_WRITE]);

	}
	else
	{
		pid2 = fork();
		if(pid2 < 0){
			fprintf(stderr, "Fork Failed" );
		}
		else if(pid2 == 0){
			
			 close(fd[M_READ]);
        	 	 close(fd[M_WRITE]);
        	 	 close(fd[A_READ]);
			int sum=0;
			float average;
			printf("In Child Process # 2 \n");

			for(i=0;i<n;i++){
				
				sum+=ray[i];}
			average = (float)sum/n;
	
			val[1]= average;

		if (write(fd[A_WRITE], &val[1], sizeof(val[1])) != sizeof(val[1]))
            {
                perror("Child: Failed to write  value");
                exit(EXIT_FAILURE);
            }
	
		close(fd[A_WRITE]);

		}
		else{
			int n =2;
			while(n>0){

			pid = wait(&status);
		printf("Child with PID %ld exited with status 0x%x.\n", (long)pid, status);
		n--;
				}
				
		close(fd[M_WRITE]);
		close(fd[A_WRITE]);
		
		len = read(fd[M_READ], &val[0], sizeof(val[0]));
    if (len < 0)
    {
        perror("Parent: failed to read value of maximum from pipe");
        exit(EXIT_FAILURE);
    }
    else if (len == 0)
    {
        // not an error, but certainly unexpected
        fprintf(stderr, "Parent: Read EOF from pipe");
    }
    else
    {
        // report what we received
        printf("Parent: Received %.0f as maximum\n", val[0]);
    }
	



		len = read(fd[A_READ], &val[1], sizeof(val[1]));
    if (len < 0)
    {
        perror("Parent: failed to read value of average from pipe");
        exit(EXIT_FAILURE);
    }
    else if (len == 0)
    {
        // not an error, but certainly unexpected
        fprintf(stderr, "Parent: Read EOF from pipe");
    }
    else
    {
        // report what we received
        printf("Parent: Received %.2f as average\n", val[1]);
    }
		close(fd[M_READ]);
		close(fd[A_READ]);
	


		}
		
	}
	return 0;
}
