/**
 * @author Felix Weiglhofer
 */
#include "sine_generator.h"

#include <cmath>

SineGenerator::SineGenerator(float amplitude, float freq, float sampleRate, size_t bufferSize)
    : _freq(freq)
    , _amplitude(amplitude)
    , _buffer(new int16_t[bufferSize])
    , _bufferSize(bufferSize)
    , _sampleRate(sampleRate)
    , _t(0) {
}

SineGenerator::~SineGenerator() {
    delete[] _buffer;
}

bool SineGenerator::fetch() {
    for (size_t i = 0; i < _bufferSize; i++) {
        _buffer[i] = _amplitude * sin(2.0f * M_PI * _freq * _t) ;
        _t += 1.0f / _sampleRate;
        float cycleLen = 1.0f / _freq;
        if (_t > cycleLen) {
            _t -= cycleLen;
        }
    }
    return true;
}

// vim: set ts=4 sw=4 sts=4 expandtab:
