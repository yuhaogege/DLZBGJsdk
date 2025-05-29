#!/usr/bin/env python3
# coding=utf-8

# -----------------------------------------------------------------------------------------
#Purpose:part of hupg build scripts
#Copyright CompanyNameMagicTag 2018-2019.All rights reserved
#Author: CompanyName
#------------------------------------------------------------------------------------------

import os, time, string, re ,shutil, time, hashlib, binascii, sys,subprocess
import copy  
import struct
from ctypes import *
from pkt_cfg_parser import PktCfgParser
from compress import encode_file_with_lzma1

import base64
import os

from Crypto import Random

from Crypto.Hash import SHA
from Crypto.Hash import SHA256

from Crypto.PublicKey import RSA

from Crypto.Signature import PKCS1_v1_5 as Signature_pkcs1_v1_5
from Crypto.Signature import PKCS1_PSS as Signature_pss

###############################Defining basic types###############################################

g_support_secure_upg=None
g_support_partition_info=None
g_root_dir=None 
  
TD_CHAR=c_char
TD_S8=c_byte
TD_U8=c_ubyte
TD_S16=c_short
TD_U16=c_ushort
TD_S32=c_int
UAPI_U32=c_uint

class UAPI_PARTITION_VER_S(Structure):
    _fields_ = [
        ("boot_support",UAPI_U32,1),
        ("reserve",UAPI_U32,10),
        ("ver_num",UAPI_U32,5),
        ("bit_map",UAPI_U32,16),
        ]

class uapi_upg_secure_info_s(Structure):
    _fields_ = [
        ("hash_alg",UAPI_U32,16),
        ("sign_alg",UAPI_U32,6),
        ("sign_param",UAPI_U32,10),
        ("signature_1",TD_U8*256),
        ("signature_2",TD_U8*256),
        ("key_n_2",TD_U8*256),
        ]
        
class uapi_21_upg_tail_s(Structure):
    _fields_ = [
        ("file_len",UAPI_U32),
        ("head_end_magic",UAPI_U32),
        ]
        
class uapi_21_upg_head_section_s(Structure):
    _fields_ = [
        ("attribute_compress",UAPI_U32,1),
        ("attribute_pad",UAPI_U32,31),
        ("offset",UAPI_U32),
        ("compress_size", UAPI_U32),
        ]
        
class uapi_21_upg_head_product_s(Structure):
    _fields_ = [
        ("min_id",TD_U16),
        ("max_id",TD_U16),
        ]

class uapi_21_upg_head_s(Structure):# Upgrade file structure
    _fields_ = [
        ("magic", UAPI_U32),
        ("check_sum", TD_U8*32),# Does not include check_sum itself
        ("file_len", UAPI_U32),
        ("ver_magic", UAPI_U32),
        ("attribute_pad", UAPI_U32),
        ("manufacture_code", TD_U16),
        ("struct_type", TD_U8),
        ("file_type", TD_U8),
        
        ("product_list", uapi_21_upg_head_product_s*2),
        
        ("section_count", TD_U8),
        ("pad", TD_U8*3),
        
        ("section_list_offset", UAPI_U32),
        ("section_list_size", UAPI_U32),
        
        ("secure_info_offset", UAPI_U32),
        ("secure_info_size", UAPI_U32),
        
        ("partition_info_offset", UAPI_U32),
        ("partition_info_size", UAPI_U32),
        ("partion_ver", UAPI_PARTITION_VER_S),
        ]

class uapi_start_tbl_section_s(Structure):
    _fields_ = [
        ("offset", UAPI_U32),
        ("size", UAPI_U32),
        ("check_sum", UAPI_U32),
        ]        
        
class uapi_start_tbl_s(Structure):# Startup configuration area structure
    _fields_ = [
        ("check_sum", UAPI_U32),# Does not include check_sum itself
        ("tbl_len", UAPI_U32),
        ("ver_magic", UAPI_U32),
        ("attribute_check_nvfile", UAPI_U32,1),
        ("attribute_check_nvfile_ver", UAPI_U32,1),
        ("attribute_pad_1", UAPI_U32,30),
        ("tbl_type",TD_U8),
        ("pad", TD_U8*3),
        ("info_2_kernel", UAPI_U32),
        ("secure_info_offset", UAPI_U32),
        ("secure_info_size", UAPI_U32),
        ("sctions", uapi_start_tbl_section_s*1),
        ]      
        
class UAPI_TEST_HUPG_HEAD(Structure):# Startup configuration area structure
    _fields_ = [
        ("magic", UAPI_U32),# Does not include check_sum itself
        ("len", UAPI_U32),
        ]         


def fake_random(x):
    bin=bytes(32)
    return bin

def lzma_compress_bin(src_file, dst_file):
    encode_file_with_lzma1(dst_file, src_file)

def print_secure_head(secure_head_bin):
    global g_support_secure_upg
    if g_support_secure_upg==1:
        st_secure_head=uapi_upg_secure_info_s.from_buffer(secure_head_bin)  
        print('[secure_head][hash_alg=0x%x][sign_alg=0x%x][sign_param=0x%x]'
            %(st_secure_head.hash_alg,st_secure_head.sign_alg,st_secure_head.sign_param))
        print('[secure_head][signature_1[0,1,254,255]]=[0x%x][0x%x][0x%x][0x%x]'
            %(st_secure_head.signature_1[0],st_secure_head.signature_1[1],st_secure_head.signature_1[254],st_secure_head.signature_1[255]))
        print('[secure_head][signature_2[0,1,254,255]]=[0x%x][0x%x][0x%x][0x%x]'
            %(st_secure_head.signature_2[0],st_secure_head.signature_2[1],st_secure_head.signature_2[254],st_secure_head.signature_2[255]))
        print('[secure_head][key_n_2[0,1,254,255]]=[0x%x][0x%x][0x%x][0x%x]'
            %(st_secure_head.key_n_2[0],st_secure_head.key_n_2[1],st_secure_head.key_n_2[254],st_secure_head.key_n_2[255]))

def print_upg_info(image_file,hupg_bin):
    log_hupg_tail_bin=hupg_bin[-8:]
    st_hupg=uapi_21_upg_head_s.from_buffer(hupg_bin)
    st_upg_tail=uapi_21_upg_tail_s.from_buffer(log_hupg_tail_bin)

    print("-------------%s image_info:"%(image_file))
    print("[check_sum[0,1,30,31]=[0x%x][0x%x][0x%x][0x%x]]"%(st_hupg.check_sum[0],st_hupg.check_sum[1],st_hupg.check_sum[30],st_hupg.check_sum[31]))
    print("[attribute_pad=0x%x]"%(st_hupg.attribute_pad))
    print("[file_len=0x%x][magic=0x%x][ver_magic=0x%x][manufacture_code=0x%x]"%(st_hupg.file_len,st_hupg.magic,st_hupg.ver_magic,st_hupg.manufacture_code))
    print("[struct_type=0x%x][file_type=0x%x][section_count=0x%x]"%(st_hupg.struct_type,st_hupg.file_type,st_hupg.section_count))
    print("[product_type[0x%x,0x%x]][product_type[0x%x,0x%x]]"%(st_hupg.product_list[0].min_id,st_hupg.product_list[0].max_id,st_hupg.product_list[1].min_id,st_hupg.product_list[1].max_id))
    print("[section_list_offset=0x%x][section_list_size=0x%x]"%(st_hupg.section_list_offset,st_hupg.section_list_size))
    print("[secure_info_offset=0x%x][secure_info_size=0x%x]"%(st_hupg.secure_info_offset,st_hupg.secure_info_size))
    print("[partition_info_offset=0x%x][partition_info_size=0x%x]"%(st_hupg.partition_info_offset,st_hupg.partition_info_size))
    print("[head_end_magic=0x%x][end_file_len=0x%x]"%(st_upg_tail.head_end_magic,st_upg_tail.file_len))
    print("[partion_ver][boot_support=0x%x][ver_num=0x%x][bit_map=0x%x][reserve=0x%x]"
        %(st_hupg.partion_ver.boot_support,st_hupg.partion_ver.ver_num,st_hupg.partion_ver.bit_map,st_hupg.partion_ver.reserve))
    # Print security head
    print_secure_head(hupg_bin[st_hupg.secure_info_offset   :  st_hupg.secure_info_offset+st_hupg.secure_info_size])

    section_list_offset=st_hupg.section_list_offset
    i=0
    while i<st_hupg.section_count:
        section_bin=hupg_bin[section_list_offset:section_list_offset+sizeof(uapi_21_upg_head_section_s)]
        st_section=uapi_21_upg_head_section_s.from_buffer(section_bin)
        print("[section%d][attribute_compress=0x%x][attribute_pad=0x%x][offset=0x%x][compress_size=0x%x]"
            %(i,st_section.attribute_compress,st_section.attribute_pad,st_section.offset,st_section.compress_size))
        section_list_offset+=sizeof(uapi_21_upg_head_section_s)    
        i=i+1
    

def print_tbl_info(image_file,kernel_bin):
    print("-------------%s image_info TBL:"%(image_file))
    st_tbl=uapi_start_tbl_s.from_buffer(kernel_bin)  

    print("[check_sum=0x%x][tbl_len=0x%x][ver_magic=0x%x][tbl_type=0x%x][info_2_kernel=0x%x]"
        %(st_tbl.check_sum,st_tbl.tbl_len,st_tbl.ver_magic,st_tbl.tbl_type,st_tbl.info_2_kernel))
    print("[attribute_check_nvfile=0x%x][attribute_check_nvfile_ver=0x%x][attribute_pad_1=0x%x]"
        %(st_tbl.attribute_check_nvfile,st_tbl.attribute_check_nvfile_ver,st_tbl.attribute_pad_1))
    print("[secure_info_offset=0x%x][secure_info_size=0x%x]"
        %(st_tbl.secure_info_offset,st_tbl.secure_info_size))   
    print("[section][offset=0x%x][size=0x%x][check_sum=0x%x]"
        %(st_tbl.sctions[0].offset,st_tbl.sctions[0].size,st_tbl.sctions[0].check_sum))   
    
    # Print security head
    print_secure_head(kernel_bin[st_tbl.secure_info_offset   :  st_tbl.secure_info_offset+st_tbl.secure_info_size])

