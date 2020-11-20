

# The Scheduler
In this OS simulation, we have 3 levels of ready queues for 3 priority (high, norm, low). 
The scheduler for next process to run will scan through each of the ready queues to check if there are any processes ready starting from high queue, normal queue, then low queue. This scheduler algorithm is easy to understand and easy to implement, but this can potentially starve the lower priority processes in the ready queue since the process' priority is fixed at the moment it was first created. 

Because the dev time is limited, this scheduling strategy is chosen.

# Process ID naming
To make the pid of each process unique, there's a static counter that will keep increment by 1 each time a process is created.
The process' ID will be assigned based on the counter.
For the init process, its pid will be 0

# The init process
In this simulation, the init process is the first process to run on startup. 
It should run ONLY when there's no process on the ready queues.
It cannot be killed. The only to terminate this process 
Since we don't want the init process to block itself, when the init process is running, it cannot run commands that can potentially block the process (send, receive, semaphore P).
Also the init process is supposed to be an unique process that can't be copied and killed, users can't run Kill and Copy while init process is running

# Command C
Inputs: the priority of the will be created process. Ranges from 0 (high) to 2 (low)
Actions: This command will create a new process with assigned pid (check # Process ID naming) and will be put to the ready queue
If the init process is running, the new process will preemtively stop the init process and run right away
Reports: Success when a new process is created, Fail when can't add the new process to ready queue

# Command F
Inputs: N/A
Actions: This command will copy from the running process and add the newly created copy process to the ready queue
The copy process will only have different pid from the original
Can't F init process (check #The init process)
Reports: Success when the copy is in the ready queue, Fail when trying to F the init process or can't add to the ready queue

# Command K
Inputs: int, pid of the process to kill
Actions: This command will try to find if the process with given pid exists
If it does, then the command will then try to terminate it out of the system
Can't Kill the init process
If Kill the running non-init process, the simulation will try scheduling the next process similar to command E
Reports: Success when found and kill the process with given pid, Fail when try to Kill the init process or process to be killed doesn't exist

# Command E
Inputs: N/A
Actions: This command will kill the current running process and schedule the next process
If the running process is init process, the command will try to find if it's the only process running
If it is, then the simulation stops.
Reports: Success when exit the running process, Fail when tring to exit the init while there are other processes waiting

# Command Q
Inputs:
Actions:
Reports:

# Command S
Inputs:
Actions:
Reports:

# Command R
Inputs:
Actions:
Reports:

# Command Y
Inputs:
Actions:
Reports:

# Command N
Inputs:
Actions:
Reports:

# Command P
Inputs:
Actions:
Reports:

# Command V
Inputs:
Actions:
Reports:

# Command I
Inputs:
Actions:
Reports:

# Command T
Inputs:
Actions:
Reports: