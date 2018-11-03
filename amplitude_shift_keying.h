//
// Created by michal on 29.10.2018.
//

#ifndef MODEM_AMPLITUDESHIFTKEYING_H
#define MODEM_AMPLITUDESHIFTKEYING_H

#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>
#include "constants.h"
#include "sndfile.hh"


class AmplitudeShiftKeying {

public:

    AmplitudeShiftKeying();

    AmplitudeShiftKeying(unsigned int sampleRate);

    /**
     * Read bits from the input file and write a discrete time signal representing the bits to the output file.
     * @param inputFileStream Input file containing a string of '0' and '1' characters representing bits.
     * @param outputFile Output WAV file.
     * @return true on success, false otherwise.
     */
    bool modulate(std::ifstream &inputFileStream, SndfileHandle &outputFile);

    bool demodulate(SndfileHandle &inputFile, std::ofstream &outputFileStream);

private:
    enum Dibit {
        D00, D01, D10, D11, ERROR
    };

    std::map<Dibit, std::string> dibitToStringMap {
        {Dibit::D00, "00"},
        {Dibit::D01, "01"},
        {Dibit::D10, "10"},
        {Dibit::D11, "11"}
    };

    unsigned int samplesPerPeriod;
    unsigned int samplesPerSymbol;
    unsigned int sampleRate;

    /**
     * Returns Dibit enum value corresponding to the given characters.
     * @param dibitChars Two characters representing dibit.
     * @return Dibit enum value corresponding to the given characters or Dibit enum error on failure.
     */
    Dibit getDibitFromTwoChars(const char *dibitChars);

    std::vector<int> getSignalForDibit(Dibit dibit);

    std::vector<int> getSyncSignal();

};


#endif //MODEM_AMPLITUDESHIFTKEYING_H
