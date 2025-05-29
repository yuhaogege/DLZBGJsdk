
ifeq ($(PRODUCT_CFG_BUILD_RELEASE), yes)
MAIN_OUT_DIR_NAME = $(project_name)_release
else
MAIN_OUT_DIR_NAME = $(project_name)_debug
endif
MAIN_BUILD_TMP_OUT := $(RELATIVE_DIR)/build_tmp/$(MAIN_OUT_DIR_NAME)
MAIN_BUILD_OUTPUT := $(RELATIVE_DIR)/output/$(MAIN_OUT_DIR_NAME)

TARGET := $(MAIN_BUILD_TMP_OUT)/$(DEV_NAME)/bin

empty :=
comma :=/
app_dir := $(subst $(comma), $(empty), $(MOD_PATH))


ifeq ($(word 1,$(app_dir)),app)
LIBS := $(MAIN_BUILD_TMP_OUT)/$(DEV_NAME)/lib/app
CFLAGS += -DSOC_$(APP_NAME)_CONFIG
else
LIBS := $(MAIN_BUILD_TMP_OUT)/$(DEV_NAME)/lib
endif
BUILD := $(MAIN_BUILD_TMP_OUT)/$(DEV_NAME)/build

LIBOUT = $(BUILD)/$(MOD_PATH)

MOD_INCLUDE := $(addprefix -I $(RELATIVE_DIR),$(shell cat Makefile.inc))
MOD_INCLUDE += $(addprefix -I $(RELATIVE_DIR),$(shell cat $(RELATIVE_DIR)/Makefile.inc))

ifeq ($(IS_DYN),yes)
LIB = $(LIBS)/$(LIB_NAME)
LIB_COMPARE_NAME = $(LIB)
else
LIB_PERMANENT = $(LIBS)/$(LIB_NAME)
LIB_COMPARE_NAME = $(LIB_PERMANENT)
endif


ifeq ($(DEV_NAME),cco)
CFLAGS += -DHB_IMAGE_CCO
endif
ifeq ($(DEV_NAME),sta)
CFLAGS += -DHB_IMAGE_STA
endif
ifeq ($(DEV_NAME),ndm)
CFLAGS += -DHB_IMAGE_NDM
endif

SRC_DIRS := $(MOD_NAME)
ifeq ($(MOD_NAME)/Makefile.src, $(wildcard $(MOD_NAME)/Makefile.src))
SRC_DIRS += $(shell cat $(MOD_NAME)/Makefile.src)
endif

SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.c))
OBJS = $(patsubst %.c,$(LIBOUT)/%.o,$(SRCS))

ASM_SRCS := $(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.S ))
ASM_OBJS = $(patsubst %.S,$(LIBOUT)/%.o,$(ASM_SRCS))

all: $(LIB) $(LIB_PERMANENT)
$(OBJS): $(LIBOUT)/%.o : %.c
	$(Q)echo Compile  $<;
	$(Q)if [ ! -d $(dir $@) ]; \
	then \
	mkdir -p $(dir $@);\
	fi;
	$(Q)if [ -f $< ]; \
	then \
	$(CC) $(CFLAGS) $(MOD_INCLUDE) -c $< -o $@ ; \
	if [ "$$?" != "0" ]; then \
	exit 1;\
	fi;\
	fi;

$(ASM_OBJS): $(LIBOUT)/%.o : %.S
	$(Q)echo Compile  $<;
	$(Q)if [ ! -d $(dir $@) ]; \
	then \
	mkdir -p $(dir $@);\
	fi;
	$(Q)if [ -f $< ]; \
	then \
	$(CC) $(ASFLAGS)  -c $< -o $@ ; \
	if [ "$$?" != "0" ]; then \
	exit 1;\
	fi;\
	fi;

$(LIB_PERMANENT): $(OBJS) $(ASM_OBJS)
	$(Q)if [ ! -d $(dir $@) ]; \
	then \
	mkdir -p $(dir $@);\
	fi;
	$(Q)$(LD) -r -o $@ $(sort $^)

$(LIB): $(OBJS) $(ASM_OBJS)
	$(Q)if [ ! -d $(dir $@) ]; \
	then \
	mkdir -p $(dir $@);\
	fi;
	$(Q)$(AR) $(ARFLAGS) $@ $(sort $^)


clean:
	$(Q)$(RM) $(OBJS) $(LIB) $(LIB_PERMANENT) $(LIBOUT) *.bak *~

clean_target_lib:
	$(Q)$(RM) $(LIB) *.bak *~

check:
	$(Q)if [ -f $(LIB_COMPARE_NAME) ] ; \
	then \
	exit 0; \
	else \
	exit 1;\
	fi

.PHONY: all clean check clean_target_lib


