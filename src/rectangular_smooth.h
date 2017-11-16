/**
 * @author Felix Weiglhofer
 */
#pragma once

#include <cstddef>

/**
 * Smoothen input data by replacing every entry with the average of n adjacent
 * points.
 */
class RectangularSmooth {
    
public:

    /**
     * @param smoothWidth: Number of adjacent points to build the average of.
     * Must be uneven.
     * @param input: Input data.
     * @param inSize: Size of input.
     * @param freqSpacing: Spacing between points in input.
     */
    explicit RectangularSmooth(unsigned int smoothWidth,  
            const float * const input, size_t inSize, float freqSpacing); 

    ~RectangularSmooth();

    void execute();

    const float *output() { return _output; }
    size_t outSize() { return _outSize; }

    float freqSpacing() { return _freqSpacing; }
    float freqOffset()  { return _freqOffset; }

private:

    const float * const _input;
    const size_t _inSize;

    float *_output;
    size_t _outSize;

    unsigned int _smoothWidth;

    float _freqSpacing;
    float _freqOffset;

};

// vim: set ts=4 sw=4 sts=4 expandtab:
