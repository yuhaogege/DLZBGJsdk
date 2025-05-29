#!/usr/bin/env python3
# coding=utf-8


# -----------------------------------------------------------------------------------------
#Purpose:part of hupg build scripts
#Copyright CompanyNameMagicTag 2018-2019.All rights reserved
#Author: CompanyName
#------------------------------------------------------------------------------------------

import os,codecs
import struct
#import shutil
from io import BytesIO
import sys
# Manipulate ini files
from pkt_cfg_parser import PktCfgParser

#product_type=sys.argv[1]
#out_dir=sys.argv[2]
#in_dir=sys.argv[3]
reserve_data = 0
commom_name = 'flash_partition_table.bin'
pt_list = []
change_add_list = []
change_ctrl_list = []
BLOCK_SIZE = 0x1000

# Defining a partition class
class Partition_Info(object):
    def __init__(self, name, addr, id, dir, size):
        self.name = name
        self.addr = int(addr/BLOCK_SIZE)
        self.id = id
        self.dir = dir
        self.size = int(size/BLOCK_SIZE)
    def get_uint_array(self):
        a = (self.addr &0x00000fff)|((self.size&0x00000fff)<<12)|((self.dir&0x1)<<31)
        data=[a]
        return data

class Pt_ver_Info(object):
    def __init__(self, boot_support, ver, bitmap):
        self.boot_support = boot_support
        self.ver = ver
        self.bitmap = bitmap
    def get_uint_array(self):
        a = (self.boot_support&0x1)|((self.ver&0x1f)<<11)|((self.bitmap&0xffff)<<16)
        data = [a]
        return data
        
class Pt_reserve_Info(object):
    def __init__(self, res1,res2):
        self.res1 = res1
        self.res2 = res2
    def get_uint_array(self):
        data = [self.res1,self.res2]
        return data
        
class Change_detail_Info(object):
    def __init__(self,name,order,id,tb):
        self.name = name
        self.order = order
        self.id = id
        self.tb = tb
    def get_uchar_array(self):
        a = (self.id &0x1f)|((self.tb&0x3)<<6)
        data=[a]
        return data         

class Change_ctrl_Info(object):
    def __init__(self,ver,temp_addr,total_num):
        self.data = ver
        #self.data.extend(ver)
        self.temp_addr = int(temp_addr/BLOCK_SIZE)
        self.total_num = total_num
    def get_uint_array(self):
        a = (self.temp_addr &0x00000fff)|((self.total_num&0x1f)<<12)|(0x1<<31)
        self.data.append(a)
        return self.data

# Partition table modification information


# Parsing the ini file
# File / directory operations
# Serialization (not recommended)
# Common built-in module struct
# List / dictionary operations
# Class, inheritance

# Direct list
def make_struct2(output, product_type='com'):
    with open(output, 'ab') as f_out:
        f_temp = BytesIO()
        for u32_val in change_ctrl_list:
            bin_temp = struct.pack('<I', u32_val)
            f_temp.write(bin_temp)
        for u8_val in change_add_list:
            bin_temp = struct.pack('B', u8_val)
            f_temp.write(bin_temp)
        add_alian = 4-(len(change_add_list)%4)
        for i in range(add_alian):
            f_temp.write(b'\x00')
        f_out.write(f_temp.getvalue())
    size = os.path.getsize(output)
    print('%s Bytes(str2)'%str(size))
# Method approach        
def make_struct1(output, product_type='com'):
    with open(output, 'wb') as f_out:
        pt_member = []
        f_temp = BytesIO()
        #pb_info_bin = bytearray(12)
        global pt_list
        for pt_i in pt_list:
            pt_member = pt_i.get_uint_array()
            #print(pt_member)
            offset=0
            for u32val in pt_member:
                bin_temp = struct.pack('<I', u32val)
                #print(bin_temp)          
                f_temp.write(bin_temp)
        #print(f_temp.getvalue())
        f_out.write(f_temp.getvalue())
    size = os.path.getsize(output)
    print('%s Bytes(str1)'%str(size))
        
def str_2_int(str):
    int_list=[]
    str_list=str.split(',')
    for str_unit in str_list:
        int_list.append(int(str_unit))
    return int_list

def get_config(root_path, app_name, product_type):
    print('[INFO]app_name:%s, product_type:%s'%(app_name, product_type))
    cf = PktCfgParser(root_path, app_name)
    cf.read()
    pt_cfg = cf.get_partition_table_settings()

    # Get address and size information
    ptb = pt_cfg[product_type]
    print("[INFO]make_partition_table.py: %s_%s, settings:"%(app_name, product_type), ptb)
    addr = ptb["PRODUCT_CFG_PARTITION_BOOT_ST_ADDR"]
    size = ptb["PRODUCT_CFG_PARTITION_BOOT_SIZE"]
    pt_boot_info = Partition_Info('boot', addr, 1, 0, size)
    addr = ptb["PRODUCT_CFG_PARTITION_FTM1_ST_ADDR"]
    size = ptb["PRODUCT_CFG_PARTITION_FTM1_SIZE"] + ptb["PRODUCT_CFG_PARTITION_FTM2_SIZE"]
    pt_fnv_info = Partition_Info('fnv', addr, 2, 0, size)
    addr = ptb["PRODUCT_CFG_PARTITION_NV_FILE_ST_ADDR"]
    size = ptb["PRODUCT_CFG_PARTITION_NV_FILE_SIZE"]
    pt_nv_back_info = Partition_Info('nv_back', addr, 3, 0, size)
    addr = ptb["PRODUCT_CFG_PARTITION_KERNEL_ST_ADDR"]
    size = ptb["PRODUCT_CFG_PARTITION_KERNEL_SIZE"]
    pt_kernel_info = Partition_Info('kernel', addr, 4, 0, size)
    addr = ptb["PRODUCT_CFG_PARTITION_UPG_ST_ADDR"]
    size = ptb["PRODUCT_CFG_PARTITION_UPG_SIZE"]
    pt_upg_info = Partition_Info('upg', addr, 5, 1, size)
    addr = ptb["PRODUCT_CFG_PARTITION_NV_WORK_ST_ADDR"]
    size = ptb["PRODUCT_CFG_PARTITION_NV_WORK_SIZE"]
    pt_nv_info = Partition_Info('nv', addr, 6, 0, size)
    addr = ptb["PRODUCT_CFG_PARTITION_CRASH_ST_ADDR"]
    size = ptb["PRODUCT_CFG_PARTITION_CRASH_SIZE"]
    pt_crash_info = Partition_Info('crash', addr, 7, 0, size)
    addr = ptb["PRODUCT_CFG_PARTITION_USER_ST_ADDR"]
    size = ptb["PRODUCT_CFG_PARTITION_USER_SIZE"]
        #User modify according to actual situation
    pt_user_info1 = Partition_Info('user1', addr, 8, 0, size)
    pt_user_info2 = Partition_Info('user2',0, 9, 0, 0)
    pt_user_info3 = Partition_Info('user3',0, 10, 0, 0)
    pt_user_info4 = Partition_Info('user4',0, 11, 0, 0)
    pt_user_info5 = Partition_Info('user5',0, 12, 0, 0)
    # Get ver info
    boot_support=ptb["boot_support"]
    ver_num=ptb["ver_num"]
    bit_map=ptb["bit_map"]
    bit_map_support=ptb["bit_map_support"]
    pt_new_ver = Pt_ver_Info(boot_support,ver_num,bit_map)
    pt_upg_ver = Pt_ver_Info(boot_support,ver_num,bit_map_support)
    # Reserved area information
    pt_reserve = Pt_reserve_Info(0,0)
    # Get control information
    temp_addr = ptb["PRODUCT_CFG_PARTITION_LOG_TEMP_ADDR"]
        # Get id information
    id_str_list = ptb["to_change_id"]
    id_num_list = str_2_int(id_str_list)
    tb_str_list = ptb["to_change_tb"]
    tb_num_list = str_2_int(tb_str_list)
    count = len(id_num_list)
    global change_ctrl_list
    change_ctrl = Change_ctrl_Info(pt_upg_ver.get_uint_array(), temp_addr, count)
    change_ctrl_list = change_ctrl.get_uint_array()
    global change_add_list
    change_add_list = []
    for i in range(count):
        change_add_list.append((id_num_list[i]&0x1f)|((tb_num_list[i]&0x3)<<6))
        #print(change_add_list)
        
    global pt_list
    pt_list = [pt_new_ver, pt_boot_info, pt_fnv_info, pt_nv_back_info, pt_kernel_info, pt_upg_info, pt_nv_info, pt_crash_info, pt_user_info1, pt_user_info2, pt_user_info3, pt_user_info4,pt_user_info5, pt_reserve]

def make_partition_table_bin(product_type, root_path, cfg_file, dst_dir, app_name):
    out_dir = os.path.join(root_path, dst_dir)
    file_name = '%s_%s_%s'%(app_name.lower(), product_type.lower(),commom_name)
    output_file = os.path.join(out_dir,file_name)
    print("HHHHHHHHHHHHHHHHHHHHHHHH=",output_file)
    get_config(root_path, app_name.lower(), product_type.lower())
    make_struct1(output_file, product_type)
    make_struct2(output_file, product_type) 
    size = os.path.getsize(output_file)
    print('%s Bytes'%str(size))
    return size

#make_partition_table_bin(product_type, out_dir, in_dir)   
#make_partition_table_bin() 
