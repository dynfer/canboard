#include "ch.h"
#include "hal.h"
#include "adc.h"
#include "hal_adc_lld.h"
#include "Can.h"

adcsample_t adc1_samples[10] = {0};

static const ADCConversionGroup adc1_cfg = {
  .circular = false,
  .num_channels = ADC1_NUM_CHANNELS,
  .end_cb = NULL,
  .error_cb = NULL,
  .cr1 = 0,
  .cr2 = ADC_CR2_SWSTART,
  .smpr1 = 0,
  .smpr2 = ADC_SMPR2_SMP_AN0(ADC_SAMPLE_28P5) | ADC_SMPR2_SMP_AN1(ADC_SAMPLE_28P5) |
           ADC_SMPR2_SMP_AN2(ADC_SAMPLE_28P5) | ADC_SMPR2_SMP_AN3(ADC_SAMPLE_28P5) |
           ADC_SMPR2_SMP_AN4(ADC_SAMPLE_28P5) | ADC_SMPR2_SMP_AN5(ADC_SAMPLE_28P5) |
           ADC_SMPR2_SMP_AN6(ADC_SAMPLE_28P5) | ADC_SMPR2_SMP_AN7(ADC_SAMPLE_28P5) |
           ADC_SMPR2_SMP_AN8(ADC_SAMPLE_28P5) | ADC_SMPR2_SMP_AN9(ADC_SAMPLE_28P5),
  .sqr1 = ADC_SQR1_NUM_CH(ADC1_NUM_CHANNELS),
  .sqr2 = ADC_SQR2_SQ7_N(ADC_CHANNEL_IN6) | ADC_SQR2_SQ8_N(ADC_CHANNEL_IN7) |
          ADC_SQR2_SQ8_N(ADC_CHANNEL_IN8) | ADC_SQR2_SQ8_N(ADC_CHANNEL_IN9),
  .sqr3 = ADC_SQR3_SQ1_N(ADC_CHANNEL_IN0) | ADC_SQR3_SQ2_N(ADC_CHANNEL_IN1) |
          ADC_SQR3_SQ3_N(ADC_CHANNEL_IN2) | ADC_SQR3_SQ4_N(ADC_CHANNEL_IN3) |
          ADC_SQR3_SQ5_N(ADC_CHANNEL_IN4) | ADC_SQR3_SQ6_N(ADC_CHANNEL_IN5)
};

static THD_WORKING_AREA(waAdcThread, 128);
static THD_FUNCTION(AdcThread, p)
{
    (void)p;
    chRegSetThreadName("Adc_Thread");

    while (true)
    {   
        chMtxLock(&adcSampleMutex);
        adcConvert(&ADCD1, &adc1_cfg, adc1_samples, ADC1_BUF_DEPTH);
        chMtxUnlock(&adcSampleMutex);
        chThdSleepMilliseconds(250);
    }
}

void startAdcThread(void)
{
    adcStart(&ADCD1, NULL);
    chThdCreateStatic(waAdcThread, sizeof(waAdcThread), NORMALPRIO + 30, AdcThread, NULL);
}