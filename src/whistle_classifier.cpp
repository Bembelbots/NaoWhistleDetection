/**
 * @author Felix Weiglhofer
 */
#include "whistle_classifier.h"

#include <iostream>

#include <iterator>
#include <algorithm>


WhistleClassifier::WhistleClassifier(
        const float * const input, 
        size_t inSize,
        float whistleThreshold,
        unsigned int minWhistleLength,
        float bufferLength,
        float freqSpacing,
        float freqOffset)
    : _input(input)
    , _inSize(inSize)
    , _bufferLength(bufferLength)
    , _freqSpacing(freqSpacing)
    , _freqOffset(freqOffset)
    , _matchLength(0)
    , _minMatchLength(minWhistleLength)
    , _whistleThreshold(whistleThreshold)
{

}

void WhistleClassifier::execute() {
    _currPeak = findPeak();
    if (_currPeak < _whistleThreshold) {
        reset();
    } else {
        _matchLength += _bufferLength;
    }
    _whistleDetected = (_matchLength >= _minMatchLength);
    if (_whistleDetected) {
        reset();
    }
}

float WhistleClassifier::findPeak() {
    size_t maxInd = std::distance(_input, std::max_element(_input, _input + _inSize));
    return (maxInd + 1) * _freqSpacing + _freqOffset;
}

void WhistleClassifier::reset() {
    _matchLength = 0;
}

// vim: set ts=4 sw=4 sts=4 expandtab:
