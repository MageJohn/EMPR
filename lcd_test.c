#include "lpc17xx_i2c.h"

#include "utils.h"
#include "serial.h"
#include "ioboard_lcd.h"
#include "ioboard_i2c.h"


int main(void) {
    serial_init();
    write_usb_serial_blocking("start\n\r", 7);

    ioboard_i2c_init();

    ioboard_lcd_init();

    ioboard_lcd_clear_display();

    ioboard_lcd_write_ascii("hello", 0x00);
    ioboard_lcd_write_ascii("world", 0x40);

    return 0;
}

