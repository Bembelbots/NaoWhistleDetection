#pragma once

#include <vector>

class AudioContainer;

class FrequencySpectrum {

public:
    FrequencySpectrum(AudioContainer *);
    FrequencySpectrum(AudioContainer *audio, int filterStrength);
    ~FrequencySpectrum();

    float getPeak();

private:
    static const int DEFAULT_FILTER_STRENGTH;

    std::vector<float> *spectrum;
    float frequencySpacing;

    void init(AudioContainer *audio, int filterStrength);
    void smoothenData(int strength);
    void fourierTransformation(std::vector<float> *audio);
};

// vim: set ts=4 sw=4 sts=4 expandtab:
