
#include <stdio.h>
#include <stdlib.h> /* This is the library where qsort is from */
#include <string.h>
#include "process_variables.h"
	
int sortPriority(const void *a, const void *b)
{
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->Priority - p2->Priority;
}


int main(int argc, char *argv[]) /* main entry for program and has parameters arguments passed into command line and what those arguments are */
{
	if (argc < 3) /* if argc on command line passed arguments is less than 2 this usage help will output */
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
	
	printf("\nThe Original Processes \n");
	for (int iterate = 0; iterate < count; iterate++)
	{
		printf("\nProcess [%d]:\n", iterate);
		printf("Process [%d]'s PID: %d\n", iterate, processes[iterate].PID);
		printf("Process [%d]'s Arrival Time: %d\n", iterate, processes[iterate].Arrival_Time);
		printf("Process [%d]'s Burst Time: %d\n", iterate, processes[iterate].Burst_Time);
		printf("Process [%d]'s Priority: %d\n", iterate, processes[iterate].Priority);
	}
	

	if (strcmp(argv[2], "priority") == 0)
	{
		qsort(processes, count, sizeof(Process), sortPriority);
	}
	else
	{
		return 1;
	}
	
	printf("\nThe Priority Sorted Processes \n");
	for (int iterate = 0; iterate < count; iterate++)
	{
		printf("\nProcess [%d]:\n", iterate);
		printf("Process [%d]'s PID: %d\n", iterate, processes[iterate].PID);
		printf("Process [%d]'s Arrival Time: %d\n", iterate, processes[iterate].Arrival_Time);
		printf("Process [%d]'s Burst Time: %d\n", iterate, processes[iterate].Burst_Time);
		printf("Process [%d]'s Priority: %d\n", iterate, processes[iterate].Priority);
	}

	return 0;


}
