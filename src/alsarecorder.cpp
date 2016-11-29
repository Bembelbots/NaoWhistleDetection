#include "alsarecorder.h"

#include "whistlesettings.h"

#include <cmath>
#include <iostream>

using namespace std;


AlsaRecorder::AlsaRecorder(WhistleSettings *settings)
    : audioBuffer(NULL)
    , bb(settings)
    , captureHandle(NULL)
{
}

AlsaRecorder::~AlsaRecorder()
{
    destroyAlsa();
}

void AlsaRecorder::free() {
    destroyAlsa();
}

bool AlsaRecorder::readFromDevice()
{
    int err;
    while (true) {
        if ( (err = snd_pcm_readi(captureHandle, audioBuffer, getBufferSize())) == getBufferSize()) {
            return true;
        }
        cerr << "read from audio interface failed " << snd_strerror(err) << endl;
        /* try to recover */
        if ( (err = xrunRecovery(captureHandle, err)) !=  0 ) {
            cerr << "couldn't recover " << snd_strerror(err) << endl;
            return false;
        }
    }
}

int16_t *AlsaRecorder::getBuffer()
{
    return audioBuffer;
}

int AlsaRecorder::getBufferSize()
{
    return bb->bufferSize; 
}

short AlsaRecorder::getChannelNum() {
    return bb->numChannels;
}

double AlsaRecorder::getSampleRate() {
    return bb->sampleRate;
}

/*******************************************************************/
/**
 * Must be called before reading data.
 */
void AlsaRecorder::init()
{
    if (audioBuffer) {
        cerr << "Double initialization" << endl;
        return;
    }

    int err;
    snd_pcm_hw_params_t *hwParams;

    if((err = snd_pcm_open(&captureHandle, bb->soundDevice, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        cerr << "cannot open audio device " << bb->soundDevice << "(" << snd_strerror(err) << ")" << endl;
        return;
    }

    if((err = snd_pcm_hw_params_malloc(&hwParams)) < 0) {
        cerr << "cannot allocate hardware parameter structure " << snd_strerror(err) << endl;
        return;
    }

    if((err = snd_pcm_hw_params_any(captureHandle, hwParams)) < 0) {
        cerr << "cannot initialize hardware parameter structure " << snd_strerror(err) << endl;
        return;
    }

    if((err = snd_pcm_hw_params_set_access(captureHandle, hwParams, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) {
        cerr << "cannot set access type " << snd_strerror(err) << endl;
        return;
    }

    if((err = snd_pcm_hw_params_set_format(captureHandle, hwParams, SND_PCM_FORMAT_S16_LE)) < 0) {
        cerr << "cannot set sample format " << snd_strerror(err) << endl;
        return;
    }

    unsigned int oSR;
    oSR = getSampleRate();
    if((err = snd_pcm_hw_params_set_rate_near(captureHandle, hwParams, &oSR, 0)) < 0) {
        cerr << "cannot set sample rate " << snd_strerror(err) << endl;
        return;
    }

    if(oSR != getSampleRate()) {
        cerr << "cannot set sample rate, correct should be: " << getSampleRate() << ", is " << oSR << endl;
        return;
    }

    cerr << "ALSA-RX opened with a samplerate of " << oSR << " (requested: " << getSampleRate() << ")." << endl;

    if((err = snd_pcm_hw_params_set_channels(captureHandle, hwParams, getChannelNum())) < 0) {
        cerr << "cannot set channel count " << snd_strerror(err) << endl;
        return;
    }

    if((err = snd_pcm_hw_params(captureHandle, hwParams)) < 0) {
        cerr << "cannot set parameters " << snd_strerror(err) << endl;
        return;
    }

    snd_pcm_hw_params_free(hwParams);

    if((err = snd_pcm_prepare(captureHandle)) < 0) {
        cerr << "cannot prepare audio interface for use " << snd_strerror(err) << endl;
        return;
    }

    audioBuffer = new int16_t[getChannelNum() * getBufferSize()];

    setVolume(bb->soundSubDevice);
}

void AlsaRecorder::setVolume(const char *subdevice)
{
    int err;
    long vmin;
    long vmax;
    snd_mixer_t *mixer;
    snd_mixer_selem_id_t *selemid;

    int mode = 0;
    if((err = snd_mixer_open(&mixer, mode)) < 0) {
        cerr << "unable to open mixer " << snd_strerror(err) << endl;
        return;
    }
    if((err = snd_mixer_attach(mixer, bb->soundDeviceVol)) < 0) {
        cerr << "unable to attach card to mixer " << snd_strerror(err) << endl;
        return;
    }
    if((err = snd_mixer_selem_register(mixer, NULL, NULL)) < 0) {
        cerr << "unable to register mixer " << snd_strerror(err) << endl;
        return;
    }
    if((err = snd_mixer_load(mixer)) < 0) {
        cerr << "unable to load mixer " << snd_strerror(err) << endl;
        return;
    }
    snd_mixer_selem_id_malloc(&selemid);
    if(selemid == NULL) {
        cerr << "unable to allocate selemid." << endl;
        return;
    }
    snd_mixer_selem_id_set_index(selemid, 0);
    snd_mixer_selem_id_set_name(selemid, subdevice);
    snd_mixer_elem_t* elem = snd_mixer_find_selem(mixer, selemid);
    if(elem == NULL) {
        cerr << "unable to find selem." << endl;
        return;
    }
    if((err = snd_mixer_selem_get_capture_volume_range(elem, &vmin, &vmax)) < 0) {
        cerr << "unable to get capture volume range " << snd_strerror(err) << endl;
        return;
    }
    if((err = snd_mixer_selem_set_capture_volume_all(elem, (vmax * bb->volume) / 100)) < 0) {
        cerr << "unable to set capture volume " << snd_strerror(err) << endl;
        return;
    }
    snd_mixer_selem_id_free(selemid);
    if((err = snd_mixer_close(mixer)) < 0) {
        cerr << "unable to close mixer " << snd_strerror(err) << endl;
        return;
    }
}

int AlsaRecorder::xrunRecovery(snd_pcm_t *handle, int err)
{
    cerr << "stream recovery" << endl;

    if(err == -EPIPE) {
        /* underrun */
        err = snd_pcm_prepare(handle);
        if(err < 0) {
            cerr << "Can't recover from underrun, prepare failed: " << snd_strerror(err) << endl;
        }
        return 0;
    }
    return err;
}

void AlsaRecorder::destroyAlsa()
{
    if(!audioBuffer) {
        cerr << "Not initialized!" << endl;
        return;
    }

    cerr << "ALSA-RX closed." << endl;

    snd_pcm_drop(captureHandle);
    snd_pcm_close(captureHandle);

    delete audioBuffer;
}


// vim: set ts=4 sw=4 sts=4 expandtab:
