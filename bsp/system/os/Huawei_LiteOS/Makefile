
include $(MAIN_TOPDIR)/build_tmp/config/soc_product_config.mk

HB_USE_REMAP := YES

HB_LITEOS_TESTDRIVER = NO
HB_LITEOS_COMPILE_TESTCASE = NO

UNWIND_BACKTRACE = n

ifeq ($(HB_DEBUG_COMPILE_ENABLE),y)
DEBUG             = y
endif

OBJ_MKDIR         = if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi

ifeq ($(HB_USE_REMAP), YES)
TARGET_LIB        := liblitekernel.o
TARGET_LIBSEC     := libsec.o
LIBC        := libc.o
else
TARGET_LIB        := liblitekernel.a
TARGET_LIBSEC     := libsec.a
LIBC        := libc.a
endif
TEST_LIB := libtest.a
TARGET_RAM_LIB := liblitekernelram.o

OBJCPFLAGS_ELF_TO_BIN   = -Obinary
OBJCPFLAGS_ELF_TO_HEX   = -Oihex
OBJCPFLAGS_BIN_TO_HEX   = -Ibinary -Oihex
OBJCPFLAGS_ELF_TO_SREC  = -Osrec
OBJCPFLAGS_ELF_TO_LIST  = -S

# PROJECTBASE = $(PWD)/Huawei_LiteOS
# override PROJECTBASE    := $(abspath $(PROJECTBASE))
# relative path
PROJECTBASE := .
OS_ROM_DIR := $(PROJECTBASE)/../../../../rom/bsp/system/os/Huawei_LiteOS
OS_RAM_DIR := $(PROJECTBASE)
OUT = out

CFLAGS = -mcpu=cortex-m3 -mthumb -fno-builtin -fdata-sections -ffunction-sections -fstack-protector -fno-short-enums -std=c99 -fno-inline -falign-functions=4 -nostdinc -DLIB_CONFIGURABLE

ASFLAG = -mcpu=cortex-m3 -mthumb -nostdinc -fstack-protector -fno-common

ifeq ($(HB_USE_REMAP), YES)
	ASFLAG += -D LIB_CONFIGURABLE
endif

LIBC_ASFLAG = -mcpu=cortex-m3 -mthumb


#-nostdlib

ifeq ($(DEBUG), y)
    CFLAGS += -O0 -g
    ASFLAG += -O0 -g
else
    CFLAGS += -O2
    ASFLAG += -O2
endif

ifeq ($(HB_LITEOS_COMPILE_TESTCASE), YES)
    CFLAGS += -D DW21TEST
else

    CFLAGS += -Werror
    ASFLAG += -Werror
endif

ifeq ($(UNWIND_BACKTRACE), y)
    CFLAGS += -D LOSCFG_UNWIND_BACKTRACE -funwind-tables -mpoke-function-name
endif

ARFLAGS = cr

CFLAGS +=  -I $(OS_ROM_DIR)/kernel/include  \
    -I $(OS_ROM_DIR)/kernel/base/include  \
    -I $(OS_ROM_DIR)/kernel/extended/include  \
    -I $(OS_ROM_DIR)/arch/arm/common/cmsis \
    -I $(OS_ROM_DIR)/arch/arm/arm-m/include \
    -I $(OS_ROM_DIR)/arch/arm/arm-m/cortex-m3 \
    -I $(OS_ROM_DIR)/components/lib/libc/include  \
    -I $(OS_ROM_DIR)/components/lib/libc/libwc/include  \
    -I $(OS_ROM_DIR)/components/lib/libm/include \
    -I $(OS_ROM_DIR)/components/posix/include \
    -I $(OS_ROM_DIR)/targets/dw21v100/config \
    -I $(OS_ROM_DIR)/targets/dw21v100/commons \
    -I $(OS_ROM_DIR)/targets/dw21v100/extend/include \
    -I $(OS_ROM_DIR)/targets/dw21v100/user \
    -I $(OS_RAM_DIR)/targets/dw21v100/config \
    -I $(OS_RAM_DIR)/kernel/include  \
    -I $(OS_RAM_DIR)/../../../system/inc \
	-I $(OS_RAM_DIR)/../../../drivers/uart 
##########################################################################  make libc.a  #####################################################################################################################################

LIBC_SRC = ${wildcard $(OS_ROM_DIR)/components/lib/libc/src/*.c} \
    ${wildcard $(OS_ROM_DIR)/components/lib/libc/src/stdio/*.c} \
    ${wildcard $(OS_ROM_DIR)/components/lib/libc/src/string/*.c} \
    ${wildcard $(OS_ROM_DIR)/components/lib/libc/src/stdlib/*.c} \
    ${wildcard $(OS_ROM_DIR)/components/lib/libc/src/time/*.c} \
    ${wildcard $(OS_ROM_DIR)/components/lib/libc/libwc/*.c} \
    ${wildcard $(OS_ROM_DIR)/components/lib/libm/src/*.c}

LIBC_CFLAGS := $(CFLAGS)
LIBC_CFLAGS += -I $(OS_ROM_DIR)/components/lib/libc/include  \
    -I $(OS_ROM_DIR)/components/lib/libc/libwc/include  \
    -I $(OS_ROM_DIR)/components/lib/libm/include

LIBC_OBJ = $(patsubst $(OS_ROM_DIR)/%.c, $(OUT)/%.o, $(LIBC_SRC))

$(LIBC): $(LIBC_OBJ)
	@echo "+++++++++++++++++++++++++++++++++++++++++++++start compile LIBC+++++++++++++++++++++++++++++++++++++++++++++" $(LIBC)
ifeq ($(HB_USE_REMAP), YES)
	$(Q)$(LD) -r -o $@ $(sort $(LIBC_OBJ))
else
	$(Q)$(AR) $(ARFLAGS) $@ $(LIBC_OBJ)
endif
	mv $(LIBC) $(OUT)/

$(LIBC_OBJ) : $(OUT)/%.o : $(OS_ROM_DIR)/%.c
	$(Q)$(OBJ_MKDIR)
	$(Q) echo "Compile $<"
	$(Q)$(CC) $(LIBC_CFLAGS) -c $< -o $@

##########################################################################  make libsec.a  ######################################################################################################################################

LIBSEC_SRC = ${wildcard $(OS_ROM_DIR)/components/lib/libsec/src/*.c}

LIBSEC_CFLAGS := $(CFLAGS)
LIBSEC_CFLAGS += -I $(OS_ROM_DIR)/components/lib/libsec/include

LIBSEC_OBJ = $(patsubst $(OS_ROM_DIR)/%.c, $(OUT)/%.o, $(LIBSEC_SRC))

$(TARGET_LIBSEC): $(LIBSEC_OBJ)
	@echo "+++++++++++++++++++++++++++++++++++++++++++++start compile TARGET_LIBSEC+++++++++++++++++++++++++++++++++++++++++++++" $(TARGET_LIBSEC)
ifeq ($(HB_USE_REMAP), YES)
	$(Q)$(LD) -r -o $@ $(sort $(LIBSEC_OBJ))
else
	$(Q)$(AR) $(ARFLAGS) $@ $(LIBSEC_OBJ)
endif
	$(Q)mv $(TARGET_LIBSEC) $(OUT)/

$(LIBSEC_OBJ) : $(OUT)/%.o : $(OS_ROM_DIR)/%.c
	$(Q)$(OBJ_MKDIR)
	$(Q) echo "Compile $<"
	$(Q)$(CC) $(LIBSEC_CFLAGS) -c $< -o $@
##########################################################################  make liblitekernel.a  ###############################################################################################################################
ifeq ($(HB_PROJECT_NAME_LIST), MPW)
HB_USE_REMAP := NO
CFLAGS += -DMPW
endif

ifeq ($(HB_USE_REMAP), YES)
CFLAGS += -DUSE_REMAP -DREMAP_WITH_LRU
ASFLAG += -DUSE_REMAP
endif

SRC = ${wildcard $(OS_ROM_DIR)/components/posix/src/time.c}  \
    ${wildcard $(OS_ROM_DIR)/components/linux/src/workqueue.c}  \
    ${wildcard $(OS_ROM_DIR)/kernel/base/core/*.c} \
    ${wildcard $(OS_ROM_DIR)/kernel/base/ipc/*.c} \
    ${wildcard $(OS_ROM_DIR)/kernel/*.c} \
    ${wildcard $(OS_ROM_DIR)/kernel/base/mem/bestfit/*.c} \
    ${wildcard $(OS_ROM_DIR)/kernel/base/misc/*.c} \
    ${wildcard $(OS_ROM_DIR)/kernel/base/om/*.c} \
    ${wildcard $(OS_ROM_DIR)/kernel/extended/cpup/*.c} \
    ${wildcard $(OS_ROM_DIR)/targets/dw21v100/*.c} \
    ${wildcard $(OS_ROM_DIR)/targets/dw21v100/commons/*.c} \
    ${wildcard $(OS_ROM_DIR)/targets/dw21v100/config/*.c} \
    ${wildcard $(OS_ROM_DIR)/arch/arm/arm-m/src/*.c} \
    ${wildcard $(OS_ROM_DIR)/arch/arm/arm-m/cortex-m3/*.c} \
    ${wildcard $(OS_ROM_DIR)/targets/dw21v100/extend/src/*.c}

ASM_SRC = ${wildcard $(OS_ROM_DIR)/arch/arm/arm-m/cortex-m3/los_dispatch.S} 


RAM_SRC = ${wildcard $(OS_RAM_DIR)/kernel/redirect/*.c} \
    ${wildcard $(OS_RAM_DIR)/targets/dw21v100/user/*.c}

RAM_ASM_SRC = ${wildcard $(OS_RAM_DIR)/targets/dw21v100/los_startup.S}


ifeq ($(HB_LITEOS_COMPILE_TESTCASE), YES)
TEST_SRC = ${wildcard $(OS_RAM_DIR)/test/src/*.c}
#    ${wildcard $(OS_RAM_DIR)/test/sample/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/event/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/hwi/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/mux/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/queue/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/sem/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/swtmr/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/task/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/exc/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/mem/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/cpup/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/fpb/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/track/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/time/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/timeslice/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/kernel/workqueue/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/libc/ctype/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/libc/stdio/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/libc/stdlib/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/libc/string/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/libc/time/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/libm/ieee754/*.c} 
endif

RELATIVE_DIR = ../../../../
CFLAGS += $(addprefix -I $(RELATIVE_DIR),$(shell cat $(MAIN_TOPDIR)/Makefile.inc))

		

ifeq ($(HB_LITEOS_COMPILE_TESTCASE), YES)
    TEST_CFLAGS := $(CFLAGS)
    TEST_CFLAGS += -I $(OS_RAM_DIR)/test/include \
        -I $(OS_RAM_DIR)/test/sample/kernel/event \
        -I $(OS_RAM_DIR)/test/sample/kernel/hwi \
        -I $(OS_RAM_DIR)/test/sample/kernel/mux \
        -I $(OS_RAM_DIR)/test/sample/kernel/queue \
        -I $(OS_RAM_DIR)/test/sample/kernel/sem \
        -I $(OS_RAM_DIR)/test/sample/kernel/swtmr \
        -I $(OS_RAM_DIR)/test/sample/kernel/task \
        -I $(OS_RAM_DIR)/test/sample/kernel/mem \
        -I $(OS_RAM_DIR)/test/sample/kernel/exc \
        -I $(OS_RAM_DIR)/test/sample/kernel/cpup \
        -I $(OS_RAM_DIR)/test/sample/kernel/fpb \
        -I $(OS_RAM_DIR)/test/sample/kernel/track \
        -I $(OS_RAM_DIR)/test/sample/kernel/time \
        -I $(OS_RAM_DIR)/test/sample/kernel/timeslice \
        -I $(OS_RAM_DIR)/test/sample/kernel/workqueue \
        -I $(OS_RAM_DIR)/test/sample/cmsis/event \
        -I $(OS_RAM_DIR)/test/sample/cmsis/hwi \
        -I $(OS_RAM_DIR)/test/sample/cmsis/mux \
        -I $(OS_RAM_DIR)/test/sample/cmsis/msg \
        -I $(OS_RAM_DIR)/test/sample/cmsis/sem \
        -I $(OS_RAM_DIR)/test/sample/cmsis/swtmr \
        -I $(OS_RAM_DIR)/test/sample/cmsis/task \
        -I $(OS_RAM_DIR)/test/sample/libc/ctype \
        -I $(OS_RAM_DIR)/test/sample/libc/stdlib \
        -I $(OS_RAM_DIR)/test/sample/libc/stdio \
        -I $(OS_RAM_DIR)/test/sample/libc/string \
        -I $(OS_RAM_DIR)/test/sample/libm/ieee754 \
        -I $(OS_RAM_DIR)/test/sample/libm/portable-api \
        -I $(OS_RAM_DIR)/../../../rom/bsp/drivers_in_rom/chip/gpio \
        -I $(OS_RAM_DIR)/../../../rom/bsp/drivers_in_rom/chip/hispiflash \
        -I $(OS_RAM_DIR)/../../../rom/bsp/drivers_in_rom/chip/hrtimer \
        -I $(OS_RAM_DIR)/../../../rom/bsp/drivers_in_rom/chip/led \
        -I $(OS_RAM_DIR)/../../../rom/bsp/drivers_in_rom/chip/time_cnt \
        -I $(OS_RAM_DIR)/../../../rom/bsp/drivers_in_rom/chip/watchdog \
        -I $(OS_RAM_DIR)/../../../cmn/upg \
        -I $(OS_RAM_DIR)/test/sample/libm/math
endif

ifeq ($(HB_LITEOS_COMPILE_TESTCASE), YES)
ifeq ($(HB_LITEOS_TESTDRIVER), YES)
TEST_SRC += ${wildcard $(OS_RAM_DIR)/test/sample/drivers/test_gpio/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/drivers/test_flash/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/drivers/test_led/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/drivers/test_time_cnt/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/drivers/test_uart/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/drivers/test_watchdog/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/drivers/test_clock/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/drivers/test_hrtimer/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/drivers/secure/*.c}\
#    ${wildcard $(OS_RAM_DIR)/test/sample/drivers/test_spi/*.c} \
    ${wildcard $(OS_RAM_DIR)/test/sample/drivers/test_i2c/*.c}


TEST_CFLAGS += -I $(OS_RAM_DIR)/test/sample/drivers/test_common

endif
endif
CFLAGS += -fno-common

OBJ = $(patsubst $(OS_ROM_DIR)/%.c, $(OUT)/%.o, $(SRC))
ASMOBJ = $(patsubst $(OS_ROM_DIR)/%.S, $(OUT)/%.o, $(ASM_SRC))

RAM_OBJ = $(patsubst $(OS_RAM_DIR)/%.c, $(OUT)/%.o, $(RAM_SRC))
RAM_ASMOBJ = $(patsubst $(OS_RAM_DIR)/%.S, $(OUT)/%.o, $(RAM_ASM_SRC))
TEST_OBJ = $(patsubst $(OS_RAM_DIR)/%.c, $(OUT)/%.o, $(TEST_SRC))

$(TARGET_LIB): $(TARGET_RAM_LIB) $(OBJ) $(ASMOBJ) $(TEST_OBJ)
	@echo "+++++++++++++++++++++++++++++++++++++++++++++start compile TARGET_LIB+++++++++++++++++++++++++++++++++++++++++++++" $(TARGET_LIB)
ifeq ($(HB_USE_REMAP), YES)
	$(Q)$(LD) -r -o $@ $(sort $(OBJ)) $(sort $(ASMOBJ))
else
	$(Q)$(AR) $(ARFLAGS) $@ $(OBJ) $(ASMOBJ)
endif
	$(Q)mv $(TARGET_LIB) $(OUT)/

$(TARGET_RAM_LIB): $(RAM_OBJ) $(RAM_ASMOBJ) $(TEST_OBJ)
	mkdir -p $(OUT)/arch/arm/arm-m/cortex-m3
	$(Q)if [ -f $(OS_RAM_DIR)/arch/arm/arm-m/cortex-m3/los_hw_exc.S ]; \
	then \
	$(CC) $(ASFLAG) -c $(OS_RAM_DIR)/arch/arm/arm-m/cortex-m3/los_hw_exc.S -o $(OUT)/arch/arm/arm-m/cortex-m3/los_hw_exc.o;\
	cp -a $(OUT)/arch/arm/arm-m/cortex-m3/los_hw_exc.o $(MAIN_TOPDIR)/target/libs/liteos;\
	else \
	cp $(MAIN_TOPDIR)/target/libs/liteos/los_hw_exc.o $(OUT)/arch/arm/arm-m/cortex-m3;\
	fi 
	$(Q)if [ -f $(OS_RAM_DIR)/kernel/redirect/los_exc_redirect.c ]; \
	then \
	$(CC) $(CFLAGS) -c $(OS_RAM_DIR)/kernel/redirect/los_exc_redirect.c -o $(OUT)/kernel/redirect/los_exc_redirect.o;\
	cp -a $(OUT)/kernel/redirect/los_exc_redirect.o $(MAIN_TOPDIR)/target/libs/liteos;\
	else \
	cp $(MAIN_TOPDIR)/target/libs/liteos/los_exc_redirect.o $(OUT)/kernel/redirect/;\
	fi 
	
	$(Q)$(LD) -r -o $@ $(sort $(RAM_OBJ) $(OUT)/kernel/redirect/los_exc_redirect.o ) $(sort $(RAM_ASMOBJ) $(OUT)/arch/arm/arm-m/cortex-m3/los_hw_exc.o)
	$(Q)mv $(TARGET_RAM_LIB) $(OUT)/
ifeq ($(HB_LITEOS_COMPILE_TESTCASE), YES)
	$(AR) $(ARFLAGS) $(TEST_LIB) $(TEST_OBJ)
	$(Q)mv $(TEST_LIB) $(OUT)/
endif

$(OBJ) : $(OUT)/%.o : $(OS_ROM_DIR)/%.c
	$(Q)$(OBJ_MKDIR)
	$(Q) echo "Compile $<"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

$(ASMOBJ) : $(OUT)/%.o : $(OS_ROM_DIR)/%.S
	$(Q)$(OBJ_MKDIR)
	$(Q) echo "Compile $<"
	$(Q)$(CC) $(ASFLAG) -c $< -o $@

$(RAM_OBJ) : $(OUT)/%.o : $(OS_RAM_DIR)/%.c
	$(Q)$(OBJ_MKDIR)
	$(Q) echo "Compile $<"
	$(Q)$(CC) $(CFLAGS) -Wall -c $< -o $@

$(RAM_ASMOBJ) : $(OUT)/%.o : $(OS_RAM_DIR)/%.S
	$(Q)$(OBJ_MKDIR)
	$(Q) echo "Compile $<"
	$(Q)$(CC) $(ASFLAG) -Wall -c $< -o $@

$(TEST_OBJ) : $(OUT)/%.o : $(OS_RAM_DIR)/%.c
	$(Q)$(OBJ_MKDIR)
	$(Q) echo "Compile $<"
	$(Q)$(CC) $(TEST_CFLAGS) -c $< -o $@
##########################################################################  execution  ###############################################################################################################################
.PHONY: clean lib

ifeq ($(HB_LITEOS_COMPILE_ROM), YES)
lib: $(LIBC) $(TARGET_LIB) $(TARGET_LIBSEC)
else
lib: $(TARGET_RAM_LIB)
endif

clean:
	$(Q)rm -rf $(OUT)
	$(Q)rm -rf *.a

