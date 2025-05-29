#include "demo_pwm.h"

void demo_pwm_init(void)
{
    //eg: Use pwm0 as an example
    //pwm0 is obtained by multiplexing the GPIO_0 pin to pwm
    //steps 1: GPIO0 is multiplexed in PWM mode
    uapi_io_set_func( EXT_GPIO_IDX_0, 3);
    //steps 2: Setting the pwm clock source
    uapi_pwm_set_clock(PWM_CLK_XTAL);
    //steps 3: Initialize pwm0
    uapi_pwm_init(EXT_PWM_PORT_PWM0);
    //steps 4: stop the PWM output
    uapi_pwm_stop(EXT_PWM_PORT_PWM0);
    //steps 5: start the PWM output
    uapi_pwm_start(EXT_PWM_PORT_PWM0, 25000, 25000);
    
}


void demo_pwm_output(td_u32 index, td_u32 pwmduty)
{
    if(index != EXT_GPIO_IDX_0 || pwmduty > 100 ){
        return ;
    }
    printf("set pwm : %d, pwmduty : %d\n",index, pwmduty);
    uapi_pwm_stop(EXT_PWM_PORT_PWM0);
    //steps 5: start the PWM output
    uapi_pwm_start(EXT_PWM_PORT_PWM0, pwmduty*250, 25000);
}
