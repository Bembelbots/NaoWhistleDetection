#include "audiocontainer.h"

#include "audiostream.h"
#include "const.h"

#include <math.h>
#include <iostream>


using namespace std;

int16_t max(int16_t *arr, int size) {
    if (size < 1) {
        cerr << "Attempting to get maximum of an empty array. Bye, bye!" << endl << endl;
    }
    int16_t m = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > m) {
            m = arr[i];
        }
    }
    return m;
}

AudioContainer::AudioContainer(AudioStream *as) 
    : samples()
    , sampleRate(0)
{
    init(as);
}

AudioContainer::AudioContainer(int16_t *rsamples
            , int size, short chNum, float sampleRate, int16_t maxValue) 
    : samples()
    , sampleRate(0)
{
    init(rsamples, size, chNum, sampleRate, maxValue);
}

void AudioContainer::init(int16_t *data, int size, short chNum, float sampleRate, int16_t maxValue) {
    this->sampleRate = sampleRate;
    // move audio into a single channel,
    // to make processing simpler
    initSamples(data, size, chNum);
    frameToDb(data, size, maxValue);
}

void AudioContainer::init(AudioStream *as) {
    init(as->getBuffer(), as->getBufferSize(), as->getChannelNum(), 
        as->getSampleRate(), INT16_MAX);
}

vector<float> *AudioContainer::getSamples() {
    return &samples;
}

float AudioContainer::getSampleRate() {
    return sampleRate;
}

int AudioContainer::getLengthMs() {
    return samples.size() / sampleRate * MS_PER_SEC;
}

float AudioContainer::getVolume_dB() {
    return dB;
}

void AudioContainer::initSamples(int16_t *rsamples, int size, short chNum) {
    int mergedSize = size / chNum;
    samples.resize(mergedSize);
    int dPos = 0;
    for (int i = 0; i < mergedSize; ++i) {
        float v = 0;
        for (int j = 0; j < chNum; ++j) {
            v += (float) rsamples[dPos];
            dPos++;
        }
        samples[i] = v / chNum;
    }
}

void AudioContainer::frameToDb(int16_t *frame, int size, int16_t maxValue) {
    float amplitude = ((float) max(frame, size))/((float) maxValue);
    float logAmp = log10(amplitude);
    dB = 20 * logAmp;
}

// vim: set ts=4 sw=4 sts=4 expandtab:
