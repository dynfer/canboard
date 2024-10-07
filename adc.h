#ifndef ADC_H
#define ADC_H

#include <inttypes.h>
#include "ch.h"
#include "hal.h"

#define ADC1_NUM_CHANNELS   10
#define ADC1_BUF_DEPTH      1

extern adcsample_t adc1_samples[ADC1_NUM_CHANNELS * ADC1_BUF_DEPTH];

void startAdcThread(void);

#endif