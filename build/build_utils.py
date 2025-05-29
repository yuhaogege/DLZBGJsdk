#!/usr/bin/env python3
# coding=utf-8

"""
* Copyright (c) CompanyNameMagicTag 2020-2020. All rights reserved.
* Description: Utilities of building system.
* Author: CompanyName
* Create: 2020-11-15
"""
import subprocess

__init__ = ['shell']

def shell(cmdlist, logfile=None):
    print("Command:", cmdlist)
    try:
        subp = subprocess.Popen(cmdlist, shell=False, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, encoding="utf-8")
        while True:
            output = subp.stdout.readline()
            if output == '' and subp.poll() is not None:
                break
            if output:
                print(output.strip())
                if logfile is not None:
                    logfile.write(output)
        return subp.returncode
    except Exception as err:
        print(err)
        if logfile is not None:
            logfile.write(str(err))
        return -1
