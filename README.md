# Process Scheduler Implementation

## Overview
This project implements various process scheduling algorithms commonly used in operating systems. The schedulers take a list of processes with their attributes (PID, arrival time, burst time, priority) and simulate their execution according to different scheduling policies. Through this implementation, we demonstrate the fundamental concepts of process management, CPU scheduling, and resource allocation in operating systems.

## Algorithms Implemented

### First-Come-First-Served (FCFS)
- **Description**: Non-preemptive scheduling algorithm where processes are executed in the order they arrive in the ready queue
- **Characteristics**: Simple to implement but may lead to the "convoy effect" where short processes wait behind long processes
- **Best for**: Batch systems where throughput is more important than response time

### Priority Scheduling
- **Description**: Non-preemptive scheduling algorithm where processes are executed based on priority values (lower number = higher priority)
- **Characteristics**: Can lead to starvation of lower priority processes if higher priority processes continuously arrive
- **Best for**: Systems where certain processes need preferential treatment
- **Implementation details**: Our version uses arrival time as a tie-breaker when priority values are equal

## Project Structure
- `processes.txt` - Sample input file containing process information
- `process_variables.h` - Header file with process structure definition and sorting function prototypes
- `parser_fix.c` - Standalone process file parser that demonstrates proper file I/O and data handling
- `fcfs_parsed_scheduler.c` - Complete implementation of FCFS scheduling algorithm with visualization
- `priority_process_algorithm.c` - Implementation of Priority scheduling algorithm with process simulation

## Detailed Component Descriptions

### process_variables.h
This header file defines the core data structure used throughout the project:
- Declares the `Process` struct that stores all necessary process information
- Contains function prototypes for comparison functions used in sorting processes
- Ensures consistent data representation across all components

### parser_fix.c
A standalone utility that demonstrates:
- Proper file handling with error checking
- Data parsing techniques for structured input
- Memory management for process information
- Validation of input data
- Formatted output of parsed process information

### fcfs_parsed_scheduler.c
A complete FCFS scheduler implementation that:
- Parses process information from input files
- Sorts processes based on arrival time
- Simulates CPU execution using actual system processes via `fork()`
- Calculates key performance metrics (waiting time, turnaround time)
- Creates a visual Gantt chart for process execution visualization
- Handles idle CPU time between process arrivals
- Implements proper process synchronization and termination

### priority_process_algorithm.c
A priority-based scheduler that:
- Sorts processes based on priority values (with arrival time as a secondary criterion)
- Simulates real-time execution of processes
- Handles process synchronization between parent and child processes
- Demonstrates priority-based CPU allocation
- Provides detailed execution timeline showing when each process starts and finishes

## Process File Format
The input file should be structured as follows:
```
PID Arrival_Time Burst_Time Priority
1   0           5          2
2   2           3          1
...
```

Where:
- **PID**: Unique process identifier
- **Arrival_Time**: Time at which the process enters the ready queue
- **Burst_Time**: Total CPU time required by the process to complete
- **Priority**: Importance value of the process (lower number = higher priority)

## Compilation Instructions

### Compiling the FCFS Scheduler
```bash
gcc fcfs_parsed_scheduler.c -o fcfs_scheduler
```

### Compiling the Priority Scheduler
```bash
gcc priority_process_algorithm.c -o priority_scheduler
```

### Compiling the Standalone Parser
```bash
gcc parser_fix.c -o process_parser
```

## Usage Instructions

### FCFS Scheduler
```bash
./fcfs_scheduler processes.txt
```

### Priority Scheduler
```bash
./priority_scheduler processes.txt
```

### Standalone Parser
```bash
./process_parser processes.txt
```

## Features

### FCFS Scheduler
- Simulates process execution based on arrival time
- Creates visual Gantt chart showing execution timeline
- Calculates and displays:
  - Start time
  - Completion time
  - Waiting time
  - Turnaround time
- Computes average waiting and turnaround times
- Handles process creation and termination using system calls
- Provides detailed progress information during execution
- Visualizes idle CPU time between process executions

### Priority Scheduler
- Executes processes based on priority value
- Uses arrival time as a secondary sorting criterion for processes with equal priority
- Simulates process execution using actual system processes
- Displays process start and finish times
- Handles CPU idle time when no processes are available
- Prevents priority inversion by respecting arrival order for equal priorities
- Provides real-time execution status updates
- Implements proper process synchronization with parent-child relationships

### Process Parser
- Efficiently reads and parses process information from input files
- Validates input data format and content
- Displays parsed process details for verification
- Handles file I/O errors gracefully
- Implements proper memory management techniques
- Provides detailed output for process verification

## Example Output

### FCFS Scheduler
```
Parsed Processes:
Process [0]: PID=1, Arrival=0, Burst=5, Priority=2
Process [1]: PID=2, Arrival=2, Burst=3, Priority=1
...

========== FCFS Scheduling ==========

Execution Order:
PID     Arrival  Burst   Start   Completion      Waiting Turnaround
1       0       5       0       5               0       5
2       2       3       5       8               3       6
3       4       2       8       10              4       6
4       1       6       10      16              9       15
5       3       4       16      20              13      17
...

Average Waiting Time: 12.50
Average Turnaround Time: 16.90

Gantt Chart:
0-----|P1|5----|P2|8--|P3|10------|P4|16----|P5|20...
```

### Priority Scheduler
```
Process (PID:2) started at time 2
Process (PID:2) finished at time 5
Process (PID:4) started at time 5
Process (PID:4) finished at time 11
Process (PID:1) started at time 11
Process (PID:1) finished at time 16
Process (PID:5) started at time 16
Process (PID:5) finished at time 20
Process (PID:3) started at time 20
Process (PID:3) finished at time 22
...
```

## Implementation Details and Algorithms

### FCFS Implementation
The FCFS scheduler works by:
1. Parsing process information from the input file
2. Sorting processes based on arrival time using the `qsort()` function
3. Initializing arrays to track start time, completion time, waiting time, and turnaround time
4. For each process:
   - Wait until the process arrival time if necessary
   - Create a child process using `fork()` to simulate CPU execution
   - Use `sleep()` to simulate the burst time
   - Wait for the child process to complete using `waitpid()`
   - Calculate completion time, turnaround time, and waiting time
   - Terminate the child process using `kill()`
5. Calculate and display average waiting and turnaround times
6. Generate a visual Gantt chart showing the execution timeline

### Priority Scheduling Implementation
The Priority scheduler works by:
1. Parsing process information from the input file
2. Sorting processes based on priority values (with arrival time as tie-breaker) using `qsort()`
3. For each process:
   - Wait until the process arrival time if necessary (idle CPU time)
   - Create a child process using `fork()` to simulate CPU execution
   - Use `sleep()` to simulate the burst time
   - Display process start and finish times
   - Wait for the child process to complete using `waitpid()`
4. The `sortPriority()` function ensures that:
   - Processes with lower priority values (higher importance) are executed first
   - For processes with equal priority, the one that arrived earlier is executed first

### Key Formulas Used
- **Waiting Time** = Start Time - Arrival Time
- **Turnaround Time** = Completion Time - Arrival Time
- **Completion Time** = Start Time + Burst Time
- **Average Waiting Time** = Sum of all waiting times / Number of processes
- **Average Turnaround Time** = Sum of all turnaround times / Number of processes

## Performance Considerations
- The schedulers create actual system processes to simulate the execution of each process
- Time units in the input file are mapped to seconds in the simulation
- For large process sets, expect execution time to scale with the total burst time
- Memory usage increases linearly with the number of processes
- The Gantt chart visualization may become unwieldy for large process sets
- Process creation overhead may impact performance measurements

## Technical Concepts Demonstrated

### System Calls
- `fork()`: Creates new processes to simulate CPU execution
- `waitpid()`: Synchronizes parent and child processes
- `kill()`: Terminates processes when necessary
- `exit()`: Properly terminates child processes

### Memory Management
- Dynamic allocation for process information
- Proper memory deallocation to prevent leaks
- Array-based data structures for performance metrics

### File I/O
- File opening and error handling
- Line-by-line parsing of structured data
- Proper file closure to prevent resource leaks

### Process Scheduling Concepts
- CPU scheduling algorithms implementation
- Process state transitions (ready, running, terminated)
- CPU utilization visualization
- Performance metric calculations

## Future Enhancements
- **Round Robin (RR) scheduling**: Implement time-quantum based scheduling
- **Shortest Job First (SJF)**: Implement non-preemptive SJF
- **Shortest Remaining Time First (SRTF)**: Implement preemptive version of SJF
- **Multi-level Queue Scheduling**: Implement multiple priority queues
- **Aging Mechanism**: Prevent starvation in priority scheduling
- **Interactive Mode**: Allow users to input processes interactively
- **GUI Interface**: Create a graphical interface for visualization
- **Real-time Statistics**: Display CPU utilization and other metrics in real-time
- **Configurable Parameters**: Allow users to set parameters like time quantum
- **Multi-Core Simulation**: Extend to simulate multiple CPU cores

## Contributors
- Niko Avradopoulos
- Hemant Kosaraju
- Naol Seyum
- Kaleab Abebe

## Submitted to
Dr. Roya Hosseini

## License
This project is licensed under the ProcessSync Team Open Source License - see the LICENSE file for details.

## Acknowledgments
- Dr. Roya Hosseini for project guidance
- Visual Studio Code and C development environment
- Operating System Concepts (10th Edition) by Abraham Silberschatz, Peter B. Galvin, Greg Gagne
- The Linux Programming Interface by Michael Kerrisk
