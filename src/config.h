#pragma once

/******************************************************************************
 *** YOU PROBABLY DONT WANT TO CHANGE THESE ***
 *****************************************************************************/
#define DEFAULT_NUM_CHANNELS 1
#define DEFAULT_SOUND_DEVICE "default"
#define DEFAULT_SOUND_DEVICE_VOL "default"
#define DEFAULT_SOUND_SUBDEVICE "Capture"
#define DEFAULT_SAMPLE_RATE_HZ 12000
#define DEFAULT_VOLUME 51
#define DEFAULT_AMPLITUDE INT16_MAX


/******************************************************************************
 *** Adjust these parameters to configure whistle detection ***
 *****************************************************************************/

// Size of the buffer when recording from microphone.
#define DEFAULT_BUFFER_SIZE_MS 50

// Accept a signal as a potential whistle while the peak
// is above this threshold. Probably the most important parameter
// for detecting a whislte.
#define DEFAULT_THRESHOLD_HZ 2000

// Discard any whistle candidate quieter than this parameter.
#define DEFAULT_VOLUME_THRESHOLD_DB -20

// Discard any whistle candidate shorter than DEFAULT_MIN_LENGTH. Unit: ms.
#define DEFAULT_MIN_LENGTH 400

// vim: set ts=4 sw=4 sts=4 expandtab:
