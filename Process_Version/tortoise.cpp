#include<iostream>

using namespace std;

const char *hare_fifo       = "/tmp/hare_fifo";
const char *tortoise_fifo   = "/tmp/tortoise_fifo";

int main() {

    cout<<"Tortoise House"<<endl;
    int tortoise_write = open(tortoise_fifo, O_WRONLY);
    int tortoise_read  = open(tortoise_fifo, O_RDONLY);
    return 0;
}