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

    std::string outputFileName(FilenameHelper::getFilenameWithoutExtension(inputFileName));
    outputFile = SndfileHandle(
            FilenameHelper::addExtensionToFilename(outputFileName, FilenameHelper::FileExtension::WAV),
            SFM_WRITE, FORMAT, CHANELS, SAMPLE_RATE);

    outputFile.write(amplitudeModulation.modulate(inputFileStream), SAMPLE_RATE);

    inputFileStream.close();

    return EXIT_SUCCESS;
}