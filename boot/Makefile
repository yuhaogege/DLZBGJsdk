# A simple Makefile for lib(libxxx.a)
include $(MAIN_TOPDIR)/build_tmp/config/soc_product_config.mk
ifeq ($(PRODUCT_CFG_BUILD_RELEASE), yes)
MAIN_OUT_DIR_NAME = $(project_name)_release
else
MAIN_OUT_DIR_NAME = $(project_name)_debug
endif

MAIN_BUILD_TMP_OUT :=	$(MAIN_TOPDIR)/build_tmp/$(MAIN_OUT_DIR_NAME)
ALL_TARGET := $(MAIN_BUILD_TMP_OUT)/ALL/bin

all:
	$(Q)make -C flashboot -f Makefile.start_from_no_rom clean
	$(Q)make -C flashboot -f Makefile.start_from_no_rom all
	$(Q)if [ "$$?" != "0" ]; then\
		exit 1;\
	fi
	
	$(Q)make -C flashboot -f Makefile.start_from_no_rom clean

clean:
	$(Q)$(RM) $(ALL_TARGET)/flashboot


check:
	$(Q)echo -n "HB BUILD boot [SUCCESS]" 
