#include <stdio.h>

#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"

#include "serial.h"
#include "wait.h"

#define ADC_RATE 200000

// Reads the value of ADC channel 0 (mbed p15, chip pin p0.23) and writes it to the serial line

int main(void) {
    uint32_t data;
    char outstr[13];

    PINSEL_CFG_Type adc_select = {
        .Funcnum = 1,
        .OpenDrain = PINSEL_PINMODE_NORMAL,
        .Pinmode = PINSEL_PINMODE_TRISTATE,
        .Pinnum = 23,
        .Portnum = 0
    };

    PINSEL_ConfigPin(&adc_select);

    serial_init();

    write_usb_serial_blocking("start\n\r", 7);

    ADC_Init(LPC_ADC, 100);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);

    ADC_BurstCmd(LPC_ADC, 1);

    while(1) {
        data = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0);
        snprintf(outstr, 13, "%010lu\n\r", data);
        write_usb_serial_blocking(outstr, 13);
        wait_ms(10);
    }
}
