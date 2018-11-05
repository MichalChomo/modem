#include <cstdlib>
#include <istream>
#include <fstream>
#include <iostream>

#include "sndfile.hh"

#include "filename_helper.h"
#include "amplitude_shift_keying.h"

/*
 *
 */
int main(int argc, char **argv) {

    AmplitudeShiftKeying amplitudeModulation;
    SndfileHandle outputFile;

    std::string inputFileName(argv[1]);
    std::ifstream inputFileStream(inputFileName);
    if (inputFileStream.fail()) {
        std::cerr << "Unable to open the input file." << std::endl;
        return EXIT_FAILURE;
    }

    std::string outputFileName(FilenameHelper::addExtensionToFilename(
            FilenameHelper::getFilenameWithoutExtension(inputFileName), FilenameHelper::FileExtension::WAV));
    outputFile = SndfileHandle(outputFileName, SFM_WRITE, FORMAT, CHANNELS, SAMPLE_RATE);

    bool success = amplitudeModulation.modulate(inputFileStream, outputFile);

    if (!success) {
        std::remove(outputFileName.c_str());
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}