
################################################################################
# Main Makefile for Socbuilder 
#
# History
################################################################################

#A simple Makefile for lib(libxxx.a)

#$(warning "the value of LOCAL_PATH is $(MAIN_TOPDIR)")

include $(MAIN_TOPDIR)/build_tmp/config/soc_product_config.mk

ifeq ($(PRODUCT_CFG_BUILD_RELEASE), yes)
MAIN_OUT_DIR_NAME = $(project_name)_release
else
MAIN_OUT_DIR_NAME = $(project_name)_debug
endif

MAIN_BUILD_TMP_OUT :=	$(MAIN_TOPDIR)/build_tmp/$(MAIN_OUT_DIR_NAME)
ALL_LIBS := $(MAIN_BUILD_TMP_OUT)/all/lib

# CSEC requires library copy function to be turned off by default
HB_LITEOS_COMPILE_TESTCASE = NO

HB_USE_REMAP:=YES

LIB_SUBDIRS := Huawei_LiteOS
ifeq ($(HB_USE_REMAP), YES)
LITEOS_LIBS := liblitekernelram.o
else
LITEOS_LIBS := liblitekernelram.a 
endif
MODDIR = liteos/Huawei_LiteOS
exist = notexist

prepare:
	@mkdir -p $(ALL_LIBS)
# Need to clean before compiling£¬Because if you want to compile SDKs for other product forms, 
# the code in Os will not be recompiled because it has not been modified.

all: clean prepare
# Compile source code
	$(Q)if [ -f  $(MAIN_TOPDIR)/rom/Makefile ];\
	then \
	$(MAKE) HB_LITEOS_COMPILE_ROM=YES MAIN_TOPDIR=$(MAIN_TOPDIR) project_name=$(project_name) -C Huawei_LiteOS lib;\
	if [ "$$?" != "0" ]; then \
	exit 1;\
	fi;\
	fi
	$(MAKE) MAIN_TOPDIR=$(MAIN_TOPDIR) project_name=$(project_name) -C Huawei_LiteOS lib;
	$(Q)if [ "$$?" != "0" ]; then \
	exit 1;\
	fi;

	$(Q)cp $(LIB_SUBDIRS)/out/*.o  $(ALL_LIBS)
ifeq ($(HB_LITEOS_COMPILE_TESTCASE), YES)
	$(Q)cp $(LIB_SUBDIRS)/out/*.a  $(ALL_LIBS)
endif

clean:
	$(Q)for dir in $(LIB_SUBDIRS); \
	do $(MAKE) -C $$dir clean|| exit 1; \
	done
	$(Q)for dir in $(LITEOS_LIBS); \
	do if [ -f $(ALL_LIBS)/$$dir ]; \
	then \
	rm $(ALL_LIBS)/$$dir;\
	fi \
	done
	
check:
	$(Q)for dir in $(LITEOS_LIBS);\
	do if [ -f $(ALL_LIBS)/$$dir ]; \
	then \
	exit 0;\
	else \
	exit 1;\
	fi;\
	done
.PHONY: all clean  check

