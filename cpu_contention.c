/**
 * Simulate CPU bound process by infinite loop
 */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void do_stuff() {
    int n = 1;
    while (n < 2000000000) {
        n = n + 1;
    }
    sleep(1);
}

int main(int argc, char** argv) {

    while (1) {
        do_stuff();
    }

    return 0;
}