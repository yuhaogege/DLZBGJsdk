#!/usr/bin/env python3
# coding=utf-8


# -----------------------------------------------------------------------------------------
#Purpose:hupg main build scripts
#Copyright CompanyNameMagicTag 2018-2020.All rights reserved
#Author: CompanyName
#------------------------------------------------------------------------------------------

import os, sys, time, string, re ,shutil, time, xml.etree.ElementTree as ET
from configparser import ConfigParser
from functools import reduce
from ctypes import *
import struct
import binascii
from zlib import crc32
import random

import make_image_module as MAKE_IMAGE
import make_partition_table as MAKE_PARTITION
sys.path.append(os.path.join(os.getcwd(), 'scripts'))
from analyze_dependence  import user_pannel_config
#=================================================#
def modify_config_ini():
    cfg = ConfigParser()
    cfg.read('config.ini')
    cfg.set('app','app_list',reduce(lambda x,y:'%s,%s'%(x,y),g_user_pannel_config.get_app_name(g_project_name)))
    cfg.set('product','chip',g_user_pannel_config.get_product_chip())
    cfg.set('product','product_ver',g_user_pannel_config.get_product_ver())
    cfg.set('product','product_type',reduce(lambda x,y:'%s,%s'%(x,y),g_user_pannel_config.get_dev_list()))
    cfg.set('path','product_name',g_project_name)
    cfg.set('path','debug_mode',g_user_pannel_config.get_debug_mode())
    with open('config.ini','w') as fp:
        cfg.write(fp)
#=================================================#

time.sleep(1)

g_user_pannel_config = user_pannel_config()
g_project_name = g_user_pannel_config.get_project_name()
modify_config_ini()

root_path = os.path.join(os.getcwd(), '..')
cfg = ConfigParser()
cfg.read('config.ini')
#cfg.sections()
hb_cfg_file=os.path.join(root_path,r"hb.cfg")
pkt_path         = os.path.join(root_path,cfg.get('path','pkt_path'))
chip_name       = cfg.get('product','chip')
product_name = cfg.get('path','product_name')
product_ver     = cfg.get('product','product_ver')
product_type     = cfg.get('product','product_type')
boot_ver           = cfg.get('mode','boot_ver')
target_type      = cfg.get('mode','target_type')
app_list           = cfg.get('app','app_list')
#Configuration file address#
boot_bin_path = os.path.join(root_path,cfg.get('path','boot_bin_path'))
cco_bin_path = os.path.join(root_path,cfg.get('path','cco_bin_path'))
sta_bin_path= os.path.join(root_path,cfg.get('path','sta_bin_path'))
ndm_bin_path= os.path.join(root_path,cfg.get('path','ndm_bin_path'))
cco_nv_path = os.path.join(root_path,cfg.get('path','cco_nv_path'))
sta_nv_path = os.path.join(root_path,cfg.get('path','sta_nv_path'))
ndm_nv_path = os.path.join(root_path,cfg.get('path','ndm_nv_path'))
hupg_xml_file   = os.path.join(os.getcwd(),"packet_cfg/21_hupg.xml")
hupg_ini_path   = os.path.join(os.getcwd(),"packet_cfg")

# Source file address#
bin_src_dir  = os.path.join(pkt_path,"src/")

# Destination file address#
image_dir    = os.path.join(pkt_path,"image/" )
image_list_file = os.path.join(pkt_path,"image/image_list.txt")

# Version magic header file: upgrade code, upgrade file, NV need to match
g_ver_h_file=os.path.join(os.getcwd(),"../build_tmp/config/hb_magic.h")

g_ver_magic=0

#-----------------------------------Basic type definition-----------------------------------
TD_CHAR=c_char
TD_S8=c_byte
TD_U8=c_ubyte
TD_S16=c_short
TD_U16=c_ushort
TD_S32=c_int
UAPI_U32=c_uint

