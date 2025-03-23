
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
	int waiting_times[256];
	int turnaround_times[256];
	double total_waiting_time = 0;
	double total_turnaround_time = 0;
	int start_times[256];
	int gantt_chart[256];
	int gantt_index = 0;


	for (int i = 0; i < count; i++)
	{
		while (current_time < processes[i].Arrival_Time)
		{
			sleep(1);
			current_time++;
		}
		
		start_times[i] = current_time;

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
			waiting_times[i] = current_time - processes[i].Arrival_Time;
			current_time += processes[i].Burst_Time;
			turnaround_times[i] = current_time - processes[i].Arrival_Time;

			gantt_chart[gantt_index++] = processes[i].PID; 

			int status;
			waitpid(pid, &status, 0);
		
        		printf("Process P%d: Waiting Time = %d, Turnaround Time = %d\n",
               		processes[i].PID, waiting_times[i], turnaround_times[i]);
        		total_waiting_time += waiting_times[i];
        		total_turnaround_time += turnaround_times[i];

		}
		else
		{
			perror("Fork Failed");
			return 1;
		}
	}

	int max_width = 0;
	for (int w = 0; w < count; w++)
	{
		int width = snprintf(NULL, 0, "%d", processes[w].Burst_Time);
		if (width > max_width) 
		{
			max_width = width;
		}
	}

	printf("\n");
	for (int g = 0; g < gantt_index; g++)
	{
		printf("| P%-*d ", max_width, gantt_chart[g]);
	}

	printf("|\n");
	current_time = 0;
	for (int j = 0; j < count; j++)
	{
		printf("%-*d", max_width + 4, current_time);
		current_time += processes[j].Burst_Time; /* Changed/Modified: current_time = processes[j].Arrival_Time + processes[j].Burst_Time to current_time += processes[j].Burst_Time */
	}
	printf("%-*d\n", max_width + 4, current_time);

    	// Calculate and Print Average Waiting and Turnaround Times
    	printf("\nAverage Waiting Time: %.2f\n", total_waiting_time / count);
    	printf("Average Turnaround Time: %.2f\n", total_turnaround_time / count);


	return 0;


}
