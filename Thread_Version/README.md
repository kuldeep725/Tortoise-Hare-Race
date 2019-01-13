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
