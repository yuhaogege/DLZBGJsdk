#include "demo_uart.h"
#include "demo_plc.h"
#include "securec.h"
//Experiment
#define number_of_variable  50
#define string_length_size  15
//td_char new_string[number_of_variable+1][string_length_size+1]={0};
//td_char gga_string[number_of_variable+1][string_length_size+1]={0};
td_char new_string[50][50]={0};
//td_char gga_string[50][50]={0};
td_char temporarry_buffer[512]={0};
 
//stop

#define PLC_BROADCAST_ADDR (char *)"\xff\xff\xff\xff\xff\xff"
ext_uart_port uart_port = EXT_UART_0;
ext_uart_init_type uart_cfg = { 115200, 8, 1, 2};
typedef struct{
    td_u32 plclen;
    td_char plcbuf[512];
}uart_to_plc_t;


void demo_uart_init(void)
{
    td_u32 ret;
    uapi_io_set_func(EXT_GPIO_IDX_9,  1);     /* GPIO_9_SEL:  UART0_RXD */	
    uapi_io_set_func(EXT_GPIO_IDX_10, 1);     /* GPIO_10_SEL: UART0_TXD */	

    uapi_uart_close(uart_port);
    ret = uapi_uart_open(uart_port);
    ret |= uapi_uart_ioctl(uart_port, UART_CFG_SET_ATTR, (uintptr_t)&uart_cfg);
    ret |= uapi_uart_ioctl(uart_port, UART_CFG_RD_BLOCK, (uintptr_t)EXT_UART_RD_NONBLOCK);
	ret |= uapi_uart_ioctl(uart_port, UART_CFG_WD_BLOCK, (uintptr_t)EXT_UART_WD_NONBLOCK);
    
    if (ret != EXT_ERR_SUCCESS) {
        printf("open uart %d failed.\n", uart_port);
        return ;
    }
    return ;
}

td_void parsing_data(td_char *source_string)
{	
    char *token = NULL;
    char* ptr = NULL;
    printf("user parse data : %s \n", source_string);
    td_u32 variable_index = 0;
    memset(new_string, 0, sizeof(new_string));
    token = strtok_s(source_string, ",", &ptr);//相较于strtok()函数，strtok_s函数需要用户传入一个指针，用于函数内部判断从哪里开始处理字符串
	while (token != NULL) {
        memcpy(new_string[variable_index], token, strlen(token));
        variable_index ++;
		token = strtok_s(NULL, ",", &ptr);//其他的使用与strtok()函数相同
	}
    for(td_u32 i = 0; i < variable_index; i++){
        printf(" %s \n",new_string[i]);// out put new-string, over uart1 see data
    }
    return ;
    
}

//Function interface for receiving data from uart0
td_void demo_uart_recv_task(td_void)
{
//Experiments
    td_s32 len = 0;
    td_u8 ch = 0;
    uart_to_plc_t uartplcbuf;
    uartplcbuf.plclen = 0;
    memset(uartplcbuf.plcbuf, 0, 512);
    for (;;)
    {
        len = uapi_uart_read(uart_port, &ch, 1);
        if (len == 1){ 
            uartplcbuf.plcbuf[uartplcbuf.plclen] = ch;
            uartplcbuf.plclen ++;
            continue;
        }
        //在这里进行发送数据
        if(uartplcbuf.plclen > 0){
            //发送数据
            //printf("plc send: %s\n", uartplcbuf.plcbuf);
 	    parsing_data(uartplcbuf.plcbuf);
            demo_plc_send( uartplcbuf.plcbuf, PLC_BROADCAST_ADDR);
        }
        //printf("\n");
		break;
    }
}
//Function interface for sending data from serial port 0
td_u32 demo_uart_tx_data(char *data, td_u16 data_length)
{
    td_u8 echoBuf[512] = { 0 };
    if (data == TD_NULL)
    {
        return EXT_ERR_INVALID_PARAMETER;
    }
    memcpy(echoBuf, data, data_length);
    if (uapi_uart_write(uart_port, echoBuf, strlen((char *)echoBuf)) != (td_s32)data_length)
    {
        printf("[-IFO-] app_uart_tx(): send failed.\n");
        return EXT_ERR_FAILURE;
    }
    return EXT_ERR_SUCCESS;
}

