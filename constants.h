//
// Created by michal on 26.10.2018.
//

#ifndef MODEM_CONSTANTS_H
#define MODEM_CONSTANTS_H


#include "sndfile.h"

const int SAMPLE_RATE = 18000;
const int CHANELS = 1;
const int FORMAT = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
const double AMPLITUDE = 1.0 * 0x7F000000;
const double FREQ = 1000.0 / SAMPLE_RATE;

#endif //MODEM_CONSTANTS_H