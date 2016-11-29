#include "wavfile.h"

#include "const.h"

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> // for int16_t and int32_t


using namespace std;


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

int is_big_endian(void) {
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01000000};
    return bint.c[0]==1;
}

WavFile::WavFile(const string &fileName, int buffer_ms) 
    : _channelNum(0)
    , _sampleRate(0)
    , _windowSize(0)
    , _windowStart(0)
{
    readFile(fileName);
    _windowSize = ((float) _sampleRate) / ((float) MS_PER_SEC) * buffer_ms;
    _windowStart = -_windowSize;
}

int16_t *WavFile::getBuffer() {
    int16_t *winPos = &(_data.data()[_windowStart]);
    return winPos;
}

int WavFile::getBufferSize() {
    return _windowSize;
}

short WavFile::getChannelNum() {
    return _channelNum;
}

double WavFile::getSampleRate() {
    return _sampleRate;
}

bool WavFile::readFromDevice() {
    if (endOfData()) {
        return false;
    }
    _windowStart += _windowSize;
    int bufferTailSize = remainingBuffer();

    if (bufferTailSize < _windowSize) {
        _windowSize = bufferTailSize;
    }

    return true;
}

int WavFile::remainingBuffer() {
    return _data.size() - _windowStart;
}

bool WavFile::endOfData() {
    unsigned int windowEnd = _windowStart + _windowSize;
    return windowEnd == _data.size();
}

float WavFile::getLength_s() {
    return _data.size() / _sampleRate;
}

int WavFile::position_ms() {
    return _windowStart / _sampleRate * 1000;
}

bool WavFile::readFile(const string &fileName) {

    const char *filename=fileName.c_str();
    FILE *wav = fopen(filename,"rb");
    struct wavfile header;

    if ( wav == NULL ) {
        fprintf(stderr,"Can't open input file %s\n", filename);
        return false;
    }

    // read header
    if ( fread(&header,sizeof(header),1,wav) < 1 ) {
        fprintf(stderr,"Can't read input file header %s\n", filename);
        return false;
    }

    // if wav file isn't the same endianness than the current environment
    // we quit
    if ( is_big_endian() ) {
        if (   memcmp( header.id,"RIFX", 4) != 0 ) {
            fprintf(stderr,"ERROR: %s is not a big endian wav file\n", filename); 
            return false;
        }
    } else {
        if (   memcmp( header.id,"RIFF", 4) != 0 ) {
            fprintf(stderr,"ERROR: %s is not a little endian wav file\n", filename); 
            return false;
        }
    }

    if (   memcmp( header.wavefmt, "WAVEfmt ", 8) != 0 
        || memcmp( header.data, "data", 4) != 0 
            ) {
        fprintf(stderr,"ERROR: Not wav format\n"); 
        return false; 
    }
    if (header.format != 16) {
        fprintf(stderr,"\nERROR: not 16 bit wav format.");
        return false;
    }
    fprintf(stderr,"format: %d bits", header.format);
    if (header.format == 16) {
        fprintf(stderr,", PCM");
    } else {
        fprintf(stderr,", not PCM (%d)", header.format);
    }
    if (header.pcm == 1) {
        fprintf(stderr, " uncompressed" );
    } else {
        fprintf(stderr, " compressed" );
    }
    fprintf(stderr,", channel %d", header.pcm);
    fprintf(stderr,", freq %d", header.frequency );
    fprintf(stderr,", %d bytes per sec", header.bytes_per_second );
    fprintf(stderr,", %d bytes by capture", header.bytes_by_capture );
    fprintf(stderr,", %d bits per sample", header.bytes_by_capture );
    fprintf(stderr,"\n" );

    if ( memcmp( header.data, "data", 4) != 0 ) { 
        fprintf(stderr,"ERROR: Prrroblem?\n"); 
        return false; 
    }
    fprintf(stderr,"wav format\n");

    _channelNum = header.channels;
    _sampleRate = header.frequency;


    // read data
    int16_t value;
    fprintf(stderr,"---\n");
    while( fread(&value,sizeof(value),1,wav) ) {
        _data.push_back(value);
    }
    return true;
}
