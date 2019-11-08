#include <stdio.h>
#include <stdbool.h>

#include "lpc17xx_i2c.h"

#include "serial.h"
#include "ioboard_i2c.h"
#include "ioboard_lcd.h"
#include "ioboard_keypad.h"

#define case_any_digit case 0: case 1: case 2: case 3: case 4: case 5: case 6 :case 7: case 8: case 9:
#define case_any_op case 0x0a: case 0x0c: case 0x0d: case 0x0e:

void fail(void);
void success(int8_t left_sign, uint8_t left, int8_t right_sign, int8_t right, uint8_t op);


uint8_t scancode_math_lut[16] = {
    0x0d, 0x0c, 0x0b, 0x0a,
    0x0f, 0x09, 0x06, 0x03,
    0x00, 0x08, 0x05, 0x02,
    0x0e, 0x07, 0x04, 0x01
};

uint8_t scancode_lcd_lut[16] = {
    0xaf, 0xad, 0x0b, 0xab,
    0xbd, 0xb9, 0xb6, 0xb3,
    0xb0, 0xb8, 0xb5, 0xb2,
    0xaa, 0xb7, 0xb4, 0xb1
};

int main(void){
    bool key_pressed;
    uint8_t scancode;
    uint8_t lcd_bytes[] = {Control_byte(0, 0), Set_DDRAM(0x00)};
    uint8_t state = 0;
    int8_t left;
    int8_t right;
    int8_t left_sign = 1;
    int8_t right_sign = 1;
    uint8_t op;

    serial_init();
    ioboard_i2c_init();
    ioboard_lcd_init();
    ioboard_lcd_clear_display();
    ioboard_lcd_send_bytes(lcd_bytes, 2);

    lcd_bytes[0] = Control_byte(0, 1);

    write_usb_serial_blocking("start\n\r", 7);

    while (1) {
        key_pressed = ioboard_keypad_get_key(&scancode);
        if (key_pressed) {
            lcd_bytes[1] = scancode_lcd_lut[scancode];
            ioboard_lcd_send_bytes(lcd_bytes, 2);
            scancode = scancode_math_lut[scancode];

            switch (state) {
                case 0:
                    switch (scancode) {
                        case 0x0c:
                            left_sign = -1;
                            state = 1;
                            break;
                        case_any_digit
                            left_sign = 1;
                            left = scancode;
                            state = 2;
                            break;
                        default:
                            fail();
                            state = 0;
                    }
                    break;
                case 1:
                    switch (scancode) {
                        case_any_digit
                            left = scancode;
                            state = 2;
                            break;
                        default:
                            fail();
                            state = 0;
                    }
                    break;
                case 2:
                    switch (scancode) {
                        case_any_digit
                            left *= 10;
                            left += scancode;
                            break;
                        case_any_op
                            op = scancode;
                            state = 3;
                            break;
                        default:
                            fail();
                            state = 0;
                    }
                    break;
                case 3:
                    switch (scancode) {
                        case 0x0c:
                            right_sign = -1;
                            state = 4;
                            break;
                        case_any_digit
                            right_sign = 1;
                            right = scancode;
                            state = 5;
                            break;
                        default:
                            fail();
                            state = 0;
                    }
                    break;
                case 4:
                    switch (scancode) {
                        case_any_digit
                            right = scancode;
                            state = 5;
                            break;
                        default:
                            fail();
                            state = 0;
                    }
                    break;
                case 5:
                    switch (scancode) {
                        case_any_digit
                            right *= 10;
                            right += scancode;
                            break;
                        case 0x0f:
                            success(left_sign, left, right_sign, right, op);
                            state = 0;
                            break;
                        default:
                            fail();
                            state = 0;
                    }
                    break;

            }
        }
    }

    return 0;
}

void fail(void) {
    bool key_pressed;
    uint8_t scancode;

    ioboard_lcd_write_ascii("error", 0x00);
    ioboard_lcd_write_ascii("press any key", 0x40);
    while (1) {
        key_pressed = ioboard_keypad_get_key(&scancode);
        if (key_pressed) {
            ioboard_lcd_clear_display();
            return;
        }
    }
}

void success(int8_t left_sign, uint8_t left, int8_t right_sign, int8_t right, uint8_t op) {
    bool key_pressed;
    uint8_t scancode;
    char str_result[5];
    int8_t result;

    left *= left_sign;
    right *= right_sign;

    switch (op) {
        case 0x0a:
            result = left + right;
            break;
        case 0x0c:
            result = left - right;
            break;
        case 0x0d:
            result = left / right;
            break;
        case 0x0e:
            result = left * right;
            break;
    }

    snprintf(str_result, 5, "%d", result);
    ioboard_lcd_write_ascii(str_result, 0x40);

    while (1) {
        key_pressed = ioboard_keypad_get_key(&scancode);
        if (key_pressed) {
            ioboard_lcd_clear_display();
            return;
        }
    }
}