def uapi_str_to_int(line,num):
    if num!=None:
        return int(line,num)
    match1=re.match(r'\s*0x',line)
    match2=re.match(r'\s*0X',line)
    if match1 or match2:
        return int(line,16)
    else:
        return int(line,10)
    
def str_product_2_int_product(str):
    str_list=str.split(',')
    return (uapi_str_to_int(str_list[0],None),uapi_str_to_int(str_list[1],None))  


def int_2_bin256(val):
    L=bytearray(256)
    i=255

    while i!=0:
        L[i]=val%256
        val=val//256
        i=i-1
    L[0]=val
    return (L)

    
def make_secure_head(root_dir,secure_head_type,content):#file_type EXT_SECURE_HEAD_TYPE_X
    global g_root_dir
    g_root_dir=root_dir
    
    first_key_dir= os.path.join(root_dir, 'tools', 'upg', 'master_private_1.pem')
    second_key_dir= os.path.join(root_dir, 'tools', 'upg', 'master_private_2.pem')
    
    with open(first_key_dir,'rb') as f:
        temp_bin = f.read()
        first_key = RSA.importKey(temp_bin)
        key_n_1=int_2_bin256(first_key.n)
        
    with open(second_key_dir,'rb') as f:
        temp_bin = f.read()    
        second_key = RSA.importKey(temp_bin)
        key_n_2=int_2_bin256(second_key.n)
    #signer = Signature_pss.new(first_key,randfunc=fake_random)        
    signer = Signature_pss.new(first_key)
    digest = SHA256.new()
    digest.update(bytes(key_n_2))
    signature_1 = signer.sign(digest)
    
    if secure_head_type==0:
        print("key_1[0x%x][0x%x][0x%x][0x%x]"%(key_n_1[0],key_n_1[1],key_n_1[254],key_n_1[255]))
        print("key_2[0x%x][0x%x][0x%x][0x%x]"%(key_n_2[0],key_n_2[1],key_n_2[254],key_n_2[255]))
        print("sig_1[0x%x][0x%x][0x%x][0x%x]"%(signature_1[0],signature_1[1],signature_1[254],signature_1[255]))
        
    #signer = Signature_pss.new(second_key,randfunc=fake_random)    
    signer = Signature_pss.new(second_key)
    digest = SHA256.new()
    digest.update(bytes(content))
    signature_2 = signer.sign(digest)
    
    # print("------------------secure_info------------------:")
    # print("key_n_2:",key_n_2)
    # print("signature_1:",signature_1)
    # print("signature_2:",signature_2)
    
    return (key_n_2,signature_1,signature_2)


def make_part_offset_size(temp_offset,bin_size):
    if bin_size!=0:
        section_offset=temp_offset[0]
        section_size=bin_size
    else:
        section_offset=0
        section_size=0
    
    temp_offset[0]=temp_offset[0]+section_size
    return (section_offset,section_size)

def make_upg_init_cfg(cfg_parser, image_file, section):
    global g_support_secure_upg
    global g_support_partition_info
    
    magic=cfg_parser.getint('MACRO','PRODUCT_CFG_CHIP_MAGIC')
    manufacture_code=cfg_parser.getint('MACRO','PRODUCT_CFG_MANUCODE')
    str_list_1=cfg_parser.get(section,'product_list_1')
    str_list_2=cfg_parser.get(section,'product_list_2')
    
    g_support_secure_upg = cfg_parser.option_is_enabled('MACRO','PRODUCT_CFG_SECURE_UPG')

    settings = cfg_parser.get_partition_table_settings();
    g_support_partition_info = settings['PRODUCT_CFG_UPG_CONTAIN_PARTITION_INFO']
    
    product_list_1=str_product_2_int_product(str_list_1)
    product_list_2=str_product_2_int_product(str_list_2)
    tmp_dir=os.path.dirname(image_file)
    tmp_dir=os.path.join(tmp_dir,'tmp')

    if(not os.path.exists(tmp_dir)):
        os.makedirs(tmp_dir)
    return (tmp_dir,product_list_1,product_list_2,magic,manufacture_code)

def make_ker_upg_init_file_name(kernel_file,normal_nv_file,tmp_dir):
    (kernel_base_name,str_temp)=os.path.splitext(os.path.basename(kernel_file))
    (nv_base_name,str_temp)=os.path.splitext(os.path.basename(normal_nv_file))
    normal_nv_z_file=os.path.join(tmp_dir,'%s.hnv.z'%nv_base_name)# NV compressed file path
    tbl_kernel_file = os.path.join(tmp_dir,'%s_tbl.bin'%kernel_base_name)# NV compressed file path
    tbl_kernel_z_file = os.path.join(tmp_dir,'%s_tbl.bin.z'%kernel_base_name)# NV compressed file path
    return (normal_nv_z_file,tbl_kernel_file,tbl_kernel_z_file)

def make_ker_upg_make_tbl_kernel_file(cfg_parser,root_dir,kernel_file,tbl_kernel_file,ver_magic,normal_nv_file):
    global g_support_secure_upg
    with open(kernel_file,'rb') as fp:
        kernel_bin=fp.read()

    with open(normal_nv_file,'rb') as fp:
        nv_bin=fp.read()

    if g_support_secure_upg==1:
        secure_info_offset=sizeof(uapi_start_tbl_s)
        secure_info_size=sizeof(uapi_upg_secure_info_s)
    else:
        secure_info_offset=0
        secure_info_size=0

    tbl_and_kernel_bin=bytearray(sizeof(uapi_start_tbl_s)+secure_info_size+len(kernel_bin))
    st_tbl=uapi_start_tbl_s.from_buffer(tbl_and_kernel_bin)
    st_tbl.ver_magic=ver_magic

    settings = cfg_parser.get_partition_table_settings()
    st_tbl.attribute_check_nvfile=settings['PRODUCT_CFG_START_TBL_CHECK_NV_FILE_CRC']

    st_tbl.attribute_check_nvfile_ver=settings['PRODUCT_CFG_START_TBL_CHECK_NV_VER_MAGIC']
        
    st_tbl.attribute_pad_1=0
    st_tbl.tbl_len=sizeof(uapi_start_tbl_s)+secure_info_size
    st_tbl.secure_info_offset=secure_info_offset
    st_tbl.secure_info_size=secure_info_size
    st_tbl.tbl_type=0
    st_tbl.pad[0]=0
    st_tbl.pad[1]=0
    st_tbl.pad[2]=0
    st_tbl.info_2_kernel=len(tbl_and_kernel_bin)
    st_tbl.sctions[0].offset=st_tbl.tbl_len
    st_tbl.sctions[0].size=len(kernel_bin)
    st_tbl.sctions[0].check_sum=binascii.crc32(kernel_bin)
    
    tbl_and_kernel_bin[st_tbl.tbl_len:]=kernel_bin
    print('INFO:',st_tbl.sctions[0].check_sum,binascii.crc32(tbl_and_kernel_bin[st_tbl.tbl_len:]))
    if g_support_secure_upg==1:
        secure_head_bin=bytearray(secure_info_size)
        st_secure_head=uapi_upg_secure_info_s.from_buffer(secure_head_bin)
        (key_n_2,signature_1,signature_2)=make_secure_head(root_dir,0,kernel_bin)
        st_secure_head.hash_alg=0
        st_secure_head.sign_alg=1
        st_secure_head.sign_param=0
        secure_head_bin[4:4+256]=signature_1
        secure_head_bin[260  :260+256]=signature_2
        secure_head_bin[516  :516+256]=key_n_2
        
        a1=st_tbl.secure_info_offset
        a2=(st_tbl.secure_info_offset+st_tbl.secure_info_size)
        #print('llllllll',a1,a2,secure_info_size,len(secure_head_bin),st_tbl.tbl_len)
        tbl_and_kernel_bin[a1:a2]=secure_head_bin

    print('INFO:',binascii.crc32(tbl_and_kernel_bin[st_tbl.tbl_len:]))
    
    st_tbl.check_sum=binascii.crc32(tbl_and_kernel_bin[4:st_tbl.tbl_len])
    
    with open(tbl_kernel_file,'wb+') as fp:
        fp.write(tbl_and_kernel_bin)
    return tbl_and_kernel_bin

    
