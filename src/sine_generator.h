/**
 * @author Felix Weiglhofer
 */
#pragma once

#include "audiostream.h"

class SineGenerator : public AudioStream {
    
public:

    SineGenerator(float amplitude, float freq, float sampleRate, size_t bufferSize);
    ~SineGenerator();

    int16_t *getBuffer() override { return _buffer; }
    size_t getBufferSize() override { return _bufferSize; }
    short getChannelNum() override { return 1; }
    double getSampleRate() override { return _sampleRate; }
    bool fetch() override;
    void stop() override { _t = 0; }
    void start() override {}
    
private:

    float _freq;
    float _amplitude;

    int16_t *_buffer;
    size_t  _bufferSize;

    float _sampleRate;

    float _t;

};

// vim: set ts=4 sw=4 sts=4 expandtab:
