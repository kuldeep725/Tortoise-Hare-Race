#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

using namespace std;

const char *driver_god          = "/tmp/driver_god";
const char *driver_tortoise_rw  = "/tmp/driver_tortoise_rw";
const char *driver_tortoise_wr  = "/tmp/driver_tortoise_wr";
const char *driver_hare_rw      = "/tmp/driver_hare_rw";
const char *driver_hare_wr      = "/tmp/driver_hare_wr";
const char *hare_tortoise       = "/tmp/hare_tortoise";
const char *driver_reporter_wr  = "/tmp/driver_reporter_wr";
const char *driver_reporter_rw  = "/tmp/driver_reporter_rw";

int main() {

    int read_driver       = open(driver_tortoise_wr, O_RDONLY);
    int write_driver      = open(driver_tortoise_rw, O_WRONLY);
    int write_hare        = open(hare_tortoise, O_WRONLY);

    int tortoise = 0;

    // writer for hare tortoise pipe
    while(true) {
        read(read_driver, &tortoise, sizeof(int));

        tortoise++;
        write(write_hare, &tortoise, sizeof(int));
        write(write_driver, &tortoise, sizeof(int));

    }

    close(read_driver);
    close(write_hare);
    close(write_driver);
    return 0;
}