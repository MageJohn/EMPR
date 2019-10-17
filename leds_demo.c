#include "leds.h"

int main(void) {
    led_setup();
    led_clear();
    led_disp_num(3);
    
    return 0;
}
