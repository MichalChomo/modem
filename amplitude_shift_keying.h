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

    /**
     * Reads bits from the input file and write a discrete time signal representing the bits to the output file.
     * Writes synchronization sequence signal at the beginning of the output file.
     * Number of samples per period is equal to the number of samples per symbol and the symbol contains two bits, so
     * the bit rate is FREQUENCY * 2.
     * @param inputFileStream Input file stream containing a string of '0' and '1' characters representing bits.
     * @param outputFile      Output WAV file.
     * @return true on success, false otherwise.
     */
    bool modulate(std::ifstream &inputFileStream, SndfileHandle &outputFile);

    /**
     * Reads a discrete time signal from the input file and writes bits it represents to the output file.
     * @param inputFile        Input WAV file.
     * @param outputFileStream Output file stream.
     * @return true on success, false otherwise.
     */
    bool demodulate(SndfileHandle &inputFile, std::ofstream &outputFileStream);

private:

    /**
     * 4-state amplitude shift keying is used, therefore two bits correspond to a baud.
     */
    enum Dibit {
        D00, D01, D10, D11, ERROR
    };

    std::map<Dibit, std::string> dibitToStringMap {
        {Dibit::D00, "00"},
        {Dibit::D01, "01"},
        {Dibit::D10, "10"},
        {Dibit::D11, "11"}
    };

    /**
     * Returns the Dibit enum value corresponding to the given characters.
     * @param dibitChars Two characters representing dibit.
     * @return The Dibit enum value corresponding to the given characters or Dibit enum error on failure.
     */
    Dibit getDibitFromTwoChars(const char *dibitChars);

    /**
     * Generates a sine wave with a correct amplitude corresponding to the given dibit.
     * @param dibit            The dibit to get signal for.
     * @param samplesPerPeriod Number of discrete samples in the returned signal.
     * @return A vector containing discrete time signal representing the sine wave.
     */
    std::vector<int> getSignalForDibit(Dibit dibit, int samplesPerPeriod);

    /**
     * Generates the synchronization sequence signal that is predefined as "00110011".
     * @return The synchronization sequence as a discrete time signal.
     */
    std::vector<int> getSyncSignal(int samplesPerPeriod);

    /**
     * Determines the maximum magnitude of the given signal.
     * @param signal A discrete time signal.
     * @return The maximum magnitude of the given signal.
     */
    double getMaxMagnitudeOfSignal(std::vector<int> &signal);

    /**
     * Determines the Dibit enum value by the given maximum magnitude of the period.
     * @param maxMagnitude       The maximum magnitude from the synchronization signal.
     * @param maxPeriodMagnitude The maximum magnitude of the period.
     * @return The Dibit enum value corresponding to the maximum period magnitude.
     */
    Dibit getDibitByMagnitude(double maxMagnitude, double maxPeriodMagnitude);

    /**
     * Determines the number of samples in one baud from the synchronization sequence of the input WAV file.
     * @param inputFile Input WAV file.
     * @return The number of samples in one baud.
     */
    int getSamplesPerBaud(SndfileHandle &inputFile);

};


#endif //MODEM_AMPLITUDESHIFTKEYING_H
