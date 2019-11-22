#include <math.h>

#include "lpc17xx_pinsel.h"
#include "lpc17xx_dac.h"

#include "wait.h"

#define PI (3.14159265)
#define UINT10_MAX ((1<<10)-1)

int main(void) {
    double r;
    uint32_t value;
    PINSEL_CFG_Type dac_select = {
        .Funcnum = 2,
        .OpenDrain = PINSEL_PINMODE_NORMAL,
        .Pinmode = PINSEL_PINMODE_TRISTATE,
        .Pinnum = 26,
        .Portnum = 0
    };

    PINSEL_ConfigPin(&dac_select);

    DAC_Init(LPC_DAC);

    wait_ms(1);

    while(1) {
        for(r = 0; r < 2*PI; r += (PI*2)/200) {
            value = (sin(r)+1)*((double)(UINT10_MAX/2));
            DAC_UpdateValue(LPC_DAC, value);
            wait_ms(1);
        }
    }

    return 0;
}
