#include "utils.h"
#include "serial.h"
#include "ioboard/lcd.h"
#include "ioboard/i2c.h"


int main(void) {
    serial_init();
    serial_write_b("start\n\r", 7);

    ioboard_i2c_init();

    ioboard_lcd_init();

    ioboard_lcd_clear_display();

    ioboard_lcd_write_ascii("hello", 0x00);
    ioboard_lcd_write_ascii("world", 0x40);

    return 0;
}

