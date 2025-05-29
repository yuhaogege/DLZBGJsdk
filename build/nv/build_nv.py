#!/usr/bin/env python3
# coding=utf-8

# -----------------------------------------------------------------------------------------
#Purpose:nv build scripts
#Copyright CompanyNameMagicTag 2018-2019.All rights reserved
#Author: CompanyName
#------------------------------------------------------------------------------------------

import os,sys,re,subprocess
import configparser
import shutil
import xml.etree.ElementTree as ET
import copy
from ctypes import *
import time,hashlib,binascii
import datetime
from zlib import crc32
import struct
import json
#sys.path.append("../../build/")
sys.path.append(os.path.join(os.path.split(os.path.realpath(__file__))[0], '..'))
from scripts.analyze_dependence import user_pannel_config

#-----------------------------------Basic error definition-----------------------------------
EXT_ERR_SUCCESS=0
#-----------------------------------Basic type definition-----------------------------------
TD_CHAR=c_char
TD_S8=c_byte
TD_U8=c_ubyte
TD_S16=c_short
TD_U16=c_ushort
TD_S32=c_int
UAPI_U32=c_uint
#-----------------------------------NV structure definition-----------------------------------
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

class NV_ITEM_INDEX_STRU(Structure):
    _fields_ = [
        ("nv_id", TD_U8),
        ("nv_len", TD_U8),
        ("nv_offset", TD_U16),
        ]
NV_ITEM_INDEX_LEN=4
NV_ITEM_CRC_LEN=4

HNV_MAGIC=0x24564E48
FNV_MAGIC=0x23564E46

SEC_CONN_SALT="sec conn salt"
SEC_NV_ID=0x8c

###############################NV Partition header format ###############################################
# DW21 no longer reflects different partition headers such as Factory / Modem / APP in NV

###############################NV Header###############################################
# DW21 Each NV item (entry) has no header information


#-----------------------------------Generate HNV file process-----------------------------------
def nv_process_mk_section(element_root,src_bin_dic, is_factory):
    nv_id_list=[]
    nv_keep_id_list=[]
    nv_item_bin_list=[]
    nv_item_index_bin_list=[]
    nv_total_size=0
    nv_offset=0
    nv_len=0
    nv_total_num=0

    #1. Traverse, take out all NV IDs in the factory area or non-factory area, and sort
    for element_grp in element_root:
#        print("element_grp: ", element_grp)
#        print("NAME: ", element_grp.attrib['NAME'])
        if True == is_factory:
            if element_grp.attrib['NAME'] == 'Factory':
                for nv_element in element_grp:
                    nv_id_list.append(int(nv_element.attrib['ID'],16))
                break;
        else:
            if element_grp.attrib['NAME'] != 'Factory':
                for nv_element in element_grp:
                    nv_id_list.append(int(nv_element.attrib['ID'],16))
                    if element_grp.attrib['NAME'] == 'Keep':
                        nv_keep_id_list.append(int(nv_element.attrib['ID'],16))

    nv_id_list.sort()
    nv_keep_id_list.sort()
    nv_total_num = len(nv_id_list)
    nv_offset=sizeof(NV_FILE_STRU)+nv_total_num*4# The offset of NV from this area is initialized after the index area

    #print(nv_id_list)

    #2.Generate nv_item_bin (including nv_item header and nv_item content) in order of ID, and add it to nv_item_bin_list
    for nv_id in nv_id_list:
        # Apply for nv_item_bin space and convert to nv_item_st
        src_bin=src_bin_dic[hex(nv_id)]
        nv_len=len(src_bin)
        nv_item_bin=bytearray(nv_len+NV_ITEM_CRC_LEN)# Apply for space

        # Populate nv_item_bin and add nv_item_bin to nv_item_bin_list
        nv_item_bin[0:]=src_bin# Copy bin
        nv_item_bin[nv_len:]=struct.pack('<L', crc32(src_bin))
        nv_item_bin_list.append(nv_item_bin)

        # Cumulative total size of nv_item_bin
        nv_total_size=nv_total_size+nv_len+NV_ITEM_CRC_LEN# Calculate all nv length

        # NV Index
        nv_item_index_bin=bytearray(NV_ITEM_INDEX_LEN)
        nv_item_index_st=NV_ITEM_INDEX_STRU.from_buffer(nv_item_index_bin)
        nv_item_index_st.nv_id=nv_id
        nv_item_index_st.nv_len=nv_len
        nv_item_index_st.nv_offset=nv_offset
        nv_item_index_bin_list.append(nv_item_index_bin)
        nv_offset=nv_offset+nv_len+NV_ITEM_CRC_LEN


    #3.Generate section_bin
    # Apply for section_bin space
    section_bin=bytearray(nv_total_size)

    # Copy nv_item_bin to section_bin in turn
    offset=0
    for nv_item_bin in nv_item_bin_list:
        section_bin[offset:]=nv_item_bin
        offset=offset+len(nv_item_bin)

    #4.Generate index_bin
    index_bin=bytearray(NV_ITEM_INDEX_LEN*nv_total_num)
    offset=0
    for nv_item_index_bin in nv_item_index_bin_list:
        index_bin[offset:]=nv_item_index_bin
        offset=offset+NV_ITEM_INDEX_LEN

    return (EXT_ERR_SUCCESS,nv_total_num,section_bin,index_bin,nv_keep_id_list)



