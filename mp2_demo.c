#define MP1

#include "i2c_scanner.h"
#include "wait.h"
#include "ioboard_lcd.h"

int main(void) {
    // setup functions
    ioboard_lcd_init();

    // start sequence
    i2c_scan();
    wait_ms(1000);
    ioboard_lcd_write_ascii("hello world", 0x00);

    return 0;
}
