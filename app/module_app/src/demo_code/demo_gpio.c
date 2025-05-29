#include "demo_gpio.h"

void demo_gpio_init(void)
{
    //eg:init GPIO_PIN_18
    //step 1: The GPIO port is used for GPIO port function
    uapi_io_set_func(EXT_GPIO_IDX_18,  0);
    //step 2: Set the GPIO port to input or output mode
    uapi_io_set_dir(EXT_GPIO_IDX_18, EXT_GPIO_DIRECTION_OUT);//Set the GPIO_18 to output mode
    //step 3: Initialize GPIO_18 output to high level
    uapi_io_set_ouput_val(EXT_GPIO_IDX_18, EXT_GPIO_VALUE1);
}

void demo_gpio_output(int port, int level)
{
    if(port != EXT_GPIO_IDX_18){
        return ;
    }
    printf("set gpio : %d , output level : %d\n", port, level);
    uapi_io_set_ouput_val(EXT_GPIO_IDX_18, level);
    return ;
}
