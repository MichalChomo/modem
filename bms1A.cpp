/*
 * File:   bms1A.cpp
 */

#include <cstdlib>
#include <istream>
#include <fstream>
#include <iostream>

#include "sndfile.hh"

#include "filename_helper.h"
#include "amplitude_modulation.h"

/*
 *
 */
int main(int argc, char **argv) {

    AmplitudeModulation amplitudeModulation;
    SndfileHandle outputFile;

    std::string inputFileName(argv[1]);
    std::ifstream inputFileStream(inputFileName);

    std::string outputFileName(FilenameHelper::addExtensionToFilename(
            FilenameHelper::getFilenameWithoutExtension(inputFileName), FilenameHelper::FileExtension::WAV));
    outputFile = SndfileHandle(outputFileName, SFM_WRITE, FORMAT, CHANNELS, SAMPLE_RATE);

    bool success = amplitudeModulation.modulate(inputFileStream, outputFile);
    inputFileStream.close();

    if (!success) {
        std::remove(outputFileName.c_str());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}