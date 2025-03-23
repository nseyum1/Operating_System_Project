# Operating System Process Scheduling

## Contributors
- Kaleab Abebe
  - GitHub: [Kal-debug](https://github.com/Kal-debug)
  - LinkedIn: https://www.linkedin.com/in/kaleababebe/
- Niko Avradopoulos
  - GitHub: [navrade1](https://github.com/navrade1)
  - LinkedIn: https://www.linkedin.com/in/nikoavra/
- Hemant Kosaraju
  - GitHub: [Hemantdesignscode](https://github.com/Hemantdesignscode)
  - LinkedIn: https://www.linkedin.com/in/hemant-kosaraju-84631b257/
- Naol Seyum
  - GitHub: [nseyum1](https://github.com/nseyum1)
  - LinkedIn: https://www.linkedin.com/in/naol-seyum-8b24b1280/


## Overview
This project implements various process scheduling algorithms commonly used in operating systems. The schedulers take a list of processes with their attributes (PID, arrival time, burst time, priority) and simulate their execution according to different scheduling policies.

## Algorithms Implemented

### First-Come-First-Served (FCFS)
- **Description**: Non-preemptive scheduling algorithm where processes are executed in the order they arrive.
- **Characteristics**: Simple but may lead to the "convoy effect".
- **Best for**: Batch systems.

### Priority Scheduling
- **Description**: Non-preemptive scheduling algorithm where processes are executed based on priority values.
- **Characteristics**: Can lead to starvation of lower priority processes.
- **Best for**: Systems where certain processes need preferential treatment.

## Project Structure
- `data/processes.txt` - Sample input file containing process information.
- `src/process.h` - Header file with process structure definition.
- `src/fcfs_parsed_scheduler.c` - FCFS scheduling algorithm implementation.
- `src/priority_process_algorithm.c` - Priority scheduling algorithm implementation.

## Process File Format
The input file should be structured as follows:
```
PID Arrival_Time Burst_Time Priority
1   0           5          2
2   2           3          1
...
```

## Compilation Instructions

### Compiling the FCFS Scheduler
```bash
gcc src/fcfs_parsed_scheduler.c -o fcfs_scheduler
```

### Compiling the Priority Scheduler
```bash
gcc src/priority_process_algorithm.c -o priority_scheduler
```

## Usage Instructions

### FCFS Scheduler
```bash
./fcfs_scheduler data/processes.txt
```

### Priority Scheduler
```bash
./priority_scheduler data/processes.txt
```

## Example Output

### FCFS Scheduler
```
Parsed Processes:
Process [0]: PID=1, Arrival=0, Burst=5, Priority=2
...

========== FCFS Scheduling ==========

Execution Order:
PID     Arrival  Burst   Start   Completion      Waiting Turnaround
Process 1 started execution at time 0
1       0       5       0       5               0       5
...

Average Waiting Time: 12.50
Average Turnaround Time: 16.90

Gantt Chart:
0-----|P1|5----|P2|8--|P3|10------|P4|16----|P5|20...
```

### Priority Scheduler
```
Process (PID: 2) started at time 2
Process (PID: 2) finished at time 5
...

| P3 | P5 | ... |
0    7    9 ... 33

Average Waiting Time: 10.00
Average Turnaround Time: 14.00
```
