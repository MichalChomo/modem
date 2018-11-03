//
// Created by michal on 29.10.2018.
//

#ifndef MODEM_AMPLITUDEMODULATION_H
#define MODEM_AMPLITUDEMODULATION_H

#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include "constants.h"
#include "sndfile.hh"


class AmplitudeModulation {

public:

    AmplitudeModulation();

    /**
     * Read bits from the input file and write a discrete time signal representing the bits to the output file.
     * @param inputFileStream Input file containing a string of '0' and '1' characters representing bits.
     * @param outputFile Output WAV file.
     * @return true on success, false otherwise.
     */
    bool modulate(std::ifstream &inputFileStream, SndfileHandle &outputFile);

private:
    enum Dibit {
        D00, D01, D10, D11, ERROR
    };

    int samplesPerPeriod;
    int samplesPerSymbol;

    /**
     * Returns Dibit enum value corresponding to the given characters.
     * @param dibitChars Two characters representing dibit.
     * @return Dibit enum value corresponding to the given characters or Dibit enum error on failure.
     */
    Dibit getDibitFromTwoChars(const char *dibitChars);

    std::vector<int> getSignalForDibit(Dibit dibit);

    std::vector<int> getSyncSignal();

};


#endif //MODEM_AMPLITUDEMODULATION_H
