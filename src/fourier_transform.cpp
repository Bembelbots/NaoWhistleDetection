/**
 * @author Felix Weiglhofer
 */
#include "fourier_transform.h"

#include <cmath>


FourierTransform::FourierTransform(float * const input, 
        size_t inSize, float sampleRate)
    : _input(input)
    , _inSize(inSize)
    , _outSize(inSize / 2 + 1)
    , _fftwOut( (fftwf_complex *) fftwf_malloc(_outSize * sizeof(fftwf_complex)) )
    , _output(new float[_outSize])
    , _plan(fftwf_plan_dft_r2c_1d(_inSize, _input, _fftwOut, FFTW_ESTIMATE))
    , _freqSpacing((sampleRate / 2) / _outSize)
{
}

FourierTransform::~FourierTransform() {
    fftwf_free(_fftwOut);
    delete[] _output;
}

float FourierTransform::length(const fftwf_complex &c) {
    return sqrt( pow(c[0], 2) + pow(c[1], 2) );
}

void FourierTransform::execute() {
    fftwf_execute(_plan);
    for (size_t i = 0; i < _outSize; i++) {
        _output[i] = log(length(_fftwOut[i]));
    }
}

// vim: set ts=4 sw=4 sts=4 expandtab:
