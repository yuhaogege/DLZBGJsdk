
#ifndef DEMO_PLC_H
#define DEMO_PLC_H

#include "demo_main.h"
#include "soc_types.h"
#include <string.h>
#include "app_proto_proc.h"
#include "stdio.h"
#include "soc_mdm_mem.h"
#include "soc_config.h"
#include <stdlib.h>

struct atCmd {
	td_char *name;
	td_u32 (*function)(char *payload);
};

void demo_plc_recv(td_s8 *buf, td_u32 len);

td_u32  demo_atCmdFunction_GPIO(char *payload);
td_u32  demo_atCmdFunction_PWM(char *payload);
td_u32  demo_atCmdFunction_UART(char *payload);
void demo_plc_send(char* sendplcbuf, char* mac);

#endif