# Note The NV structure here must be the same as the NV structure of build_nv_xx.y!
class NV_FILE_STRU(Structure):
    _fields_ = [
        ("magic", UAPI_U32),
        ("crc", UAPI_U32),
        ("ver", TD_U8),
        ("head_len", TD_U8),
        ("total_num", TD_U16),
        ("seq", UAPI_U32),
        ("ver_magic", UAPI_U32),
        ("flash_size", UAPI_U32),
        ("keep_id_range", TD_U8*2),
        ("reserve", TD_U8*2),
        ("nv_item_index", TD_U8*0),
        ]

def modify_nv_ver_magic(nv_file, ver_magic):
    with open(nv_file,'rb') as nv_file_fp:
        nv_file_bin=nv_file_fp.read()

    nv_tmp_bin=bytearray(len(nv_file_bin))
    nv_tmp_bin[0:len(nv_file_bin)]=nv_file_bin

    nv_file_st=NV_FILE_STRU.from_buffer(nv_tmp_bin)
    nv_file_st.ver_magic=ver_magic
    # Recalculate crc32 for entire region
    nv_file_st.crc=crc32(nv_tmp_bin[8:len(nv_tmp_bin)])

    with open(nv_file,'wb+') as nv_file_fp:
        nv_file_fp.write(nv_tmp_bin)

def get_ver_magic(ver_h_file,app_name):
    ver_magic=0
    with open(ver_h_file, 'r') as f:
        find = False
        for line in f.readlines():
            if app_name in line:
                find = True
                continue
            if 'PRODUCT_CFG_BUILD_MAGIC' in line and find == True:
                L=line.split()
                ver_magic=L[2].strip()
                print("app_name:%s             ver_magic:0x%x"%(app_name,int(ver_magic)))
            find = False
    return int(ver_magic)

class ImageBuild:
    def __init__(self):
        if not os.path.exists(pkt_path):
            os.makedirs(pkt_path)
        if not os.path.exists(image_dir):
            os.makedirs(image_dir)
        if not os.path.exists(bin_src_dir):
            os.makedirs(bin_src_dir)    
        self.__bootBinPath    	= boot_bin_path
        self.__ccoBinPath    	= cco_bin_path
        self.__staBinPath     	= sta_bin_path
        self.__ndmBinPath     	= ndm_bin_path
        self.cco_nv_path        = cco_nv_path
        self.sta_nv_path        = sta_nv_path
        self.ndm_nv_path        = ndm_nv_path
        
        self.__binSrcDir     = bin_src_dir
        self.__imageDir     = image_dir
        self.__chipName      = chip_name
        self.__prodVer     = product_ver
        self.__app_list     = app_list
        self.__product_type = product_type
        self.__product_name = product_name
        self.__imageListFile =  open(image_list_file, "wb")   

    def work(self):
        self.__GetSrcBin()
        self.__BuildImage()
    
    # Copy bin file #
    def __GetSrcBin(self):
        for name in os.listdir(self.__binSrcDir):
            os.remove(os.path.join(self.__binSrcDir.encode('utf-8'), name.encode('utf-8')))

        if os.path.exists(self.__bootBinPath):
            bootBinFileNames = os.listdir(self.__bootBinPath)
            for a in bootBinFileNames:
                if '.bin' in a:
                    shutil.copy(os.path.join(self.__bootBinPath.encode('utf-8'), a.encode('utf-8')),self.__binSrcDir.encode('utf-8'))

        if os.path.exists(self.__ccoBinPath):
            ccoBinFileNames = os.listdir(self.__ccoBinPath)
            for b in ccoBinFileNames:
                if '.bin' in b:
                    shutil.copy(os.path.join(self.__ccoBinPath.encode('utf-8'), b.encode('utf-8')),self.__binSrcDir.encode('utf-8'))
 
        if os.path.exists(self.__staBinPath):
            staBinFileNames = os.listdir(self.__staBinPath)
            for c in staBinFileNames:
                if '.bin' in c:
                    shutil.copy(os.path.join(self.__staBinPath.encode('utf-8'), c.encode('utf-8')),self.__binSrcDir.encode('utf-8'))
        
        if os.path.exists(self.__ndmBinPath):
            ndmBinFileNames = os.listdir(self.__ndmBinPath)
            for d in ndmBinFileNames:
                if '.bin' in d:
                    shutil.copy(os.path.join(self.__ndmBinPath.encode('utf-8'), d.encode('utf-8')),self.__binSrcDir.encode('utf-8'))

        if os.path.exists(self.cco_nv_path):
            nvHnvFileNames = os.listdir(self.cco_nv_path)
            for e in nvHnvFileNames:
                if '.hnv' in e:
                    shutil.copy(os.path.join(self.cco_nv_path.encode('utf-8'), e.encode('utf-8')),self.__binSrcDir.encode('utf-8'))
                    
        if os.path.exists(self.sta_nv_path):
            nvHnvFileNames = os.listdir(self.sta_nv_path)
            for e in nvHnvFileNames:
                if '.hnv' in e:
                    shutil.copy(os.path.join(self.sta_nv_path.encode('utf-8'), e.encode('utf-8')),self.__binSrcDir.encode('utf-8'))
        
        if os.path.exists(self.ndm_nv_path):
            nvHnvFileNames = os.listdir(self.ndm_nv_path)
            for e in nvHnvFileNames:
                if '.hnv' in e:
                    shutil.copy(os.path.join(self.ndm_nv_path.encode('utf-8'), e.encode('utf-8')),self.__binSrcDir.encode('utf-8'))
                    
    def __BuildImage(self):
        product_name       =   self.__product_name.upper()
        product_ver        = self.__prodVer
        app_name             =   self.__app_list.upper()
        # Read xml configuration #
        print('read from xml ...')
        tree = ET.parse(hupg_xml_file) 
        list_product = tree.findall("PROJECT")
        for product in list_product:
            # Select project name #
            if (product.attrib['name'].upper() == product_name):
                print('PROJECT name : %s'%product.attrib['name'])
                for app in product:
                    # Choose APP name #
                    app_name_in_xml = app.attrib['NAME'].upper()
                    if app_name_in_xml in app_name :
                        print('APP name : %s'%app.attrib['NAME'])
                        g_ver_magic = get_ver_magic(g_ver_h_file,app_name_in_xml.lower())

                        for prod_type in app:
                            if 'CFG_FILE' in prod_type.attrib.keys():
                                config_file=prod_type.attrib['CFG_FILE'].strip()
                            else:
                                config_file=None

                            src_files = prod_type.attrib['SRC_NAME'].split(',')
                            prod_name = prod_type.attrib['ID']

                            # hupg configuration
                            if prod_type.attrib['TYPE'] == 'hupg':
                                build_flag=0
                                for X in prod_type.attrib['DEV'].lower().split('_'):
                                    for Y in self.__product_type.lower().split(','):
                                        if (X == Y):
                                            build_flag =1
                                if build_flag:
                                    kernel_file = os.path.join(bin_src_dir,src_files[0])
                                    normal_nv_file = os.path.join(bin_src_dir,src_files[1])
                                    partition_bin_file = os.path.join(bin_src_dir,src_files[2])
                                    if not os.path.exists(normal_nv_file):
                                        continue

                                    modify_nv_ver_magic(normal_nv_file,g_ver_magic)
                                    # Construct output file name #
                                    image_file_name = '%s_%s_%s_%s.%s'%(product_name,app.attrib['NAME'].lower(),product_ver.lower(),prod_name,prod_type.attrib['TYPE'])
                                    self.__imageListFile.write(('%s\r\n'%image_file_name).encode('utf-8'))
                                    image_file = os.path.join(image_dir, image_file_name) 
                                    #print("%s %s"%(normal_nv_file,image_file_name))
                                    MAKE_IMAGE.make_hupg(root_path,kernel_file,normal_nv_file,partition_bin_file,
                                        image_file,config_file,product_ver,g_ver_magic, app.attrib['NAME'].lower())
                                    print(image_file_name)

                            # hbin configuration
                            elif prod_type.attrib['TYPE'] == 'hbin':
                                build_flag=0
                                for X in prod_type.attrib['DEV'].lower().split('_'):
                                    for Y in self.__product_type.lower().split(','):
                                        if (X == Y):
                                            build_flag =1
                                if build_flag:
                                    flash_boot_file = os.path.join(bin_src_dir,src_files[0])
                                    factory_nv_file = os.path.join(bin_src_dir,src_files[1])
                                    normal_nv_file = os.path.join(bin_src_dir,src_files[2])
                                    kernel_file = os.path.join(bin_src_dir,src_files[3].lower())
                                    if not os.path.exists(normal_nv_file) or not os.path.exists(factory_nv_file):
                                        continue

                                    modify_nv_ver_magic(normal_nv_file,g_ver_magic)
                                    modify_nv_ver_magic(factory_nv_file,g_ver_magic)
                                    # Construct output file name #
                                    image_file_name = '%s_%s_%s_%s.%s'%(product_name,app.attrib['NAME'].lower(),product_ver.lower(),prod_name,prod_type.attrib['TYPE'])
                                    self.__imageListFile.write(('%s\r\n'%image_file_name).encode('utf-8'))
                                    image_file = os.path.join(image_dir, image_file_name)
                                    MAKE_IMAGE.make_hbin(root_path,flash_boot_file, factory_nv_file, normal_nv_file,
                                        kernel_file, image_file, config_file, product_ver,g_ver_magic, app.attrib['NAME'].lower())
                                    print(image_file_name)

                            #bootupg configuration
                            elif prod_type.attrib['TYPE'] == 'bootupg':
                                flash_boot_file = os.path.join(bin_src_dir,src_files[0])
                                # Construct output file name #
                                image_file_name = '%s_%s_%s.%s'%(product_name,product_ver.lower(),prod_name,prod_type.attrib['TYPE'])
                                self.__imageListFile.write(('%s\r\n'%image_file_name).encode('utf-8'))
                                image_file = os.path.join(image_dir, image_file_name)
                                print("*******************************************")
                                print(flash_boot_file)
                                MAKE_IMAGE.make_bootupg(root_path,flash_boot_file, image_file, config_file, product_ver, g_ver_magic)
                                print(image_file_name)
                            elif prod_type.attrib['TYPE'] == 'partion_bin':
                                build_flag=0
                                for X in prod_type.attrib['DEV'].lower().split('_'):
                                    for Y in self.__product_type.split(','):
                                        if (X == Y):
                                            build_flag =1
                                if build_flag:
                                    MAKE_PARTITION.make_partition_table_bin(prod_type.attrib['DEV'].lower(),root_path,
                                        config_file,bin_src_dir,app.attrib['NAME'].lower())
                            elif prod_type.attrib['TYPE'] == 'partion_upg':
                                build_flag=0
                                for X in prod_type.attrib['DEV'].lower().split('_'):
                                    for Y in self.__product_type.split(','):
                                        if (X == Y):
                                            build_flag =1
                                if build_flag:
                                    print("product_name",type(product_name),product_name)
                                    print("product_ver",type(product_ver),product_ver)
                                    print("prod_name",type(prod_name),prod_name)
                                    image_file_name='%s_%s_%s_%s.hupg'%(product_name, app.attrib['NAME'].lower(), product_ver.lower(),prod_name)
                                    self.__imageListFile.write(('%s\r\n'%image_file_name).encode('utf-8'))
                                    image_file = os.path.join(image_dir, image_file_name)
                                    partion_file = os.path.join(bin_src_dir,src_files[0])
                                    print("partion_file", type(partion_file), partion_file)
                                    MAKE_IMAGE.make_partion_upg(root_path,partion_file,image_file,config_file,product_ver,g_ver_magic, app.attrib['NAME'])
        self.__imageListFile.close()

def pkt_build():
    t1 = time.time()
    fu = ImageBuild()
    fu.work()

    print ("hupg and hbin ok!! \r\n")
    print('TOTAL TIME:%ss'%str(time.time() - t1))

# main function #
if __name__ == '__main__':
    pkt_build()
