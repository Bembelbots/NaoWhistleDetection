/**
 * @author Felix Weiglhofer
 */
#pragma once

#include <string>


class WhistleDebugger {

public:

    WhistleDebugger();

    int run(int argc, char *argv[]);

private:

    void consumeFlags(int arc, char *argv[]);
    int scanWavFile();
    int printMetadata();

    std::string action;
    std::string wavName;
    int buffer_ms;
    float minFreq_Hz;
    int minLen_ms;
    bool verbose;
    
};

// vim: set ts=4 sw=4 sts=4 expandtab:
