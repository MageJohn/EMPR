#include <stdbool.h>

#define KEYPAD_ADDR 0x21


bool ioboard_keypad_get_key(uint8_t *pressed);
