/*****************************************************************************
 * Testing whistledetection for dummies
 *****************************************************************************
 *
 * Scan wavfiles:
 *   Wavfiles can be scanned for whistles by running
 *      whistledebug analyze FLAGS <NAME>.wav
 *   Run
 *      arecord -fS16_LE -r12000 <NAME>.wav 
 *   to record testfiles on the robot.
 *
 *
 * Flags:
 *   --buffer-size=<VALUE>
 *     Set the buffersize when streaming wavs.
 *   
 *   --min-freq=<VALUE>
 *     The peaks have to be above this frequency to be accepted as
 *     a whistle candidate. Unit: Hz.
 *
 *   --min-len=<VALUE>
 *     Discard any whistle candidate shorter than <VALUE>. Unit: ms.
 *
 *   --min-vol=<VALUE>
 *     Discard any whistle candidate shorter than <VALUE>. Unit: dB.
 *
 *   --verbose | -v
 *     Display discarded whistle candidates.
 *
 *   If a flag is omitted the corresponding value from src/config.h is used.
 *
 * Wavfile metadata:
 *  whistledebug metadata FILE
 */

#include "alsarecorder.h"
#include "whistlerecog.h"
#include "wavfile.h"
#include "whistlesettings.h"
#include "whistlematch.h"
#include "config.h"

#include <fstream>
#include <iostream>
#include <iterator>
#include <signal.h>
#include <sstream>
#include <string.h>
#include <vector>

using namespace std;


bool streq(const char *, const char *);
int listen();
int scanWavFile();
int printMetadata();
void consumeFlags(int argc, char *argv[]);
void printHelp();


bool g_verbose       = false;
int g_buffer_ms      = DEFAULT_BUFFER_SIZE_MS;
int g_minFreq_Hz     = DEFAULT_THRESHOLD_HZ;
int g_minLen_ms      = DEFAULT_MIN_LENGTH;
int g_minVolume_dB   = DEFAULT_VOLUME_THRESHOLD_DB;
const char *g_action = NULL;
const char *g_file   = NULL;

bool streq(const char *str1, const char *str2) {
    return strcmp(str1, str2) == 0;
}

int scanWavFile() {
    bool whistle  = false;
    int whistleStart = 0;
    const char *wavName = g_file;
    WavFile mockRecorder(string(wavName), g_buffer_ms);
    WhistleSettings bb(g_minFreq_Hz, g_minVolume_dB,
            g_minLen_ms, g_buffer_ms, mockRecorder.getChannelNum(), 
            mockRecorder.getSampleRate());
    WhistleRecog w(&bb, &mockRecorder);
    do {
        bool whistleOk = w.process();

        if (w.hasWhistleCandidate() && g_verbose) {
            float peak = w.getCurrPeak();
            cerr << "Seeing whistle at " << mockRecorder.position_ms() << endl;
            cerr << "Peak is " << peak << endl;
        }
        if (w.hasWhistleCandidate() && !whistle) {
            whistleStart = mockRecorder.position_ms();
            whistle = true;
        } 
        if (!w.hasWhistleCandidate() && whistle) {
            int length = w.getLastResult()->getLength_ms();
            float maxVol = w.getLastResult()->getMaxVolume_dB();
            whistle = false;
            if (whistleOk) {
                cerr << "Whistle detected. Start = " << whistleStart 
                    << ". Length = " << length << endl
                    << ". Volume = " << maxVol << endl;
            } else if (g_verbose) {
                cerr << "Candidate discarded. Start = " << whistleStart 
                    << ". Length = " << length << endl
                    << ". Volume = " << maxVol << endl;
            }
        } 

    } while (!mockRecorder.endOfData());

    return EXIT_SUCCESS;
}

int printMetadata() {
    const char *wavName = g_file;
    WavFile wav(string(wavName), 50);
    cerr << "Samplerate = " << wav.getSampleRate() << endl;
    cerr << "ChannelNum = " << wav.getChannelNum() << endl;
    cerr << "Length (s) = " << wav.getLength_s() << endl;

    return EXIT_SUCCESS;
}

void consumeFlags(int argc, char *argv[]) {
    if (argc < 2) {
        return;
    }
    g_action = argv[1];
    for (int i=2; i < argc; i++) {
        const char *arg = argv[i];
        if (streq(arg, "--verbose") || streq(arg, "-v")) {
            g_verbose = true;
        }
        if (sscanf(arg, "--buffer-size=%d", &g_buffer_ms) == 1) {
            continue;
        }
        if (sscanf(arg, "--min-freq=%d", &g_minFreq_Hz) == 1) {
            continue;
        }
        if (sscanf(arg, "--min-len=%d", &g_minLen_ms) == 1) {
            continue;
        }
        if (sscanf(arg, "--min-vol=%d", &g_minVolume_dB) == 1) {
            continue;
        }
        g_file = arg;
        break;
    }
}

int main(int argc, char * argv[]) {
    consumeFlags(argc, argv);
    if (g_action == NULL) {
        cerr << "Error: No action specified." << endl;
        return EXIT_FAILURE;
    }
    if (streq(g_action, "analyze")) {
        if (g_file == NULL) {
            cerr << "Error: You havent specified a wavname." << endl;
            return EXIT_FAILURE;
        }
        cerr << "Start analyzing." << endl;
        return scanWavFile();
    }
    if (streq(g_action, "metadata")) {
        return printMetadata();
    }
    cerr << "Error: Unknown action " << g_action << endl;
    return EXIT_FAILURE;
}

// vim: set ts=4 sw=4 sts=4 expandtab:
