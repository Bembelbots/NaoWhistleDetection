#pragma once

class WhistleMatch {
    public:
        WhistleMatch(int minLen_ms, int minVol_dB);

        void update(int len_ms, float currVolume_dB);
        bool whistleDetected();
        int getLength_ms();
        float getMaxVolume_dB();

    private:
        int minLength_ms;
        float volumeTreshold_dB;
        int length_ms;
        float maxVolume_dB;
};

// vim: set ts=4 sw=4 sts=4 expandtab:
