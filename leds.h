#include "lpc_types.h"

#define LED_PORT 1
#define LEDS 1<<18|1<<20|1<<21|1<<23

void led_disp_num(uint8_t num);

void led_clear(void);

void led_setup(void);