'''
Function: Generate hnv file
Parameter
element_root:nv root node
src_bin_dic:nv bin dictionary  {'0xf01':bytearray(...),....}
is_factory：Whether it is a factory area NV file
return value
nv_file_Bin: type bytearray nv file content
'''
def uapi_nv_process_mk_hnv(element_root,src_bin_dic, nv_file_name, is_factory):
    #print(src_bin_dic)
    global g_nv_env

    #2.Generate index_bin and section_bin
    # print("element_root: ", element_root)
    (ret,nv_total_num,section_bin,index_bin, nv_keep_id_list)=nv_process_mk_section(element_root,src_bin_dic, is_factory)
    if ret!=EXT_ERR_SUCCESS:
        return(ret,None)

    nv_name=''
    if is_factory==True:
        nv_name='Factory NV'
    else:
        nv_name='Normal NV'

    print("%s total num:%d"%(nv_name,nv_total_num))

    nv_index_total_size=len(index_bin)
    nv_bin_total_size=len(section_bin)

    # Apply for nv_file_Bin space, convert to nv_file_st structure and fill nv_file_st
    nv_file_Bin=bytearray(g_nv_env.flash_size)

    nv_file_st=NV_FILE_STRU.from_buffer(nv_file_Bin)

    # Fill nv_file_st
    if is_factory==True:
        nv_file_st.magic=FNV_MAGIC
    else:
        nv_file_st.magic=HNV_MAGIC
    nv_file_st.ver=0
    nv_file_st.head_len=sizeof(NV_FILE_STRU)#TODO:The fixed-length part of the header is temporarily not aligned with four bytes
    nv_file_st.total_num=nv_total_num
    nv_file_st.seq=0
    nv_file_st.ver_magic=g_nv_env.ver_magic
    nv_file_st.flash_size=g_nv_env.flash_size
    nv_file_st.reserve[0]=0
    nv_file_st.reserve[1]=0
    if(len(nv_keep_id_list)==0):
        nv_file_st.keep_id_range[0]=0
        nv_file_st.keep_id_range[1]=0
    else:
        nv_file_st.keep_id_range[0]=0x80
        nv_file_st.keep_id_range[1]=0x9f

    # Add index_bin behind the management area as the index of the management area
    offset=sizeof(NV_FILE_STRU)
    nv_file_Bin[offset:offset+nv_index_total_size]=index_bin
    offset=offset+nv_index_total_size

    # Add section_bin after the index of the management area
    nv_file_Bin[offset:offset+nv_bin_total_size]=section_bin
    offset=offset+nv_bin_total_size

    print("%s total size:%d"%(nv_name,offset))
    if offset > g_nv_env.flash_size:
        print("Error: NV is larger than flash_size:%d"%(g_nv_env.flash_size))
        exit(-1)

    # Set nv_flash free area to F
    for i in range(offset, g_nv_env.flash_size):
        nv_file_Bin[i]=0xFF

    # Calculate crc32 for the entire region
    nv_file_st.crc=crc32(nv_file_Bin[8:g_nv_env.flash_size])

    return (EXT_ERR_SUCCESS,nv_file_Bin)

