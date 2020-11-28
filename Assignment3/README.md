

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
Inputs: N/A
Actions: Put the running process to ready queue, schedule the next process
If there are no processes in queue and the init process is running, continue to run the init process
Reports: Success when schedule the next process and put the current process back to the ready queues

# Command S
Inputs: int pid of the receiver, char* msg (maximum 40 characters)
Actions: This command will try to find if the receiver exists in the system.
Then if the receiver is waiting by calling R, this command will unblock the receiver and send the message
IF the receiver is not waiting by calling R, this command will just send the message
Then the running process block itself, waiting for a reply from any process
And the scheduler will try finding the next process
Can't let the init command call
Reports: Success when sent message and block running process, Fail when can't send message or block the running process

# Command R
Inputs: N/A
Actions: This command will block the running process, schedule the next process
The blocked process will wait until receive a message from and process sending with S command
Can't let the init command call
Reports: (once msg is received) the message text and source of message

# Command Y
Inputs: int pid of the process to reply to, char* msg (message with 40 char max)
Actions: This command will unblock waiting sender process, put it back to the ready queue and deliver reply
The reply message will be shown at the next time the sender process runs
Reports: Fail when can't send the reply message

# Command N
Inputs: int semID (ranging from 0 to 4)
Actions: initialize a new semaphore
Reports: Fail when semID is out of bound or if the semaphore has already been initialized

# Command P
Inputs: int semID
Actions: perform semaphore P command on running process
Reports: Fail when semaphore is not initialized or semID out of bound

# Command V
Inputs: int semID
Actions: perform semaphore V command on running process
If init process unblocks a process, the process will run at next quantum time
i.e. the unblocked process will not pre-emptively stop init process to run until next Q time
Reports: Fail when semaphore is not initialized or semID out of bound

# Command I
Inputs: int pid
Actions: Print out process pid info
Reports: see Actions

# Command T
Inputs: N/A
Actions: display all process queues and their contents
Reports: see Actions