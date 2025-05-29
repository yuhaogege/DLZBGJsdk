#!/usr/bin/env python3
# coding=utf-8
# -----------------------------------------------------------------------------------------
#Purpose:main build scripts
#Copyright CompanyNameMagicTag 2018-2019.All rights reserved
#Author: CompanyName
#------------------------------------------------------------------------------------------

# All modules are compiled by default.
# Incremental compilation is supported only during the development phase.

import os
import re
import sys
import shutil
import subprocess
import xml.etree.ElementTree as ET
from copy import deepcopy
from functools import reduce
from scripts.analyze_dependence  import user_pannel_config
from build_utils import shell

RELATIVE_DIR = '../'
g_cfg_xml_list = []
g_app_define = []
g_out_ver = 'release'

def list_num(dict, ignore_key):
    #print(dict)
    list = []
    count = 0
    for key in dict.keys():
        if key == ignore_key:
            list.append(dict[key].split(','))
        else:
            value = dict[key].split(',')
            for min,max in zip(value[::2], value[1::2]):
                x=min
                while int(x) <= int(max):
                    list.append(x)
                    x = str(int(x) + 1)
    return list
def find_ifdef(file):
    dict = {}
    with open(file,'r') as fp:
        line_num = 0
        define = ''
        for line in fp.readlines():
            line_num += 1
            if line.startswith('#ifdef'):
                define = line.strip()[len('#ifdef '):].split(' ')[0].strip()
                if define not in dict:
                    dict[define] = str(line_num)
                else:
                    dict[define] = ''.join([dict[define], ',', str(line_num)])
            elif line.startswith('#endif') and define != '':
                dict[define] = ''.join([dict[define], ',', str(line_num)])
                define = ''
    return dict

def seperate_define_by_app(app_define, head_file):
    tmp_head_file = os.path.join(RELATIVE_DIR,'build_tmp/config/%s.h'%app_define.lower())
    with open(head_file,'r') as h_fp,open(tmp_head_file,'w') as tmp_fp:
        define = ''
        tmp_str = ''
        wr_flag = False
        wr_sem = 0
        others_def = False
        oth_sem = 0
        for line in h_fp.readlines():
            if line.startswith('#ifdef'):
                define = line.strip()[len('#ifdef '):].split(' ')[0].strip()
                if define == app_define:
                    wr_flag = True
                    wr_sem += 1
                elif define in g_app_define:
                    others_def = True
                    oth_sem += 1
                elif others_def == True:
                    oth_sem += 1
                else:
                    tmp_str = ''.join([tmp_str,line])
                    if wr_flag == True:
                        wr_sem += 1
            elif line.startswith('#endif'):
                if others_def == True and oth_sem > 0:
                    if oth_sem == 1:
                        others_def = False
                    oth_sem -= 1
                elif wr_flag == True and wr_sem > 0:
                    if wr_sem > 1:
                        tmp_str = ''.join([tmp_str, line])
                    elif wr_sem == 1:
                        wr_flag = False
                    wr_sem -= 1
                else:
                    tmp_str = ''.join([tmp_str, line])
            else:
                if others_def == True:
                    pass
                else:
                    tmp_str = ''.join([tmp_str, line])
        tmp_fp.write(tmp_str)
    return tmp_head_file

def is_repeat_def_in_hfile(head_file):
    for app_define in g_app_define:
        # Strip other apps, leaving only the macros defined by the designated app
        tmp_head_file = seperate_define_by_app(app_define,head_file)
        # Find all ifdef (CCO / STA / NDM)
        def_line_dict = find_ifdef(tmp_head_file)
        for key in def_line_dict.keys():
            line_num = 0
            num_list = list_num(def_line_dict,key)
            keywords = []
            lines = []
            with open(tmp_head_file,'r') as fp:
                lines = fp.readlines()
            for line in lines:
                line_num += 1
                if str(line_num) in num_list:
                    pass
                elif line.strip().startswith('#define'):
                    keyword = line.strip()[len('#define'):].strip().split(' ')[0].strip()
                    if keyword in keywords:
                        print('error: definition repeat [%s]!'%keyword)
                        os.remove(tmp_head_file)
                        return 1
                    else:
                        keywords.append(keyword)
        os.remove(tmp_head_file)
    return 0

