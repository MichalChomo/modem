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
    if (inputFile.error()) {
        std::cerr << "Unable to open the input file." << std::endl;
        return EXIT_FAILURE;
    }

    std::string outputFileName(FilenameHelper::addExtensionToFilename(
            FilenameHelper::getFilenameWithoutExtension(inputFileName), FilenameHelper::FileExtension::TXT));
    std::ofstream outputFileStream(outputFileName);

    AmplitudeShiftKeying amplitudeModulation;

    bool success = amplitudeModulation.demodulate(inputFile, outputFileStream);

    if (!success) {
        std::remove(outputFileName.c_str());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}