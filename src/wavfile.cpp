/**
 * @author Felix Weiglhofer
 */
#include "wavfile.h"

#include "constants.h"

#include <cassert>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> // for int16_t and int32_t


using namespace std;

WavFile::WavFile(const string &fileName, int buffer_ms) 
    : _channelNum(0)
    , _sampleRate(0)
    , _buffer(nullptr)
    , _bufferSize(0)
    , _windowStart(0)
{
    readFile(fileName);
    _bufferSize = ((float) _sampleRate) / ((float) MS_PER_SEC) * buffer_ms;
    _buffer = new int16_t[_bufferSize];
}

WavFile::~WavFile() {
    delete[] _buffer;
}

bool WavFile::fetch() {
    assert(!endOfData());

    std::vector<int16_t>::iterator start = _data.begin() + _windowStart;
    std::vector<int16_t>::iterator end = start + _bufferSize;

    std::copy(start, end, _buffer);

    _windowStart += _bufferSize;

    return true;
}

int WavFile::remainingBuffer() {
    return _data.size() - _windowStart;
}

bool WavFile::endOfData() {
    return _windowStart + _bufferSize >= _data.size();
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

    if ( wav == nullptr ) {
        fprintf(stderr,"Can't open input file %s\n", filename);
        return false;
    }

    // read header
    if ( fread(&header,sizeof(header),1,wav) < 1 ) {
        fprintf(stderr,"Can't read input file header %s\n", filename);
        fclose(wav);
        return false;
    }

    // if wav file isn't the same endianness than the current environment
    // we quit
    if ( isBigEndian() ) {
        if (   memcmp( header.id,"RIFX", 4) != 0 ) {
            fprintf(stderr,"ERROR: %s is not a big endian wav file\n", filename); 
            fclose(wav);
            return false;
        }
    } else {
        if (   memcmp( header.id,"RIFF", 4) != 0 ) {
            fprintf(stderr,"ERROR: %s is not a little endian wav file\n", filename); 
            fclose(wav);
            return false;
        }
    }

    if (   memcmp( header.wavefmt, "WAVEfmt ", 8) != 0 
        || memcmp( header.data, "data", 4) != 0 
            ) {
        fprintf(stderr,"ERROR: Not wav format\n"); 
        fclose(wav);
        return false; 
    }
    if (header.format != 16) {
        fprintf(stderr,"\nERROR: not 16 bit wav format.");
        fclose(wav);
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
        fclose(wav);
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
    fclose(wav);
    return true;
}

// vim: set ts=4 sw=4 sts=4 expandtab:
