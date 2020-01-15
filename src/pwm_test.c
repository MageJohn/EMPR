#include "lpc17xx_pwm.h"
#include "lpc17xx_pinsel.h"
#include "leds.h"
#include "mbed/wait.h"

int main(void) {
    uint8_t i;
    
    led_setup();
    led_clear();
    led_disp_num(1);
    
    PINSEL_CFG_Type pwm_enable = {
        .Funcnum = 1,
        .OpenDrain = PINSEL_PINMODE_NORMAL,
        .Pinmode = PINSEL_PINMODE_PULLUP,
        .Pinnum = 1,
        .Portnum = 2
    };
    
    PINSEL_ConfigPin(&pwm_enable);
    
    PWM_TIMERCFG_Type init_struct;
    PWM_ConfigStructInit(PWM_MODE_TIMER, (void *)&init_struct);
    PWM_Init(LPC_PWM1, PWM_MODE_TIMER, (void *)&init_struct);
    
    
    PWM_MATCHCFG_Type match0_cfg = {
        .IntOnMatch = DISABLE,
        .MatchChannel = 0,
        .ResetOnMatch = ENABLE, 
        .StopOnMatch = DISABLE 
    };
    
    PWM_MATCHCFG_Type match1_cfg = {
        .IntOnMatch = DISABLE,
        .MatchChannel = 1,
        .ResetOnMatch = DISABLE, 
        .StopOnMatch = DISABLE 
    };
    
    PWM_ConfigMatch(LPC_PWM1, &match0_cfg);
    PWM_ConfigMatch(LPC_PWM1, &match1_cfg); 
    
    PWM_MatchUpdate(LPC_PWM1, 0, 100, PWM_MATCH_UPDATE_NOW);
    PWM_MatchUpdate(LPC_PWM1, 1, 50, PWM_MATCH_UPDATE_NEXT_RST);
    
    PWM_ResetCounter(LPC_PWM1);
    PWM_CounterCmd(LPC_PWM1, ENABLE);
    PWM_Cmd(LPC_PWM1, ENABLE);
    
    //while (1) {
        //for (i = 0; i <= 100; i++) { 

            //wait(2);
        //}
    //}
}
  