def replace_dollar_symbol(items,item_value):
    while '$' in item_value:
        dollar_value = item_value[item_value.find('$(')+2:item_value.find(')')]
        for item in items:
            if item.attrib['name'] == dollar_value:
                item_value = item_value.replace('$(%s)'%dollar_value,item.attrib['value'])
                break
    return item_value

def item_to_str(list,name,value):
    str = ''
    if value.lower()=='no': # item which is no is not added to the file
        str = '#undef %s\n'%(name)
    elif value.lower()=='yes':
        str = '#define %s\n'%(name)
    elif '$' in value:
        str = '#define %s %s\n'%(name, replace_dollar_symbol(list, value))
    else:
        str = '#define %s %s\n'%(name, value)
    return str

def only_value_different(module, differ):
    src_attrib = deepcopy(module.attrib)
    dif_sttrib = deepcopy(differ.attrib)
    if 'value' in src_attrib:
        src_attrib['value'] = ''
        dif_sttrib['value'] = ''
    return src_attrib == dif_sttrib

def only_value_different_in_all(module, differ):
    if only_value_different(module, differ):
        for dif_ele in differ:
            src_ele = get_module(dif_ele,module)
            if src_ele == None and dif_ele.get('value') != 'no':
                print('item not exists')
                return False
            elif src_ele == None and dif_ele.get('value') == 'no':
                continue
            elif list(dif_ele) != []:
                ret = only_value_different_in_all(src_ele,dif_ele)
                if ret == False:
                    return ret
            elif only_value_different(src_ele,dif_ele) == False:
                return False
        return True
    else:
        return False

def get_module(module, modules):
    for ele in modules:
        if module.attrib['name'] == ele.attrib['name']:
            return ele
    return None

def merge_default_and_differ_module(module, differ):
    for item in module:
        dif_item = get_module(item, differ)
        if list(item) == [] and dif_item != None:
            item.attrib = dif_item.attrib
        elif dif_item != None:
            item = merge_default_and_differ_module(item, dif_item)
    return module

def merge_default_and_differ_xml(dst_cfg_file, cfg_modules, diff_modules):
    new_root = ET.Element('CONFIG')
    for module in cfg_modules:
        # 1. Add the difference directly to the xml
        diff = get_module(module, diff_modules)
        if diff == None:
            new_root.append(module)
        elif only_value_different(module, diff) == False:
            print('only value can be different: module[%s]'%module.attrib['name'])
            return 1
        elif diff.attrib['value'].lower()=='no':
            module.clear()
            module.attrib = diff.attrib
            new_root.append(module)
        elif len(diff) > len(module) or only_value_different_in_all(module,diff) == False:
            print('differ items is much more than default config item OR not only value is different: module[%s]'%module.attrib['name'])
            return 1
        elif list(diff)==[]:
            new_root.append(diff)
        else:
            new_root.append(merge_default_and_differ_module(module,diff))
    new_tree = ET.ElementTree(new_root)
    new_tree.write(dst_cfg_file,encoding='utf-8')
    return 0

def is_differ_exists_redundant_module(cfg_modules, diff_modules):
    for diff in diff_modules:
        default = get_module(diff, cfg_modules)
        # Modules with value = 'yes' in different configuration items
		# that do not exist in the default configuration are not allowed
        if default == None and diff.attrib['value'].lower() == 'yes':
            print('module:[%s] NOT EXISTS!!!'%diff.attrib['name'])
            return True
    return False

def merge_all_default_cfg(dst_cfg_file):
    new_root=ET.Element('CONFIG')
    for cfg_xml in g_cfg_xml_list:
        tree = ET.parse(cfg_xml)
        #print('&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&',cfg_xml)
        root = tree.getroot()
        module_list = root.findall('MODULE')
        for module in module_list:
            new_root.append(module)
    new_tree = ET.ElementTree(new_root)
    new_tree.write(dst_cfg_file, encoding='utf-8')

def create_temp_config(dst_cfg_file):
    # Merge all default configurations in the project directory into one file
    merge_all_default_cfg(dst_cfg_file)
    differ_xml = os.path.join(RELATIVE_DIR,'target/config/product','%s.config.xml'%g_user_pannel_config.project_name.lower())
    # If there is no difference configuration file, the file merged with the default configuration
    if not os.path.exists(differ_xml):
        return 0

    diff_modules = ET.parse(differ_xml).getroot().findall('MODULE')
    cfg_modules = ET.parse(dst_cfg_file).getroot().findall('MODULE')
    exist = is_differ_exists_redundant_module(cfg_modules,diff_modules)
    if exist == True:
        return 1
    return merge_default_and_differ_xml(dst_cfg_file, cfg_modules, diff_modules)

def extract_from_module(module):
    str_to_head = '#define %s\n'%(module.attrib['name'])
    str_to_mk = '%s := %s\n'%(module.attrib['name'], module.attrib['value'].lower())
    for item in module:
        items = module.findall('item')
        if item.tag == 'item':
            name = item.attrib['name']
            value = item.attrib['value']
            if name == 'PRODUCT_CFG_BUILD_RELEASE' and value == 'no':
                g_out_ver = 'debug'

            if item.attrib['type'] == '0x2':
                str_to_mk = ''.join([str_to_mk, name, ' := ', value, '\n']) if name != 'EXT_DIAG_AUTH_PASSWORD' else str_to_mk
                str_to_head = ''.join([str_to_head, item_to_str(items, name, value)])
            elif item.attrib['type'] == '0x1':
                str_to_head = ''.join([str_to_head, item_to_str(items, name, value)])
            elif item.attrib['type'] == '0x0':
                str_to_mk = ''.join([str_to_mk, name, ' := ', value, '\n']) if name != 'EXT_DIAG_AUTH_PASSWORD' else str_to_mk
            else:
                pass
        elif item.tag == 'DEV':# DEV under item is no longer written to mk file, only to h file
            str_to_head = ''.join([str_to_head,'#ifdef %s\n'%item.attrib['name']])
            for dev_item in item:
                str_to_head = ''.join([str_to_head, item_to_str(items,dev_item.attrib['name'], dev_item.attrib['value'])])
            str_to_head = ''.join([str_to_head,'#endif\n'])
        else:
            pass
    return str_to_mk,str_to_head

def create_reference_files(src_cfg_file, dst_mk_file, dst_head_file):
    with open(dst_mk_file, 'a') as mk, open(dst_head_file, 'a') as head:
        module_list = ET.parse(src_cfg_file).getroot().findall('MODULE')
        str_to_mk = ''
        str_to_head = ''
        for module in module_list:
            if module.attrib['name'].lower()=='dev_select':
                continue

            if module.attrib['value'].lower()=='yes':
                module_path = ''
                if 'path' in module.attrib:
                    module_path = module.attrib['path'].replace('\\','/')

                if module_path and module_path.startswith(r'app/'):
                    app_name = module_path.replace('app/','').replace('/','').upper()
                    g_app_define.append('SOC_%s_CONFIG'%app_name)
                    mk_str,head_str = extract_from_module(module)
                    str_to_mk = ''.join([str_to_mk,mk_str])
                    str_to_head = ''.join([str_to_head,'#ifdef SOC_%s_CONFIG\n'%app_name, head_str, '#endif\n'])
                else:
                    mk_str,head_str = extract_from_module(module)
                    str_to_mk = ''.join([str_to_mk, mk_str])
                    str_to_head = ''.join([str_to_head, head_str])
            else:
                str_to_mk = ''.join([str_to_mk, module.attrib['name'], ' := ', module.attrib['value'].lower(), '\n'])
                str_to_head = ''.join([str_to_head, '#undef ', module.attrib['name'], '\n'])
        mk.write(str_to_mk)
        head.write(str_to_head)

def find_config_files(root, target):
    del g_cfg_xml_list[:]
    for dir_path,dir_names,file_names  in os.walk(root):
        # print('dir_path = %s'%dir_path)
        # print('dir_names = %s'%dir_names)
        # print('file_names = %s'%file_names)
        if '.svn' in dir_path:
            continue
        if os.path.abspath(os.path.join(root, 'output')) in os.path.abspath(dir_path):
            continue
        for file_name in file_names:
            if file_name == target:
                path = os.path.join(dir_path, file_name)
                g_cfg_xml_list.append(path)
    g_cfg_xml_list.sort()
    print(g_cfg_xml_list)

def mkdir(path):
    path=path.strip()
    path=path.rstrip("\\")
    isExists=os.path.exists(path)
    if not isExists:
        os.makedirs(path)
        print ('%s create success'% path)
        return True
    else:
        # If the directory exists, it will not be created, and it will prompt that the directory already exists
        #print ('%s is exist'% path)
        return False


def prepare(dir_name):
    main_build_tmp = os.path.join(dir_name, 'build_tmp', 'config')
    mkdir(main_build_tmp)
    main_output = os.path.join(dir_name, 'output')
    mkdir(main_output)