def make_upg_only(cfg_parser,root_dir,file_type,section_bin_list,section_compress_list,image_file,magic,
    ver_magic,manufacture_code,product_list_1,product_list_2,partition_info_bin, role_type):
    global g_support_secure_upg
    global g_support_partition_info
    section_count=len(section_bin_list)
    section_offset_list=[]
    section_size_list=[]
    
    temp_offset=[0]

    pt_cfg = cfg_parser.get_partition_table_settings()
    ptb = pt_cfg[role_type.lower()]
    
    # hupg fixed head size offset
    (hupg_head_struct_offset,hupg_head_struct_size)=make_part_offset_size(temp_offset,sizeof(uapi_21_upg_head_s))
    
    # section description size offset
    (section_list_offset,section_list_size)=make_part_offset_size(temp_offset,section_count*sizeof(uapi_21_upg_head_section_s))
    
    # Partition table size offset
    if(g_support_partition_info==1) and partition_info_bin!=None:
        (partition_info_offset,partition_info_size)=make_part_offset_size(temp_offset,len(partition_info_bin)) 
    else:
        (partition_info_offset,partition_info_size)=make_part_offset_size(temp_offset,0) 
        

    # Safety header size offset
    if g_support_secure_upg==1:
        (secure_info_offset,secure_info_size)=make_part_offset_size(temp_offset,sizeof(uapi_upg_secure_info_s))
    else:
        (secure_info_offset,secure_info_size)=make_part_offset_size(temp_offset,0)
    

    
    # Individual image sizes and offsets
    i=0
    while i<section_count:
        #(section_offset_list[i],section_size_list[i]) = make_part_offset_size(temp_offset,len(section_bin_list[i]))

        (offset,size)= make_part_offset_size(temp_offset,len(section_bin_list[i]))
        section_offset_list.append(offset)
        section_size_list.append(size)
        i=i+1
    
    # padding field size and offset
    temp_size_1=temp_offset[0]
    temp_size_2=temp_size_1//4
    temp_size_2=temp_size_2*4
    padding_size=temp_size_1-temp_size_2
    if temp_size_1==temp_size_2:
        padding_size=0
    else:
        padding_size=temp_size_2+4-temp_size_1
        
    (padding_offset,padding_size)=make_part_offset_size(temp_offset,padding_size)#hupg nv part size
    
    # Upgrade tail size and offset
    (tail_head_offset,tail_head_size)=make_part_offset_size(temp_offset,sizeof(uapi_21_upg_tail_s))# Upgrade file tail size
    
    # Initialize hubg_bin
    hupg_bin=bytearray(tail_head_offset+tail_head_size)

    if ((role_type.lower() != 'boot') and ((len(hupg_bin) > (ptb['PRODUCT_CFG_PARTITION_UPG_SIZE'] + 4)))):
        sys.exit("[ERR]upg size>PRODUCT_CFG_PARTITION_UPG_SIZE FROM:%s"%os.path.realpath(__file__))  
    
    # Fill partition_info
    if(g_support_partition_info==1) and partition_info_bin!=None:
        hupg_bin[partition_info_offset:partition_info_offset+partition_info_size]=partition_info_bin
    # Fill the description header & content of the subsection
    
    i=0
    while i<section_count:
        hupg_bin[section_offset_list[i]:section_offset_list[i]+section_size_list[i]]=section_bin_list[i]
        #print(section_list_offset,(section_list_offset+sizeof(uapi_21_upg_head_section_s)*i),(section_list_offset+sizeof(uapi_21_upg_head_section_s)*(i+1)))
        temp_bin=hupg_bin[(section_list_offset+sizeof(uapi_21_upg_head_section_s)*i) :  (section_list_offset+sizeof(uapi_21_upg_head_section_s)*(i+1))]
        st_section=uapi_21_upg_head_section_s.from_buffer(temp_bin)
        st_section.attribute_compress=section_compress_list[i]
        st_section.attribute_pad=0
        st_section.offset = section_offset_list[i]
        st_section.compress_size = section_size_list[i]
        hupg_bin[section_list_offset+sizeof(uapi_21_upg_head_section_s)*i      :   section_list_offset+sizeof(uapi_21_upg_head_section_s)*(i+1)]=temp_bin
        i=i+1
        

    # Fill the HUPG non-full-text content check (CRC security header) field   
    hupg_head_bin=hupg_bin[hupg_head_struct_offset:hupg_head_struct_offset+hupg_head_struct_size]
    st_hupg=uapi_21_upg_head_s.from_buffer(hupg_head_bin)
    st_hupg.file_len=len(hupg_bin)
    st_hupg.magic=magic
    st_hupg.ver_magic=ver_magic
    st_hupg.attribute_pad=0
    st_hupg.manufacture_code=manufacture_code
    st_hupg.struct_type=0
    st_hupg.file_type=file_type
    st_hupg.product_list[0].min_id=product_list_1[0]
    st_hupg.product_list[0].max_id=product_list_1[1]
    st_hupg.product_list[1].min_id=product_list_2[0]
    st_hupg.product_list[1].max_id=product_list_2[1]
    st_hupg.section_count=section_count
    st_hupg.pad[0]=0
    st_hupg.pad[1]=0
    st_hupg.pad[2]=0

    st_hupg.section_list_offset=section_list_offset
    st_hupg.section_list_size=section_list_size
    st_hupg.secure_info_offset=secure_info_offset
    st_hupg.secure_info_size=secure_info_size
    st_hupg.partition_info_offset=partition_info_offset
    st_hupg.partition_info_size=partition_info_size 
    st_hupg.partion_ver.boot_support=ptb['boot_support']
    st_hupg.partion_ver.ver_num=ptb['ver_num']
    st_hupg.partion_ver.bit_map=ptb['bit_map_support']
    st_hupg.partion_ver.reserve=0
    
    hupg_bin[hupg_head_struct_offset:hupg_head_struct_offset+hupg_head_struct_size]=hupg_head_bin
    
    #print_upg_info(image_file,hupg_bin[hupg_head_struct_offset:hupg_head_struct_offset+hupg_head_struct_size])
    
    
    
    # Generate file tail
    upg_tail_bin=hupg_bin[tail_head_offset    :   tail_head_offset+tail_head_size]
    st_upg_tail=uapi_21_upg_tail_s.from_buffer(upg_tail_bin)
    st_upg_tail.head_end_magic=cfg_parser.getint('MACRO','PRODUCT_CFG_UPG_FILE_END_SIGNATURE')
    st_upg_tail.file_len=st_hupg.file_len
    hupg_bin[tail_head_offset    :   tail_head_offset+tail_head_size]=upg_tail_bin
    
    # Generate security header
    if g_support_secure_upg==1:
        (key_n_2,signature_1,signature_2)=make_secure_head(root_dir,1,hupg_bin[36:st_hupg.secure_info_offset]+hupg_bin[st_hupg.secure_info_offset+st_hupg.secure_info_size:st_hupg.file_len])
        secure_head_bin=bytearray(secure_info_size)
        st_secure_head=uapi_upg_secure_info_s.from_buffer(secure_head_bin)     
        st_secure_head.hash_alg=0
        st_secure_head.sign_alg=1
        st_secure_head.sign_param=0
        secure_head_bin[4:4+256]=signature_1
        secure_head_bin[260  :260+256]=signature_2
        secure_head_bin[516  :516+256]=key_n_2
        hupg_bin[secure_info_offset:secure_info_offset+secure_info_size] = secure_head_bin
    else:
        pass
    
    # Generate HASH check header
    sh = hashlib.sha256()
    sh.update(hupg_bin[36:st_hupg.file_len])
    hash_result=sh.digest()
    hupg_bin[4:36]=hash_result  
    #print('hash_result=[0x%X][0x%X][0x%X][0x%X]'%(hash_result[0],hash_result[1],hash_result[2],hash_result[3]))
    return hupg_bin

    
