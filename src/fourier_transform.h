/**
 * @author Felix Weiglhofer
 */
#pragma once

#include <fftw3.h>

/**
 * Performs a fourier transform on the input data.
 */
class FourierTransform {
    
public:
    /**
     * @param input: Input signal.
     * @param inSize: Size of input.
     * @param sampleRate: Samplerate of input signal.
     */
    explicit FourierTransform(float * const input, size_t inSize, float sampleRate);
    ~FourierTransform();

    void execute();

    const float *output() { return _output; }
    size_t outSize() { return _outSize; }

    float freqSpacing() { return _freqSpacing; }

private:
    static inline float length(const fftwf_complex &);

    float * const _input;
    const size_t _inSize;

    const size_t _outSize;
    fftwf_complex * const _fftwOut; 
    float * const _output; 

    const fftwf_plan _plan;

    const float _freqSpacing;

};

// vim: set ts=4 sw=4 sts=4 expandtab:
