
include $(MAIN_TOPDIR)/tools/dbk/makescript/v10_p/MakefileInc

MAKE_HDB_PATH=$(DBK_PATH)/makescript/v10_p/dbkconfig

empty :=
comma :=,
dev_str := $(subst $(comma), $(empty), $(dev_list))

all:
	make -f $(MAKE_HDB_PATH)/MakeHDB HB_DEBUG_COMPILE_ENABLE=$(HB_DEBUG_COMPILE_ENABLE) HB_PROJECT_NAME_LIST=$(project_name) HB_APP_NAME_LIST='$(app_name_list)' HB_DEV_LIST='$(dev_str)'
check:
	make -f $(MAKE_HDB_PATH)/MakeHDB check HB_APP_NAME_LIST='$(app_name_list)'

clean:
	make -f $(MAKE_HDB_PATH)/MakeHDB clean