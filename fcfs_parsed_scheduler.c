#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "process_variables.h"

// Sort by arrival time for FCFS
int sortByArrivalTime(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->Arrival_Time - p2->Arrival_Time;
}

// Function to create a visual Gantt chart
void create_gantt_chart(Process *processes, int num_processes, int *start_time, int *completion_time) {
    printf("\nGantt Chart:\n");
    printf("0");
    int prev_end_time = 0;
    
    for (int i = 0; i < num_processes; i++) {
        // Print idle time if any
        if (start_time[i] > prev_end_time) {
            for (int j = 0; j < start_time[i] - prev_end_time; j++) {
                printf(" ");
            }
            printf("| IDLE |");
            printf("%d", start_time[i]);
        }
        
        // Print process execution
        for (int j = 0; j < processes[i].Burst_Time; j++) {
            printf("-");
        }
        printf("| P%d |", processes[i].PID);
        printf("%d", completion_time[i]);
        
        prev_end_time = completion_time[i];
    }
    printf("\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("\nUsage: %s <filename>\n", argv[0]);
        return 1;
    }
    
    // File parsing - integrated into main
    FILE *processfile = fopen(argv[1], "r");
    if (processfile == NULL) {
        perror("Error opening file");
        return 1;
    }
    
    char line_index[256];
    
    // Skip the header line
    if (fgets(line_index, sizeof(line_index), processfile) == NULL) {
        perror("Error reading the header of the file\n");
        fclose(processfile);
        return 1;
    }
    
    // Create a buffer to store processes
    Process processes[256];
    int count = 0;
    
    while (fgets(line_index, sizeof(line_index), processfile) != NULL && count < 256) {
        int pid, arrivaltime, bursttime, priority;
        if (sscanf(line_index, "%d %d %d %d", &pid, &arrivaltime, &bursttime, &priority) == 4) {
            processes[count].PID = pid;
            processes[count].Arrival_Time = arrivaltime;
            processes[count].Burst_Time = bursttime;
            processes[count].Priority = priority;
            count++;
        }
    }
    
    fclose(processfile);
    
    // Print the parsed processes
    printf("\nParsed Processes:\n");
    for (int i = 0; i < count; i++) {
        printf("Process [%d]: PID=%d, Arrival=%d, Burst=%d, Priority=%d\n", 
               i, 
               processes[i].PID, 
               processes[i].Arrival_Time, 
               processes[i].Burst_Time, 
               processes[i].Priority);
    }
    
    printf("\n========== FCFS Scheduling ==========\n");
    
    // Sort processes by arrival time (FCFS)
    qsort(processes, count, sizeof(Process), sortByArrivalTime);
    
    // Arrays to track process stats
    int completion_time[count];
    int waiting_time[count];
    int turnaround_time[count];
    int start_time[count];
    
    int current_time = 0;
    float total_waiting_time = 0;
    float total_turnaround_time = 0;
    
    printf("\nExecution Order:\n");
    printf("PID\tArrival\t Burst\tStart\tCompletion\tWaiting\tTurnaround\n");
    
    // Process each job in arrival time order
    for (int i = 0; i < count; i++) {
        // If current time is less than arrival time, update current time
        if (current_time < processes[i].Arrival_Time) {
            current_time = processes[i].Arrival_Time;
        }
        
        start_time[i] = current_time;
        
        // Create a new process
        pid_t pid = fork();
        
        if (pid < 0) {
            // Fork failed
            perror("Fork failed");
            exit(1);
        } else if (pid == 0) {
            // Child process
            printf("Process %d started execution at time %d\n", processes[i].PID, current_time);
            
            // Simulate CPU burst using sleep
            sleep(processes[i].Burst_Time);
            
            // Exit child process
            exit(0);
        } else {
            // Parent process
            pid_t process_pids[count];
            process_pids[i] = pid;
            
            // Wait for child to complete
            int status;
            waitpid(pid, &status, 0);
            
            // Update current time
            current_time += processes[i].Burst_Time;
            
            // Calculate process times
            completion_time[i] = current_time;
            turnaround_time[i] = completion_time[i] - processes[i].Arrival_Time;
            waiting_time[i] = turnaround_time[i] - processes[i].Burst_Time;
            
            // Update total times
            total_waiting_time += waiting_time[i];
            total_turnaround_time += turnaround_time[i];
            
            printf("%d\t%d\t%d\t%d\t%d\t\t%d\t%d\n", 
                   processes[i].PID, 
                   processes[i].Arrival_Time, 
                   processes[i].Burst_Time,
                   start_time[i],
                   completion_time[i],
                   waiting_time[i],
                   turnaround_time[i]);
            
            // Kill process explicitly
            kill(pid, SIGTERM);
        }
    }
    
    // Calculate and print average times
    float avg_waiting_time = total_waiting_time / count;
    float avg_turnaround_time = total_turnaround_time / count;
    
    printf("\nAverage Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
    
    // Visualization of Gantt chart
    create_gantt_chart(processes, count, start_time, completion_time);
    
    return 0;
}