//
// Created by michal on 29.10.2018.
//

#include "amplitude_shift_keying.h"
#include "sndfile.hh"

AmplitudeShiftKeying::AmplitudeShiftKeying() {
    samplesPerPeriod = static_cast<unsigned int>(SAMPLE_RATE / FREQ);
    samplesPerSymbol = samplesPerPeriod;
}

AmplitudeShiftKeying::AmplitudeShiftKeying(unsigned int sampleRate) {
    samplesPerPeriod = static_cast<unsigned int>(sampleRate / FREQ);
    samplesPerSymbol = samplesPerPeriod;
    this -> sampleRate = sampleRate;
}

bool AmplitudeShiftKeying::modulate(std::ifstream &inputFileStream, SndfileHandle &outputFile) {
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

AmplitudeShiftKeying::Dibit AmplitudeShiftKeying::getDibitFromTwoChars(const char *dibitChars) {
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

std::vector<int> AmplitudeShiftKeying::getSignalForDibit(AmplitudeShiftKeying::Dibit dibit) {
    std::vector<int> signal;
    signal.reserve(samplesPerSymbol);
    double amplitude = AMPLITUDE * dibit / 3;

    for (unsigned int i = 0; i < samplesPerSymbol; i++) {
        signal.push_back(amplitude * sin(static_cast<double>(i % samplesPerPeriod) / samplesPerPeriod * 2 * M_PI));
    }

    return signal;
}

std::vector<int> AmplitudeShiftKeying::getSyncSignal() {
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

bool AmplitudeShiftKeying::demodulate(SndfileHandle &inputFile, std::ofstream &outputFileStream) {
    std::vector<int> syncSignal(samplesPerPeriod * 4);
    inputFile.readf(syncSignal.data(), samplesPerPeriod * 4);
    double amplitude = *std::max_element(std::begin(syncSignal), std::end(syncSignal));
    std::vector<int> signal(samplesPerPeriod);
    while (inputFile.readf(signal.data(), samplesPerPeriod)) {
        double max = *std::max_element(std::begin(signal), std::end(signal));
        Dibit dibitToWrite;
        if (max == 0.0) {
            dibitToWrite = Dibit::D00;
        } else if (max > 0.0 && max <= (amplitude / 3)) {
            dibitToWrite = Dibit::D01;
        } else if (max > (amplitude / 3) && max < amplitude) {
            dibitToWrite = Dibit::D10;
        } else if (max >= amplitude) {
            dibitToWrite = Dibit::D11;
        } else {
            return false;
        }
        outputFileStream.write(dibitToStringMap.find(dibitToWrite)->second.c_str(), 2);
    }
    return true;
}