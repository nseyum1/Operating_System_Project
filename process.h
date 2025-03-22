#ifndef PROCESS_H
#define PROCESS_H

typedef struct Process {
  int pid;
  int arrival_time;
  int burst_time;
  int priority;
} Process;

#endif
