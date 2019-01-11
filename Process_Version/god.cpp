#include <iostream>
#include <unistd.h>
#include <sys/stat.h>

using namespace std;

const char *hare_fifo       = "/tmp/hare_fifo";
const char *tortoise_fifo   = "/tmp/tortoise_fifo";
char *tortoise_args[] = { "./tortoise.out", NULL };
char *hare_args[]     = { "./hare.out", NULL };
char *reporter_args[] = { "./reporter.out", NULL };

int main() {

    // creating two fifo : one for hare, other for tortoise
    mkfifo(tortoise_fifo, 0666);
    mkfifo(hare_fifo, 0666);

    int tortoise_write = open(tortoise_fifo, O_WRONLY);
    int hare_write     = open(hare_fifo, O_WRONLY);
    int x = 0;

    // Calling tortoise
    if(fork() == 0) {
        write(tortoise_write, &x, sizeof(int));
        execv (tortoise_args[0], tortoise_args);
    }
    // Calling Hare 
    if(fork() == 0) {
        write(hare_write, &x, sizeof(int));
        execv (hare_args[0], hare_args);
    }
    // Calling Reporter
    if(fork() == 0) {
        execv (reporter_args[0], reporter_args);
    }
    
    close(tortoise_write);
    close(hare_write);
    return 0;
}