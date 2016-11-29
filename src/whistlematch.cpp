#include "whistlematch.h"

#include <float.h>


WhistleMatch::WhistleMatch(int minLen_ms, int minVol_dB)
    : minLength_ms(minLen_ms)
    , volumeTreshold_dB(minVol_dB)
    , length_ms(0)
    , maxVolume_dB(-FLT_MAX)
{
}

void WhistleMatch::update(int len_ms, float currVolume_dB) {
    length_ms += len_ms;
    if (currVolume_dB > maxVolume_dB) {
        maxVolume_dB = currVolume_dB;
    }
}

bool WhistleMatch::whistleDetected() {
    bool lenOk = length_ms > minLength_ms;
    bool volOk = maxVolume_dB > volumeTreshold_dB;
    return  lenOk && volOk;
}

int WhistleMatch::getLength_ms() {
    return length_ms;
}

float WhistleMatch::getMaxVolume_dB() {
    return maxVolume_dB;
}

// vim: set ts=4 sw=4 sts=4 expandtab:
