#ifndef MODEM_CONSTANTS_H
#define MODEM_CONSTANTS_H

#include <string>
#include "sndfile.h"

const int CHANNELS = 1;
const int FORMAT = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
const int SAMPLE_RATE = 18000;
const double FREQUENCY = 1000.0; // 2 bits for a baud, this gives 2000 bits/s.
const double AMPLITUDE = 1.0 * 0x7F000000;
const std::string SYNC_STRING("00110011");

#endif //MODEM_CONSTANTS_H