#include "serial.h"

int main(void) {
    serial_init();
    write_usb_serial_blocking("Hello World!\n\r",16);
    
    return 0;
}
