#!/usr/bin/env python3
# coding=utf-8
# -----------------------------------------------------------------------------------------
#Copyright CompanyNameMagicTag 2018-2019.All rights reserved
#Author: CompanyName
#------------------------------------------------------------------------------------------

import os,re,sys,shutil
import xml.etree.ElementTree as ET
from datetime import datetime

class user_pannel_config:
    def __init__(self):
        self.app_list = []
        self.nv_list = []
        self.dependence_xml_name =  os.path.join(os.path.dirname(os.path.realpath(__file__)),'../dependence.xml')
        if not os.path.exists(self.dependence_xml_name):
            print ('dependence.xml not exist,please check!')
            return sys.exit(1)
        else:
            self.project_name = self.get_project_name()

        self.app_list = self.get_app_name(self.project_name)
        self.dev_list = None
        self.build_time = None
        self.app_dic = None

    def get_project_name(self):
        with open(self.dependence_xml_name,'r',encoding = 'utf-8') as dependence_xml:
            tree = ET.parse(dependence_xml)
            root = tree.getroot()
            project_list = root.findall('PROJECT')
            for project in project_list:
                if project.attrib['select'].lower()=='yes':
                    project_name = project.attrib['name']
                    return project_name
            sys.exit(1)
    def get_debug_mode(self):
        file = os.path.join(os.path.dirname(os.path.realpath(__file__)),'../../build_tmp/config/soc_product_config.mk')
        with open(file,'r') as cfg_mk:
            for line in list(filter(lambda x:x.split(':=')[1].strip().lower()=='yes',cfg_mk.readlines())):
                if 'PRODUCT_CFG_BUILD_RELEASE' in line:
                    return 'release'
        return 'debug'

    def get_product_chip(self):
        return self.get_usr_panel('PRODUCT_CFG_CHIP_SOLUTION_NAME')

    def get_product_ver(self):
        return self.get_usr_panel('PRODUCT_CFG_VERSION_STR')

    def get_usr_panel(self, key):
        with open(self.dependence_xml_name,'r',encoding = 'utf-8') as dependence_xml:
            root = ET.parse(dependence_xml).getroot()
            modules = root.findall('MODULE')
            for item in list(filter(lambda x:x.attrib['name'].lower()=='user_panel',modules))[0]:
                if item.attrib['name'].upper() == key.upper():
                    return item.attrib['value'].strip('\"')

    def get_dev_list(self):
        dev_list = []
        with open(self.dependence_xml_name,'r',encoding = 'utf-8') as dependence_xml:
            tree = ET.parse(dependence_xml)
            root = tree.getroot()
            modules = root.findall('MODULE')
            for module in modules:
                items = module.findall('item')
                for item in items:
                    if item.attrib['name'].upper() == 'PRODUCT_CFG_PRODUCT_TYPE_CCO' and  item.attrib['value'].lower() == 'yes':
                        dev_list.append('cco')
                    if item.attrib['name'].upper() == 'PRODUCT_CFG_PRODUCT_TYPE_STA' and  item.attrib['value'].lower() == 'yes':
                        dev_list.append('sta')
                    if item.attrib['name'].upper() == 'PRODUCT_CFG_PRODUCT_TYPE_NDM' and  item.attrib['value'].lower() == 'yes':
                        dev_list.append('ndm')
            return dev_list

    def set_dev_list(self, dev_list):
        dependence = self.dependence_xml_name
        tree = ET.parse(dependence)
        modules = tree.getroot().findall('MODULE')
        for module in modules:
            items = module.findall('item')
            for item in items:
                if item.attrib['name'].upper() == 'PRODUCT_CFG_PRODUCT_TYPE_CCO':
                    item.attrib['value'] = 'yes' if 'cco' in dev_list else 'no'
                if item.attrib['name'].upper() == 'PRODUCT_CFG_PRODUCT_TYPE_STA':
                    item.attrib['value'] = 'yes' if 'sta' in dev_list else 'no'
                if item.attrib['name'].upper() == 'PRODUCT_CFG_PRODUCT_TYPE_NDM':
                    item.attrib['value'] = 'yes' if 'ndm' in dev_list else 'no'
        tree.write(dependence, encoding='utf-8')
        self.dev_list = self.get_dev_list()

    def get_app_name(self,project_name):
        app_list = []
        with open(self.dependence_xml_name,'r',encoding = 'utf-8') as dependence_xml:
            tree = ET.parse(dependence_xml)
            root = tree.getroot()
            project_list = root.findall('PROJECT')
            for project in project_list:
                if project.attrib['name'].lower() == project_name.lower():
                    app_root_list = project.findall('APP')
                    for app in app_root_list:
                        if app.attrib['select'].lower() =='yes':
                            app_list.append(app.attrib['name'])
            return app_list
    def get_nv_list(self,app_name):
        nv_list = []
        with open(self.dependence_xml_name,'r',encoding = 'utf-8') as dependence_xml:
            tree = ET.parse(dependence_xml)
            root = tree.getroot()
            project_list = root.findall('PROJECT')
            for  project in project_list:
                if project.attrib['name'].lower() == self.project_name.lower():
                    app_root_list = project.findall('APP')
                    for app in app_root_list:
                        if app.attrib['name'].lower() == app_name:
                            nv_root = app.find('NV')
                            nv_config_root = nv_root.findall('config')
                            for nv in nv_config_root:
                                if nv.attrib['select'].lower() == 'yes':
                                    nv_list.append(nv.attrib['name'].lower())
            return nv_list
    # Get module group list
    def get_module_list(self):
        module_list = []
        with open(self.dependence_xml_name,'r',encoding = 'utf-8') as dependence_xml:
            tree = ET.parse(dependence_xml)
            root = tree.getroot()
            project_list = root.findall('PROJECT')
            for project in project_list:
                if project.attrib['name'].lower()== self.project_name.lower():
                    group_root = project.find('GROUP')
                    module_root_list = group_root.findall('module')
                    for module in module_root_list:
                        module_list.append(module.attrib['name'].lower())
            return module_list
    def get_module_dir(self,module_name):
        with open(self.dependence_xml_name,'r',encoding = 'utf-8') as dependence_xml:
            tree = ET.parse(dependence_xml)
            root = tree.getroot()
            project_list = root.findall('PROJECT')
            for project in project_list:
                if project.attrib['name'].lower()== self.project_name.lower():
                    group_root = project.find('GROUP')
                    module_root_list = group_root.findall('module')
                    for module in module_root_list:
                        if module.attrib['name'].lower() == module_name.lower() :
                            module_dir = module.attrib['dir'].lower()
                            module_dir = module_dir.replace('\\','/')
            return module_dir
    def get_build_data_time(self):
        data_time_tuple = ()
        with open(self.dependence_xml_name,'r',encoding = 'utf-8') as dependence_xml:
            tree = ET.parse(dependence_xml)
            root = tree.getroot()
            modules = root.findall('MODULE')
            for module in modules:
                items = module.findall('item')
                for item in items:
                    if item.attrib['name'].upper() == 'PRODUCT_CFG_BUILD_DATA_TIME':
                        date_time_value = item.attrib['value'];
                        date_time_value = ''.join([date_time_value,":0:0:0"])
                        date_time_value = date_time_value.replace('\"','')
                        time_list = re.split('[- :]',date_time_value)
                        data_time_tuple = tuple(time_list)
            print(data_time_tuple)
            return data_time_tuple

    def set_build_time(self, date=None):
        cur_date = datetime.now().strftime('%Y-%m-%d')
        cur_time = datetime.now().strftime('%H:%M:%S')
        if date is not None:
            if date != datetime.strptime(date, "%Y-%m-%d").strftime("%Y-%m-%d"):
                sys.exit(-1)
            cur_date = date
        cur_date_time = '%s %s' % (cur_date, cur_time)
        dependence = self.dependence_xml_name
        tree = ET.parse(dependence)
        modules = tree.getroot().findall('MODULE')
        for module in modules:
            items = module.findall('item')
            for item in items:
                if item.attrib['name'] =='PRODUCT_CFG_BUILD_DATE':
                    item.attrib['value'] = ''.join(['\"', cur_date, '\"'])
                if item.attrib['name'] =='PRODUCT_CFG_BUILD_TIME':
                    item.attrib['value'] = ''.join(['\"', cur_time, '\"'])
                if item.attrib['name'] =='PRODUCT_CFG_BUILD_DATA_TIME':
                    item.attrib['value'] = ''.join(['\"', cur_date_time, '\"'])
        tree.write(dependence, encoding='utf-8')
        self.build_time = self.get_build_data_time()

    def set_release_version(self, debug=False):
        dependence = self.dependence_xml_name
        tree = ET.parse(dependence)
        modules = tree.getroot().findall('MODULE')
        for module in modules:
            items = module.findall('item')
            for item in items:
                if item.attrib['name'] =='PRODUCT_CFG_BUILD_RELEASE':
                    item.attrib['value'] = 'yes' if debug is False else 'no'
        tree.write(dependence, encoding='utf-8')

    def set_build_version(self, version):
        if re.match('^V[0-9]{3}R[0-9]{3}C[0-9]{2}SPC[0-9]{3}B[0-9]{3}$', version) is None:
            print("[ERR] version format err.")
            sys.exit(-1)

        ver_list = re.split('[A-Z]+', version[1:])
        dependence = self.dependence_xml_name
        tree = ET.parse(dependence)
        modules = tree.getroot().findall('MODULE')
        for module in modules:
            items = module.findall('item')
            for item in items:
                if item.attrib['name'] == 'PRODUCT_CFG_VERSION_STR':
                    item.attrib['value'] = ''.join(['\"', version, '\"'])
                if item.attrib['name'] == 'PRODUCT_CFG_FULL_VERSION_STR':
                    full_verison = "%s_%s" % (self.get_usr_panel("PRODUCT_CFG_CHIP_SOLUTION_NAME"), version)
                    print("org chip: ", self.get_usr_panel("PRODUCT_CFG_CHIP_SOLUTION_NAME"))
                    item.attrib['value'] = ''.join(['\"', full_verison, '\"'])
                if item.attrib['name'] == 'PRODUCT_CFG_VERSION_V':
                    item.attrib['value'] = "%s" % int(ver_list[0])
                if item.attrib['name'] == 'PRODUCT_CFG_VERSION_R':
                    item.attrib['value'] = "%s" % int(ver_list[1])
                if item.attrib['name'] == 'PRODUCT_CFG_VERSION_C':
                    item.attrib['value'] = "%s" % int(ver_list[2])
                if item.attrib['name'] == 'PRODUCT_CFG_VERSION_SPC':
                    item.attrib['value'] = "%s" % int(ver_list[3])
                if item.attrib['name'] == 'PRODUCT_CFG_VERSION_B':
                    item.attrib['value'] = "%s" % int(ver_list[4])
        tree.write(dependence, encoding='utf-8')

    def set_build_app(self, app_name):
        flag = False
        if 'demo' in app_name or 'module_app' in app_name or 'mrs' in app_name or 'all' in app_name:
            flag = True
        if flag == False:
            print("[ERR] app name err.")
            sys.exit(-1)

        dependence = self.dependence_xml_name
        tree = ET.parse(dependence)
        root = tree.getroot()
        project_list = root.findall('PROJECT')
        for project in project_list:
            if project.attrib['name'].lower()== 'dw21':
                app_root_list = project.findall('APP')
                for app in app_root_list:
                    if app.attrib['name'].lower() == 'demo':
                        app.attrib['select'] = 'yes' if 'demo' in app_name or 'all' in app_name else 'no'
                    if app.attrib['name'].lower() == 'module_app':
                        app.attrib['select'] = 'yes' if 'module_app' in app_name or 'all' in app_name else 'no'
                    if app.attrib['name'].lower() == 'mrs':
                        app.attrib['select'] = 'yes' if 'mrs' in app_name or 'all' in app_name else 'no'
        tree.write(dependence, encoding='utf-8')
