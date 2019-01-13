# Tortoise-Hare-Race
Handling Tortoise-Hare Race Problem using processes and threads.

# Process Version
This program is for LINUX Operating System.
Run program using these two commands : (1) make (2) ./driver.out
This is the main program which creates four processes :
```
1. Tortoise
2. Hare
3. Reporter
4. God    
```
For inter-communication between different processes, named pipes (also known as 'fifo') is used. 
Eight pipes are being used for that purpose
***

* The driver process acts as an environment which helps all the four processes to work together.  
* Driver process initally writes the inital value of tortoise and hare in driver_tortoise_wr pipe
and driver_hare_wr pipe respectively.  
* We will perform the following operations in an infinite while loop until one of tortoise and hare reach
the target location :
```
1. Wait for tortoise and hare to make their move.
2. Ask God if he wants to change the positions or not. If yes, then
  (i)  Call the god processes and wait for god to decide the new positions
  (ii) Update the positions of hare and tortoise
3. Check if one of tortoise and hare has reached the target or not. If yes, then stop the loop.
4. Allow reporter to display the current status of hare and tortoise
5. Write the new positions (if updated by god, otherwise the old positions itself) of hare and 
tortoise to the pipes driver_tortoise_wr pipe and driver_hare_pipe respectively.
6. Wait for reporter to display the status.
7. Go back to line 1.
```
* Once reached out of the loop, display the winner : hare or tortoise.  
* Stop "hare, tortoise and reporter processes" which are running with signal SIGTERM.
* Close all the pipe descriptors 

# Thread Version
This program is for LINUX Operating System.
Run program using command : g++ driver.cpp -lpthread -o driver.out
This is the main program which creates four threads :
```
1. Tortoise
2. Hare
3. Reporter
4. God    
```
***
* There are three mutexes used in this program.
* Each thread runs until either tortoise reach the target or hare reach the target. 
* All the operations of each thread is done in a while loop
* Operations of threads 'tortoise', 'hare' and 'reporter' are parallel as 
   there is no mutex between them.
* When God comes into play, God halts the other three threads using mutexes :
```
1. God halts hare and tortoise threads so that "tortoise and hare do not reach to the end
   while God is making decision"
2. God halts reporter processes so that "reporter do not report in between god is making 
   decision";"
 ```
* Parent process waits for the all the four threads to complete and then it declares who is the
   winner.
