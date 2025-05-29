#!/usr/bin/env python3
# coding=utf-8
# -----------------------------------------------------------------------------------------
#Purpose:
#Copyright CompanyNameMagicTag 2018-2019.All rights reserved
#Author: CompanyName
#------------------------------------------------------------------------------------------

import os, time, string, re ,shutil, time, hashlib, binascii, sys
g_src_dir = sys.argv[1]
g_dst_dir = sys.argv[2]
g_base_datatype_def_txt=sys.argv[3]
g_first_line = r'#include "base_datatype_def.txt"'
#print(g_src_dir)
#print(g_dst_dir)
#print('hhhhhhhh',sys.argv[3])


# txt filtering
def c_2_txt_file(src_file,dst_file):
    with open(src_file, 'r') as f1,open(dst_file, 'w') as f2:
        f2.write(g_first_line)	
        for line in f1:
            if(re.match(r"[ ]*#",line)):
                pass
            elif(re.match(r"[ ]*EXT_EXTERN",line)): 	
                pass
            elif(re.match(r"[ ]*EXT_PRV",line)): 	
                pass	
            elif(re.match(r"[ ]*EXT_EAPI",line)): 	
                pass	
            elif(re.match(r"[ ]*extern",line)): 	
                pass				
            else:
                f2.write(line)	

# Traversing files
def c_2_txt_dir(src_dir,dst_dir):
    src_dir_list = os.listdir(src_dir)	
    for x in src_dir_list:
        src_file = '%s/%s'%(src_dir,x)
        dst_file = '%s/%s'%(dst_dir,x)
        c_2_txt_file(src_file,dst_file)
# Start	
c_2_txt_dir(g_src_dir,g_dst_dir)
shutil.copy(g_base_datatype_def_txt,g_dst_dir) 
