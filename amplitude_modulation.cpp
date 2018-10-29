//
// Created by michal on 29.10.2018.
//

#include "amplitude_modulation.h"

int *AmplitudeModulation::modulate(std::ifstream &inputFileStream) {
    std::streamsize size = 2;
    int *buffer = new int[SAMPLE_RATE];
    while (!inputFileStream.eof() && !inputFileStream.fail()) {
        char dibit[2];
        inputFileStream.read(dibit, size);
        if (inputFileStream.eof() || inputFileStream.fail()) {
            break;
        }
        std::cout << "dibit: " << getDibitFromTwoChars(dibit) << std::endl;
        for (int i = 0; i < SAMPLE_RATE; i++) {
            buffer[i] = AMPLITUDE * sin(FREQ * 2 * i * M_PI);
        }
    }
    return buffer;
}

AmplitudeModulation::Dibit AmplitudeModulation::getDibitFromTwoChars(const char *dibitChars) {
    if (dibitChars[0] == '0') {
        if (dibitChars[1] == '0') {
            return Dibit::D00;
        } else if (dibitChars[1] == '1') {
            return Dibit::D01;
        }
    } else if (dibitChars[0] == '1') {
        if (dibitChars[1] == '0') {
            return Dibit::D10;
        } else if (dibitChars[1] == '1') {
            return Dibit::D11;
        }
    }
    return Dibit::ERROR;
}
