#!/usr/bin/env python3
# coding=utf-8
# -----------------------------------------------------------------------------------------
#Purpose:main config parser
#Copyright CompanyNameMagicTag 2018-2019.All rights reserved
#Author: CompanyName
#------------------------------------------------------------------------------------------

import os
import sys
import re
import xml.etree.ElementTree as ET

#GLOBAL SETTINGS OF PARTITION TABLE
PT_SETTINGS_TO_CHANGE_TB = '1,1,1'

CFG_START_TBL_CHECK_NV_FILE_CRC = 0
CFG_START_TBL_CHECK_NV_VER_MAGIC = 0
CFG_UPG_CONTAIN_PARTITION_INFO = 1

class PktCfgParserException(Exception):
    pass

class PktCfgParser:
    def __init__(self, root_dir, app_name):
        self.app_name = app_name.lower()
        self.cfg_path = os.path.join(root_dir)
        self.cfg_path = os.path.join(self.cfg_path, 'app', self.app_name, 'config.xml')
        self.glb_macro_cfg = os.path.join(root_dir, 'build', 'config.xml')
        self.pt_cfg = {'cco':{}, 'sta':{}, 'ndm':{}, 'boot':{}}

    def read(self, boot=False):
        self.load_partition_default_cfg()
        self.load_partition_table(boot)

    def load_partition_default_cfg(self):
        self.pt_cfg['cco']['product_list_1'] = '1,60'
        self.pt_cfg['cco']['product_list_2'] = '0,0'
        self.pt_cfg['sta']['product_list_1'] = '101,8191'
        self.pt_cfg['sta']['product_list_2'] = '0,0'
        self.pt_cfg['ndm']['product_list_1'] = '61,100'
        self.pt_cfg['ndm']['product_list_2'] = '0,0'
        self.pt_cfg['boot']['product_list_1'] = '1,8191'
        self.pt_cfg['boot']['product_list_2'] = '0,0'
        self.pt_cfg['boot']['PRODUCT_CFG_PARTITION_BOOT_SIZE'] = 0x4000
        #boot doesn't care about these values, but shares the upg file format, make the values to be 0
        self.pt_cfg['boot']['boot_support'] = 0
        self.pt_cfg['boot']['ver_num'] = 0
        self.pt_cfg['boot']['bit_map_support'] = 0
        #Boot checks crc of the NV file if option enabled, When booting the kernel, BOOT checks whether the original NV file is correct. Impact: The startup information table.
        self.pt_cfg['PRODUCT_CFG_START_TBL_CHECK_NV_FILE_CRC'] = CFG_START_TBL_CHECK_NV_FILE_CRC
        #Boot checks magic number of the NV if option enabled, BOOT checks whether the original NV file is correct when booting the kernel. Impact: The startup information table.
        self.pt_cfg['PRODUCT_CFG_START_TBL_CHECK_NV_VER_MAGIC'] = CFG_START_TBL_CHECK_NV_VER_MAGIC
        #Hupg binary contains partition infomation if option enabled, Whether the upgrade file header contains partition upgrade information。mpact: Upgrade file headers
        self.pt_cfg['PRODUCT_CFG_UPG_CONTAIN_PARTITION_INFO'] = CFG_UPG_CONTAIN_PARTITION_INFO
    
    def load_partition_table(self, boot=False):
        if boot == False:
            tree = ET.parse(self.cfg_path)
            modules = tree.getroot().findall('MODULE')
            for module in modules:
                for dev in module.iter(tag='DEV'):
                    cfg_dict = {}
                    for item in dev.iter(tag='item'):
                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_BOOT_ST_ADDR'):
                            cfg_dict['PRODUCT_CFG_PARTITION_BOOT_ST_ADDR'] = self.convert_int(item.attrib['value'])
                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_BOOT_SIZE'):
                            cfg_dict['PRODUCT_CFG_PARTITION_BOOT_SIZE'] = self.convert_int(item.attrib['value'])

                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_FTM1_ST_ADDR'):
                            cfg_dict['PRODUCT_CFG_PARTITION_FTM1_ST_ADDR'] = self.convert_int(item.attrib['value'])
                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_FTM1_SIZE'):
                            cfg_dict['PRODUCT_CFG_PARTITION_FTM1_SIZE'] = self.convert_int(item.attrib['value'])

                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_FTM2_ST_ADDR'):
                            cfg_dict['PRODUCT_CFG_PARTITION_FTM2_ST_ADDR'] = self.convert_int(item.attrib['value'])
                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_FTM2_SIZE'):
                            cfg_dict['PRODUCT_CFG_PARTITION_FTM2_SIZE'] = self.convert_int(item.attrib['value'])

                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_NV_FILE_ST_ADDR'):
                            cfg_dict['PRODUCT_CFG_PARTITION_NV_FILE_ST_ADDR'] = self.convert_int(item.attrib['value'])
                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_NV_FILE_SIZE'):
                            cfg_dict['PRODUCT_CFG_PARTITION_NV_FILE_SIZE'] = self.convert_int(item.attrib['value'])

                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_KERNEL_ST_ADDR'):
                            cfg_dict['PRODUCT_CFG_PARTITION_KERNEL_ST_ADDR'] = self.convert_int(item.attrib['value'])
                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_KERNEL_SIZE'):
                            cfg_dict['PRODUCT_CFG_PARTITION_KERNEL_SIZE'] = self.convert_int(item.attrib['value'])

                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_UPG_ST_ADDR'):
                            cfg_dict['PRODUCT_CFG_PARTITION_UPG_ST_ADDR'] = self.convert_int(item.attrib['value'])
                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_UPG_SIZE'):
                            cfg_dict['PRODUCT_CFG_PARTITION_UPG_SIZE'] = self.convert_int(item.attrib['value'])

                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_NV_WORK_ST_ADDR'):
                            cfg_dict['PRODUCT_CFG_PARTITION_NV_WORK_ST_ADDR'] = self.convert_int(item.attrib['value'])
                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_NV_WORK_SIZE'):
                            cfg_dict['PRODUCT_CFG_PARTITION_NV_WORK_SIZE'] = self.convert_int(item.attrib['value'])

                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_USER_ST_ADDR'):
                            cfg_dict['PRODUCT_CFG_PARTITION_USER_ST_ADDR'] = self.convert_int(item.attrib['value'])
                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_USER_SIZE'):
                            cfg_dict['PRODUCT_CFG_PARTITION_USER_SIZE'] = self.convert_int(item.attrib['value'])

                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_CRASH_ST_ADDR'):
                            cfg_dict['PRODUCT_CFG_PARTITION_CRASH_ST_ADDR'] = self.convert_int(item.attrib['value'])
                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_CRASH_SIZE'):
                            cfg_dict['PRODUCT_CFG_PARTITION_CRASH_SIZE'] = self.convert_int(item.attrib['value'])

                        if (item.attrib['name'] == 'PARTITION_VER_CTL_SUPPORT_ENABLE'):
                            cfg_dict['boot_support'] = self.convert_int(item.attrib['value'])

                        if (item.attrib['name'] == 'PARTITION_VER_CTL_NUM'):
                            cfg_dict['ver_num'] = self.convert_int(item.attrib['value'])
                            cfg_dict['bit_map'] = self.convert_int(item.attrib['value'])

                        if (item.attrib['name'] == 'PARTITION_VER_CTL_COMPATIBLE'):
                            cfg_dict['bit_map_support'] = self.convert_int(item.attrib['value'])

                        if (item.attrib['name'] == 'PRODUCT_CFG_PARTITION_LOG_TEMP_ADDR'):
                            cfg_dict['PRODUCT_CFG_PARTITION_LOG_TEMP_ADDR'] = self.convert_int(item.attrib['value'])

                        if (item.attrib['name'] == 'PARTITION_TRANS_LOG_ENABLE'):
                            if self.convert_int(item.attrib['value']) == 1:
                                cfg_dict['to_save_log'] = '1,1,1'
                            else:
                                cfg_dict['to_save_log'] = '0,0,0'

                        if (item.attrib['name'] == 'PARTITION_TRANS_TB_ID'):
                            cfg_dict['to_change_id'] = item.attrib['value']
                            cfg_dict['to_change_tb'] = PT_SETTINGS_TO_CHANGE_TB

                    if (dev.attrib['name'] == 'PRODUCT_CFG_PRODUCT_TYPE_CCO'):
                        self.pt_cfg['cco'].update(cfg_dict)
                    if (dev.attrib['name'] == 'PRODUCT_CFG_PRODUCT_TYPE_STA'):
                        self.pt_cfg['sta'].update(cfg_dict)
                    if (dev.attrib['name'] == 'PRODUCT_CFG_PRODUCT_TYPE_NDM'):
                        self.pt_cfg['ndm'].update(cfg_dict)
    
    def get_partition_table_settings(self):
        return self.pt_cfg

    def get_cfg_path(self):
        return (self.cfg_path, self.glb_macro_cfg)

    def convert_int(self, value):
        match1 = re.match(r'\s*0x', value)
        match2 = re.match(r'\s*0X', value)
        if match1 or match2:
            return int(value, 16)
        else:
            return int(value, 10)

    def get(self, section, option):
        if section.upper() == 'MACRO':
            return self.get_macro(option)
        if section.lower() == 'cco':
            return self.pt_cfg['cco'][option]
        if section.lower() == 'sta':
            return self.pt_cfg['sta'][option]
        if section.lower() == 'ndm':
            return self.pt_cfg['ndm'][option]
        if section.lower() == 'boot':
            return self.pt_cfg['boot'][option]

        raise PktCfgParserException('Unkown section!!!')

    def getint(self, section, option):
        value = ''
        if section.upper() == 'MACRO':
            value = self.get_macro(option)
        return self.convert_int(value)

    def option_is_enabled(self, section, option):
        value = ''
        if section.upper() == 'MACRO':
            value = self.get_macro(option)

        if (value.lower() == 'yes') or (value == '1'):
            return 1
        return 0

    def get_macro(self, option):
        tree = ET.parse(self.glb_macro_cfg)
        modules = tree.getroot().findall('MODULE')
        for module in modules:
            for item in module.iter(tag='item'):
                if (item.attrib['type'] == '0x1'):
                    if (item.attrib['name'] == option):
                        return item.attrib['value']
        raise UapiCfgParserException('Value not found')


