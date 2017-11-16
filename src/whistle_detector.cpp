/**
 * @author Felix Weiglhofer
 */
#include "whistle_detector.h"

WhistleDetector::WhistleDetector(AudioStream &audioProvider, 
        size_t minWhistleLength, 
        float whistleThreshold) 
    : _audioProvider(audioProvider)
    , _mergeChannels(
            _audioProvider.getBuffer(), 
            _audioProvider.getBufferSize(), 
            _audioProvider.getChannelNum())
    , _fourierTransform(
            _mergeChannels.output(),
            _mergeChannels.outSize(), 
            _audioProvider.getSampleRate())
    , _rectSmooth(
            3, 
            _fourierTransform.output(), 
            _fourierTransform.outSize(),
            _fourierTransform.freqSpacing())
    , _whistleClassifier(
            _rectSmooth.output(), 
            _rectSmooth.outSize(),
            whistleThreshold,
            minWhistleLength,
            _mergeChannels.outSize(),
            _rectSmooth.freqSpacing(),
            _rectSmooth.freqOffset())
    , _running(false)
{
}

bool WhistleDetector::process() {
    assert(_running);
    _audioProvider.fetch();
    _mergeChannels.execute();
    _fourierTransform.execute();
    _rectSmooth.execute();
    _whistleClassifier.execute();

    return _whistleClassifier.whistleDetected();
}

void WhistleDetector::start() {
    assert(!_running);
    _audioProvider.start();
    _running = true;
}

void WhistleDetector::stop() {
    assert(_running);
    _audioProvider.stop();
    _whistleClassifier.reset();
    _running = false;
}

// vim: set ts=4 sw=4 sts=4 expandtab:
