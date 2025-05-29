# A simple Makefile for lib(libxxx.a)

include $(MAIN_TOPDIR)/build_tmp/config/soc_product_config.mk

ifeq ($(PRODUCT_CFG_BUILD_RELEASE), yes)
MAIN_OUT_DIR_NAME = $(project_name)_release
else
MAIN_OUT_DIR_NAME = $(project_name)_debug
endif
MAIN_OUT := $(MAIN_TOPDIR)/build_tmp/$(MAIN_OUT_DIR_NAME)

#Directory path xml
HDB_DST_XML_ROOT_DIR := $(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/hdb/hdbcfg/
#Directory path dbk Resource
HDB_DST_DBK_RESOURCE_DIR :=  $(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/hdb/dbk/
#Source path: NV XML
HDB_NV_XML_SRC_DIR :=$(MAIN_OUT)/$(HB_PRODUCT_NAME_LIST)/nv
#Source path：Other xml
HDB_OTHER_XML_SRC_DIR := $(MAIN_TOPDIR)/target/config/hdbcfg

do_all: do_hdb_cp_xml do_hdb_cp_dbk_resource do_hdb_creat_diag_txt do_hdb_create_prim_xml

do_hdb_cp_xml:
#@rm -rf $(HDB_DST_XML_ROOT_DIR)
	$(Q)mkdir -p $(HDB_DST_XML_ROOT_DIR)
ifeq ($(HDB_NV_XML_SRC_DIR)/mss_nvi_db.xml, $(wildcard $(HDB_NV_XML_SRC_DIR)/mss_nvi_db.xml))
	$(Q)cp -f $(HDB_NV_XML_SRC_DIR)/mss_nvi_db.xml $(HDB_DST_XML_ROOT_DIR)
endif
	$(Q)cp -f $(HDB_OTHER_XML_SRC_DIR)/*.xml $(HDB_DST_XML_ROOT_DIR)
	
do_hdb_cp_dbk_resource:
	$(Q)mkdir -p $(HDB_DST_DBK_RESOURCE_DIR)
	cp -f $(MAIN_TOPDIR)/target/config/dbk/base_datatype_def.txt $(HDB_DST_DBK_RESOURCE_DIR)
	
	
do_hdb_creat_nv_txt:
	$(Q)$(MAKE) -C $(MAIN_TOPDIR)/nv/scripts -f make_nv_hdb.mk
	
do_hdb_creat_diag_txt:
	$(MAKE) -f $(MAIN_TOPDIR)/target/config/diag/script/make_diag_hdb.mk

include $(MAIN_TOPDIR)/build/hdb/mk_prim_xml_step2.mk	

do_hdb_create_prim_xml:do_prim_xml_step2

.PHONY:do_hdb_main do_hdb_cp_xml do_hdb_cp_dbk_resource do_hdb_creat_nv_txt do_hdb_creat_diag_txt do_hdb_create_prim_xml
	
