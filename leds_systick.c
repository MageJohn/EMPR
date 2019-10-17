#include "lpc_types.h"
#include "lpc17xx_systick.h"
#include "lpc17xx_clkpwr.h"
#include "leds.h"

int main(void) {
    led_setup();
    SYSTICK_InternalInit(167);
    SYSTICK_IntCmd(ENABLE);
    SYSTICK_Cmd(ENABLE);
    
    return 0;
}

void SysTick_Handler(void) {
    static uint16_t count = 0;
    led_clear();
    led_disp_num(count>>2);
    count = (count + 1) % 64;
}
