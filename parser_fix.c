
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Process { /* A structure defined with typedef data type and named Process */
	int pid;
	int arrival_time;
	int burst_time;
	int priority;
} Process;

int main(int argc, char *argv[]) /* main entry for program and has parameters arguments passed into command line and what those arguments are */
{
	if (argc < 2) /* if argc on command line passed arguments is less than 2 this usage help will output */
	{
		printf("\nUsage: %s <filename>\n", argv[0]);
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
			processes[count].pid = pid;
			processes[count].arrival_time = arrivaltime;
			processes[count].burst_time = bursttime;
			processes[count].priority = priority;
			count++;
		}
	}

	fclose(processfile); /* To close the file after data has been parsed or read */

	for (int iterate = 0; iterate < count; iterate++)
	{
		printf("\nProcess [%d]:\n", iterate);
		printf("Process [%d]'s PID: %d\n", iterate, processes[iterate].pid);
		printf("Process [%d]'s Arrival Time: %d\n", iterate, processes[iterate].arrival_time);
		printf("Process [%d]'s Burst Time: %d\n", iterate, processes[iterate].burst_time);
		printf("Process [%d]'s Priority: %d\n", iterate, processes[iterate].priority);
	}
	
	return 0;


}
