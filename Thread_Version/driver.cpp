//=================== DRIVER PROGRAM ===================
// This program is for LINUX Operating System.
// Run program using command : g++ driver.cpp -lpthread -o driver.out
/* This is the main program which creates four threads :
      1. Tortoise
      2. Hare
      3. Reporter
      4. God    
>> There are three mutexes used in this program.
>> Each thread runs until either tortoise reach the target or hare reach the target. 
>> All the operations of each thread is done in a while loop
>> Operations of threads 'tortoise', 'hare' and 'reporter' are parallel as 
   there is no mutex between them.
>> When God comes into play, God halts the other three threads using mutexes :
   1. God halts hare and tortoise threads so that "tortoise and hare do not reach to the end
      while God is making decision"
   2. God halts reporter processes so that "reporter do not report in between god is making 
      decision";"
>> Parent process waits for the all the four threads to complete and then it declares who is the
   winner.
*/
#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

using namespace std;

// counters for tortoise and hare
int hare = 0;
int tortoise = 0;

// constants used in the program
const int TARGET = 300000;
const int STEPS = 300;
const int MIN_DIST_FOR_SLEEP = 100000;

// mutex to write value of tortoise
pthread_mutex_t tortoise_mutex = PTHREAD_MUTEX_INITIALIZER;
// mutex to write value of hare
pthread_mutex_t hare_mutex     = PTHREAD_MUTEX_INITIALIZER;
// mutex to write in the terminal
pthread_mutex_t terminal_mutex = PTHREAD_MUTEX_INITIALIZER;

// thread for tortoise
void *tortoise_turn(void *args) {

    while(tortoise < TARGET && hare < TARGET) {
        pthread_mutex_lock(&tortoise_mutex);
        tortoise++;
        pthread_mutex_unlock(&tortoise_mutex);
    }
    return (void *) 0;
}

// thread for hare
void *hare_turn(void *args) {

    while(tortoise < TARGET && hare < TARGET) {
        if(hare-tortoise >= MIN_DIST_FOR_SLEEP) {
            /* initialize random seed */
            srand (time(NULL));
            int random_sleep = rand()%500 + 500;
            // make hare sleep for a random time
            usleep(random_sleep);
        }
        pthread_mutex_lock(&hare_mutex);
        hare += STEPS;
        pthread_mutex_unlock(&hare_mutex);
    }
    return (void *) 0;
}

// thread for reporter
void *reporter_turn(void *args) {

    while(tortoise < TARGET && hare < TARGET) {
        pthread_mutex_lock(&terminal_mutex);
        cout<<"\n========= STATUS ===========\n";
        cout<<"\nTortoise = "<< tortoise;
        cout<<"\nHare     = "<< hare<<"\n";
        pthread_mutex_unlock(&terminal_mutex);
        usleep(500);
    }
    return (void *) 0;
}

// thread for god
void *god_turn(void *args) {

    while(tortoise < TARGET && hare < TARGET) {
        // halt reporter, tortoise and hare using mutex
        pthread_mutex_lock(&terminal_mutex);
        pthread_mutex_lock(&tortoise_mutex);
        pthread_mutex_lock(&hare_mutex);

        char choice;
        cout<<"God, do you want to change positions ? (y/n)\n>> ";
        cin>>choice;
        if(choice != 'y') {

            pthread_mutex_unlock(&terminal_mutex);
            pthread_mutex_unlock(&tortoise_mutex);
            pthread_mutex_unlock(&hare_mutex);
            // when god doesn't want to change the positions of hare and tortoise,
            // then we will allow god to sleep for a while
            usleep(500);
            continue;

        }

        cout<<"Give new position for tortoise : ";
        cin>>tortoise;
        cout<<"Give new position for hare : ";
        cin>>hare;

        //
        pthread_mutex_unlock(&terminal_mutex);
        pthread_mutex_unlock(&tortoise_mutex);
        pthread_mutex_unlock(&hare_mutex);
    
    }
    return (void *) 0;
}

// main function
int main() {

    // thread ids for tortoise, hare, reporter, god threads
    pthread_t tortoise_tid, hare_tid, reporter_tid, god_tid;

    // creating four threads
    pthread_create(&tortoise_tid, NULL, tortoise_turn, NULL);
    pthread_create(&hare_tid, NULL, hare_turn, NULL);
    pthread_create(&reporter_tid, NULL, reporter_turn, NULL);
    pthread_create(&god_tid, NULL, god_turn, NULL);

    // wait for each thread to finish their execution
    pthread_join(tortoise_tid, NULL);
    pthread_join(hare_tid, NULL);
    pthread_join(reporter_tid, NULL);
    pthread_join(god_tid, NULL);

    // print who has won the race
    if(tortoise >= TARGET) {
        cout << "WINNER is tortoise\n";
    } else {
        cout << "WINNER is hare\n";
    }

    return 0;
}