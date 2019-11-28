#include <string.h>

#include "lpc17xx_dac.h"
#include "lpc17xx_adc.h"
#include "lpc17xx_pinsel.h"

#include "leds.h"

#define SIGNAL_RATE (1000)
#define ADC_RATE (SIGNAL_RATE*100)
#define MEDIAN_WINDOW 7

void selection_sort(uint32_t *arr, uint8_t n);
void ADC_IRQHandler(void);

int main(void) {
    PINSEL_CFG_Type pinenable = {
        .Funcnum = 1,
        .OpenDrain = PINSEL_PINMODE_NORMAL,
        .Pinmode = PINSEL_PINMODE_TRISTATE,
        .Pinnum = 23,
        .Portnum = 0
    };

    // enable adc
    PINSEL_ConfigPin(&pinenable);

    pinenable.Funcnum = 2;
    pinenable.Pinnum = 26;

    // enable dac
    PINSEL_ConfigPin(&pinenable);

    ADC_Init(LPC_ADC, ADC_RATE);
    ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0, ENABLE);
    ADC_IntConfig(LPC_ADC, ADC_ADINTEN0, SET);
    ADC_BurstCmd(LPC_ADC, ENABLE);

    DAC_Init(LPC_DAC);

    led_setup();
    led_disp_num(0xf);

    // for some reason ADC_IntConfig doesn't actually enable the ADC interrupt,
    // just sets which channels can trigger an interrupt. this enables the
    // interrupt manually
    uint32_t *ISER0 = (uint32_t *)0xE000E100;
    *ISER0 |= 1<<22;

    return 0;
}

void ADC_IRQHandler(void) {
    static uint32_t data[MEDIAN_WINDOW];
    uint32_t sorted_data[MEDIAN_WINDOW];
    static uint8_t i = 0;

    data[i] = ADC_ChannelGetData(LPC_ADC, ADC_CHANNEL_0) >> 2;
    i = i<MEDIAN_WINDOW-1?i+1:0;

    memcpy((void*)sorted_data, (void*)data, MEDIAN_WINDOW*sizeof(uint32_t));

    // sort the data points to find the median
    selection_sort(sorted_data, MEDIAN_WINDOW);

    DAC_UpdateValue(LPC_DAC, sorted_data[MEDIAN_WINDOW/2]);
}

// based on code from https://www.geeksforgeeks.org/selection-sort/
void selection_sort(uint32_t *arr, uint8_t n) {
    uint8_t i, j, min_idx;
    uint32_t tmp;
  
    for (i = 0; i <= n-1; i++) { 

        min_idx = i; 

        for (j = i+1; j <= n; j++) {
          if (arr[j] < arr[min_idx]) {
            min_idx = j; 
          }
        }

        tmp = arr[i];
        arr[i] = arr[min_idx];
        arr[min_idx] = tmp;
    } 
} 
