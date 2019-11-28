#include <math.h>
#include <stdbool.h>

#include "lpc17xx_pinsel.h"
#include "lpc17xx_dac.h"

#include "mbed_dac.h"

#define PI (3.14159265)
#define ONEHZ 1000000000

void mbed_dac_init(void) {
    PINSEL_CFG_Type dac_select = {
        .Funcnum = 2,
        .OpenDrain = PINSEL_PINMODE_NORMAL,
        .Pinmode = PINSEL_PINMODE_TRISTATE,
        .Pinnum = 26,
        .Portnum = 0
    };

    PINSEL_ConfigPin(&dac_select);

    DAC_Init(LPC_DAC);
}


void mbed_dac_waveform_loop(uint16_t ampl, uint32_t freq) {
    uint32_t value;

    // should add some feedback here
    if (ampl > MBED_DAC_WAVEFORM_AMPL_MAX) {
        return;
    } else if (freq > MBED_DAC_FREQ_MAX) {
        return;
    }

    uint32_t period = ONEHZ / freq; 

    while(1) {
        for(r = 0; r < 2*PI; r += (PI*2)/200) {
            wait_ns(period);
            value = (sin(r)+1)*((double)(ampl));
            DAC_UpdateValue(LPC_DAC, value);
        }
    }
}
