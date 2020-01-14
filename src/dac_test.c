#include <math.h>

#include "lpc17xx_pinsel.h"
#include "lpc17xx_dac.h"

#include "wait.h"
#include "mbed_dac.h"

int main(void) {
    uint32_t ampl=MBED_DAC_WAVEFORM_AMPL_MAX/8, freq=MBED_DAC_FREQ_MAX/8;
    mbed_dac_init();
    while (1) {
        ampl = ampl<MBED_DAC_WAVEFORM_AMPL_MAX?ampl*2:MBED_DAC_WAVEFORM_AMPL_MAX/8;
        mbed_dac_waveform(ampl, freq, 10e6);
        freq = freq<MBED_DAC_FREQ_MAX?freq*2:MBED_DAC_FREQ_MAX/8;
        mbed_dac_waveform(ampl, freq, 10e6);
    }
    return 0;
}
