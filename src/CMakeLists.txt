cmake_minimum_required(VERSION 2.6)

project(bb-whistle)

set (CMAKE_CXX_FLAGS "-g -std=c++11 -Wall -Werror")

file(GLOB WHISTLE_SRC
	"alsarecorder.cpp"
	"alsarecorder.h"
	"audiocontainer.cpp"
	"audiocontainer.h"
	"audiostream.h"
	"config.h"
	"frequencyspectrum.cpp"
	"frequencyspectrum.h"
	"whistlematch.cpp"
	"whistlematch.h"
	"whistlerecog.cpp"
	"whistlerecog.h"
	"whistlesettings.cpp"
	"whistlesettings.h"
)

file(GLOB WHISTLE_DEBUG_SRC
    "wavfile.cpp"
    "whistledebug.cpp"
)

add_library(bb-whistle STATIC ${WHISTLE_SRC})
target_link_libraries(bb-whistle fftw3f asound)

add_executable(whistledebug ${WHISTLE_DEBUG_SRC})
target_link_libraries(whistledebug bb-whistle)

file(GLOB WHISTLE_EXAMPLE_SRC "example.cpp")
add_executable(minimal-example ${WHISTLE_EXAMPLE_SRC})
target_link_libraries(minimal-example bb-whistle)
