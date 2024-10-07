#include "ch.h"
#include "hal.h"
#include "out.h"
#include "Can.h"

uint8_t digitalOutputs[(4 + 7) / 8] = {0}; // Definition and initialization

static THD_WORKING_AREA(waOutThread, 128);
static THD_FUNCTION(OutThread, p)
{
    (void)p;
    chRegSetThreadName("Out_Thread");

    while (true)
    {
        chMtxLock(&digitalOutputsMutex);

        for(uint8_t i = 0; i < 4; i++)
        {
            switch (i)
            {
            case 0:
                if(ODCZYTAJ_BIT(digitalOutputs, i)) palSetPad(GPIOD, 0);
                else palClearPad(GPIOD, 0);
                break;
            case 1:
                if(ODCZYTAJ_BIT(digitalOutputs, i)) palSetPad(GPIOC, 13);
                else palClearPad(GPIOC, 13);
                break;
            case 2:
                if(ODCZYTAJ_BIT(digitalOutputs, i)) palSetPad(GPIOC, 14);
                else palClearPad(GPIOC, 14);
                break;
            case 3:
                if(ODCZYTAJ_BIT(digitalOutputs, i)) palSetPad(GPIOC, 15);
                else palClearPad(GPIOC, 15);
                break;
            default:
                break;
            }
        }

        chMtxUnlock(&digitalOutputsMutex);
        
        chThdSleepMilliseconds(250);
    }
}

void startOutThread(void)
{
    palSetPadMode(GPIOD, 0, PAL_MODE_OUTPUT_PUSHPULL);
    palClearPad(GPIOD, 0);
    palSetPadMode(GPIOC, 13, PAL_MODE_OUTPUT_PUSHPULL);
    palClearPad(GPIOC, 13);
    palSetPadMode(GPIOC, 14, PAL_MODE_OUTPUT_PUSHPULL);
    palClearPad(GPIOC, 14);
    palSetPadMode(GPIOC, 15, PAL_MODE_OUTPUT_PUSHPULL);
    palClearPad(GPIOC, 15);
    chThdCreateStatic(waOutThread, sizeof(waOutThread), NORMALPRIO, OutThread, NULL);
}