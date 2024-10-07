#include "ch.h"
#include "hal.h"
#include "digital.h"
#include "Can.h"

uint8_t digitalInputs[(8 + 7) / 8] = {0}; // Definition and initialization

static THD_WORKING_AREA(waDigitalThread, 128);
static THD_FUNCTION(DigitalThread, p)
{
    (void)p;
    chRegSetThreadName("Digital_Thread");

    while (true)
    {
        chMtxLock(&digitalInputsMutex);
        for (uint8_t i = 0; i < 8; i++)
        {
            switch (i)
            {
            case 0:
                if(palReadPad(GPIOB, 3)) USTAW_BIT(digitalInputs, i);
                else CZYSC_BIT(digitalInputs, i);
                break;
            case 1:
                if(palReadPad(GPIOB, 4)) USTAW_BIT(digitalInputs, i);
                else CZYSC_BIT(digitalInputs, i);
                break;
            case 2:
                if(palReadPad(GPIOB, 5)) USTAW_BIT(digitalInputs, i);
                else CZYSC_BIT(digitalInputs, i);
                break;
            case 3:
                if(palReadPad(GPIOB, 6)) USTAW_BIT(digitalInputs, i);
                else CZYSC_BIT(digitalInputs, i);
                break;
            case 4:
                if(palReadPad(GPIOB, 7)) USTAW_BIT(digitalInputs, i);
                else CZYSC_BIT(digitalInputs, i);
                break;
            case 5:
                if(palReadPad(GPIOB, 15)) USTAW_BIT(digitalInputs, i);
                else CZYSC_BIT(digitalInputs, i);
                break;
            case 6:
                if(palReadPad(GPIOA, 15)) USTAW_BIT(digitalInputs, i);
                else CZYSC_BIT(digitalInputs, i);
                break;
            case 7:
                if(palReadPad(GPIOA, 8)) USTAW_BIT(digitalInputs, i);
                else CZYSC_BIT(digitalInputs, i);
                break;
            default:
                break;
            }
        }
        chMtxUnlock(&digitalInputsMutex);
        chThdSleepMilliseconds(250);
    }
}

void startDigitalThread(void)
{
    palSetPadMode(GPIOB, 3, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 4, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 5, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 6, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 7, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOB, 15, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 15, PAL_MODE_INPUT_PULLUP);
    palSetPadMode(GPIOA, 8, PAL_MODE_INPUT_PULLUP);
    chThdCreateStatic(waDigitalThread, sizeof(waDigitalThread), NORMALPRIO + 2, DigitalThread, NULL);
}