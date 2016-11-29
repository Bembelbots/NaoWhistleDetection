#pragma once


class WhistleSettings {
    
    public:
        /* Number of capture channels. */
        const short numChannels;
        const char *soundDevice;
        const char *soundDeviceVol;
        const char *soundSubDevice;
        
        /* Recorded samples per second */
        const int sampleRate;
        /* Volume of capture device. 
         * Range: 0--100
         */
        const int volume;
        const int amplitude;
              int bufferSize;
        const int threshold_Hz;
        const int volumeThreshold_dB;
        const int minSignalLength_ms;

        WhistleSettings(int threshold_Hz, int threshold_dB, int minLength_ms, int buffer_ms);
        WhistleSettings(int threshold_Hz, int threshold_dB, int minLength_ms, int buffer_ms,
            short numCh, int sampleRate);
};

// vim: set ts=4 sw=4 sts=4 expandtab:
