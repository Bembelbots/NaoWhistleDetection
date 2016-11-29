# NaoWhistleDetection

This repository contains the whistledetection of the team Bembelbots.
Additionally to the library for whistledetection, a small debugger
is included to test and run it on Wav-files.

The whistledetection requires the libraries Fftw3 and Alsa to run.
Install these dependencies on Debian/Ubuntu with
    $ apt install libfftw3-dev libasound2-dev

Build the whistle-detection:
    $ cmake src
    $ make

Run the whistledebugger (for more documentation, see src/whistledebug.cpp):
    $ ./whistledebug analyze <filename>.wav

For a minimal example on how to use the whistle detection, see src/example.cpp.
To test it, run
    $ ./minimal-example