def clear_exist_file(file_list):
    for file in file_list:
        if os.path.exists(file):
            os.remove(file)
def prepare_config_file():
    global g_user_pannel_config
    build_root=os.getcwd()
    project_root=os.path.abspath(os.path.dirname(build_root))
    prepare(project_root)
    temp_config = os.path.join(RELATIVE_DIR, 'build_tmp', 'config', 'temp.config.xml')
    mk_file = os.path.join(RELATIVE_DIR,'build_tmp', 'config', 'soc_product_config.mk')
    head_file = os.path.join(RELATIVE_DIR,'build_tmp', 'config', 'soc_product_config.h')

    dependence = os.path.join(build_root,'dependence.xml')
    modules = ET.parse(dependence).getroot().findall('MODULE')

    clear_exist_file([temp_config, mk_file, head_file])
    del g_app_define[:]
    # Find all files and merge them into one file.
    find_config_files(project_root, 'config.xml')
    ret = create_temp_config(temp_config)
    if ret!=0:
        return ret

    with open(head_file, 'w') as head:
        head.write('#ifndef __SOC_PRODUCT_CONFIG_H__\n#define __SOC_PRODUCT_CONFIG_H__\n\n')
        head.write('#ifdef HB_IMAGE_CCO\n#define PRODUCT_CFG_PRODUCT_TYPE_CCO\n#endif\n')
        head.write('#ifdef HB_IMAGE_STA\n#define PRODUCT_CFG_PRODUCT_TYPE_STA\n#endif\n')
        head.write('#ifdef HB_IMAGE_NDM\n#define PRODUCT_CFG_PRODUCT_TYPE_NDM\n#endif\n\n')
    create_reference_files('dependence.xml', mk_file, head_file)
    create_reference_files(temp_config, mk_file, head_file)
    with open(head_file, 'a') as head:
        head.write('#endif\n')
    ret = is_repeat_def_in_hfile(head_file)
    return ret


def del_file(path):
    if not os.path.exists(path):
        return 0
    ls = os.listdir(path)
    for i in ls:
        c_path = os.path.join(path, i)
        if i == '.svn':
            pass
        elif os.path.isdir(c_path):
            del_file(c_path)
        else:
            try:
                os.remove(c_path)
            except PermissionError:
                print ('HB BUILD clean %s  %s' % (c_path,r'[FAILURE]'))
                pass

def clean_all():
    build_root=os.getcwd()
    project_root=os.path.abspath(os.path.dirname(build_root))
    del_file(os.path.join(project_root, 'build_tmp'))
    del_file(os.path.join(project_root, 'output'))
    try:
        shutil.rmtree(os.path.join(project_root, 'build_tmp'))
    except FileNotFoundError:
        pass

    # Create the required directory structure
    prepare(project_root)
    return 0

# Single module compilation
def make_process(modules_path, mod_name, app_name, is_dyn, net_attr, lib_name, make_args):
    ret = 1
    dev_list = g_user_pannel_config.get_dev_list()
    modules_path_relative = os.path.join(RELATIVE_DIR,modules_path)
    top_path = g_top_dir
    top_path = top_path.replace('\\','/')
    command = ''.join(['MOD_PATH=', modules_path , ' MOD_NAME=' , mod_name ,' LIB_NAME=',lib_name , ' MAIN_TOPDIR=' , top_path , ' project_name=',g_user_pannel_config.project_name])
    if is_dyn == 'yes' :
        command = '%s IS_DYN=yes'%command
    else:
        command = '%s IS_DYN=no'%command

    if app_name != '':
        command = '%s APP_NAME=%s'%(command,app_name.upper())

    if net_attr == 'all':
        command1 = ' '.join([command, 'DEV_NAME=all'])
        command_new = ' '.join(['make', '-C ', modules_path_relative, command1, make_args])
        #print("%s"% command_new)
        print("all [cur_path]:", os.getcwd(), " [command]:", command_new)
        ret = subprocess.run(command_new, shell=True).returncode
        if ret != 0:
            return ret
    elif net_attr == 'separate':
        #print("dev_list %s"%dev_list)
        for dev in dev_list:
            command_new = ''.join([command,' DEV_NAME=',dev])
            command_new = ' '.join(['make', '-C', modules_path_relative, command_new, make_args])
            #print("%s"% command_new)
            ret = subprocess.run(command_new, shell=True).returncode
            if ret != 0:
                return ret
    return ret
