#pragma once


class WhistleSettings;
class AudioStream;
class WhistleMatch;

class WhistleRecog {
public:
    WhistleRecog(WhistleSettings *, AudioStream *);
    WhistleRecog();
    virtual ~WhistleRecog();

    bool process();
    void free();
    bool hasWhistleCandidate();
    float getCurrPeak();
    WhistleMatch* getLastResult();

private:
    AudioStream *aStream;
    bool ownsAStream;
    WhistleSettings *settings;
    bool ownsSettings;
    WhistleMatch *currmatch;
    WhistleMatch *lastResult;
    int prevPeak;

    void init();
    void saveResult();
};

// vim: set ts=4 sw=4 sts=4 expandtab:
