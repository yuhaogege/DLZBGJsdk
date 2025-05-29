#!/usr/bin/env python3
# coding=utf-8

"""
* Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
* Description: Compressed file with LZMA. There is a bit difference between LZMA format under linux xz
    and windows lzma format, adapter also be implatmented.
* Author: CompanyName
* Create: 2020-11-2
"""

import lzma

__all__ = ['encode_file_with_lzma1']

def len_2_bytes(length, max_len):
    '''
    make data to bytes
    '''
    data_byte = int(length).to_bytes(length=max_len, byteorder='little', signed=True)
    return data_byte

def encode_file_with_lzma1(encode_path, src_path, max_len=8):
    with open(src_path, 'rb') as f:
        data = f.read()

    # lzma1 parameters
    filters = [
        {
            "id": lzma.FILTER_LZMA1,
            "dict_size": 64 * 1024,
            "lc": 2,
            "lp": 2,
            "pb": 2,
            "nice_len": 128,
            "mf": lzma.MF_BT4,
            "mode": lzma.MODE_NORMAL
        },
    ]

    pack_data = lzma.compress(data, format=lzma.FORMAT_ALONE, filters=filters)
    lzmadata = bytearray()
    # Makes lzma file format close to the one made with lzma.exe.
    for i in range(0, 5):
        lzmadata.append(pack_data[i])

    data_size = len_2_bytes(len(data), max_len)
    for size in data_size:
        lzmadata.append(size)

    for i in range(13, len(pack_data)):
        lzmadata.append(pack_data[i])

    with open(encode_path, 'wb') as f:
        f.write(lzmadata)

