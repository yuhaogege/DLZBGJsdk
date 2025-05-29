
include $(MAIN_TOPDIR)/build_tmp/config/soc_product_config.mk
ifeq ($(PRODUCT_CFG_BUILD_RELEASE), yes)
MAIN_OUT_DIR_NAME = $(project_name)_release
else
MAIN_OUT_DIR_NAME = $(project_name)_debug
endif

MAIN_BUILD_TMP_OUT := $(MAIN_TOPDIR)/build_tmp/$(MAIN_OUT_DIR_NAME)
comma :=,
empty :=
space := $(empty) $(empty)
dev_str := $(subst $(comma), $(space),$(dev_list))
MODDIR  = nv

.PHONY:all clean
all: clean
	@echo dev=$(dev_str)
	$(foreach X,$(dev_str), \
	$(MAKE) HB_PRODUCT_NAME_LIST=$(X) MAIN_TOPDIR=$(MAIN_TOPDIR) -f $(MAIN_TOPDIR)/nv/scripts/make_nv_hdb.mk;)
	$(Q)python3 $(MAIN_TOPDIR)/build/nv/build_nv.py  $(MAIN_TOPDIR) $(MAIN_BUILD_TMP_OUT) 4096
check:
	@echo -e -n "\033[42;37m HB BUILD $(MODDIR) [SUCCESS] \033[0m"

clean:
	$(foreach X,$(dev_str), \
	rm -rf $(MAIN_BUILD_TMP_OUT)/$(X)/nv; )
