#include <stdio.h>
#include <stdbool.h>

#include "lpc17xx_i2c.h"

#include "ioboard_i2c.h"
#include "ioboard_lcd.h"
#include "ioboard_keypad.h"
#include "serial.h"

uint8_t scancode_lut[16] = {
    0xc4, 0xc3, 0xc2, 0xc1,
    0xa3, 0xb9, 0xb6, 0xb3,
    0xb0, 0xb8, 0xb5, 0xb2,
    0xaa, 0xb7, 0xb4, 0xb1
};


int main(void){
    bool key_pressed;
    uint8_t scancode;
    uint8_t lcd_bytes[] = {Control_byte(0, 0), Set_DDRAM(0x00)};

    ioboard_i2c_init();
    ioboard_lcd_init();
    ioboard_lcd_clear_display();
    ioboard_lcd_send_bytes(lcd_bytes, 2);

    serial_init();

    lcd_bytes[0] = Control_byte(0, 1);

    while (1) {
        key_pressed = ioboard_keypad_rl_get_key(&scancode, 255);
        if (key_pressed) {
            write_usb_serial_blocking("pressed\n\r", 9);
            lcd_bytes[1] = scancode_lut[scancode];
            ioboard_lcd_send_bytes(lcd_bytes, 2);
        }
    }

    return 0;
}

