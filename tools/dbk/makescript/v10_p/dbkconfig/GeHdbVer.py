#!/usr/bin/env python
# coding=utf-8
# -----------------------------------------------------------------------------------------
#Purpose:main build scripts
#Copyright (c) CompanyNameMagicTag. 2018-2019.All rights reserved
#------------------------------------------------------------------------------------------

#从hb_ver_cfg.h中获取版本号，写入到datavase_ver.txt中

import os
import re
import sys
import time

def GetValFromFile(pattern, file_path):
    with open(file_path) as file:
        file_lines = file.readlines()

    for strLine in file_lines:
        re_result = pattern.match(strLine)
        if re_result != None:
            bFound = True
            strVal = re_result.group(1)
            break
    return (bFound, strVal)

vr_path = os.path.abspath(os.path.join(os.path.dirname(__file__), os.pardir, os.pardir, os.pardir, os.pardir, os.pardir))
#print ("top path: " , vr_path)

if (len(sys.argv) == 3):
    source_version_file_path = os.path.join(vr_path , "code", "include", "soc_product_config.h")
else:
    source_version_file_path = os.path.join(vr_path , "build_tmp", "config", "soc_product_config.h")
#print ("source version file path: " , source_version_file_path)

database_path = sys.argv[1]
dbk_cfg_file_path = os.path.join(database_path,"database_ver.txt")
#print ("database_ver.txt path: " , dbk_cfg_file_path)

patternChip = re.compile(r"[ \t]*#define[ \t]*PRODUCT_CFG_CHIP_SOLUTION_NAME[ \t]*(\S*)\s*")
patternSof = re.compile(r"[ \t]*#define[ \t]*PRODUCT_CFG_VERSION_STR[ \t]*(\S*)\s*")

bFoundChipVer, strChipVer = GetValFromFile(patternChip, source_version_file_path)
bFoundSofVer, strSofVer = GetValFromFile(patternSof, source_version_file_path)

if bFoundChipVer and bFoundSofVer:
    strChipVer = strChipVer.strip("\"")
    strSofVer = strSofVer.strip("\"")
    strProductVer = ''.join([strChipVer , " " , strSofVer])
    strProductVer = ''.join([strProductVer , "  " , time.ctime()])
    dbk_cfg_file_lines = []
    dbk_cfg_file_lines.append(strProductVer)

    with open(dbk_cfg_file_path, "w") as dbk_cfg_file:
        dbk_cfg_file.writelines(dbk_cfg_file_lines)
    print ("generate hdb ver success")    
else:
    print ("File version not found in: " , source_version_file_path)



