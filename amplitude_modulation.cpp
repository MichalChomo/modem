//
// Created by michal on 29.10.2018.
//

#include "amplitude_modulation.h"
#include "sndfile.hh"

AmplitudeModulation::AmplitudeModulation() {
    samplesPerPeriod = static_cast<int>(SAMPLE_RATE / FREQ);
    samplesPerSymbol = samplesPerPeriod;
}

bool AmplitudeModulation::modulate(std::ifstream &inputFileStream, SndfileHandle &outputFile) {
    std::streamsize size = 2;
    std::vector<int> syncSignal = getSyncSignal();
    outputFile.write(syncSignal.data(), syncSignal.size());
    while (!inputFileStream.eof() && !inputFileStream.fail()) {
        char dibitChars[2];
        inputFileStream.read(dibitChars, size);
        if (inputFileStream.eof() || inputFileStream.fail()) {
            break;
        }
        Dibit dibit = getDibitFromTwoChars(dibitChars);
        if (dibit == Dibit::ERROR) {
            std::cerr << "Incorrect character found, exiting." << std::endl;
            return false;
        }
        std::vector<int> signal = getSignalForDibit(dibit);
        outputFile.write(signal.data(), signal.size());
    }
    return true;
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

std::vector<int> AmplitudeModulation::getSignalForDibit(AmplitudeModulation::Dibit dibit) {
    std::vector<int> signal;
    signal.reserve(samplesPerSymbol);
    double amplitude = AMPLITUDE * dibit / 3;

    for (double i = 0; i < samplesPerSymbol; i++) {
        i = static_cast<int>(i) % samplesPerPeriod;
        signal.push_back(amplitude * sin((i / samplesPerPeriod) * 2 * M_PI));
    }

    return signal;
}

std::vector<int> AmplitudeModulation::getSyncSignal() {
    std::vector<int> syncSignal;
    const char *syncSignalChar = SYNC_STRING.c_str();
    for (unsigned int i = 0; i < SYNC_STRING.size(); i += 2) {
        char dibit[2];
        dibit[0] = syncSignalChar[i];
        dibit[1] = syncSignalChar[i+1];
        std::vector<int> dibitSignal = getSignalForDibit(getDibitFromTwoChars(dibit));
        syncSignal.insert(std::end(syncSignal), std::begin(dibitSignal), std::end(dibitSignal));
    }
    return syncSignal;
}