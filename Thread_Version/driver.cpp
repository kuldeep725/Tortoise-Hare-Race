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
   2. God halts reporter processes so that "reporter do not report in between when god is making 
      decision";"
>> Parent process waits for the all the four threads to complete and then it declares who is the
   winner.
*/
#include <bits/stdc++.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

using namespace std;

// distance counters for tortoise and hare
long hare = 0;
long tortoise = 0;
// time counter for tortoise and hare
long hare_time = 0;
long tortoise_time = 0;

// constants used in the program
// these constants can be tweaked to see various cases
const long TARGET = 5e8;
const long STEPS = 5;
const long MIN_DIST_FOR_SLEEP = 1e8;

// mutex to write value of tortoise
pthread_mutex_t tortoise_mutex = PTHREAD_MUTEX_INITIALIZER;
// mutex to write value of hare
pthread_mutex_t hare_mutex     = PTHREAD_MUTEX_INITIALIZER;
// mutex to write in the terminal
pthread_mutex_t terminal_mutex = PTHREAD_MUTEX_INITIALIZER;

//Utility function
long getRandomPosition() {
    return rand() % (TARGET+1);
}

// thread for tortoise
void *tortoise_turn(void *args) {

    while(tortoise < TARGET) {
        pthread_mutex_lock (&tortoise_mutex);
        tortoise++;
        tortoise_time++;
        pthread_mutex_unlock (&tortoise_mutex);
    }
    return (void *) 0;

}

// thread for hare
void *hare_turn(void *args) {

    while(hare < TARGET) {
        if(hare-tortoise >= MIN_DIST_FOR_SLEEP) {
            // hare sleeps for a random amount of time
            long sleep_time = rand()%(100000);
            hare_time += sleep_time;

            long random_sleep = rand()%1000; // between 0 and 999
            // make hare sleep for a random time
            usleep(random_sleep);
        }
        pthread_mutex_lock (&hare_mutex);
        hare += STEPS;
        hare_time++;
        pthread_mutex_unlock (&hare_mutex);
    }
    return (void *) 0;

}

// thread for reporter
void *reporter_turn(void *args) {

    while(tortoise < TARGET || hare < TARGET) {
        pthread_mutex_lock (&terminal_mutex);

        cout << "\n================= RACE STATUS ==================\n";
        cout << "\n Tortoise is at position : " << tortoise   << "\t, at time : "
             << tortoise_time << " iterations";
        cout << "\n Hare is at position     : " << hare       << "\t, at time : " 
             << hare_time     << " iterations\n";

        pthread_mutex_unlock (&terminal_mutex);
        usleep(500);
    }
    return (void *) 0;

}

// thread for god
void *god_turn(void *args) {

    while(tortoise < TARGET || hare < TARGET) {
        // halt reporter, tortoise and hare using mutex
        pthread_mutex_lock (&terminal_mutex);
        pthread_mutex_lock (&tortoise_mutex);
        pthread_mutex_lock (&hare_mutex);

        if(( (double) rand() / (RAND_MAX) ) >= 0.75) {
            cout<<"\n God has changed positions...\n";
            cout<<"\n========= NEW POSITIONS ===========\n\n";
            if(tortoise < TARGET) {
                tortoise = getRandomPosition();
                cout<<" Tortoise = "<< tortoise << "\n";
            }
            if(hare < TARGET) {
                hare     = getRandomPosition();
                cout<<" Hare     = "<< hare << "\n";
            }

        } else {

            // unlock all mutexes locked by god
            pthread_mutex_unlock (&terminal_mutex);
            pthread_mutex_unlock (&tortoise_mutex);
            pthread_mutex_unlock (&hare_mutex);

            // when god doesn't want to change the positions of hare and tortoise,
            // then we will allow god to sleep for a while
            usleep(500);
            continue;

        }

        pthread_mutex_unlock (&terminal_mutex);
        pthread_mutex_unlock (&tortoise_mutex);
        pthread_mutex_unlock (&hare_mutex);
    
    }
    return (void *) 0;

}

// main function
int main() {

    /* initialize random seed */
    srand (time(NULL));

    // thread ids for tortoise, hare, reporter, god threads
    pthread_t tortoise_tid, hare_tid, reporter_tid, god_tid;

    // creating four threads
    pthread_create (&tortoise_tid, NULL, tortoise_turn, NULL);
    pthread_create (&hare_tid, NULL, hare_turn, NULL);
    pthread_create (&reporter_tid, NULL, reporter_turn, NULL);
    pthread_create (&god_tid, NULL, god_turn, NULL);

    // wait for each thread to finish their execution
    pthread_join (tortoise_tid, NULL);
    pthread_join (hare_tid, NULL);
    pthread_join (reporter_tid, NULL);
    pthread_join (god_tid, NULL);

    cout<< "\n======================= RACE IS OVER ===========================\n";
    cout<< "\n The results are as follows : \n\n";
    cout<< "\tTime taken by tortoise : " << tortoise_time << " iterations\n";
    cout<< "\tTime taken by hare     : " << hare_time << " iterations\n";

    if (tortoise_time < hare_time) {
        cout << "\n\tWINNER of the race is 'tortoise'\n";
    } else if (tortoise_time > hare_time) {
        cout << "\n\tWINNER of the race is 'hare'.\n";
    } else {
        cout << "\n\tThe race is drawn\n";
    }
    cout<< "\n================================================================\n";
    return 0;

}