# Module group compilation
def compile_module(module_name, build_arg):
    ret = 1
    module_dir_path = g_user_pannel_config.get_module_dir(module_name);
    temp_config = os.path.join(RELATIVE_DIR,'build_tmp/config/temp.config.xml')
    tree = ET.parse(temp_config)
    root = tree.getroot()
    module_list = root.findall('MODULE')
    for module in module_list:
        if module.attrib['name'] == 'overall':
            continue
        if  module_dir_path in module.attrib['path'].lower():
            select = os.path.basename(module.attrib['value'].lower())
            if select == 'no':
                continue
            #eg bsp/drivers/adc
            modules_path = module.attrib['path'].lower()
            modules_path = modules_path.replace('\\','/')
            makefile_path = '%s/'%module_dir_path
            #eg adc
            if modules_path != module_dir_path:
                mod_path = re.sub(makefile_path,"",modules_path)
            else:
                mod_path = "."

            app_name = ''
            if modules_path.startswith(r'app/'):
                app_name = module_name
            is_dyn = module.attrib['is_dyn'].lower()
            net_attr = module.attrib['net_attr'].lower()
            lib_name = module.attrib['lib_name'].lower()
            ret = make_process(module_dir_path, mod_path, app_name, is_dyn, net_attr, lib_name, build_arg)
            if (ret != 0):
                return ret
    return ret

def compile_common_module(module_name, build_arg):
    dev_list = g_user_pannel_config.get_dev_list()
    dev_str = ''
    dev_str = reduce(lambda x,y:'%s,%s'%(x,y),dev_list)
    rom_arg_list = ['BUILD_DRIVERS', 'BUILD_ALG', 'BUILD_OSA', 'BUILD_PHY']
    if  module_name in rom_arg_list:
        module_dir = 'rom'
        command_args = ''.join([' ',module_name,'=YES'])
    else:
        module_dir = g_user_pannel_config.get_module_dir(module_name)
        command_args = ''.join(['dev_list=\'', dev_str, '\''])
    top_path = g_top_dir
    top_path = top_path.replace('\\', '/')
    modules_path_relative = os.path.join(RELATIVE_DIR, module_dir)
    app_str = ','.join(g_user_pannel_config.app_list)
    dev_list = ','.join(g_user_pannel_config.get_dev_list())
    if os.path.exists(os.path.join(modules_path_relative, 'Makefile')):
        command = ['make', 'project_name=%s'%g_user_pannel_config.project_name, command_args, 'MAIN_TOPDIR=%s'%top_path, 'app_name_list=%s'%app_str, 'dev_list=%s'%dev_list, '-C', modules_path_relative, build_arg]
        print("%s" % command)
        ret = subprocess.run(command, shell=False).returncode
        return ret
    else:
        return 0

def make_image_proc(arg):
    print('make_image_proc...')
    ret = 0

    dev_list = g_user_pannel_config.get_dev_list()
    dev_str = reduce(lambda x,y:'%s %s'%(x,y),dev_list)

    app_list = g_user_pannel_config.app_list
    app_str = reduce(lambda x,y:'%s,%s'%(x,y),app_list)

    top_path = g_top_dir
    top_path = top_path.replace('\\','/')

    app_list = g_user_pannel_config.app_list
    nv_str = ''
    for app in app_list:
        nv_list = g_user_pannel_config.get_nv_list(app)
        for name in nv_list:
            nv_str = '%s%s,'%(nv_str,name)

    command = ['make', 'project_name=%s'%g_user_pannel_config.project_name, 'app_str=%s'%app_str, 'dev_list=%s'%dev_str, 'MAIN_TOPDIR=%s'%top_path, 'nv_list=%s'%nv_str, arg]
    print ("%s" %command)
    ret += subprocess.run(command, shell=False).returncode
    return ret

