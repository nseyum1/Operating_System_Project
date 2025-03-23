
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "process.h"

// Comparison function for `qsort()`
// - Primary sort by priority
// - Secondary sort by arrival time
int sortPriority(const void *a, const void *b)
{
	Process *p1 = (Process *)a;
	Process *p2 = (Process *)b;
	if (p1->priority != p2->priority)
	{
		return p1->priority - p2->priority;
	}
	else
	{
		return p1->arrival_time - p2->arrival_time;
	}
}

int main(int argc, char *argv[])
{
	// Usage
	if (argc < 2)
	{
		printf("\nUsage: %s <filename> sorting <priority>\n", argv[0]);
		return 1;
	}

	//================================================================================================
	// Parse supplied file containing processes
	/*

		Files must be formatted as follows:

		PID Arrival_Time Burst_Time Priority
		1   0           5          2
		...

	*/

	FILE *processfile = fopen(argv[1], "r");
	if (processfile == NULL)
	{
		perror("Error opening file");
		return 1;
	}

	char line_index[256];

	// To skip the header line in the file
	// - This assumes the header line holds schema information
	if (fgets(line_index, sizeof(line_index), processfile) == NULL)
	{
		perror("Error reading the header of the file\n");
		fclose(processfile);
		return 1;
	}

	// Parse processes
	Process processes[256];
	int count = 0;
	while (fgets(line_index, sizeof(line_index), processfile) != NULL && count < 256)
	{
		int pid, arrivaltime, bursttime, priority;
		if (sscanf(line_index, "%d %d %d %d", &pid, &arrivaltime, &bursttime, &priority) == 4)
		{
			processes[count].pid = pid;
			processes[count].arrival_time = arrivaltime;
			processes[count].burst_time = bursttime;
			processes[count].priority = priority;
			count++;
		}
	}
	fclose(processfile);
	//================================================================================================

	//================================================================================================
	// Priority Scheduler

	// Sort processes by priority
	qsort(processes, count, sizeof(Process), sortPriority);

	// Arrays to track process stats
	int waiting_times[256];
	int turnaround_times[256];
	int start_times[256];

	// Scheduler metrics
	double total_waiting_time = 0;
	double total_turnaround_time = 0;

	// Gantt chart utils
	int gantt_chart[256];
	int gantt_index = 0;

	// Simulate process scheduling & execution
	int current_time = 0;
	for (int i = 0; i < count; i++)
	{
		// Wait for process to arrive
		while (current_time < processes[i].arrival_time)
		{
			sleep(1);
			current_time++;
		}

		// Process arrived...

		// Record start
		start_times[i] = current_time;

		// Simulate execution
		pid_t pid = fork();
		if (pid == 0)
		{
			// Child/current process running
			printf("Process (PID: %d) started at time %d\n", processes[i].pid, current_time);
			sleep(processes[i].burst_time);
			printf("Process (PID: %d) finished at time %d\n", processes[i].pid, current_time + processes[i].burst_time);
			exit(0);
		}
		else if (pid > 0)
		{
			// Parent process
			// - Record process stats
			waiting_times[i] = current_time - processes[i].arrival_time;
			current_time += processes[i].burst_time;
			turnaround_times[i] = current_time - processes[i].arrival_time;
			gantt_chart[gantt_index++] = processes[i].pid;

			// Wait for child process to finish execution
			int status;
			waitpid(pid, &status, 0);

			printf(
				"Process %d: Waiting Time = %d, Turnaround Time = %d\n",
				processes[i].pid, waiting_times[i], turnaround_times[i]);

			// Update scheduler metrics
			total_waiting_time += waiting_times[i];
			total_turnaround_time += turnaround_times[i];
		}
		else
		{
			perror("Fork Failed");
			return 1;
		}
	}
	//================================================================================================

	//================================================================================================
	// Display results

	// Gantt Chart
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

	// Scheduler metrics
	printf("\nAverage Waiting Time: %.2f\n", total_waiting_time / count);
	printf("Average Turnaround Time: %.2f\n", total_turnaround_time / count);
	//================================================================================================

	return 0;
}
