/**
 * @author Felix Weiglhofer
 */
#include "rectangular_smooth.h"

#include <cassert>

RectangularSmooth::RectangularSmooth(unsigned int smoothWidth, 
        const float * const input, 
        size_t inSize, float freqSpacing) 
    : _input(input)
    , _inSize(inSize)
    , _output(nullptr)
    , _outSize(0)
    , _smoothWidth(smoothWidth)
    , _freqSpacing(freqSpacing)
{
    assert(_input != nullptr);
    assert(_smoothWidth % 2 == 1);
    assert(_inSize >= _smoothWidth);

    unsigned int cutOff = _smoothWidth - 1;

    _outSize = _inSize - cutOff;
    _output  = new float[_outSize];

    _freqOffset = (cutOff / 2) * _freqSpacing;
}

RectangularSmooth::~RectangularSmooth() {
    delete[] _output;
}

void RectangularSmooth::execute() {
    for (size_t i = 0; i < _outSize; i++) {
        float acc = 0;
        for (unsigned int j = 0; j < _smoothWidth; j++) {
            acc += _input[i+j];
        }
        _output[i] = acc / _smoothWidth;
    }
}

// vim: set ts=4 sw=4 sts=4 expandtab:
