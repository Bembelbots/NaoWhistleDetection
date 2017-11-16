/**
 * @author Felix Weiglhofer
 */
#pragma once

#include <cstddef>

/**
 * Classifies audio data as a whistle or not.
 */
class WhistleClassifier {
    
public:

    /**
     * @param input: Audio data in frequency domain.
     * @param inSize: Size of input.
     * @param whistleThreshold: Classify input as a whistle if the peak in
     * input is higher than whistleThreshold (Unit: Hz)
     * @param minWhistleLength: Only accept a signal as a whistle if its at
     * least minWhistleLength samples long.
     * @param bufferLength: Number of audio samples that is processed with
     * every processing step.
     * @param freqSpacing: Spacing between entries in input.
     * @param freqOffset: Frequency offset of the first entry of input.
     */
    explicit WhistleClassifier(
            const float * const input, 
            size_t inSize, 
            float whistleThreshold,
            unsigned int minWhistleLength, 
            float bufferLength,
            float freqSpacing,
            float freqOffset);

    void execute();

    bool whistleDetected() { return _whistleDetected; }

    bool hasCandidate() { return _matchLength > 0; }

    float currPeak() { return _currPeak; }

    void reset(); 

private:
    const float * const _input;
    const size_t _inSize;

    const float _bufferLength;
    const float _freqSpacing;
    const float _freqOffset;

    size_t _matchLength;
    const size_t _minMatchLength;
    float _currPeak;

    const float _whistleThreshold;

    bool _whistleDetected;

    float findPeak();

};

// vim: set ts=4 sw=4 sts=4 expandtab:
