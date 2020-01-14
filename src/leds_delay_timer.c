#include "lpc_types.h"
#include "leds.h"

int main(void) {
    uint8_t count = 0;
    uint32_t delay;
    led_setup();
    led_clear();
    while (1) {
        led_clear();
        count = (count + 1) % 16;
        led_disp_num(count);
        for (delay = 1; delay > 0; delay = (delay + 1) % (1<<23)) {}
    }
}