def build_module_proc(build_arg, module_name):
    build_ret = 0

    if module_name == 'drv':
        #build boot rom liteos etc.
        build_ret += compile_common_module('BUILD_DRIVERS', build_arg)
    elif module_name == 'system':
        build_ret += compile_common_module('BUILD_ALG', build_arg)
        build_ret += compile_common_module('BUILD_OSA', build_arg)
    elif module_name == 'phy':
        build_ret += compile_common_module('BUILD_PHY', build_arg)
        logfile = None
        freq_config_py = os.path.join(os.getcwd(), 'plc_phy_cfg', 'freq_config.py')
        command = ['python3', '%s' % freq_config_py]
        build_ret += shell(command, logfile)
    elif module_name == 'boot' or module_name == 'liteos' or module_name=='nv' or module_name == 'dbk_config':
        build_ret += compile_common_module(module_name, build_arg)
        if build_ret == 0:
            print ('HB BUILD %s %s' % (module_name, r'[SUCCESS]'))
        else:
            print ('HB BUILD %s %s' % (module_name, r'[FAILURE]'))
        return build_ret

    build_ret += compile_module(module_name,build_arg)
    if build_arg == 'check':
        if build_ret == 0:
            print ('HB BUILD %s %s' % (module_name, r'[SUCCESS]'))
        else:
            print ('HB BUILD %s %s' % (module_name, r'[FAILURE]'))
    return build_ret

'''
build.py ## 删除所有交付件,再重新构建
## 其他参数使用方法:  build.py [-c/-a/-t] [-m] [-r/-dt] [-d | -p | -v]
build.py -a                   ## 构建所有产品形态的交付件
build.py -t=产品形态           ## 构建指定产品形态的交付件
build.py -r                   ## 使用墙上时间作为构建时间
build.py -d                   ## 构建Debug版本
build.py -dt=日期             ## 使用指定日期作为构建日期
build.py -v=版本号             ## 使用指定版本号作为构建版本
build.py -p=app名             ## 构建指定的app
build.py -m=组件名             ## 构建指定组件
build.py -c -m=组件名          ## 删除指定组件的编译结果
build.py -a -m=组件名          ## 构建指定组件的所有产品形态的交付件
build.py -t=产品形态 -m=组件名  ## 构建指定组件的指定产品形态的交付件
# -c选项说明只执行清除动作，故指定-c选项的同时不能再使用-a指定编译所有产品形态
# -a选项说明构建所有产品形态，故指定-a选项的同时不能再使用-t指定编译特定产品形态
# -r选项说明使用墙上时间进行构建，故指定-r选项的同时不能再使用-dt指定特定日期进行构建
'''
# Generate soc_product_config.h and soc_product_config.mk based on the project configuration file.
g_user_pannel_config = user_pannel_config()
g_top_dir =  os.path.dirname(os.getcwd())
print("TOP_DIR = %s"% g_top_dir)
# Judge and get input
args = len(sys.argv)

def usage():
    '''
    Display help info.
    '''
    print("\nUsage: python3 build.py [-c/-a/-t] [-m] [-r/-dt] [-d | -p | -v] [-h]")
    print("Try 'build.py -h' for usage information.\n")

    print("\nExcute clean OR build product types\n")
    print("  -c, --clean                    [clean all build files]\n \n")
    print("  -a, --all                      [build all product types]\n")
    print("  -t=PATTERN, --type=PATTERN     [build specific product types]\n")
    print("                                 PATTERN is combination of 'cco' 'sta' and 'ndm' separated by commas (,).\n")
    print("                                 Example: -t=cco,sta  -t=cco  --type=sta,ndm\n")

    print("\nSpecify module\n")
    print("  -m=PATTERN, --module=PATTERN   [build specific module]\n")
    print("                                 PATTERN is module name.\n")
    print("                                 Example: -m=drv --module=nv\n")

    print("\nSpecify build time\n")
    print("  -r, --realtime                 [build with real time] By default build with time in dependence.xml.\n")
    print("  -d, --debug                    [build debug image] By default build release image.\n")

    print("\nSpecify other build configuration\n")
    print("  -p=PATTERN, --app=PATTERN      [build specific app] PATTERN is app name.\n")
    print("                                 By default build the first selected app in dependence.xml.\n")
    print("  -v=PATTERN, --version=PATTERN  [build with specific product version number]\n")
    print("                                 PATTERN is version string.\n")
    print("                                 Example: -v=V100R001C00SPC060B013\n")
    print("  -dt=PATTERN, --date=PATTERN    [build with specific date] PATTERN is YYYY-MM-DD.\n")
    print("                                 By default build with time in dependence.xml.\n")

def argv_parse(argv):
    '''
    parse input string.
    '''
    ret = []
    key = None
    val = []
    for idx in range(0, len(argv)):
        if argv[idx].startswith('-'):
            if '=' in argv[idx]:
                key = argv[idx].split('=')[0]
                val = argv[idx].split('=')[1]
                ret.append((key, val))
            else:
                ret.append((argv[idx], None))
        else:
            usage()
            sys.exit(0)
    return ret

