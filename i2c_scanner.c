#include "lpc17xx_i2c.h"

int main(void) {
    PINSEL_CFG_Type i2c1_select;
    i2c1_select = (PINSEL_CFG_Type){
        .Funcnum = 3;
        .OpenDrain = PINSEL_PINMODE_NORMAL;
        .Pinmode = PINSEL_PINMODE_PULLUP;
        .Pinnum = 0;
        .Portnum = 0;
    };

    PINSEL_ConfigPin(i2c1_select);

    i2c1_select.Pinnum = 1;
    PINSEL_ConfigPin(i2c1_select);

    I2C_Init(LPC_I2C0, 100000);
}