class NvEnv:
    def __init__(self):
        self.app_name=[]# app name
        self.root_dir=[]# Compile the root directory
        self.mdm_xml=[]#mdm_xml Full path
        self.app_xml=[]#app_xml Full path
        self.app_sdk_level=[]# app sdk level
        self.app_cfg_dir=[]# Customized folder path with product form path
        self.src_bin_dir=[]# Original bin path, with product form path

        self.product_type=[]# Product Form Name
        self.temp_dir=[]# Path to temporary folder with product form
        self.combin_xml=[]# The full path of the merged xml, with or without product form
        self.nv_tool_dir=[]#
        self.nv_h_dir=[]#
        self.nv_bin_max_size=0
        self.flash_size=0
        self.ver_magic=0

g_nv_env = NvEnv()

def uapi_str_to_int(line,num):
    if num!=None:
        return int(line,num)
    match1=re.match(r'\s*0x',line)
    match2=re.match(r'\s*0X',line)
    if match1 or match2:
        return int(line,16)
    else:
        return int(line,10)

def uapi_nv_set_cfg(root_dir, product_lib_out, xml_hfile_dir, product_name, app_name, hnv_dir, flash_size, ver_magic):
    global g_nv_env
    global g_subver_name
    ret = 0
    subver = g_subver_name

    if hnv_dir==None:#hnv Storage location
        g_nv_env.hnv_dir=product_lib_out
    else:
        g_nv_env.hnv_dir=hnv_dir

    g_nv_env.nv_h_dir=xml_hfile_dir# Storage structure txt file storage location
    g_nv_env.app_name=app_name#app name
    g_nv_env.product_type=product_name# Product Form Name
    g_nv_env.root_dir=root_dir# Root directory
    g_nv_env.temp_dir=os.path.join(product_lib_out,app_name,subver)# Temporary file directory
    dependence = os.path.join(os.path.split(os.path.realpath(__file__))[0], '../dependence.xml')

    # root = ET.parse(os.path.join(root_dir,'build/dependence.xml')).getroot()
    root = ET.parse(dependence).getroot()
    project = list(filter(lambda x:x.attrib['name'].upper()==g_project_name.upper(),root.findall('PROJECT')))[0]
    for app in project.iter('APP'):
        if app.attrib['name'].lower() == app_name:
            for cfg in app.find('NV').iter('config'):
                if cfg.attrib['name'] == subver:
                    cfg_dir = cfg.attrib['app_nv_cfg_dir']
                    if g_dev_src == 0:
                        g_nv_env.mdm_xml = os.path.join(root_dir,cfg.attrib['main_scripts'])
                    else:
                        g_nv_env.mdm_xml = os.path.join(root_dir, 'code/config', cfg.attrib['main_scripts'])
                    g_nv_env.app_xml = os.path.join(root_dir,'app',app_name,'nv/app_nvi_db.xml')
                    product_cfg_dir = os.path.join(root_dir,cfg_dir,'cfg',product_name.lower())
                    if cfg_dir == '' or not os.path.exists(product_cfg_dir) or os.listdir(product_cfg_dir)==[]:
                        ret = 1
                    else:
                        g_nv_env.app_cfg_dir = os.path.join(root_dir,cfg_dir,'cfg',g_nv_env.product_type)
    g_nv_env.combin_xml=os.path.join(product_lib_out,app_name,'mss_nvi_db.xml')#combin.xml path after merging
    g_nv_env.combin_product_xml=os.path.join(g_nv_env.temp_dir,'mss_nvi_db_%s.xml'%(g_nv_env.product_type))#Combine_coo.xml path after merging
    g_nv_env.combin_product_security_xml=os.path.join(g_nv_env.temp_dir,'combin_%s_security.xml'%(g_nv_env.product_type))#Combined Combine_coo.xml path

    g_nv_env.src_bin_dir=os.path.join(root_dir,'app',g_nv_env.app_name,'nv','src_bin',g_nv_env.product_type)# Path to the source src bin file
    g_nv_env.nv_tool=os.path.join(root_dir,'tools','nv','cdbm')# nv tool path
    g_nv_env.pwd_nv_tool=os.path.join(root_dir,'tools','nv','MakePwdNVTool')#pwd nv production tool path
    g_nv_env.nv_bin_max_size=65536#Tentative support for up to 64K flash

    g_nv_env.flash_size=flash_size
    g_nv_env.ver_magic=ver_magic

    return ret

def uapi_nv_fomat_nv_id(root):
    ret = 0
    for grp in root:
        for nv in grp:
            if 'ID' in nv.attrib.keys():
                str_id=nv.attrib['ID']
                int_id=uapi_str_to_int(str_id,None)
                str_id=hex(int_id)
                nv.attrib['ID']=str_id
    return 0

"""
Merge xml; nv id standardized; nv sorted by group number
"""
def UAPI_NV_XmlCombineXml(src_xml_list,combin_xml):
    root_list=[]
    group_dics={}
    exist_item_id_list=[]
    global g_nv_env

    for src_xml in src_xml_list:
        tree = ET.parse(src_xml)
        root = tree.getroot()
        uapi_nv_fomat_nv_id(root)# Standardized NVID
        root_list.append(root)

    new_root=ET.Element('SourceData')

    # Determine if there are duplicate IDs
    for root in root_list:
        grp_list=root.findall('GROUP')# Find all grp
        for grp in grp_list:
            item_list=grp.findall('NV')
            for item in item_list:
                str_id=item.attrib['ID']
                int_id=uapi_str_to_int(str_id,None)
                if int_id in exist_item_id_list:
                    print(exist_item_id_list)
                    return -1
                exist_item_id_list.append(int_id)

    #Merge xml
    for root in root_list:
        grp_list=root.findall('GROUP')# Find all grp
        for grp in grp_list:
            grp_name=grp.attrib['NAME']
            if not grp_name in group_dics.keys():
                new_root.append(grp)
                group_dics[grp_name]=grp
            else:
                old_grp=group_dics[grp_name]
                sub_elements=grp.findall('NV')
                old_grp.extend(sub_elements)

    # Sort new xml
    element_grp_list=[]
    for element_grp in new_root:
        element_grp_list.append(element_grp)

    element_grp_list.sort(key=lambda x:int(x.attrib['ID'],16))
    i=0
    while i< len(element_grp_list):
        new_root[i]=element_grp_list[i]
        i=i+1

    # Write back xml
    new_tree=ET.ElementTree(new_root)
    new_tree.write(combin_xml,encoding="utf-8")

def uapi_nv_mk_temp_dir(temp_dir,hnv_dir):
    global g_nv_env
    if os.path.exists(temp_dir):
        shutil.rmtree(temp_dir)
        os.makedirs(temp_dir)
    else:
        os.makedirs(temp_dir)

    if hnv_dir == None:
        pass
    elif not os.path.exists(hnv_dir):
        os.makedirs(hnv_dir)
    else:
        pass

"""
Merge mss and app xml
"""
def uapi_nv_combine_mss_and_app_xml():
    global g_nv_env
    src_xml_list=[]
    src_xml_list.append(g_nv_env.mdm_xml)
    if os.path.exists(g_nv_env.app_xml):
        src_xml_list.append(g_nv_env.app_xml)
    print('src_xml_list:',src_xml_list)
    print('combin_xml:%s'%g_nv_env.combin_xml)
    UAPI_NV_XmlCombineXml(src_xml_list,g_nv_env.combin_xml)

def UAPI_NV_XmlSplitByProductType(src_xml,product_list,dst_xml_list):
    new_root_list=[]
    tree = ET.parse(src_xml)
    root = tree.getroot()
    i=0
    #print('src_xml',src_xml)
    #print('product_list',product_list)
    #print('dst_xml_list',dst_xml_list)
    while i<len(product_list):
        new_root=copy.deepcopy(root)
        for grp in new_root:
            del_nv_list=[]
            for nv in grp:
                list=nv.attrib['DEV'].lower().split('-')
                #print('list',list)
                if product_list[i] not in list:
                    del_nv_list.append(nv)
                else:
                    pass
                    #del nv.attrib['DEV']#Remove dev attribute
            for nv in del_nv_list:
                grp.remove(nv)
        new_tree=ET.ElementTree(new_root)
        new_tree.write(dst_xml_list[i],encoding="utf-8")
        i=i+1