def make_hupg(root_dir,kernel_file, normal_nv_file,partition_bin_file, image_file,role_type,product_ver,ver_magic,app_name):
    global g_support_partition_info
    print("kernel_file=",type(kernel_file),kernel_file)
    print("normal_nv_file=",type(normal_nv_file),normal_nv_file)
    print("image_file=",type(image_file),image_file)
    print("role_type=",type(role_type),role_type)
    print("product_ver=",type(product_ver),product_ver)
    print("ver_magic=",type(ver_magic),ver_magic)
    cfg_parser = PktCfgParser(root_dir, app_name)
    cfg_parser.read()
    
    (tmp_dir,product_list_1,product_list_2,magic,manufacture_code)=make_upg_init_cfg(cfg_parser,image_file, role_type)
    (normal_nv_z_file,tbl_kernel_file,tbl_kernel_z_file)=make_ker_upg_init_file_name(kernel_file,normal_nv_file,tmp_dir)
    
    tbl_and_kernel_bin=make_ker_upg_make_tbl_kernel_file(cfg_parser,root_dir,kernel_file,tbl_kernel_file,ver_magic,normal_nv_file)
     
    lzma_compress_bin(tbl_kernel_file,tbl_kernel_z_file)
    lzma_compress_bin(normal_nv_file,normal_nv_z_file)
    
    with open(normal_nv_z_file,'rb') as fp:
        nv_z_bin=fp.read()
    
    with open(tbl_kernel_z_file,'rb') as fp:
        tbl_kernel_z_bin=fp.read()
    
    if (len(tbl_and_kernel_bin)>cfg_parser.get(role_type,'PRODUCT_CFG_PARTITION_KERNEL_SIZE')):
        sys.exit("[ERR]kernel size>PRODUCT_CFG_PARTITION_KERNEL_SIZE FROM:%s"%os.path.realpath(__file__))  
    
    section_bin_list=[]
    section_bin_list.append(tbl_kernel_z_bin)
    section_bin_list.append(nv_z_bin)
    
    section_compress_list = []
    section_compress_list.append(1)
    section_compress_list.append(1)
    
    hupg_bin=make_upg_only(cfg_parser,root_dir,1,section_bin_list,section_compress_list,image_file,magic,
        ver_magic,manufacture_code,product_list_1,product_list_2,None, role_type)
    
    print_upg_info(image_file,hupg_bin)
    print_tbl_info(image_file,tbl_and_kernel_bin)
    
    with open(image_file,'wb+') as fp:
        fp.write(hupg_bin)
    return

