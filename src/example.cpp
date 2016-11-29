#include "whistlerecog.h"

#include <iostream>
#include <unistd.h>

using namespace std;

int main() {

    //Create a new whistle detector.
    WhistleRecog w;

    while(true) {
        // Scan for whistle.
        if (w.process()) {
            cout << "Whisle detected." << endl;
        }
        usleep(30);
    }
    
    return 0;
}

// vim: set ts=4 sw=4 sts=4 expandtab:
