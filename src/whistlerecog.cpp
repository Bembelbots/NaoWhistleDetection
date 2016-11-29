#include "whistlerecog.h"

#include "whistlesettings.h"
#include "audiostream.h"
#include "alsarecorder.h"
#include "whistlematch.h"
#include "audiocontainer.h"
#include "frequencyspectrum.h"
#include "config.h"


using namespace std;


WhistleRecog::WhistleRecog(WhistleSettings *b, AudioStream *as) 
    : aStream(as)
    , ownsAStream(false)
    , settings(b)
    , ownsSettings(false)
    , currmatch(NULL)
    , lastResult(NULL)
    , prevPeak(0)
{
    init();
}

WhistleRecog::WhistleRecog()
    : aStream(NULL)
    , ownsAStream(true)
    , settings(NULL)
    , ownsSettings(true)
    , currmatch(NULL)
    , lastResult(NULL)
    , prevPeak(0)
{
}

WhistleRecog::~WhistleRecog() {
    free();
}

void WhistleRecog::init() {
    if (settings == NULL) {
        settings = new WhistleSettings(
                DEFAULT_THRESHOLD_HZ, 
                DEFAULT_VOLUME_THRESHOLD_DB, 
                DEFAULT_MIN_LENGTH, 
                DEFAULT_BUFFER_SIZE_MS);
        ownsSettings = true;
    }
    if (aStream == NULL) {
        AlsaRecorder *rec = new AlsaRecorder(settings);
        rec->init();
        aStream = rec;
        ownsAStream = true;
    }
}

void WhistleRecog::free() {
    if (currmatch != NULL) {
        delete currmatch;
        currmatch = NULL;
    }
    if (lastResult != NULL) {
        delete lastResult;
        lastResult = NULL;
    }
    if (ownsSettings && settings != NULL) {
        delete settings;
        settings = NULL;
    }
    if (ownsAStream && aStream != NULL) {
        delete aStream;
        aStream = NULL;
    }
}

bool WhistleRecog::process() {
    init();
    if (!aStream->readFromDevice()) {
        return false;
    }
    AudioContainer audio(aStream);
    float volume_dB = audio.getVolume_dB();
    FrequencySpectrum f(&audio);
    float peak_Hz = f.getPeak();
    bool result = false;
    if (currmatch != NULL) {
        if (peak_Hz < settings->threshold_Hz) {
            result = currmatch->whistleDetected();
            saveResult();
        } else {
            currmatch->update(audio.getLengthMs(), audio.getVolume_dB());
        } 
    } else if (peak_Hz >= settings->threshold_Hz) {
        currmatch = new WhistleMatch(settings->minSignalLength_ms, settings->volumeThreshold_dB);
        currmatch->update(audio.getLengthMs(), volume_dB);
    }
    prevPeak = peak_Hz;
    return result;
}

bool WhistleRecog::hasWhistleCandidate() {
    return currmatch != NULL;
}

void WhistleRecog::saveResult() {
    if (lastResult != NULL) {
        delete lastResult;
    }
    lastResult = currmatch;
    currmatch  = NULL;
}

float WhistleRecog::getCurrPeak() {
    return prevPeak;
}

WhistleMatch *WhistleRecog::getLastResult() {
    return lastResult;
}

// vim: set ts=4 sw=4 sts=4 expandtab:
