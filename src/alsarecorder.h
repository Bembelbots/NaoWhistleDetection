#pragma once

#include "audiostream.h"

#include <stdint.h>
#include <alsa/asoundlib.h>

class WhistleSettings;

class AlsaRecorder : public AudioStream
{
public:
    AlsaRecorder(WhistleSettings *);
    ~AlsaRecorder();

    void init();
    void free();
    bool readFromDevice() override;

    bool isRunning() const;
    int16_t *getBuffer() override;
    int getBufferSize() override;
    short getChannelNum() override;
    double getSampleRate() override;

protected:
    void setVolume(const char *subdevice);
    void destroyAlsa();

    int xrunRecovery(snd_pcm_t *handle, int err);

    int16_t *audioBuffer;

    WhistleSettings *bb;

    snd_pcm_t *captureHandle;
};


// vim: set ts=4 sw=4 sts=4 expandtab:
