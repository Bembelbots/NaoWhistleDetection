#pragma once

#include "audiostream.h"

#include <stdint.h>
#include <string>
#include <vector>


class WavFile : public AudioStream {

    public:
        WavFile(const std::string &wavName, int buffer_ms);
        ~WavFile() {};
        int16_t *getBuffer() override;
        int getBufferSize() override;
        short getChannelNum() override;
        double getSampleRate() override;
        bool readFromDevice() override;
        bool endOfData();
        float getLength_s();
        int position_ms();

    private:
        short _channelNum;
        float _sampleRate;
        int _windowSize;
        int _windowStart;
        std::vector<int16_t> _data;

        bool readFile(const std::string &);
        int remainingBuffer();
        
};
