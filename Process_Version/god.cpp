#include <bits/stdc++.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

using namespace std;

const char *driver_god = "/tmp/driver_god";

int main() {

    int tortoise, hare;
    int write_god = open(driver_god, O_WRONLY);
    cout<<"Give new position for tortoise : ";
    cin>>tortoise;
    cout<<"Give new position for hare : ";
    cin>>hare;
    
    write(write_god, &tortoise, sizeof(int));
    write(write_god, &hare, sizeof(int));
    
    close(write_god);
    return 0;
    
}