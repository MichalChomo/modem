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


class AmplitudeModulation {

public:
    int *modulate(std::ifstream &inputFileStream);

private:
    enum Dibit {
        D00, D01, D10, D11, ERROR
    };

    /**
     * Returns Dibit enum value corresponding to the given characters.
     * @param dibitChars Two characters representing dibit.
     * @return Dibit enum value corresponding to the given characters or Dibit enum error on failure.
     */
    Dibit getDibitFromTwoChars(const char *dibitChars);

};


#endif //MODEM_AMPLITUDEMODULATION_H
