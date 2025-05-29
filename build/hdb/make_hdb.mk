
#########################################################################################################
#hdb Process
#########################################################################################################
HDB_TEMP := $(wildcard $(HDB_TXT_SRC_DIR)/*_struct_def.txt)
HDB_TXT_SRC_FILE_LIST = $(notdir $(HDB_TEMP))
DBK_BASE_DATATYPE_TXT:=$(MAIN_TOPDIR)/target/config/dbk/base_datatype_def.txt


CFLAGS += $(addprefix -I $(RELATIVE_DIR),$(shell cat $(MAIN_TOPDIR)/Makefile.inc))

SCRIPT_PATH = $(RELATIVE_DIR)/build/hdb

do_hdb:
	@if [ -d $(HDB_TEMP_FILE_ROOT_DIR) ]; \
	then \
	rm -rf $(HDB_TEMP_FILE_ROOT_DIR); \
	fi;

	@if [ -d $(HDB_DST_FILE_ROOT_DIR) ]; \
	then \
	rm -rf $(HDB_DST_FILE_ROOT_DIR); \
	fi;

	@mkdir -p $(HDB_TEMP_FILE_ROOT_DIR)
	@mkdir -p $(HDB_DST_FILE_ROOT_DIR)

#@echo HDB_TEMP ============$(HDB_TEMP)
#@echo HDB_TEMP_FILE_ROOT_DIR ============$(HDB_TEMP_FILE_ROOT_DIR)
#@echo HDB_TXT_SRC_FILE_LIST ============$(HDB_TXT_SRC_FILE_LIST)

	$(foreach x,$(HDB_TXT_SRC_FILE_LIST),\
		cp -f $(HDB_TXT_SRC_DIR)/$(x) $(HDB_TEMP_FILE_ROOT_DIR)/$(x).c;\
		$(CC) -E $(HDB_TEMP_FILE_ROOT_DIR)/$(x).c $(CFLAGS) -I $(MAIN_TOPDIR)/target/config/dbk  -I $(MAIN_TOPDIR)/mdm/inc -I $(MAIN_TOPDIR)/bsp/system/inc -I $(MAIN_TOPDIR)/mdm/mac/inc > $(HDB_TEMP_FILE_ROOT_DIR)/$(x);\
		rm -f $(HDB_TEMP_FILE_ROOT_DIR)/$(x).c;\
		)
	python3 $(SCRIPT_PATH)/hdb_txt_simplyfy.py $(HDB_TEMP_FILE_ROOT_DIR) $(HDB_DST_FILE_ROOT_DIR) $(DBK_BASE_DATATYPE_TXT)

.PHONY:do_hdb
