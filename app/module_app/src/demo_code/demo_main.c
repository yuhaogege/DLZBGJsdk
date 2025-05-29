#include "demo_main.h"

td_void micro_demo_task_body(td_u32 param)
{
	(td_void)param;
	
    for (;;)
    {
        demo_uart_recv_task();
        uapi_sleep(MICRO_PERI_MODULE_SLEEP_DURTION_MS/10);
        /*delay one second, You must add a delay to the task, otherwise the task will keep grabbing the CPU and cause other tasks to fail to run normally*/
    }
}

td_u32 micro_demo_task_init(td_void)
{
	static td_u32 task_id = 2;
	td_u32 ret;

	ret = uapi_task_create(&task_id, "MICRO_DEMO_TASK", micro_demo_task_body, NULL, MICRO_PERI_SRV_TASK_STACK_SIZE,MICRO_PERI_SRV_TASK_PRIORITY);
    if (ret != EXT_ERR_SUCCESS)
    {
        printf("[-IFO-] hi_task_create() MICRO_DEMO_TASK failed.\n");
        return EXT_ERR_FAILURE;
    }
	return EXT_ERR_SUCCESS;
}

void demo_app_main(void)
{
    //creat a task to running our application 
    printf("Starting demo application driver init and creat demo_app task! 11111111111\n");
    //driver init
    demo_gpio_init();
    //pwm init
    demo_pwm_init();
    //uart init
    demo_uart_init();
    //creat task to running our application
    micro_demo_task_init();
    //response ready
    demo_uart_tx_data("+READY\r\n", strlen("+READY\r\n"));
    return ;
}