def arg_valid(argvs):
    if ('-a' in argvs or '--all' in argvs) and ('-c' in argvs or '--clean' in argvs):
        return False
    if ('-a' in argvs or '--all' in argvs) and ('-t' in argvs or '--type' in argvs):
        return False
    if ('-r' in argvs or '--realtime' in argvs) and ('-dt' in argvs or '--date' in argvs):
        return False
    return True

def build_all():
    ret = prepare_config_file()
    if ret != 0:
        print ('HB BUILD CLEAN [FAILURE]')
        sys.exit(1)

    build_args = ['clean','all','check']
    module_list = g_user_pannel_config.get_module_list()
    for module_name in module_list:
        if module_name == 'make_image':
            make_image_list = ['link','link_check','hupg','hupg_check']
            for arg in make_image_list:
                ret = make_image_proc(arg)
                if ret == 0:
                    print ('HB BUILD %s %s' % (arg,r'[SUCCESS]'))
                    continue
                else:
                    print ('HB BUILD %s %s' % (arg,r'[FAILURE]'))
                    sys.exit(1)
                    raise
            sys.exit(0)
        else:
            for build_arg in build_args:
                ret = build_module_proc(build_arg, module_name)
                if ret == 0:
                    continue
                else:
                    sys.exit(1)

def build_module(module_name, clean=False):
    find = False
    build_ret = 0

    build_ret = prepare_config_file()
    if build_ret != 0:
        sys.exit(1)

    # Get module group list
    module_list = g_user_pannel_config.get_module_list()
    for module in module_list:
        if module == module_name:
            find = True
            break;
    if (find == False):
        print('module name is not correct!')
        sys.exit(1)

    if clean is True:
        build_ret = build_module_proc('clean',module_name)
    else:
        build_ret = build_module_proc('all', module_name)
        build_ret += build_module_proc('check', module_name)
    if build_ret == 0:
        sys.exit(0)
    else:
        sys.exit(1)

def main(argv):
    '''
    Analyse input parameters, doesn't use getopt, cause it is not good with multiable inputs.
    '''
    g_user_pannel_config.set_release_version(debug=False)
    default_type_list = ['cco', 'sta', 'ndm']
    if len(argv) == 0:
        clean_all()
        # build all
        build_all()
        return

    argvs = argv_parse(argv)
    if arg_valid(argvs) is False:
        print("[ERR] args invalid.")
        sys.exit(-1)

    module = None
    clean = False
    #print("argvs: ", argvs)
    for arg in argvs:
        if arg[0] in ["-a", "--all"]:
            # build with all product types
            g_user_pannel_config.set_dev_list(default_type_list)
        elif arg[0] in ["-c", "--clean"]:
            # module clean or clean all
            clean = True
        elif arg[0] in ["-h", "--help"]:
            usage()
            sys.exit(0)
        elif arg[0] in ["-r", "--realtime"]:
            g_user_pannel_config.set_build_time()
        elif arg[0] in ["-d", "--debug"]:
            g_user_pannel_config.set_release_version(debug=True)
        # params followed need value
        elif arg[1] is None:
            print("[ERR] Params invalid!!!")
            usage()
            sys.exit(1)
        elif arg[0] in ["-dt", "--date"]:
            g_user_pannel_config.set_build_time(arg[1])
        elif arg[0] in ["-t", "--type"]:
            # build with input product types
            g_user_pannel_config.set_dev_list(arg[1])
        elif arg[0] in ["-m", "--module"]:
            # build with input product types
            module = arg[1]
        elif arg[0] in ["-v", "--version"]:
            # build with specific version number
            g_user_pannel_config.set_build_version(arg[1])
        elif arg[0] in ["-p", "--app"]:
            g_user_pannel_config.set_build_app(arg[1])
        else:
            usage()
            sys.exit(1)

    if module is None:
        clean_all()

        # if build option with -c/--clean, then clean only. Otherwise build after clean.
        if clean is False:
            # build all
            build_all()
        sys.exit(0)
    elif module is not None:
        # if build option with -c/--clean, then clean only. Otherwise build module directly.
        build_module(module, clean=clean)
        sys.exit(0)

if __name__ == '__main__':
    main(sys.argv[1:])






