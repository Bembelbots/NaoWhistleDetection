/**
 * @author Felix Weiglhofer
 */
#include "whistle_debugger.h"

#include "wavfile.h"
#include "whistle_detector.h"

#include <iostream>
#include <cstring>


WhistleDebugger::WhistleDebugger() 
    : action("")
    , wavName("")
    , buffer_ms(20)
    , minFreq_Hz(2000)
    , minLen_ms(200)
    , verbose(false)
{
}


int WhistleDebugger::run(int argc, char *argv[]) {
    consumeFlags(argc, argv);

    if (action == "") {
        std::cerr << "Error: no action specified." << std::endl;
        return EXIT_FAILURE;
    } else if (action == "analyze") {
        if (wavName == "") {
            std::cerr << "Error: You havent specified a wavname." << std::endl;
            return EXIT_FAILURE;
        }
        return scanWavFile();
    } else if (action == "metadata") {
        return printMetadata();
    }

    std::cerr << "Error: Unknown action " << action << std::endl;
    return EXIT_FAILURE;
}

void WhistleDebugger::consumeFlags(int argc, char *argv[]) {
    if (argc < 2) {
        return;
    }
    action = argv[1];
    for (int i=2; i < argc; i++) {
        const char *arg = argv[i];
        if (strcmp(arg, "--verbose") == 0|| strcmp(arg, "-v") == 0) {
            verbose = true;
        }
        if (sscanf(arg, "--buffer-size=%d", &buffer_ms) == 1) {
            continue;
        }
        if (sscanf(arg, "--min-freq=%f", &minFreq_Hz) == 1) {
            continue;
        }
        if (sscanf(arg, "--min-len=%d", &minLen_ms) == 1) {
            continue;
        }
        wavName = arg;
        break;
    }
}

int WhistleDebugger::scanWavFile() {
    bool whistle = false;
    int whistleStart = 0;
    WavFile wav(wavName, buffer_ms);
    int minWhistleLength = minLen_ms / 1000.0 * wav.getSampleRate();
    WhistleDetector w(wav, minWhistleLength, minFreq_Hz);

    w.start();

    do {
        bool whistleOk = w.process();

        if (w.hasCandidate()) {
            if (verbose) {
                float peak = w.currPeak();
                std::cerr << "Seeing whistle at " << wav.position_ms() << ". "
                          << "Peak is " << peak << std::endl;
            }
            if (!whistle) {
                whistleStart = wav.position_ms();
                whistle = true;
            }
        } else if (whistle) {
            int length = wav.position_ms() - whistleStart;
            whistle = false;

            if (whistleOk) {
                std::cerr << "Whistle detected:"
                          << "  Start  = " << whistleStart 
                          << "  Length = " << length << std::endl;
            } else if (verbose) {
                std::cerr << "Candidate discarded:"
                          << "  Start  = " << whistleStart
                          << "  Length = " << length << std::endl;
            }
        }

    } while (!wav.endOfData());

    w.stop();

    return EXIT_SUCCESS;
}

int WhistleDebugger::printMetadata() {
    WavFile wav(wavName, 50);
    std::cerr << "Samplerate = " << wav.getSampleRate() << std::endl;
    std::cerr << "ChannelNum = " << wav.getChannelNum() << std::endl;
    std::cerr << "Length (s) = " << wav.getLength_s()   << std::endl;

    return EXIT_SUCCESS;
}


int main(int argc, char *argv[]) {
    WhistleDebugger w;
    return w.run(argc, argv);
}

// vim: set ts=4 sw=4 sts=4 expandtab:
