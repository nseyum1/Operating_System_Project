
#include <stdio.h>
#include <stdlib.h> /* This is the library where qsort is from */
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "process_variables.h"
	
int sortPriority(const void *a, const void *b)
{
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    if (p1->Priority != p2->Priority)
    {
	return p1->Priority - p2->Priority;
    }
    else
    {
        return p1->Arrival_Time - p2->Arrival_Time;
    }
}


int main(int argc, char *argv[]) /* main entry for program and has parameters arguments passed into command line and what those arguments are */
{
	if (argc < 2) /* if argc on command line passed arguments is less than 2 this usage help will output */
	{
		printf("\nUsage: %s <filename> sorting <priority>\n", argv[0]);
		return 1;
	}

	FILE *processfile = fopen(argv[1], "r"); /* File pointer to processfile which is the filename passed to read after the first command line argument inputted which would be the executable file */
	if (processfile == NULL) /* If processfile is NULL */
	{
		perror("Error opening file");
		return 1;
	}

	char line_index[256]; /* Tracks the line count or line number */

	/* To skip the header line in the file */
	if (fgets(line_index, sizeof(line_index), processfile) == NULL)
	{
		perror("Error reading the header of the file\n");
		fclose(processfile);
		return 1;
	}

	/* Create a buffer to store up to 256 characters */

	Process processes[256];
	int count = 0;

	while (fgets(line_index, sizeof(line_index), processfile) != NULL && count < 256)
	{
		int pid, arrivaltime, bursttime, priority;
		if (sscanf(line_index, "%d %d %d %d", &pid, &arrivaltime, &bursttime, &priority) == 4)
		{
			processes[count].PID = pid;
			processes[count].Arrival_Time = arrivaltime;
			processes[count].Burst_Time = bursttime;
			processes[count].Priority = priority;
			count++;
		}
	}

	fclose(processfile); /* To close the file after data has been parsed or read */
	
	qsort(processes, count, sizeof(Process), sortPriority);

	int current_time = 0;

	for (int i = 0; i < count; i++)
	{
		while (current_time < processes[i].Arrival_Time)
		{
			sleep(1);
			current_time++;
		}

		pid_t pid = fork();

		if (pid == 0)
		{
			printf("Process (PID:%d) started at time %d\n", processes[i].PID, current_time);
			sleep(processes[i].Burst_Time);
			printf("Process (PID:%d) finished at time %d\n", processes[i].PID, current_time + processes[i].Burst_Time);
			exit(0);
		}
		else if (pid > 0)
		{
			processes[i].process_id = pid;
			current_time += processes[i].Burst_Time;

			int status;
			waitpid(pid, &status, 0);
		}

		else
		{
			perror("Fork Failed");
			return 1;
		}
	}
	

	return 0;


}
