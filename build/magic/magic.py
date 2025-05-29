#!/usr/bin/env python3
# coding=utf-8
# -----------------------------------------------------------------------------------------
#Purpose:
#Copyright CompanyNameMagicTag 2018-2019.All rights reserved
#Author: CompanyName
#------------------------------------------------------------------------------------------

import os,sys,re
import shutil
import copy
from ctypes import *
import random
import time
sys.path.append(".")
from scripts.analyze_dependence  import user_pannel_config

g_user_pannel_config = user_pannel_config()
#WRITE_VER_H_FILE = sys.argv[1]
g_app_list = []
if __name__ == '__main__': 
    root_path = sys.path[0]
    root_path=os.path.dirname(root_path)
    root_path=os.path.dirname(root_path)
    g_app_str = sys.argv[1]
    g_app_str = g_app_str.strip(',')
    g_app_list = g_app_str.split(',')
    time_list = []
    hb_time_file=os.path.join(root_path,r'build_tmp/config/hb_magic.h')
    with open(hb_time_file,'w') as fp:
        build_time_tuple = g_user_pannel_config.get_build_data_time()
        for item in build_time_tuple:
            time_list.append(int(item))
        time_list_tuple = tuple(time_list)
        build_time=time.mktime(time_list_tuple)
        time1=time.mktime((2019, 1, 1, 0, 0, 0, 0, 0, 0))
        magic_val=build_time-time1
        print('#ifndef __HB_MAGIC_H__',file=fp)
        print('#define __HB_MAGIC_H__',file=fp)
        num = 0
        for app in g_app_list:
            num = num + 1
            print('#ifdef %s'%app.lower(),file=fp)
            print('#define PRODUCT_CFG_BUILD_MAGIC', int(magic_val//1)+num,file=fp)
            print('#endif',file=fp)
        print('#endif',file=fp)
 
