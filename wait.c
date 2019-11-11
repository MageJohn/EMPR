#include "lpc17xx_rit.h"
#include "lpc_types.h"

void wait_ms(int time) {
    RIT_Init(LPC_RIT);

    RIT_TimerConfig(LPC_RIT, time);

    RIT_Cmd(LPC_RIT, ENABLE);

    while (RIT_GetIntStatus(LPC_RIT) != SET) {
        continue;
    }
}
