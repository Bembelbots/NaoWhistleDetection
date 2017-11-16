/**
 * @author Felix Weiglhofer
 */
#pragma once

#include "merge_channels.hpp"
#include "fourier_transform.h"
#include "rectangular_smooth.h"
#include "whistle_classifier.h"
#include "alsarecorder.h"

#include <fftw3.h>

class WhistleDetector {

public:
    WhistleDetector(AudioStream &, size_t minWhistleLength, 
            float whistleThreshold);
    
    bool process();

    /**
     * Start whistle detection. Must be called before calling process.
     */
    void start();

    void stop();

    bool isRunning() { return _running; }

    bool hasCandidate() { return _whistleClassifier.hasCandidate(); }
    float currPeak() { return _whistleClassifier.currPeak(); }

private:
    AudioStream &_audioProvider;

    // Whistle detection pipeline
    MergeChannels<fftwf_malloc, fftwf_free> _mergeChannels;
    FourierTransform _fourierTransform;
    RectangularSmooth _rectSmooth;
    WhistleClassifier _whistleClassifier;

    bool _running;

};

// vim: set ts=4 sw=4 sts=4 expandtab:
