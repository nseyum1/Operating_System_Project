
#ifndef process_details
#define process_details

typedef struct Process { /* A structure defined with typedef data type and named Process */
   int PID;
   int Arrival_Time;
   int Burst_Time;
   int Priority;
 } Process;

 /* Comparison functions to use for sorting with qsort */

 int sortPriority(const void *a, const void *b);

#endif
