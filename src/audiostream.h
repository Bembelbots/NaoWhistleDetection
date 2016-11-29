#pragma once

#include <stdint.h>

class AudioStream {
public:
    virtual ~AudioStream() {};
    virtual int16_t *getBuffer() = 0;
    virtual int getBufferSize() = 0;
    virtual short getChannelNum() = 0;
    virtual double getSampleRate() = 0;
    virtual bool readFromDevice() = 0;
};
