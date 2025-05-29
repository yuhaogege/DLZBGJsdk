# A simple Makefile for lib(libxxx.a)

#########################################################################################################
#1.cfg Configuration section
#########################################################################################################
include $(MAIN_TOPDIR)/build_tmp/config/soc_product_config.mk
ifeq ($(PRODUCT_CFG_BUILD_RELEASE), yes)
MAIN_OUT_DIR_NAME = $(project_name)_release
else
MAIN_OUT_DIR_NAME = $(project_name)_debug
endif

MAIN_BUILD_TMP_OUT :=	$(MAIN_TOPDIR)/build_tmp/$(MAIN_OUT_DIR_NAME)
ALL_LIBS := $(MAIN_BUILD_TMP_OUT)/all/bin

HDB_TEMP_FILE_ROOT_DIR = $(MAIN_BUILD_TMP_OUT)/TEMP_DIR/$(HB_PRODUCT_NAME_LIST)/hdb/nv
#dst file dir  target\delivery\SocMDL_release\cco\hdb\nv
HDB_DST_FILE_ROOT_DIR =  $(MAIN_BUILD_TMP_OUT)/$(HB_PRODUCT_NAME_LIST)/hdb/nv
#src file dir target\config\nv
HDB_TXT_SRC_DIR = $(MAIN_TOPDIR)/nv/src/tmp_txt_header

#########################################################################################################
#1.do hdb
#########################################################################################################
RELATIVE_DIR = ../../
include $(MAIN_TOPDIR)/build/hdb/make_hdb.mk

