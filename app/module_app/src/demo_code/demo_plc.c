#include "demo_plc.h"

static struct atCmd atcmdList[] = {
	{ "+GPIO", demo_atCmdFunction_GPIO },
	{ "+PWM", demo_atCmdFunction_PWM },
    { "+UART", demo_atCmdFunction_UART},

};

char plcbuf[512] = {0};
void demo_plc_recv(td_s8 *buf, td_u32 len)
{
    memset(plcbuf, 0, 512);
    memcpy(plcbuf, buf, len);
    printf("plcbuf : %s\n", plcbuf);
    //data handle 
    for(td_u32 i = 0; i < ext_array_count(atcmdList); i ++){
        if (strncmp(plcbuf, atcmdList[i].name, strlen(atcmdList[i].name)) == 0){
            atcmdList[i].function(plcbuf);
            break;
        }
    }
    return ;
}

td_u32  demo_atCmdFunction_GPIO(char *payload)
{
    char gpio[6] = {0};
    char *cp = NULL, *cp1 = NULL;
    cp = strchr(payload, ':');
    if(cp == NULL) return 0;
    cp1 = strchr(cp+1, ',');
    if(cp1 == NULL) return 0;
    memcpy(gpio, cp+1, cp1 - cp -1);
    demo_gpio_output( atoi(gpio), atoi(cp1+1));
    return 1;
}

td_u32  demo_atCmdFunction_PWM(char *payload)
{
    char pwmvalue[6] = {0};
    char *cp = NULL, *cp1 = NULL;
    cp = strchr(payload, ':');
    if(cp == NULL) return 0;
    cp1 = strchr(cp+1, ',');
    if(cp1 == NULL) return 0;
    memcpy(pwmvalue, cp+1, cp1 - cp -1);
    demo_pwm_output( atoi(pwmvalue), atoi(cp1+1));
    return 1;
}
td_u32  demo_atCmdFunction_UART(char *payload)
{
    char *cp = NULL, *cp1 = NULL, *cp2 = NULL;
    cp = strchr(payload, ':');
    if(cp == NULL) return 0;
    cp1 = strchr(cp+1, '"');
    if(cp1 == NULL) return 0;
    cp2 = strchr(cp1+1, '"');
    demo_uart_tx_data(cp1+1, cp2 - cp1 -1);
    return 1;
}

//Function interface for sending data to PLC
void demo_plc_send(char* sendplcbuf, char* mac)
{
    app_proto_plc_tx((td_u8 *)sendplcbuf, strlen(sendplcbuf), (td_u8 *)mac, 0x52);
    return ;
}
