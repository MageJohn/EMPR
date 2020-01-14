#include "wait.h"
#include "leds.h"

int main(void) {
    led_setup();
    while (1) {
        led_disp_num(3);
        wait(1);
        led_clear();
        wait(1);
    }
}
