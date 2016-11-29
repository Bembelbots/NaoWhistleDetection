#include "frequencyspectrum.h"

#include "audiocontainer.h"

#include <fftw3.h>
#include <cmath>
#include <iostream>


using namespace std;

const int FrequencySpectrum::DEFAULT_FILTER_STRENGTH = 3;

/**
 * length of a 2d-vector
 */
float length(fftwf_complex &c) {
    return sqrt( pow(c[0], 2) + pow(c[1], 2) );
}

FrequencySpectrum::FrequencySpectrum(AudioContainer *audio)
    : spectrum(NULL)
    , frequencySpacing(0)
{
    init(audio, DEFAULT_FILTER_STRENGTH);
}

FrequencySpectrum::FrequencySpectrum(AudioContainer *audio, int filterStrength)
    : spectrum(NULL)
    , frequencySpacing(0)
{
    init(audio, filterStrength);
}

void FrequencySpectrum::init(AudioContainer *audio, int filterStrength) {
    vector<float> *samples = audio->getSamples();
    fourierTransformation(samples);

    if (filterStrength <= 0) {
        cerr << "Not a valid filterstrength: " << endl
                << filterStrength 
                << ". Assuming default value " 
                << DEFAULT_FILTER_STRENGTH << endl;
        filterStrength = DEFAULT_FILTER_STRENGTH;
    }

    //Spacing on the x-axis of the magnitude-curve
    frequencySpacing  = audio->getSampleRate() / samples->size()
        * filterStrength;

    //Flatten peaks produced by noise
    smoothenData(filterStrength);

}

FrequencySpectrum::~FrequencySpectrum() {
    if (spectrum != NULL) {
        delete spectrum;
        spectrum = NULL;
    }
}

void FrequencySpectrum::fourierTransformation(vector<float> *audio) {
    int dSize = audio->size();
    fftwf_complex* out = (fftwf_complex*) fftwf_malloc(
            sizeof(fftwf_complex) * dSize);

    fftwf_plan p = fftwf_plan_dft_r2c_1d(
            dSize, audio->data(), out, FFTW_ESTIMATE);
    fftwf_execute(p);

    // The FFT gives us a sequence of complex numbers.
    // We are only interested in the magnitudes of 
    // this sequence to check how much of certain
    // frequencies is in the audio.
    if (spectrum == NULL) {
        spectrum = new vector<float>(dSize / 2);
    } else {
        spectrum->resize(dSize / 2);
    }

    for (size_t i = 0; i < spectrum->size(); i++) {
        spectrum->at(i) = length(out[i]); 
    }

    //cleanup
    fftwf_destroy_plan(p);
    fftwf_free(out);
}

void FrequencySpectrum::smoothenData(int strength) {
    int tail    = spectrum->size() % strength;
    int newSize = spectrum->size() / strength;
    if (tail > 0) {
        newSize++;
    }

    vector<float> *newSpectrum = new vector<float>(newSize);

    // get the average of adjacent values
    int sPos = 0;
    for (int i = 0; i < newSize; i++) {
        float tmp = 0;
        for (int j = 0; j < strength; ++j) {
            tmp += spectrum->at(sPos);
            sPos++;
        }
        newSpectrum->at(i) = tmp / strength;
    }

    float tmp = 0;
    for (int i = 0; i < tail; i++) {
        tmp += spectrum->at(sPos);
        sPos++;
    }
    if (tail > 0) {
        newSpectrum->at(newSize-1) = tmp / strength;
    }
    delete spectrum;
    spectrum = newSpectrum;
}

float FrequencySpectrum::getPeak() {
    // find the largest magnitude
    int maxInd = 0;
    for (size_t i = 1; i < spectrum->size(); i++) {
        if (spectrum->at(i) > spectrum->at(maxInd)) {
            maxInd = i;
        }
    }
    // return the related frequency
    return maxInd * frequencySpacing;
}

// vim: set ts=4 sw=4 sts=4 expandtab:
