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

    int read_driver = open(driver_reporter_wr, O_RDONLY);
    int write_driver = open(driver_reporter_rw, O_WRONLY);

    int share_msg = 0;
    int tortoise, hare;
    while(true) {
       read(read_driver, &tortoise, sizeof(int));
       read(read_driver, &hare, sizeof(int)); 
       cout<<"\n========= STATUS ===========\n";
       cout<<"Tortoise = "<<tortoise<<endl;
       cout<<"Hare = "<<hare<<endl;

       write(write_driver, &share_msg, sizeof(int));
       if(tortoise >= 30 || hare >= 30) {
           break;
       }
    }
    close(read_driver);
    return 0;
}