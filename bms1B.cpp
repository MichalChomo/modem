/* 
 * File:   bms1B.cpp
 */

#include <cstdlib>
#include <math.h>

#include "sndfile.hh"
#include "amplitude_shift_keying.h"
#include "filename_helper.h"


/*
 * 
 */
int main(int argc, char **argv) {

    std::string inputFileName(argv[1]);
    SndfileHandle inputFile = SndfileHandle(inputFileName);
    int sampleRate = inputFile.samplerate();

    std::string outputFileName(FilenameHelper::addExtensionToFilename(
            FilenameHelper::getFilenameWithoutExtension(inputFileName), FilenameHelper::FileExtension::TXT));
    std::ofstream outputFileStream(outputFileName);

    AmplitudeShiftKeying amplitudeModulation(static_cast<unsigned int>(sampleRate));

    bool success = amplitudeModulation.demodulate(inputFile, outputFileStream);

    if (!success) {
        std::remove(outputFileName.c_str());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}