def make_hbin(root_dir,flash_boot_file, factory_nv_file, normal_nv_file, kernel_file, image_file, role_type, product_ver, ver_magic, app_name):
    print("make_hbin flash_boot_file=",flash_boot_file)
    print("make_hbin factory_nv_file=",factory_nv_file)
    print("make_hbin normal_nv_file=",normal_nv_file)
    print("make_hbin kernel_file=",kernel_file)
    print("make_hbin image_file=",image_file)
    print("make_hbin role_type=",role_type)
    print("make_hbin product_ver=",product_ver)
    print("make_hbin ver_magic=",ver_magic)
    cfg_parser = PktCfgParser(root_dir, app_name)
    cfg_parser.read()
    pt_cfg = cfg_parser.get_partition_table_settings()
    image_dir=os.path.dirname(image_file)
    tmp_dir=os.path.join(image_dir,'tmp')
    (kernel_base_name,str_temp)=os.path.splitext(os.path.basename(kernel_file))
    (nv_base_name,str_temp)=os.path.splitext(os.path.basename(normal_nv_file))

    tbl_kernel_file = os.path.join(tmp_dir,'%s_tbl.bin'%kernel_base_name)# NV compressed file path
    tbl_kernel_z_file = os.path.join(tmp_dir,'%s_tbl.bin.z'%kernel_base_name)# NV compressed file path

    ptl = pt_cfg[role_type.lower()]
    with open(flash_boot_file,'rb') as fp:
        flash_boot_bin=fp.read()
        if len(flash_boot_bin)>ptl['PRODUCT_CFG_PARTITION_BOOT_SIZE']:
            sys.exit("[ERR]boot size>PRODUCT_CFG_PARTITION_BOOT_SIZE FROM:%s"%os.path.realpath(__file__))

    with open(factory_nv_file,'rb') as fp:
        factory_nv_bin=fp.read()

    with open(normal_nv_file,'rb') as fp:
        normal_nv_bin=fp.read()

    with open(tbl_kernel_file,'rb') as fp:
        tbl_kernel_bin=fp.read()

    par_boot_st_addr=ptl['PRODUCT_CFG_PARTITION_BOOT_ST_ADDR']
    par_boot_size=ptl['PRODUCT_CFG_PARTITION_BOOT_SIZE']
    par_ftm1_st_addr=ptl['PRODUCT_CFG_PARTITION_FTM1_ST_ADDR']
    par_ftm1_size=ptl['PRODUCT_CFG_PARTITION_FTM1_SIZE']
    par_ftm2_st_addr=ptl['PRODUCT_CFG_PARTITION_FTM2_ST_ADDR']
    par_ftm2_size=ptl['PRODUCT_CFG_PARTITION_FTM2_SIZE']

    par_nv_file_st_addr=ptl['PRODUCT_CFG_PARTITION_NV_FILE_ST_ADDR']
    par_nv_file_size=ptl['PRODUCT_CFG_PARTITION_NV_FILE_SIZE']
    par_kernel_st_addr=ptl['PRODUCT_CFG_PARTITION_KERNEL_ST_ADDR']
    par_kernel_size=ptl['PRODUCT_CFG_PARTITION_KERNEL_SIZE']
    par_upg_st_addr=ptl['PRODUCT_CFG_PARTITION_UPG_ST_ADDR']
    par_upg_size=ptl['PRODUCT_CFG_PARTITION_UPG_SIZE']

    par_bin_total_size=par_boot_size+par_ftm1_size+par_ftm2_size+par_nv_file_size+len(tbl_kernel_bin)

    boot_nv_kernel_bin = bytearray(par_bin_total_size)
    boot_nv_kernel_bin[par_boot_st_addr:par_boot_st_addr+par_boot_size]=flash_boot_bin
    boot_nv_kernel_bin[par_ftm1_st_addr:par_ftm1_st_addr+par_ftm1_size]=factory_nv_bin
    boot_nv_kernel_bin[par_ftm2_st_addr:par_ftm2_st_addr+par_ftm2_size]=factory_nv_bin
    boot_nv_kernel_bin[par_nv_file_st_addr:par_nv_file_st_addr+par_nv_file_size]=normal_nv_bin
    boot_nv_kernel_bin[par_kernel_st_addr:par_kernel_st_addr+len(tbl_kernel_bin)]=tbl_kernel_bin

    print_tbl_info(image_file,bytearray(tbl_kernel_bin))

    with open(image_file,"wb+") as fp:
        fp.write(boot_nv_kernel_bin)
    return

def make_bootupg(root_dir,flash_boot_file, image_file, role_type, product_ver, ver_magic_not_use):
    #print('make_bootupg flash_boot_file: ',flash_boot_file)
    #print('make_bootupg image_file: ',image_file)
    #print('make_bootupg role_type: ',role_type)
    #print('make_bootupg product_ver: ',product_ver)
    #print('make_bootupg ver_magic: ',ver_magic)
    cfg_parser = PktCfgParser(root_dir, 'boot')
    cfg_parser.read(boot=True)
    pt_cfg = cfg_parser.get_partition_table_settings()
    ver_magic=cfg_parser.getint('MACRO','PRODUCT_CFG_BOOT_VER_MAGIC')
    (tmp_dir,product_list_1,product_list_2,magic,manufacture_code)=make_upg_init_cfg(cfg_parser,image_file, 'boot')
    
    tmp_dir=os.path.dirname(image_file)
    tmp_dir=os.path.join(tmp_dir,'tmp')


    with open(flash_boot_file,'rb') as fp:
        boot_bin=fp.read()
    
    if len(boot_bin)>pt_cfg['boot']['PRODUCT_CFG_PARTITION_BOOT_SIZE']:
        sys.exit("[ERR]boot size>PRODUCT_CFG_PARTITION_BOOT_SIZE FROM:%s"%os.path.realpath(__file__))
    
    section_bin_list=[]
    section_bin_list.append(boot_bin)
    
    section_compress_list = []
    section_compress_list.append(0)
    
    hupg_bin=make_upg_only(cfg_parser, root_dir, 3, section_bin_list, section_compress_list, image_file, magic,
        ver_magic, manufacture_code, product_list_1, product_list_2, None, 'boot')
    
    
    print_upg_info(image_file,hupg_bin)
    with open(image_file,'wb+') as fp:
        fp.write(hupg_bin)
    return
    

def make_partion_upg(root_dir,partition_bin_file, image_file,role_type,product_ver,ver_magic, app_name): 
    cfg_parser = PktCfgParser(root_dir, app_name)
    cfg_parser.read()
    (tmp_dir,product_list_1,product_list_2,magic,manufacture_code)=make_upg_init_cfg(cfg_parser,image_file, role_type)
    
    tmp_dir=os.path.dirname(image_file)
    tmp_dir=os.path.join(tmp_dir,'tmp')

    with open(partition_bin_file,'rb') as fp:
        partition_info_bin=fp.read()
    
    section_bin_list=[]
    section_compress_list = []

    
    hupg_bin=make_upg_only(cfg_parser, root_dir, 4, section_bin_list, section_compress_list, image_file, magic,
        ver_magic, manufacture_code, product_list_1, product_list_2, partition_info_bin, role_type)
    
    print_upg_info(image_file,hupg_bin)
    with open(image_file,'wb+') as fp:
        fp.write(hupg_bin)
    return
	
