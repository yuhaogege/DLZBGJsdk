#!/usr/bin/env python3
# coding=utf-8


# -----------------------------------------------------------------------------------------
#Purpose:part of hupg build scripts
#Copyright (c) CompanyNameMagicTag 2018-2019.All rights reserved
#Author: CompanyName
#------------------------------------------------------------------------------------------

import os,sys
from configparser import ConfigParser

root_path        	= os.path.join(os.getcwd(),"../")
ver_cfg_file    	= os.path.join(root_path,'target/socbuilder/ver_cfg/hb_ver_cfg.h')
hb_cfg_file    	= os.path.join(root_path,'hb.cfg')

project_name_key 	= 'HB_PROJECT_NAME_LIST'
product_type_key 	= 'HB_SELECT_PRODUCT_TYPE_LIST'
app_list_key 		= 'HB_APP_COMPILE_LIST'
debug_mode_key 	= 'HB_DEBUG_COMPILE_ENABLE'
chip_name_key             = 'PRODUCT_CFG_CHIP_SOLUTION_NAME'
ver_str_key                    = 'PRODUCT_CFG_VERSION_STR'

cfg = ConfigParser()
cfg.read('config.ini')	

with open(hb_cfg_file, 'rb') as f:
    for line in  f.readlines():
        if project_name_key.encode('utf-8') in line :
            cfg.set('path', 'product_name', line[(line.find('='.encode('utf-8')))+1:].decode('utf-8').strip())
        if product_type_key.encode('utf-8') in line :
            cfg.set('product', 'product_type', line[(line.find('='.encode('utf-8')))+1:].decode('utf-8').strip().lower())
        if app_list_key.encode('utf-8') in line :
            cfg.set('app', 'app_list', line[(line.find(','.encode('utf-8'))):(line.rfind(','.encode('utf-8')))+1].decode('utf-8').strip().lower())    
        if debug_mode_key.encode('utf-8') in line :
            if (line[(line.find('='.encode('utf-8') ))+1:].strip().decode('utf-8')) == 'NO':
                cfg.set('path', 'debug_mode', 'release')
            else:
                cfg.set('path', 'debug_mode', 'debug')

with open(ver_cfg_file, 'rb') as fv:
    for line in  fv.readlines():
        if chip_name_key.encode('utf-8') in line :
            cfg.set('product', 'chip', line.split('"'.encode('utf-8'))[len(line.split('"'.encode('utf-8')))-2].decode('utf-8'))
        if ver_str_key.encode('utf-8') in line :
            cfg.set('product', 'product_ver', line.split('"'.encode('utf-8'))[len(line.split('"'.encode('utf-8')))-2].decode('utf-8'))
with open('config.ini', 'w') as fp:
    cfg.write(fp)
