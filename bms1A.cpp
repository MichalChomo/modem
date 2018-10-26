/*
 * File:   bms1A.cpp
 */

#include <cstdlib>
#include <math.h>
#include <istream>
#include <fstream>
#include <iostream>

#include "constants.h"
#include "sndfile.hh"

#include "FileNameHelper.h"

/*
 *
 */
int main(int argc, char **argv) {

    SndfileHandle outputFile;

    std::string inputFileName(argv[1]);
    std::string outputFileName(FileNameHelper::getFileNameWithoutExtension(inputFileName));

    std::ifstream inputFileStream(inputFileName);

    int *buffer = new int[SAMPLE_RATE];

    for (int i = 0; i < SAMPLE_RATE; i++) {
        buffer[i] = AMPLITUDE * sin(FREQ * 2 * i * M_PI);
        std::cout << AMPLITUDE * sin(FREQ * 2 * i * M_PI) << " ";
    }
    std::cout << std::endl;

    outputFile = SndfileHandle(
            FileNameHelper::addExtensionToFileName(outputFileName, FileNameHelper::FileExtension::WAV),
            SFM_WRITE, FORMAT, CHANELS, SAMPLE_RATE);

    outputFile.write(buffer, SAMPLE_RATE);

    delete[] buffer;
    return EXIT_SUCCESS;
}