#!/usr/bin/env python3
# coding=utf-8

import json
import re
import sys

# 从符号表fixrom.sym获取非函数内容, 保存到例外配置文件
# 参数(fixrom.sym路径, 例外配置文件路径)
def GetDataFixrom(sym_path, except_file):
    result = {}
    # 变量列表
    var_list = ['crc16_tab', '_ctype', 'crc16_tab', '__rom_copy_start', '__rom_copy_ram_start', '__rom_bss_end']
    with open(sym_path, 'r') as fixrom_data:
        for line in fixrom_data.readlines():
            # 正则匹配，提取变量/函数名和地址
            re_bds = "(.*) = (.*);"
            p = re.findall(re_bds, line, re.S)
            # 字符串转数字
            tmp = int(p[0][1], 16)
            # 初步筛选 堆,栈,BSS+DATA段的内容
            if ((tmp >= 0x02017000 and tmp <= 0x02039000) or \
                    (tmp >= 0x0203D000 and tmp <= 0x0203F000) or \
                    (tmp >= 0x0203f000 and tmp <= 0x02040000) or\
                    (tmp >= 0x00010800 and tmp <= 0x00040800)):
                # 根据名字再次筛选非函数内容
                if p[0][0][0:2] == "g_" or \
                        p[0][0] in var_list:
                    result[p[0][0]] = p[0][1]
    # 将字典里的内容根据键值对，写入到例外配置文件
    with open(except_file, 'w') as wr_j:
        json.dump(result, wr_j)
    return


# 从符号表Firmware_Base.sym获取非函数内容，保存到例外配置文件
# 参数(Firmware_Base.sym路径, 例外配置文件路径)
def GetDataRom(sym_path, except_file):
    result = {}
    # 变量列表
    with open(sym_path, 'r') as fixrom_data:
        for line in fixrom_data.readlines():
            # 正则匹配，提取变量/函数名和地址
            re_bds = "(.*) = (.*);"
            p = re.findall(re_bds, line, re.S)
            tmp = (int(p[0][1], 16))
            # 根据二进制范围,提取 DATA/BSS段的内容,放到字典中
            if ((tmp >= 0x02000400 and tmp <= 0x02003400) or \
                    (tmp >= 0x02001000 and tmp <= 0x02001420) or \
                    (p[0][0][0:2] == 'g_')):
                result[p[0][0]] = p[0][1]
    # 将字典里的内容根据键值对，写入到例外配置文件
    with open(except_file, 'w') as wr_j:
        json.dump(result, wr_j)
    return

# 功能:从asm文件中提取16进制数
# 参数:output路径
# 返回值: 返回.word后的十六进制数列表
def get_asm_data(asm_path):
    # 用来存放 .word 后的十六进制数
    word_list = []
    with open(asm_path, 'r') as fp:
        # 逐行读取, 并提取 .word 后面的十六进制数
        for word in fp.readlines():
            re_bds = ".word	(.*)\n"
            p = re.findall(re_bds, word, re.S)
            # 正则匹配成功,把十六进制数放到列表里
            if len(p) > 0:
                word_list.append(p[0])
    return word_list


# 参数: asm文件路径, 符号表路径, 例外配置文件路径
def ArmThumbAsmCheck(asm_path, symbol_table_path, exception_file_path):
    # 从.asm文件获取 .word后的地址,返回值为列表
    word_list = get_asm_data(asm_path)
    # 相同函数的报错信息，只输出一次
    flag_print = []
    # 根据.word后面的地址。在符号表文件中和例外配置文件中查找是否存在
    with open(symbol_table_path, 'r') as sym_fp:
        with open(exception_file_path, 'r') as expt_fp:
            expt_text = expt_fp.read()
            sym_text = sym_fp.read()
            for word in word_list:
                if word in sym_text:
                    if word not in expt_text:
                        re_bds = '(.*) = ' + word
                        func = re.findall(re_bds, sym_text)[0]
                        print('%-30s%-30s' %(func,"Error：函数中调用rom化的函数不符合当先rom化函数调用规范"))
                        return -1;
    return


if __name__ == '__main__':
    # start_time = time.time()
    # asm文件路径
    asm_path = sys.argv[1]
    # 符号表路径
    sym_path = sys.argv[2]
    # 例外配置文件路径
    expt_file = sys.argv[3]
    try:
        f_exec = open(expt_file, 'r')
        f_exec.close()
    except IOError:
        # 从符号表fixrom.sym获取非函数内容, 保存到例外配置文件
        if sym_path[-10:]=="fixrom.sym":
            GetDataFixrom(sym_path, expt_file)
        elif sym_path[-17:]=="Firmware_Base.sym":
            GetDataRom(sym_path, expt_file)
    # 测试函数
    ret = ArmThumbAsmCheck(asm_path, sym_path, expt_file)
    if ret==-1:
        sys.exit(-1)
    # end_time = time.time()
    # print('(>_<)执行时间: ' + str(end_time - start_time))
