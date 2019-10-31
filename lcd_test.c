#include "lpc17xx_i2c.h"

#include "utils.h"
#include "serial.h"
#include "ioboard_lcd.h"
#include "ioboard_i2c.h"


int main(void) {
    I2C_M_SETUP_Type lcd_write;
    uint8_t instructions[] = {
        Control_byte(0, 0),
        Set_DDRAM(0x00)
        };
    uint8_t string[] = (uint8_t)"hello\X91world"

    serial_init();
    write_usb_serial_blocking("start\n\r", 7);

    ioboard_i2c_init();

    ioboard_lcd_init();

    ioboard_lcd_clear_display();

    ioboard_lcd_send_bytes(instructions, LEN(instructions));
    ioboard_lcd_send_bytes(string, LEN(string));

    return 0;
}