def uapi_nv_split_by_product_type():
    global g_nv_env
    product_list=[]
    product_list.append(g_nv_env.product_type)
    dst_xml_list=[]
    dst_xml_list.append(g_nv_env.combin_product_xml)
    UAPI_NV_XmlSplitByProductType(g_nv_env.combin_xml,product_list,dst_xml_list)

def get_app_item_value(cfg_name):
    define = ''
    if g_nv_env.product_type.lower() == 'cco':
        define = 'PRODUCT_CFG_PRODUCT_TYPE_CCO'
    elif g_nv_env.product_type.lower() == 'sta':
        define = 'PRODUCT_CFG_PRODUCT_TYPE_STA'
    elif g_nv_env.product_type.lower() == 'ndm':
        define = 'PRODUCT_CFG_PRODUCT_TYPE_NDM'
    else:
        print('product_type ERR')
        sys.exit(1)

    app_xml = os.path.join(g_nv_env.root_dir,'app',app_name.lower(),'config.xml')
    root = ET.parse(app_xml).getroot()
    for module in root:
        if module.attrib['value'].lower() == 'yes':
            for dev in module.findall('DEV'):
                if dev.attrib['name'] == define:
                    for item in dev:
                        if item.attrib['name'] == cfg_name:
                            return item.attrib['value']

def UAPI_NV_Partition_Cfg(cfg_xml,id_list):
    partition_cfg_list = ['PARTITION_VER_CTL_SUPPORT_ENABLE', 'PARTITION_VER_CTL_NUM',
                          'PARTITION_VER_CTL_COMPATIBLE',
                          'PRODUCT_CFG_PARTITION_BOOT_ST_ADDR','PRODUCT_CFG_PARTITION_BOOT_SIZE',
                          'PRODUCT_CFG_PARTITION_FTM1_ST_ADDR','PRODUCT_CFG_PARTITION_FTM1_SIZE',
                          'PRODUCT_CFG_PARTITION_NV_FILE_ST_ADDR','PRODUCT_CFG_PARTITION_NV_FILE_SIZE',
                          'PRODUCT_CFG_PARTITION_KERNEL_ST_ADDR','PRODUCT_CFG_PARTITION_KERNEL_SIZE',
                          'PRODUCT_CFG_PARTITION_UPG_ST_ADDR','PRODUCT_CFG_PARTITION_UPG_SIZE',
                          'PRODUCT_CFG_PARTITION_NV_WORK_ST_ADDR','PRODUCT_CFG_PARTITION_NV_WORK_SIZE',
                          'PRODUCT_CFG_PARTITION_CRASH_ST_ADDR','PRODUCT_CFG_PARTITION_CRASH_SIZE',
                          'PRODUCT_CFG_PARTITION_USER_ST_ADDR','PRODUCT_CFG_PARTITION_USER_SIZE']
    root = ET.parse(cfg_xml).getroot()
    new_root = copy.deepcopy(root)
    for grp in new_root.findall('GROUP'):
        for nv in grp:
            id = hex(uapi_str_to_int(nv.attrib['ID'],None))
            if id in id_list:
                i = 0
                while i<len(partition_cfg_list):
                    define = partition_cfg_list[i]
                    i += 1
                    if define == 'PRODUCT_CFG_PARTITION_FTM1_SIZE':
                        value1 = uapi_str_to_int(get_app_item_value('PRODUCT_CFG_PARTITION_FTM1_SIZE'),None)//0x1000
                        value2 = uapi_str_to_int(get_app_item_value('PRODUCT_CFG_PARTITION_FTM2_SIZE'),None)//0x1000
                        define_value = value1 + value2
                        nv.attrib['PARAM_VALUE'] = nv.attrib['PARAM_VALUE'].replace(define,hex(define_value))
                    elif ((define == 'PARTITION_VER_CTL_SUPPORT_ENABLE') or (define == 'PARTITION_VER_CTL_NUM') or
                        (define == 'PARTITION_VER_CTL_COMPATIBLE')):
                        define_value = get_app_item_value(define)
                        nv.attrib['PARAM_VALUE'] = nv.attrib['PARAM_VALUE'].replace(define,define_value)
                    else:
                        define_value = uapi_str_to_int(get_app_item_value(define),None)//0x1000
                        nv.attrib['PARAM_VALUE'] = nv.attrib['PARAM_VALUE'].replace(define,hex(define_value))
    new_tree=ET.ElementTree(new_root)
    new_tree.write(cfg_xml,encoding="utf-8")


