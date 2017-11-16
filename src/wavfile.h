/**
 * @author Felix Weiglhofer
 */
#pragma once

#include "audiostream.h"

#include <cstdint>
#include <string>
#include <vector>


class WavFile : public AudioStream {

public:
    WavFile(const std::string &wavName, int buffer_ms);
    ~WavFile();

    int16_t *getBuffer() override { return _buffer; }
    size_t getBufferSize() override { return _bufferSize; }
    short getChannelNum() override { return _channelNum; }
    double getSampleRate() override { return _sampleRate; }
    bool fetch() override;
    void start() override {};
    void stop() override {};
    bool endOfData();
    float getLength_s();
    int position_ms();

private:

    struct wavfile {
        char    id[4];          // should always contain "RIFF"
        int32_t totallength;    // total file length minus 8
        char    wavefmt[8];     // should be "WAVEfmt "
        int32_t format;         // 16 for PCM format
        int16_t pcm;            // 1 for PCM format
        int16_t channels;       // channels
        int32_t frequency;      // sampling frequency
        int32_t bytes_per_second;
        int16_t bytes_by_capture;
        int16_t bits_per_sample;
        char    data[4];        // should always contain "data"
        int32_t bytes_in_data;
    } __attribute__((__packed__));

    static int isBigEndian(void) {
        union {
            uint32_t i;
            char c[4];
        } bint = {0x01000000};
        return bint.c[0]==1;
    }

    short _channelNum;
    float _sampleRate;
    std::vector<int16_t> _data;
    int16_t *_buffer;
    size_t _bufferSize;
    size_t _windowStart;

    bool readFile(const std::string &);
    int remainingBuffer();

};
