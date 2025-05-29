/*
 * (CompanyNameMagicTag) Copyright 2000
 * Wolfgang Denk, DENX Software Engineering, wd@denx.de.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */

/*
 *  Definitions for Command Processor
 */
#ifndef __COMMAND_H
#define __COMMAND_H

#ifndef NULL
#define NULL 0
#endif

#define CONFIG_CMD_MEMORY

#ifndef __ASSEMBLY__
/*
 * Monitor Command Table
 */

struct cmd_tbl_s {
    char *name;     /* Command Name */
    int maxargs;    /* Maximum number of parameters in a command. If no parameter is specified, 
                       set this parameter to 1. The rest may be deduced by analogy. */
    int repeatable; /* Indicates whether to support repeated command execution. This function is not supported */
    /* Implementation function */
    int (*cmd)(struct cmd_tbl_s *, int, int, const char *const[]); /* Processing function. The input parameters 
                                               include the command pointer, flag (meaningless), argc, and argv. */
    char *usage;                                                       /* Command Description */
#ifdef CFG_LONGHELP
    char *help; /* Help  message    (long) */
#endif
#ifdef CONFIG_AUTO_COMPLETE
    /* do auto completion on the arguments */
    int (*complete)(int argc, char *argv[], char last_char, int maxv, char *cmdv[]);
#endif
};

typedef struct cmd_tbl_s cmd_tbl_t;

extern cmd_tbl_t __u_boot_cmd_start;
extern cmd_tbl_t __u_boot_cmd_end;

/* common/command.c */
extern cmd_tbl_t *find_command(const char *cmd, cmd_tbl_t *start_cmd, cmd_tbl_t *end_cmd);
extern cmd_tbl_t *find_cmd(const char *cmd);

#define DBG_MSG(_fmt, arg...)
#define DBG_WARN(_fmt, arg...)
#define DBG_BUG(_fmt, arg...)

/*
 * Monitor Command
 *
 * All commands use a common argument format:
 *
 * void function (cmd_tbl_t *cmdtp, int flag, int argc, char *argv[]);
 */

typedef void command_t(cmd_tbl_t *, int, int, char *[]);

#if defined(CONFIG_CMD_MEMORY)        \
    || defined(CONFIG_CMD_I2C)        \
    || defined(CONFIG_CMD_ITEST)    \
    || defined(CONFIG_CMD_PCI)        \
    || defined(CONFIG_CMD_PORTIO)
#define CMD_DATA_SIZE
extern int cmd_get_data_size(const char *arg, int default_size);
#endif

#endif /* __ASSEMBLY__ */

/*
 * Command Flags:
 */
#define CMD_FLAG_REPEAT 0x0001 /* repeat last command */
#define DISP_LINE_LEN   16

////////////////////////
#define CFG_CBSIZE  128
#define CFG_MAXARGS 16 /* max number of command args */
#define puts        serial_puts
#define putc        serial_putc
#define getc        serial_getc
#define tstc        serial_tstc

int ctrlc(void);
int check_dbg(void);
extern int print_buffer(unsigned int addr, void *data, unsigned int width, unsigned int count,
                        unsigned int linelen);

int run_command_repeatable(const char *cmd, int flag);

int cmd_process(int flag, int argc, const char *const argv[],
                int *repeatable, unsigned long *ticks);

void print_cmd_result(const char *str, unsigned ret, unsigned char b_success_print);

#define Struct_Section __attribute__((unused, section(".u_boot_cmd")))

#ifdef CFG_LONGHELP

#define U_BOOT_CMD(name, maxargs, rep, cmd, usage, help) \
    cmd_tbl_t __u_boot_cmd_##name Struct_Section = { #name, maxargs, rep, cmd, usage, help }

#else /* no long help info */

#define U_BOOT_CMD(name, maxargs, rep, cmd, usage) \
    cmd_tbl_t __u_boot_cmd_##name Struct_Section = { #name, maxargs, rep, cmd, usage }

#endif /* CFG_LONGHELP */

#endif /* __COMMAND_H */
