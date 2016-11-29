#include "whistlesettings.h"

#include <stdint.h>

#include "config.h"


WhistleSettings::WhistleSettings(int threshold_Hz, int threshold_dB, 
        int minLength_ms, int buffer_ms)
    : numChannels(DEFAULT_NUM_CHANNELS)
    , soundDevice(DEFAULT_SOUND_DEVICE)
    , soundDeviceVol(DEFAULT_SOUND_DEVICE_VOL)
    , soundSubDevice(DEFAULT_SOUND_SUBDEVICE)
    , sampleRate(DEFAULT_SAMPLE_RATE_HZ)
    , volume(DEFAULT_VOLUME)
    , amplitude(DEFAULT_AMPLITUDE)
    , threshold_Hz(threshold_Hz)
    , volumeThreshold_dB(threshold_dB)
    , minSignalLength_ms(minLength_ms)
{
    bufferSize = sampleRate / 1000 * buffer_ms;
}

WhistleSettings::WhistleSettings(int threshold_Hz, int threshold_dB, 
        int minLength_ms, int buffer_ms, short numCh, int smplRate)
    : numChannels(numCh)
    , soundDevice(DEFAULT_SOUND_DEVICE)
    , soundDeviceVol(DEFAULT_SOUND_DEVICE_VOL)
    , soundSubDevice(DEFAULT_SOUND_SUBDEVICE)
    , sampleRate(smplRate)
    , volume(DEFAULT_VOLUME)
    , amplitude(DEFAULT_AMPLITUDE)
    , threshold_Hz(threshold_Hz)
    , volumeThreshold_dB(threshold_dB)
    , minSignalLength_ms(minLength_ms)
{
    bufferSize = sampleRate / 1000 * buffer_ms;
}