def nv_replace_val(root,str_id,str_val,bForce):
    #print(str_id,str_val)

    for grp in root:
        for nv in grp:
            if nv.attrib['ID']==str_id:
                if 0:#nv.attrib['SECURITY']=='PRIVATE':
                    return -1# Private NV does not allow customization
                else:
                    nv.attrib['PARAM_VALUE']=str_val

    return 0

def UAPI_NV_XmlModifyByCfg(cfg_file,src_xml,dst_xml):
    tree = ET.parse(src_xml)
    root = tree.getroot()
    str_id=''
    str_val=''

    with open(cfg_file) as cfg_fp:
        for line in cfg_fp.readlines():
            L=line.split('=')
            if len(L)==2:
                key=L[0].strip()
                val=L[1].strip()
                if key=='ID':
                    int_id=uapi_str_to_int(val,None)
                    str_id=hex(int_id)
                elif key=='PARAM_VALUE':
                    str_val=val
                    nv_replace_val(root,str_id,str_val,False)
                else:
                    pass

    new_tree=ET.ElementTree(root)
    new_tree.write(dst_xml,encoding="utf-8")


def UAPI_NV_Xml2Bin(nv_tool,src_xml,dst_bin_dir,include_dir):
    #cmd_line='%s %s %s %s'%(nv_tool,src_xml,include_dir,dst_bin_dir)
    cmd_line=[nv_tool,src_xml,include_dir,dst_bin_dir]
    #cmd_line=cmd_line+' >> '+r'F:\code\v20_0819\target\socbuilder\scripts\nv\log.txt'
    #print("cmd_line=",cmd_line)
    #ret=os.system(cmd_line)
    ret=subprocess.run(cmd_line, shell=False).returncode
    if ret!=0:
        print('error',cmd_line)
        sys.exit(ret)

def UAPI_NV_HNV_Build(src_xml,dst_hnv,bin_dir):
    global g_nv_env
    #print("src_xml=",src_xml)
    #print("dst_hnv=",dst_hnv)
    #print("bin_dir=",bin_dir)
    #input("continue")

    src_bin_dic={}
    tree = ET.parse(src_xml)
    root = tree.getroot()


    bin_file_list=os.listdir(bin_dir)

    #generate bin dictionary
    for bin_file in bin_file_list:
        if bin_file.endswith(".bin"):
            bin_id=bin_file[:-len('.bin')]
            bin_file_full_name=os.path.join(bin_dir,bin_file)
            with open(bin_file_full_name,'rb') as fp:
                bin=fp.read()
                src_bin_dic[bin_id]=bin
                #print(src_bin_dic)
                #input("continue")

    # Generate factory area NV file_factory.hnv
    (ret,bytearray_hnv)=uapi_nv_process_mk_hnv(root,src_bin_dic,os.path.basename(dst_hnv), True)
    factory_hnv = dst_hnv.replace('.hnv', '_factory.hnv')
    if len(bytearray_hnv)>=g_nv_env.nv_bin_max_size:# Detecting NV file size, borders are also problematic
        print(factory_hnv,"error nv size large than nv_bin_max_size(%u)"%(g_nv_env.nv_bin_max_size))
        exit(-1)
    if ret==EXT_ERR_SUCCESS:
        with open(factory_hnv,'wb+') as hnvfp:
            hnvfp.write(bytearray_hnv)

    # Generate non-factory area NV file_normal.hnv
    (ret,bytearray_hnv)=uapi_nv_process_mk_hnv(root,src_bin_dic,os.path.basename(dst_hnv), False)
    normal_hnv = dst_hnv.replace('.hnv', '_normal.hnv')
    if len(bytearray_hnv)>=g_nv_env.nv_bin_max_size:# Detecting NV file size, borders are also problematic
        print(normal_hnv,"error nv size large than nv_bin_max_size(%u)"%(g_nv_env.nv_bin_max_size))
        exit(-1)
    if ret==EXT_ERR_SUCCESS:
        with open(normal_hnv,'wb+') as hnvfp:
            hnvfp.write(bytearray_hnv)


def generate_pwd_nv(nv_bin_dir):
    BinFilePATH = os.path.join(nv_bin_dir, ''.join([hex(SEC_NV_ID),'.bin']))
    # 打开json文件, 获取密钥
    json_file = os.path.join(os.path.abspath(os.path.dirname(__file__)), r"passwd.json")
    if not os.path.isfile(json_file):
        print("passwd file not exist")
        sys.exit(1)
    with open(json_file, 'r') as file:
        key_dict = json.load(file)
    if 'key_list' not in key_dict.keys():
        print("passwd file is invalid")
        sys.exit(1)
    # bin文件第一部分 密钥
    key_list = key_dict['key_list']
    if len(key_list)<= 0:
        print("passwd file is empty")
        sys.exit(1)
    # bin文件第二部分盐值
    salt_list = []
    for i in range(0,20):
        salt_list.append(0)
    salt_key = '"' + SEC_CONN_SALT + '"'    
    i = 0
    for s in salt_key:
        salt_list[i] = ord(s)
        i += 1
    # bin文件第三部分, 盐值长度
    len_list = [0,0,0,0]
    len_list[0] = len(salt_key)
    # 生成.bin文件
    with open(BinFilePATH, "wb+") as file:
        file.write(bytes(key_list))
        file.write(bytes(salt_list))
        file.write(bytes(len_list))
    # 判断.bin文件是否成功生成
    try:
        f_exec = open(BinFilePATH, 'r')
        f_exec.close()
    except IOError:    
        print('error: key file not exit')
        sys.exit(ret)

def check_hnv_result(file_name):
    if os.path.exists(file_name.replace('.hnv', '_factory.hnv')):
        if os.path.exists(file_name.replace('.hnv', '_normal.hnv')):
            return 0
    return -1

def UAPI_NV_Xml_2_Hnv(xml_file,bin_dir,hnv_file):
    global g_nv_env
    if os.path.exists(bin_dir):
        shutil.rmtree(bin_dir)
        os.makedirs(bin_dir)
    else:
        os.makedirs(bin_dir)

    UAPI_NV_Xml2Bin(g_nv_env.nv_tool,xml_file,bin_dir,g_nv_env.nv_h_dir)
    # Replace 0x90 secure connection NV
    generate_pwd_nv(bin_dir)
    # Combine xml and bin files into hnv files
    UAPI_NV_HNV_Build(xml_file,hnv_file,bin_dir)

def uapi_nv_make_default_hnv():
    default_bin_dir = os.path.join(g_nv_env.temp_dir,'%s_default'%(g_nv_env.product_type))
    default_hnv_file = os.path.join(g_nv_env.hnv_dir,'%s_%s_%s.hnv'%(g_nv_env.app_name,g_subver_name,g_nv_env.product_type))
    UAPI_NV_Partition_Cfg(g_nv_env.combin_product_xml,['0x4','0x5'])
    UAPI_NV_Xml_2_Hnv(g_nv_env.combin_product_xml,default_bin_dir,default_hnv_file)
    ret = check_hnv_result(default_hnv_file)
    if ret!=0:
        print('Build hnv ERROR:%s'%default_hnv_file)
        sys.exit(ret)
    print('build hnv OK:%s'%default_hnv_file)


def uapi_nv_modify_by_cfg():
    print('uapi_nv_modify_by_cfg')
    global g_nv_env
    if os.path.exists(g_nv_env.app_cfg_dir):
        cfg_file_list=os.listdir(g_nv_env.app_cfg_dir)
    else:
        return 0
    for cfg_file in cfg_file_list:
        if cfg_file.endswith(".cfg"):
            cfg_file=cfg_file[:-len('.cfg')]
            cfg_file_full_name=os.path.join(g_nv_env.app_cfg_dir,'%s.cfg'%cfg_file)
            cfg_xml_full_name=os.path.join(g_nv_env.temp_dir,'%s.xml'%cfg_file)
            cfg_hnv_full_name=os.path.join(g_nv_env.hnv_dir,'%s_%s_%s.hnv'%(g_nv_env.app_name,g_subver_name,cfg_file))
            cfg_bin_dir=os.path.join(g_nv_env.temp_dir,cfg_file)

            UAPI_NV_XmlModifyByCfg(cfg_file_full_name,g_nv_env.combin_product_xml,cfg_xml_full_name)
            UAPI_NV_Partition_Cfg(cfg_xml_full_name,['0x4','0x5'])
            UAPI_NV_Xml_2_Hnv(cfg_xml_full_name, cfg_bin_dir, cfg_hnv_full_name)

            ret = check_hnv_result(cfg_hnv_full_name)
            if ret!=0:
                print('Build hnv ERROR:%s'%cfg_hnv_full_name)
                sys.exit(ret)
            print('build hnv OK:%s'%cfg_hnv_full_name)

def uapi_nv_mk_app_subtemp_dir(product_lib_out,app_name):
    if os.path.exists(product_lib_out):
        if os.path.exists(g_nv_env.temp_dir):
            shutil.rmtree(g_nv_env.temp_dir)
        else:
            os.makedirs(g_nv_env.temp_dir)
            print('make app_subver_temp_dir:',g_nv_env.temp_dir)

g_root_dir=sys.argv[1]#ver1
g_out_dir=sys.argv[2] #nv Compile result root directory，ver1/build_tmp/DW21_release
g_flash_size=int(sys.argv[3])
g_product_type = []
g_app_name = []
if (len(sys.argv) == 7):
    # g_diag_auth_pwd = sys.argv[4]
    g_dev_src = int(sys.argv[4])
    g_product_type.append(sys.argv[5])
    g_app_name.append(sys.argv[6])
else:
    g_dev_src = 0
g_ver_magic=0
# ver_magic It will be re-written when making hupg.
# The default value is 0 here. It can be used for upgrading NV scenes individually (there is no such scene normally). Fixed  assignment.
print(os.getcwd())
g_user_pannel_config = user_pannel_config()
g_project_name = g_user_pannel_config.get_project_name()
g_subver_name=''
def UAPI_NV_MakeMain(root_dir,product_lib_out,xml_hfile_dir,product_name,app_name,app_nv_list,hnv_dir,flash_size,ver_magic):
    global g_subver_name
    if app_nv_list == []:
        print(app_name.upper(),'selected 0 nv config!!!please select at least one!!!')
        sys.exit(1)
    for subver_name in app_nv_list:
        print('current subver_name:%s'%subver_name)
        g_subver_name=subver_name
        ret = uapi_nv_set_cfg(root_dir,product_lib_out,xml_hfile_dir,product_name,app_name,hnv_dir,flash_size,ver_magic)
        uapi_nv_mk_app_subtemp_dir(product_lib_out,app_name)
        uapi_nv_combine_mss_and_app_xml()
        uapi_nv_split_by_product_type()
        if ret != 0:
            uapi_nv_make_default_hnv()
        else:
            uapi_nv_modify_by_cfg()

if __name__ == '__main__':
    if len(g_product_type) == 0:
        product_name_list=g_user_pannel_config.get_dev_list()
    else:
        product_name_list = g_product_type
    if len(g_app_name) == 0:
        app_name_list = g_user_pannel_config.get_app_name(g_project_name)
    else:
        app_name_list = g_app_name
    print('product_name_list:',product_name_list)
    print('app_name_list:',app_name_list)
    for product_name in product_name_list:
        product_out_dir = os.path.join(g_out_dir,product_name)
        product_lib_out = os.path.join(product_out_dir,'nv')
        xml_hfile_dir = os.path.join(product_out_dir,'hdb/nv')
        hnv_dir = os.path.join(product_out_dir,'nv/hnv')
        uapi_nv_mk_temp_dir(product_lib_out, hnv_dir)
        #print(product_out_dir)
        for app_name in app_name_list:
            app_nv_list = g_user_pannel_config.get_nv_list(app_name)
            #print('app_subver_list:',app_subver_list)
            UAPI_NV_MakeMain(g_root_dir,product_lib_out,xml_hfile_dir,product_name,app_name,app_nv_list,hnv_dir, g_flash_size,g_ver_magic)



