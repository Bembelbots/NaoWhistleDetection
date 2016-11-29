#pragma once

#include <vector>
#include <stdint.h>

class AudioStream;

/**
 * Store audio and related data
 */
class AudioContainer {
public:
    AudioContainer (int16_t *data, int size, short chNum, float sampleRate, int16_t maxValue);
    AudioContainer (AudioStream *);

    std::vector<float> *getSamples();
    float getSampleRate();
    int getLengthMs();
    float getVolume_dB();

private:
    std::vector<float> samples;
    float dB;
    float sampleRate;

    void frameToDb(int16_t *data, int size, int16_t maxValue);
    void initSamples(int16_t *data, int size, short chNum);

    void init(AudioStream *);
    void init(int16_t *data, int size, short chNum, float sampleRate, int16_t maxValue);
};

// vim: set ts=4 sw=4 sts=4 expandtab:
