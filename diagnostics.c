#include <stdio.h>
#include <string.h>
#include "lpc17xx_clkpwr.h"
#include "serial.h"

#define BUFSIZE 20

int main(void) {
    char out[BUFSIZE];
    serial_init();
    SystemCoreClockUpdate();

    snprintf(out, BUFSIZE, "%u\n\r", (unsigned int)CLKPWR_GetPCLK(CLKPWR_PCLKSEL_RIT));
    serial_write_b(out, strlen(out));

    return 0;
}
