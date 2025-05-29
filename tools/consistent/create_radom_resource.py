#!/usr/bin/env python
# coding=utf-8
# -----------------------------------------------------------------------------------------
#Purpose:main build scripts
#Copyright (c) CompanyNameMagicTag 2018-2019.All rights reserved
#Author: CompanyName
#------------------------------------------------------------------------------------------

import os, time, string, re ,shutil, hashlib, binascii, sys
import copy  
import struct
from ctypes import *
from configparser import ConfigParser

import base64
import os

from Crypto import Random

from Crypto.Hash import SHA
from Crypto.Hash import SHA256


from Crypto.PublicKey import RSA

from Crypto.Signature import PKCS1_v1_5 as Signature_pkcs1_v1_5
from Crypto.Signature import PKCS1_PSS as Signature_pss


###############################定义基本类型###############################################
if __name__ == '__main__': 
    root_path=os.path.dirname(sys.path[0])
    root_path=os.path.dirname(root_path)
    resource_path=os.path.join(sys.path[0],r'resource')
    temp_path=os.path.join(sys.path[0],r'temp')
    
    if os.path.exists(resource_path):
        shutil.rmtree(resource_path)
    os.mkdir(resource_path)
        
    if os.path.exists(temp_path):    
        shutil.rmtree(temp_path)
    os.mkdir(temp_path)
    
    #创建时间文件
    fp=open(os.path.join(resource_path,r'hb_time.h'),'w+')
    cur_time=time.time()
    localtime=time.localtime(cur_time)
    str_ymd=time.strftime('%Y-%m-%d',localtime)
    str_hms=time.strftime('%H:%M:%S',localtime)
    time1=time.mktime((2019, 1, 1, 0, 0, 0, 0, 0, 0))
    magic_val=cur_time-time1
    print('#ifndef __HB_TIME_H__',file=fp)
    print('#define __HB_TIME_H__',file=fp)
    print(''.join(['#define PRODUCT_CFG_BUILD_DATE','\"',str_ymd,'\"']),file=fp)
    print(''.join(['#define PRODUCT_CFG_BUILD_TIME','\"',str_hms,'\"']),file=fp)
    print(''.join(['#define PRODUCT_CFG_BUILD_BOOT_DATA_TIME','\"',str_ymd,str_hms,'\"']),file=fp)
    print('#define PRODUCT_CFG_BUILD_MAGIC',int(magic_val//1),file=fp)
    print('#endif',file=fp)
    fp.close()
 
    '''
    #创建RSA随机资源
    i=0
    rsa_count=20
    while i<rsa_count:
        rsa_bin=Random.get_random_bytes(32)
        rsa_file=os.path.join(resource_path,'rsa_bin%d.bin'%(i))
        fp=open(rsa_file,'wb+')
        fp.write(rsa_bin)
        fp.close()
        i=i+1
        
    #创建RSA当前状态
    rsa_status_file=os.path.join(temp_path,'rsa_status.txt')
    fp=open(rsa_status_file,'w+')
    fp.write('0')
    fp=fp.close()
    ''' 
        
    
    