#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "lpc_types.h"
#include "lpc17xx_systick.h"

#include "leds.h"
#include "serial.h"


#define COUNT_STRING_LEN 16
#define COUNT_SHIFT 2

long int itob(int x, size_t digits);

int main(void) {
    serial_init();
    
    led_setup();
    led_clear();
    
    SYSTICK_InternalInit(100);
    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE);
    
    write_usb_serial_blocking("Starting count\n\r", 16);
    
    return 0;
}

void SysTick_Handler(void) {
    static uint8_t count = 0;
    static char interrupt_counter = 0;
    char count_string[COUNT_STRING_LEN];
    unsigned int printable_count;
    long int binary_count;
    
    if (interrupt_counter < 10) {
        interrupt_counter++;
    } else {
        interrupt_counter = 0;
    
        led_clear();
        led_disp_num(count);
        count = (count + 1) % 16;
    
        printable_count = (unsigned int)count;
        binary_count = itob((long int)count, 4);
        sprintf(count_string, "%02u 0x%x 0b%04d\n\r", printable_count, 
                                               printable_count,
                                               binary_count);
        write_usb_serial_blocking(count_string, COUNT_STRING_LEN);
        
        if (count == 15) {
            SYSTICK_IntCmd(DISABLE);
            SYSTICK_Cmd(DISABLE);
            write_usb_serial_blocking("Finished count\n\r", 16);
        }

    }
}

long int itob(int x, size_t digits) {
    long int binary = 0;
    int i = 1;
    int ndigits = 0;
    
    while (ndigits <= digits && x>0) {
        binary += (x%2) * i;
        x /= 2;
        i *= 10;
        ndigits++;
    }
    
    return binary;
}

