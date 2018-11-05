#include "amplitude_shift_keying.h"
#include "sndfile.hh"

bool AmplitudeShiftKeying::modulate(std::ifstream &inputFileStream, SndfileHandle &outputFile) {
    int samplesPerPeriod = static_cast<int>(SAMPLE_RATE / FREQUENCY);
    std::vector<int> syncSignal = getSyncSignal(samplesPerPeriod);
    // Write the synchronization sequence signal to the beginning of the output file.
    outputFile.write(syncSignal.data(), syncSignal.size());

    // Read the bits from the input file and write the corresponding discrete time signal to the output file.
    while (!inputFileStream.eof() && !inputFileStream.fail()) {
        char dibitChars[2];
        inputFileStream.read(dibitChars, 2);
        if (inputFileStream.eof() || inputFileStream.fail()) {
            break;
        }
        Dibit dibit = getDibitFromTwoChars(dibitChars);
        if (dibit == Dibit::ERROR) {
            std::cerr << "Incorrect character found, exiting." << std::endl;
            return false;
        }
        std::vector<int> signal = getSignalForDibit(dibit, samplesPerPeriod);
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

std::vector<int> AmplitudeShiftKeying::getSignalForDibit(Dibit dibit, int samplesPerPeriod) {
    std::vector<int> signal;
    signal.reserve(static_cast<unsigned long>(samplesPerPeriod));
    double dibitAmplitude = AMPLITUDE * dibit / 3;

    // Samples per period is equal to samples per baud.
    for (int i = 0; i < samplesPerPeriod; i++) {
        signal.push_back(dibitAmplitude * sin(static_cast<double>(i) / samplesPerPeriod * 2 * M_PI));
    }

    return signal;
}

std::vector<int> AmplitudeShiftKeying::getSyncSignal(int samplesPerPeriod) {
    std::vector<int> syncSignal;
    const char *syncSignalChar = SYNC_STRING.c_str();

    for (unsigned int i = 0; i < SYNC_STRING.size(); i += 2) {
        char dibit[2];
        dibit[0] = syncSignalChar[i];
        dibit[1] = syncSignalChar[i+1];
        std::vector<int> dibitSignal = getSignalForDibit(getDibitFromTwoChars(dibit), samplesPerPeriod);
        syncSignal.insert(std::end(syncSignal), std::begin(dibitSignal), std::end(dibitSignal));
    }

    return syncSignal;
}

bool AmplitudeShiftKeying::demodulate(SndfileHandle &inputFile, std::ofstream &outputFileStream) {
    int samplesPerBaud = getSamplesPerBaud(inputFile);
    int restOfSyncSignalSize = ((samplesPerBaud * 2) - 1);
    std::vector<int> syncSignal(restOfSyncSignalSize);
    inputFile.readf(syncSignal.data(), restOfSyncSignalSize);
    double maxMagnitude = getMaxMagnitudeOfSignal(syncSignal);
    std::vector<int> signal(samplesPerBaud);

    while (inputFile.readf(signal.data(), samplesPerBaud)) {
        Dibit dibitToWrite = getDibitByMagnitude(maxMagnitude, getMaxMagnitudeOfSignal(signal));
        if (dibitToWrite != Dibit::ERROR) {
            outputFileStream.write(dibitToStringMap.find(dibitToWrite)->second.c_str(), 2);
        } else {
            return false;
        }
    }

    return true;
}

AmplitudeShiftKeying::Dibit AmplitudeShiftKeying::getDibitByMagnitude(double maxMagnitude, double maxPeriodMagnitude) {
    if (maxPeriodMagnitude >= 0.0 && maxPeriodMagnitude < (maxMagnitude / 6)) {
        return Dibit::D00;
    } else if (maxPeriodMagnitude >= (maxMagnitude / 6) && maxPeriodMagnitude < (maxMagnitude / 2)) {
        return Dibit::D01;
    } else if (maxPeriodMagnitude >= (maxMagnitude / 2) && maxPeriodMagnitude < (5 * maxMagnitude / 6)) {
        return Dibit::D10;
    } else if (maxPeriodMagnitude >= (5 * maxMagnitude / 6)) {
        return Dibit::D11;
    } else {
        return Dibit::ERROR;
    }
}

double AmplitudeShiftKeying::getMaxMagnitudeOfSignal(std::vector<int> &signal) {
    // Do not use edge values of the signal.
    auto beginIterator = std::begin(signal) + 2;
    auto endIterator = std::end(signal) - 2;
    double maxAmplitude = *std::max_element(beginIterator, endIterator);
    double maxNegativeAmplitude = std::abs(*std::min_element(beginIterator, endIterator));

    return maxAmplitude > maxNegativeAmplitude ? maxAmplitude : maxNegativeAmplitude;
}

int AmplitudeShiftKeying::getSamplesPerBaud(SndfileHandle &inputFile) {
    int sample = 0;

    // Read until a non-zero value.
    while (sample == 0) {
        inputFile.read(&sample, 1);
    }

    bool endOfBaud = false;
    int i = 2; // First zero sample and second non-zero sample are already read.
    while (!endOfBaud) {
        ++i;
        inputFile.read(&sample, 1);
        if (sample == 0) {
            inputFile.read(&sample, 1);
            // On two consecutive zeroes, assume end of baud.
            if (sample == 0) {
                endOfBaud = true;
            }
        }
    }

    return i;
}
