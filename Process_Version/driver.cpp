//=================== DRIVER PROGRAM ===================
// This program is for LINUX Operating System.
// Run program using these two commands : (1) make (2) ./driver.out
/* This is the main program which creates four processes :
      1. Tortoise
      2. Hare
      3. Reporter
      4. God    

For inter-communication between different processes, named pipes (also known as 'fifo') is used. 
Eight pipes are being used for that purpose.
>> The driver process acts as an environment which helps all the four processes to work together.
>> Driver process initally writes the inital value of tortoise and hare in driver_tortoise_wr pipe
   and driver_hare_wr pipe respectively.
>> We will perform the following operations in an infinite while loop until one of tortoise and hare reach
   the target location :
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
>> Once reached out of the loop, display the winner : hare or tortoise.
>> Stop "hare, tortoise and reporter processes" which are running with signal SIGTERM.
>> Close all the pipe descriptors   
   
*/ 
#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>

using namespace std;

// (1) In this pipe, driver acts as reader, god acts as writer
const char *driver_god          = "/tmp/driver_god";

// (2) In this pipe, driver acts as reader, tortoise acts as writer
const char *driver_tortoise_rw  = "/tmp/driver_tortoise_rw";

// (3) In this pipe, driver acts as writer, tortoise acts as reader
const char *driver_tortoise_wr  = "/tmp/driver_tortoise_wr";

// (4) In this pipe, driver acts as reader, hare acts as writer
const char *driver_hare_rw      = "/tmp/driver_hare_rw";

// (5) In this pipe, driver acts as writer, hare acts as reader
const char *driver_hare_wr      = "/tmp/driver_hare_wr";

// (6) In this pipe, hare acts as reader, tortoise acts as writer
const char *hare_tortoise       = "/tmp/hare_tortoise";

// (7) In this pipe, driver acts as writer, tortoise acts as reader
const char *driver_reporter_wr  = "/tmp/driver_reporter_wr";

// (8) In this pipe, driver acts as reader, tortoise acts as writer
const char *driver_reporter_rw  = "/tmp/driver_reporter_rw";

char *tortoise_args[] = { "./tortoise.out", NULL };
char *hare_args[]     = { "./hare.out", NULL };
char *reporter_args[] = { "./reporter.out", NULL };
char *god_args[]      = { "./god.out", NULL };

int main() {

    mkfifo(driver_god, 0666);
    mkfifo(driver_tortoise_rw, 0666);
    mkfifo(driver_tortoise_wr, 0666);
    mkfifo(driver_hare_rw, 0666);
    mkfifo(driver_hare_wr, 0666);
    mkfifo(hare_tortoise, 0666);
    mkfifo(driver_reporter_rw, 0666);
    mkfifo(driver_reporter_wr, 0666);

    int tortoise = 0;
    int hare = 0;
    const int TARGET = 30;
    int pid_tortoise, pid_hare, pid_reporter, pid_god;

    // Calling tortoise
    if((pid_tortoise = fork()) == 0) {
        execv (tortoise_args[0], tortoise_args);
    }
    int write_tortoise = open(driver_tortoise_wr, O_WRONLY);
    int read_tortoise  = open(driver_tortoise_rw, O_RDONLY);
    write(write_tortoise, &tortoise, sizeof(int));

    // Calling Hare 
    if((pid_hare = fork()) == 0) {
        execv (hare_args[0], hare_args);
    }
    int write_hare = open(driver_hare_wr, O_WRONLY);
    int read_hare  = open(driver_hare_rw, O_RDONLY);
    write(write_hare, &hare, sizeof(int));

    // Calling Reporter
    if((pid_reporter = fork()) == 0) {
        execv (reporter_args[0], reporter_args);
    }
    int write_reporter = open(driver_reporter_wr, O_WRONLY);
    int read_reporter  = open(driver_reporter_rw, O_RDONLY);

    // winner = 0 for Tortoise
    // winner = 1 for Hare
    int winner = 0;
    int share_msg = 0;
    while(1) {

        // wait for tortoise and hare to make their move
        read(read_tortoise, &tortoise, sizeof(int));
        read(read_hare, &hare, sizeof(int));

        char choice;
        cout<<"God, do you want to change positions ? (y/n)\n >> ";
        cin>>choice;
        if(choice == 'y') {
            if((pid_god = fork()) == 0) {
                execv(god_args[0], god_args);
            }
            int read_god = open(driver_god, O_RDONLY);
            read(read_god, &tortoise, sizeof(int));
            read(read_god, &hare, sizeof(int));
            close(read_god);
            wait(NULL);
        }
        
        if(hare >= TARGET) {
            winner = 1;
            break;
        } else if(tortoise >= TARGET) {
            break;
        }
        // write to the reporter values of tortoise and hare
        write(write_reporter, &tortoise, sizeof(int));
        write(write_reporter, &hare, sizeof(int));

        // writer to tortoise and hare their current values 
        write(write_tortoise, &tortoise, sizeof(int));
        write(write_hare, &hare, sizeof(int));

        // wait for reporter to display the status
        read(read_reporter, &share_msg, sizeof(int));
    }

    printf("\nWinner is : %s", (winner == 0) ? "Tortoise\n" : "Hare\n");

    // kill the tree processes
    kill(pid_tortoise, SIGTERM);
    kill(pid_hare, SIGTERM);
    kill(pid_reporter, SIGTERM);

    // close all the pipe descriptors
    close(write_reporter);
    close(read_hare);
    close(read_tortoise);
    close(write_hare);
    close(write_tortoise);
    return 0;
}