g_test_root_dir=r'F:\code\upg_test_3' 
g_test_image_dir=os.path.join(g_test_root_dir,r'target\delivery\DW21_Release\mk_image\image') 
g_test_src_dir=os.path.join(g_test_root_dir,r'target\delivery\DW21_Release\mk_image\src')

def test_make_hupg(ver_magic,product_ver,image_name_only):
    kernel_file= os.path.join(g_test_src_dir,r'dw21_CCO_demo.bin')
    normal_nv_file= os.path.join(g_test_src_dir,r'cco_demo_normal.hnv')
    partition_bin_file= os.path.join(g_test_src_dir,r'cco_flash_partition_table.bin')
    image_file= os.path.join(g_test_image_dir,image_name_only)
    role_type='cco'
             
    make_hupg(g_test_root_dir,kernel_file, normal_nv_file,partition_bin_file,image_file,role_type,product_ver,ver_magic)

def test_make_bootupg(ver_magic,product_ver,image_name_only):
    flash_boot_file=os.path.join(g_test_src_dir,r'dw21_flash_boot_secure_flash.bin')
    image_file=os.path.join(g_test_image_dir,image_name_only)
    role_type='boot'
    make_bootupg(g_test_root_dir,flash_boot_file, image_file, role_type, product_ver, ver_magic)

def test_make_hbin(ver_magic,product_ver,image_name_only):
    flash_boot_file=os.path.join(g_test_src_dir,r'dw21_flash_boot_secure_flash.bin')
    factory_nv_file= os.path.join(g_test_src_dir,r'cco_demo_factory.hnv')
    normal_nv_file= os.path.join(g_test_src_dir,r'cco_demo_normal.hnv')
    kernel_file= os.path.join(g_test_src_dir,r'dw21_CCO_demo.bin')
    image_file= image_file= os.path.join(g_test_image_dir,image_name_only)
    role_type='cco'
    make_hbin(g_test_root_dir,flash_boot_file, factory_nv_file, normal_nv_file, kernel_file, image_file, role_type, product_ver, ver_magic, 'demo')

def test_make_hbin_with_hupg_phase_1(end_addr,hbin_dir,hupg_dir,image_dir):
    with open(hbin_dir,'rb') as fp:
        hbin_bin=fp.read()
    
    with open(hupg_dir,'rb') as fp:
        hupg_bin=fp.read()
    
    print("total_size=0x%08x"%(len(hbin_bin)+len(hupg_bin)))
    
    temp_bin=bytearray(end_addr)
    temp_bin[0:len(hbin_bin)]=hbin_bin
    temp_bin[end_addr-len(hupg_bin):end_addr]=hupg_bin
    
    with open(image_dir,'wb+') as fp:
        fp.write(temp_bin)
    
def test_make_hbin_with_hupg(hbin_name,hupg_name,hbin_hupg_bin):
    hbin_dir=os.path.join(g_test_image_dir,hbin_name)
    hupg_dir=os.path.join(g_test_image_dir,hupg_name)
    image_dir=os.path.join(g_test_image_dir,hbin_hupg_bin)
    test_make_hbin_with_hupg_phase_1(0x70000,hbin_dir,hupg_dir,image_dir)
    
import random    
if __name__ == '__main__': 
    random_magic=1
    if random_magic==1:
        ver_magic=random.randint(1,0x12345678)
    else:
        ver_magic=0x11111111
        
    product_ver= r'V100R001C00B021'
    
    test_name_1_kernel_hupg="test_1_kernel.hupg"
    test_name_1_boot_hupg="test_1_boot.hupg"
    test_name_1_hbin="test_1.hbin"
    test_make_hupg(ver_magic,product_ver,test_name_1_kernel_hupg)
    test_make_bootupg(ver_magic,product_ver,test_name_1_boot_hupg)
    test_make_hbin(ver_magic,product_ver,test_name_1_hbin)
    
    
    
    if random_magic==1:
        ver_magic=random.randint(1,0x12345678)
    else:
        ver_magic=0x22222222
    product_ver= r'V100R001C00B022'  
    
    test_name_2_kernel_hupg="test_2_kernel.hupg"
    test_name_2_boot_hupg="test_2_boot.hupg"
    test_name_2_hbin="test_2.hbin"
    test_make_hupg(ver_magic,product_ver,test_name_2_kernel_hupg)
    test_make_bootupg(ver_magic,product_ver,test_name_2_boot_hupg)
    test_make_hbin(ver_magic,product_ver,test_name_2_hbin)
    
    test_make_hbin_with_hupg(test_name_1_hbin,test_name_2_kernel_hupg,'burn_1hbin_2kerupg.hbin')
    #test_make_hbin_with_hupg(test_name_1_hbin,test_name_2_boot_hupg,'burn_1hbin_2bootupg.hbin')
