#include "whistle_detector.h"

#include <iostream>

using namespace std;

int main() {

    AlsaRecorder a(AlsaRecorder::V5_SETTINGS);

    //Create a new whistle detector.
    WhistleDetector w(a, AlsaRecorder::V5_SETTINGS.sampleRate * 0.4, 2000);

    while(true) {
        // Scan for whistle.
        if (w.process()) {
            cout << "Whisle detected." << endl;
        }
    }
    
    return 0;
}

// vim: set ts=4 sw=4 sts=4 expandtab:
