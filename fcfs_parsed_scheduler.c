#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "process_variables.h"

// Comparison function for `qsort()`
// - This will sort the processes by arrival time
int sortByArrivalTime(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->arrival_time - p2->arrival_time;
}

// Create a visual Gantt chart of an array of processes against their execution times
void create_gantt_chart(Process *processes, int num_processes, int *start_time, int *completion_time) {
    printf("\nGantt Chart:\n");
    printf("0");
    int prev_end_time = 0;
    
    for (int i = 0; i < num_processes; i++) {
        // Print idle time if current process doesn't start as soon as previous process ends
        if (start_time[i] > prev_end_time) {
            for (int j = 0; j < start_time[i] - prev_end_time; j++) {
                printf(" ");
            }
            printf("| IDLE |");
            printf("%d", start_time[i]);
        }
        
        // Print process execution
        for (int j = 0; j < processes[i].burst_time; j++) {
            printf("-");
        }
        printf("| P%d |", processes[i].pid);
        printf("%d", completion_time[i]);
        
        prev_end_time = completion_time[i];
    }
    printf("\n");
}

// Parse a file of processes and simulate FCFS scheduling
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("\nUsage: %s <filename>\n", argv[0]);
        return 1;
    }
    //================================================================================================
    // File parsing
    /*

        Files must be formatted as follows:
        
        PID Arrival_Time Burst_Time Priority
        1   0           5          2
        ...

    */

    FILE *processfile = fopen(argv[1], "r");
    if (processfile == NULL) {
        perror("Error opening file... Exiting\n");
        return 1;
    }
    
    char line_index[256];
    
    // Skip the header line
    // - This assumes the header line holds schema information
    if (fgets(line_index, sizeof(line_index), processfile) == NULL) {
        perror("Error reading file header... Exiting\n");
        fclose(processfile);
        return 1;
    }
    
    // Parse process data
    Process processes[256];
    int count = 0;
    while (fgets(line_index, sizeof(line_index), processfile) != NULL && count < 256) {
        int pid, arrival_time, burst_time, priority;
        if (sscanf(line_index, "%d %d %d %d", &pid, &arrival_time, &burst_time, &priority) == 4) {
            processes[count].pid = pid;
            processes[count].arrival_time = arrival_time;
            processes[count].burst_time = burst_time;
            processes[count].priority = priority;
            count++;
        }
    }
    fclose(processfile);

    printf("\nDiscovered Processes:\n");
    for (int i = 0; i < count; i++) {
        printf("Process %d (PID=%d): Arrival=%d, Burst=%d, Priority=%d\n", 
               i, 
               processes[i].pid, 
               processes[i].arrival_time, 
               processes[i].burst_time, 
               processes[i].priority);
    }
    //================================================================================================
    
    //================================================================================================
    // FCFS Scheduling
    printf("\n========== FCFS Scheduling ==========\n");
    
    // Sort processes by arrival time (FCFS)
    qsort(processes, count, sizeof(Process), sortByArrivalTime);
    
    // Arrays to track process stats
    int completion_time[count];
    int waiting_time[count];
    int turnaround_time[count];
    int start_time[count];
    
    // Scheduler metrics
    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    
    printf("\nExecution Order:\n");
    printf("PID\tArrival\tBurst\tStart\tCompletion\tWaiting\tTurnaround\n");
    int current_time = 0;
    
    // Process each job in arrival time order (FCFS)

    for (int i = 0; i < count; i++) {
        // If current time is less than arrival time, update current time
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        
        start_time[i] = current_time;
        
        // Create a new process
        pid_t pid = fork();
        
        if (pid < 0) {
            // Fork failed
            perror("Fork failed... Exiting\n");
            exit(1);
        } else if (pid == 0) {
            // Child process (simulate process execution)
            printf("Process %d started execution at time %d\n", processes[i].pid, current_time);
            
            // Simulate CPU burst
            sleep(processes[i].burst_time);
            
            // Exit child process, continue scheduler
            exit(0);
        } else {
            // Parent process
            pid_t pids[count];
            pids[i] = pid;
            
            // When child process completes & update time
            int status;
            waitpid(pid, &status, 0);
            current_time += processes[i].burst_time;

            // Record process metrics
            completion_time[i] = current_time;
            turnaround_time[i] = completion_time[i] - processes[i].arrival_time;
            waiting_time[i] = turnaround_time[i] - processes[i].burst_time;
            
            // Update scheduler metrics
            total_waiting_time += waiting_time[i];
            total_turnaround_time += turnaround_time[i];
            
            // Display process metrics using predefined format
            printf("%d\t%d\t%d\t%d\t%d\t\t%d\t%d\n", 
                   processes[i].pid, 
                   processes[i].arrival_time, 
                   processes[i].burst_time,
                   start_time[i],
                   completion_time[i],
                   waiting_time[i],
                   turnaround_time[i]);
            
            kill(pid, SIGTERM);
        }
    }
    //================================================================================================

    //================================================================================================
    // Metrics
    // Calculate and print average times
    float avg_waiting_time = total_waiting_time / count;
    float avg_turnaround_time = total_turnaround_time / count;
    
    printf("\nAverage Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    
    // Visualization of Gantt chart
    create_gantt_chart(processes, count, start_time, completion_time);
    //================================================================================================

    return 0;
}
