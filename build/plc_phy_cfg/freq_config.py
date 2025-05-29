#!/usr/bin/env python3
# coding=utf-8
# -----------------------------------------------------------------------------------------
#Purpose:
#Copyright CompanyNameMagicTag 2018-2019.All rights reserved
#Author: CompanyName
#------------------------------------------------------------------------------------------
import os,sys
import re
from ctypes import *
import binascii
import time

#PATTERN = 'Safety'
#-----------------------------------Basic type definition-----------------------------------
TD_CHAR=c_char
TD_S8=c_byte
TD_U8=c_ubyte
TD_S16=c_short
TD_U16=c_ushort
TD_S32=c_int
UAPI_U32=c_uint
#-------------------------------------Constant definition-------------------------------------
PY_ONE = 1
PY_ZERO = 0
firstMagicLine = 5# Number of lines where the devil number first appears
secondMagicLine = 26# Number of lines where the devil number appears second
crc_result_line  = 63# The total number of lines of text. When the number of band parameters changes, this value needs to be adjusted.
crc_tmp_name = "\\crc.txt"# A temporary TXT file will be used during the devil check and crc check, and will be deleted after use
#--------------------------------Structure definition-------------------------------------
def init_tmp_st_from_file(file):
    with open(file,'r') as fo:
        lines = fo.readlines()
        tmp_st = tmp_freq_config_st()
        tmp_st.freq_info_st.freq_cur_num = int(lines[0].strip())
        tmp_st.freq_info_st.freq_mode = bytes(lines[2].strip()[0:14],"ascii")
        tmp_st.freq_info_st.freq_range = bytes(lines[1].strip()[0:14],"ascii")
        tmp_st.afe_config = int(lines[3].strip())
        tmp_st.data_start = int(lines[5].strip())
        tmp_st.data_stop = int(lines[6].strip())
        tmp_st.v200en = int(lines[7].strip())
        tmp_st.fc_num = int(lines[8].strip())
        tmp_st.valid_carrier_num = int(lines[9].strip())
        tmp_st.seq_shift = int(lines[10].strip())
        tmp_st.ch_num = int(lines[11].strip())
        tmp_st.chCarrier_start0 = int(lines[12].strip())
        tmp_st.chCarrier_start1 = int(lines[13].strip())
        tmp_st.chCarrier_start2 = int(lines[14].strip())
        tmp_st.chCarrier_start3 = int(lines[15].strip())
        tmp_st.chCarrier_stop0 = int(lines[16].strip())
        tmp_st.chCarrier_stop1 = int(lines[17].strip())
        tmp_st.chCarrier_stop2 = int(lines[18].strip())
        tmp_st.chCarrier_stop3 = int(lines[19].strip())
        tmp_st.freqHPFMode = int(lines[20].strip())
        tmp_st.freqLPFMode = int(lines[21].strip())
        #tmp_st.power_adjust = int(lines[22].strip())
        tmp_st.sampleMode = int(lines[22].strip())
        tmp_st.recipoint_param = int(lines[23].strip())
        tmp_st.aagc_quick_win_num = int(lines[24].strip())
        tmp_st.aagc_quick_used_num = int(lines[26].strip())
        tmp_st.aagc_quick_win_size = int(lines[27].strip())
        tmp_st.st23_win_num = int(lines[28].strip())
        tmp_st.st23_used_num = int(lines[29].strip())
        tmp_st.st23_win_size = int(lines[30].strip())
        tmp_st.AagcSampleNum = int(lines[31].strip())
        tmp_st.LSR_MeanNum = int(lines[32].strip())
        tmp_st.FrmEndWaitLen2Aagc = int(lines[33].strip())
        tmp_st.SunSpecLargeWinNum = int(lines[34].strip())
        tmp_st.SunSpecpower_alpha1 = int(lines[35].strip())
        tmp_st.ImpUsedWinNum = int(lines[36].strip())
        tmp_st.ImpWinNum = int(lines[37].strip())
        tmp_st.ImpWinSize = int(lines[38].strip())
        tmp_st.ImpWinInd1Set0HalfLenOldPream = int(lines[39].strip())
        tmp_st.ImpWinInd1Set0HalfLenNewPream = int(lines[40].strip())
        tmp_st.ImpWinInd2Set0HalfLenOldPream = int(lines[41].strip())
        tmp_st.ImpWinInd2Set0HalfLenNewPream = int(lines[42].strip())
        tmp_st.ImpWinInd3Set0HalfLenOldPream = int(lines[43].strip())
        tmp_st.ImpWinInd3Set0HalfLenNewPream = int(lines[44].strip())
        tmp_st.ImpWinInd1Set0HalfLenOldFCPL = int(lines[45].strip())
        tmp_st.ImpWinInd1Set0HalfLenNewFCPL = int(lines[46].strip())
        tmp_st.ImpWinInd2Set0HalfLenOldFCPL = int(lines[47].strip())
        tmp_st.ImpWinInd2Set0HalfLenNewFCPL = int(lines[48].strip())
        tmp_st.ImpWinInd3Set0HalfLenOldFCPL = int(lines[49].strip())
        tmp_st.ImpWinInd3Set0HalfLenNewFCPL = int(lines[50].strip())
        tmp_st.NbiMaxPowScale = int(lines[51].strip())
        tmp_st.time_proc_delay = int(lines[52].strip())
        tmp_st.V200_PreambledB = int(lines[53].strip())
        tmp_st.NoiseNbiCarrierNum = int(lines[54].strip())
        tmp_st.CsNbiEn = int(lines[55].strip())
        tmp_st.Tx_Power = int(lines[56].strip())
        tmp_st.pad = PY_ONE
        tmp_st.max_frame_len = int(lines[57].strip())
        tmp_st.nbi_time_period = int(lines[58].strip())
        tmp_st.nbi_check_min_level = int(lines[59].strip())
        tmp_st.nf_use2_en = int(lines[60].strip())
        tmp_st.nbi_use2_force_en = int(lines[61].strip())
        fo.close()
        return tmp_st

#################Band information############################
class phy_freq_config_freq_info_st(Structure):
    _fields_ = [
        ("freq_cur_num",TD_U8),
        ("freq_mode",TD_CHAR*15),
        ("freq_range",TD_CHAR*15),
        ("pad",TD_U8),
        ]

##########################Temporary structure##########################
class tmp_freq_config_st(Structure):
    _fields_ = [
        ("freq_info_st",phy_freq_config_freq_info_st),
        ("afe_config",TD_U16),
        ("data_start",TD_U16),
        ("data_stop",TD_U16),
        ("v200en",TD_U16),
        ("fc_num",TD_U16),
        ("valid_carrier_num",TD_U16),
        ("seq_shift",TD_U16),
        ("ch_num",TD_U16),
        ("chCarrier_start0",TD_U16),
        ("chCarrier_start1",TD_U16),
        ("chCarrier_start2",TD_U16),
        ("chCarrier_start3",TD_U16),
        ("chCarrier_stop0",TD_U16),
        ("chCarrier_stop1",TD_U16),
        ("chCarrier_stop2",TD_U16),
        ("chCarrier_stop3",TD_U16),
        ("freqHPFMode",TD_U16),
        ("freqLPFMode",TD_U16),
        #("power_adjust",TD_U16),
        ("sampleMode",TD_U16),
        ("recipoint_param",TD_U16),
        ("aagc_quick_win_num",TD_U16),
        ("aagc_quick_used_num",TD_U16),
        ("aagc_quick_win_size",TD_U16),
        ("st23_win_num",TD_U16),
        ("st23_used_num",TD_U16),
        ("st23_win_size",TD_U16),
        ("AagcSampleNum",TD_U16),
        ("LSR_MeanNum",TD_U16),
        ("FrmEndWaitLen2Aagc",TD_U16),
        ("SunSpecLargeWinNum",TD_U16),
        ("SunSpecpower_alpha1",TD_U16),
        ("ImpUsedWinNum",TD_U16),
        ("ImpWinNum",TD_U16),
        ("ImpWinSize",TD_U16),
        ("ImpWinInd1Set0HalfLenOldPream",TD_U16),
        ("ImpWinInd1Set0HalfLenNewPream",TD_U16),
        ("ImpWinInd2Set0HalfLenOldPream",TD_U16),
        ("ImpWinInd2Set0HalfLenNewPream",TD_U16),
        ("ImpWinInd3Set0HalfLenOldPream",TD_U16),
        ("ImpWinInd3Set0HalfLenNewPream",TD_U16),
        ("ImpWinInd1Set0HalfLenOldFCPL",TD_U16),
        ("ImpWinInd1Set0HalfLenNewFCPL",TD_U16),
        ("ImpWinInd2Set0HalfLenOldFCPL",TD_U16),
        ("ImpWinInd2Set0HalfLenNewFCPL",TD_U16),
        ("ImpWinInd3Set0HalfLenOldFCPL",TD_U16),
        ("ImpWinInd3Set0HalfLenNewFCPL",TD_U16),
        ("NbiMaxPowScale",TD_U16),
        ("time_proc_delay",TD_U16),
        ("V200_PreambledB",TD_U16),
        ("NoiseNbiCarrierNum",TD_U16),
        ("CsNbiEn",TD_U16),
        ("Tx_Power",TD_U16),
        ("pad",TD_U16),
	("max_frame_len",UAPI_U32),
        ("nbi_time_period", UAPI_U32),
        ("nbi_check_min_level", UAPI_U32),
        ("nf_use2_en", TD_U8),
        ("nbi_use2_force_en", TD_U8),

("pad2",TD_U16),
        ]

#######################TX module##########################
#---STD_REG register
class phy_tx_std_reg_st(Structure):
    _fields_ = [
        ("v200en",UAPI_U32,1),
        ("sampleMode",UAPI_U32,2),
        ("fc_num",UAPI_U32,6),
        ("seq_shift",UAPI_U32,9),
        ("pad",UAPI_U32,14),
        ]

#---TMSK_NUM register
class phy_tx_tmsk_num_st(Structure):
    _fields_ = [
        ("data_start",TD_U8),
        ("data_stop",TD_U8),
        ("valid_carrier_num",TD_U8),
        ("pad",TD_U8),
        ]

#---POW_INDEX register
class phy_tx_pow_index_st(Structure):
    _fields_ = [
        #("power_adjust",UAPI_U32),
        ("V200_PreambledB",UAPI_U32,3),
        ("pad1",UAPI_U32,29),
        ]

class phy_tx_freq_config_st(Structure):
    _fields_ = [
        ("std_reg_st",phy_tx_std_reg_st),
        ("tmsk_num_st",phy_tx_tmsk_num_st),
        ("pow_index_st",phy_tx_pow_index_st),
        ]
#####################AGC Module###########################

#---AAGC_WAIT register
class phy_agc_aagc_wait_st(Structure):
    _fields_ = [
        ("FrmEndWaitLen2Aagc",TD_U16),
        ("pad",TD_U16),
        ]

#---LSR_CFG register
class phy_agc_lsr_cfg_st(Structure):
    _fields_ = [
        ("LSR_MeanNum",UAPI_U32,3),
        ("pad",UAPI_U32,29),
        ]

#---QUICK_WINDOW_PARAM register
class phy_agc_qk_win_para_st(Structure):
    _fields_ = [
        ("aagc_quick_win_size",UAPI_U32,3),
        ("pad1",UAPI_U32,1),
        ("aafc_quick_win_num",UAPI_U32,3),
        ("pad2",UAPI_U32,1),
        ("aagc_quick_used_num",UAPI_U32,2),
        ("pad3",UAPI_U32,22),
        ]

#---STATE23_WINDOW_PARAM register
class phy_agc_st23_win_para_st(Structure):
    _fields_ = [
        ("st23_win_size",UAPI_U32,3),
        ("pad1",UAPI_U32,1),
        ("st23_win_num",UAPI_U32,3),
        ("pad2",UAPI_U32,1),
        ("st23_used_num",UAPI_U32,2),
        ("pad3",UAPI_U32,22),
        ]

#---SUNSPEC_ALPHA_FILTER register
class phy_agc_sunspec_alpha_filter_st(Structure):
    _fields_ = [
        ("SunSpecpower_alpha1",UAPI_U32,9),
        ("pad",UAPI_U32,23),
        ]

class phy_agc_freq_config_st(Structure):
    _fields_ = [
        ("aagc_wait",phy_agc_aagc_wait_st),
        ("lsr_cfg_st",phy_agc_lsr_cfg_st),
        ("qk_win_para_st",phy_agc_qk_win_para_st),
        ("st23_win_para_st",phy_agc_st23_win_para_st),
        ("sunspec_alpha_filter",phy_agc_sunspec_alpha_filter_st),
        ("SunSpecLargeWinNum",TD_U8),
        ("AagcSampleNum",TD_U8),
        ("Tx_Power",TD_U8),
        ("pad",TD_U8),
        ]
###############CARRIER SENSE Module#######################

#---CS_CH_CARRIER0 register
class phy_carrier_sense_0_st(Structure):
    _fields_ = [
        ("chCarrier_start0",TD_U8),
        ("chCarrier_stop0",TD_U8),
        ("chCarrier_start1",TD_U8),
        ("chCarrier_stop1",TD_U8),
        ]

#---CS_CH_CARRIER0 register
class phy_carrier_sense_1_st(Structure):
    _fields_ = [
        ("chCarrier_start2",TD_U8),
        ("chCarrier_stop2",TD_U8),
        ("chCarrier_start3",TD_U8),
        ("chCarrier_stop3",TD_U8),
        ]

class phy_carrier_sense_freq_config_st(Structure):
    _fields_ = [
        ("cs_ch_carrier_0_st",phy_carrier_sense_0_st),
        ("cs_ch_carrier_1_st",phy_carrier_sense_1_st),
        ("CsNbiEn",TD_U8),
        ("ch_num",TD_U8),
        ("pad",TD_U16),
        ]
################IMP Module###########################
#---IMP_CFG7 register
class phy_imp_imp_cfg7_st(Structure):
    _fields_ = [
        ("ImpWinInd1Set0HalfLenOldPream",UAPI_U32,6),
        ("pad1",UAPI_U32,2),
        ("ImpWinInd1Set0HalfLenNewPream",UAPI_U32,6),
        ("pad2",UAPI_U32,2),
        ("ImpWinInd2Set0HalfLenOldPream",UAPI_U32,6),
        ("pad3",UAPI_U32,2),
        ("ImpWinInd2Set0HalfLenNewPream",UAPI_U32,6),
        ("pad4",UAPI_U32,2),
        ]

#---IMP_CFG8 register
class phy_imp_imp_cfg8_st(Structure):
    _fields_ = [
        ("ImpWinInd3Set0HalfLenOldPream",UAPI_U32,6),
        ("pad1",UAPI_U32,2),
        ("ImpWinInd3Set0HalfLenNewPream",UAPI_U32,6),
        ("pad2",UAPI_U32,2),
        ("ImpWinInd1Set0HalfLenOldFCPL",UAPI_U32,6),
        ("pad3",UAPI_U32,2),
        ("ImpWinInd1Set0HalfLenNewFCPL",UAPI_U32,6),
        ("pad4",UAPI_U32,2),
        ]

#---IMP_CFG9 register
class phy_imp_imp_cfg9_st(Structure):
    _fields_ = [
        ("ImpWinInd2Set0HalfLenOldFCPL",UAPI_U32,6),
        ("pad1",UAPI_U32,2),
        ("ImpWinInd2Set0HalfLenNewFCPL",UAPI_U32,6),
        ("pad2",UAPI_U32,2),
        ("ImpWinInd3Set0HalfLenOldFCPL",UAPI_U32,6),
        ("pad3",UAPI_U32,2),
        ("ImpWinInd3Set0HalfLenNewFCPL",UAPI_U32,6),
        ("pad4",UAPI_U32,2),
        ]

class phy_imp_freq_config_st(Structure):
    _fields_ = [
        ("ImpWinSize",TD_U8),
        ("ImpUsedWinNum",TD_U8),
        ("ImpWinNum",TD_U8),
        ("pad",TD_U8),
        ("imp_cfg_7",phy_imp_imp_cfg7_st),
        ("imp_cfg_8",phy_imp_imp_cfg8_st),
        ("imp_cfg_9",phy_imp_imp_cfg9_st),
        ]
#################Symbol synchronization module###########################
class phy_symbol_align_freq_config_st(Structure):
    _fields_ = [
        ("recipoint_param",TD_U16),
        ("NoiseNbiCarrierNum",TD_U8),
        ("pad",TD_U8),
        ]

#################NBI Module###############################
class phy_nbi_freq_config_st(Structure):
    _fields_ = [
        ("NbiMaxPowScale",UAPI_U32,5),
        ("pad",UAPI_U32,27),
        ]

class phy_nbi_freq_nbi_check(Structure):
    _fields_ = [
        ("nbi_time_period",UAPI_U32),
        ("nbi_check_min_level",UAPI_U32),
        ("nf_use2_en",TD_U16,1),
        ("nbi_use2_force_en",TD_U16,1),
        ("pad",TD_U16,14),
        ("pad2",TD_U16),
        ]

#################Low power module############################
class phy_low_consume_freq_config_st(Structure):
    _fields_ = [
        ("time_proc_delay",UAPI_U32),
        ]

##################Target structure###############
class phy_all_freq_config_st(Structure):
    _fields_ =[
        ("freq_info_st",phy_freq_config_freq_info_st),
        ("tx_freq_config_st",phy_tx_freq_config_st),
        ("agc_freq_config_st",phy_agc_freq_config_st),
        ("carrier_sense_freq_config_st",phy_carrier_sense_freq_config_st),
        ("imp_freq_config_st",phy_imp_freq_config_st),
        ("symbol_align_freq_config_st",phy_symbol_align_freq_config_st),
        ("nbi_freq_config_st",phy_nbi_freq_config_st),
        ("low_consume_freq_config_st",phy_low_consume_freq_config_st),
        ("freqHPFMode",TD_U8),
        ("freqLPFMode",TD_U8),
        ("afe_config",TD_U8),
        ("pad",TD_U8),
	("max_frame_length",UAPI_U32),
        ("nbi_check_config",phy_nbi_freq_nbi_check),
        ]

#---Assign values ​​to the target structure
def init_target_st_from_tmp_st(st):
    test_bin=bytearray(sizeof(phy_all_freq_config_st))
    all_st=phy_all_freq_config_st.from_buffer(test_bin)
#Assignment of band information---------------------------#
    all_st.freq_info_st.freq_cur_num = st.freq_info_st.freq_cur_num
    all_st.freq_info_st.freq_mode = st.freq_info_st.freq_mode
    all_st.freq_info_st.freq_range = st.freq_info_st.freq_range
    all_st.freq_info_st.pad = PY_ZERO
    #all_st = phy_all_freq_config_st()
#Tx module assignment-----------------------#
    all_st.tx_freq_config_st.std_reg_st.v200en = st.v200en
    all_st.tx_freq_config_st.std_reg_st.sampleMode = st.sampleMode
    all_st.tx_freq_config_st.std_reg_st.fc_num = st.fc_num
    all_st.tx_freq_config_st.std_reg_st.seq_shift = st.seq_shift
    all_st.tx_freq_config_st.std_reg_st.pad = PY_ZERO

    all_st.tx_freq_config_st.tmsk_num_st.data_start = st.data_start
    all_st.tx_freq_config_st.tmsk_num_st.data_stop = st.data_stop
    all_st.tx_freq_config_st.tmsk_num_st.valid_carrier_num = st.valid_carrier_num
    all_st.tx_freq_config_st.tmsk_num_st.pad = PY_ZERO

        #all_st.tx_freq_config_st.pow_index_st.power_adjust = st.power_adjust
    all_st.tx_freq_config_st.pow_index_st.V200_PreambledB = st.V200_PreambledB
    all_st.tx_freq_config_st.pow_index_st.pad1 = PY_ZERO
#AGC module assignment-----------------------#
    all_st.agc_freq_config_st.aagc_wait.FrmEndWaitLen2Aagc = st.FrmEndWaitLen2Aagc
    all_st.agc_freq_config_st.aagc_wait.pad = PY_ZERO

    all_st.agc_freq_config_st.lsr_cfg_st.LSR_MeanNum = st.LSR_MeanNum
    all_st.agc_freq_config_st.lsr_cfg_st.pad = PY_ZERO

    all_st.agc_freq_config_st.qk_win_para_st.aagc_quick_win_size = st.aagc_quick_win_size
    all_st.agc_freq_config_st.qk_win_para_st.pad1 = PY_ZERO
    all_st.agc_freq_config_st.qk_win_para_st.aafc_quick_win_num = st.aagc_quick_win_num
    all_st.agc_freq_config_st.qk_win_para_st.pad2 = PY_ZERO
    all_st.agc_freq_config_st.qk_win_para_st.aagc_quick_used_num = st.aagc_quick_used_num
    all_st.agc_freq_config_st.qk_win_para_st.pad3 = PY_ZERO

    all_st.agc_freq_config_st.st23_win_para_st.st23_win_size = st.st23_win_size
    all_st.agc_freq_config_st.st23_win_para_st.pad1 = PY_ZERO
    all_st.agc_freq_config_st.st23_win_para_st.st23_win_num = st.st23_win_num
    all_st.agc_freq_config_st.st23_win_para_st.pad2 = PY_ZERO
    all_st.agc_freq_config_st.st23_win_para_st.st23_used_num = st.st23_used_num
    all_st.agc_freq_config_st.st23_win_para_st.pad3 = PY_ZERO

    all_st.agc_freq_config_st.sunspec_alpha_filter.SunSpecpower_alpha1 = st.SunSpecpower_alpha1
    all_st.agc_freq_config_st.sunspec_alpha_filter.pad = PY_ZERO

    all_st.agc_freq_config_st.SunSpecLargeWinNum = st.SunSpecLargeWinNum
    all_st.agc_freq_config_st.AagcSampleNum = st.AagcSampleNum
    all_st.agc_freq_config_st.Tx_Power = st.Tx_Power
    all_st.agc_freq_config_st.pad = PY_ZERO
#CARRIER SENSE Module Assignment-----------------------#
    all_st.carrier_sense_freq_config_st.cs_ch_carrier_0_st.chCarrier_start0 = st.chCarrier_start0
    all_st.carrier_sense_freq_config_st.cs_ch_carrier_0_st.chCarrier_stop0 = st.chCarrier_stop0
    all_st.carrier_sense_freq_config_st.cs_ch_carrier_0_st.chCarrier_start1 = st.chCarrier_start1
    all_st.carrier_sense_freq_config_st.cs_ch_carrier_0_st.chCarrier_stop1 = st.chCarrier_stop1

    all_st.carrier_sense_freq_config_st.cs_ch_carrier_1_st.chCarrier_start2 = st.chCarrier_start2
    all_st.carrier_sense_freq_config_st.cs_ch_carrier_1_st.chCarrier_stop2 = st.chCarrier_stop2
    all_st.carrier_sense_freq_config_st.cs_ch_carrier_1_st.chCarrier_start3 = st.chCarrier_start3
    all_st.carrier_sense_freq_config_st.cs_ch_carrier_1_st.chCarrier_stop3 = st.chCarrier_stop3

    all_st.carrier_sense_freq_config_st.CsNbiEn = st.CsNbiEn
    all_st.carrier_sense_freq_config_st.ch_num = st.ch_num
    all_st.carrier_sense_freq_config_st.pad = PY_ZERO
#IMP Module Assignment-----------------------#
    all_st.imp_freq_config_st.ImpWinSize= st.ImpWinSize
    all_st.imp_freq_config_st.ImpUsedWinNum= st.ImpUsedWinNum
    all_st.imp_freq_config_st.ImpWinNum= st.ImpWinNum
    all_st.imp_freq_config_st.pad= PY_ZERO
    all_st.imp_freq_config_st.imp_cfg_7.ImpWinInd1Set0HalfLenOldPream = st.ImpWinInd1Set0HalfLenOldPream
    all_st.imp_freq_config_st.imp_cfg_7.pad1 = PY_ZERO
    all_st.imp_freq_config_st.imp_cfg_7.ImpWinInd1Set0HalfLenNewPream = st.ImpWinInd1Set0HalfLenNewPream
    all_st.imp_freq_config_st.imp_cfg_7.pad2 = PY_ZERO
    all_st.imp_freq_config_st.imp_cfg_7.ImpWinInd2Set0HalfLenOldPream = st.ImpWinInd2Set0HalfLenOldPream
    all_st.imp_freq_config_st.imp_cfg_7.pad3 = PY_ZERO
    all_st.imp_freq_config_st.imp_cfg_7.ImpWinInd2Set0HalfLenNewPream = st.ImpWinInd2Set0HalfLenNewPream
    all_st.imp_freq_config_st.imp_cfg_7.pad4 = PY_ZERO

    all_st.imp_freq_config_st.imp_cfg_8.ImpWinInd3Set0HalfLenOldPream = st.ImpWinInd3Set0HalfLenOldPream
    all_st.imp_freq_config_st.imp_cfg_8.pad1 = PY_ZERO
    all_st.imp_freq_config_st.imp_cfg_8.ImpWinInd3Set0HalfLenNewPream = st.ImpWinInd3Set0HalfLenNewPream
    #print(all_st.imp_freq_config_st.imp_cfg_8.ImpWinInd3Set0HalfLenNewPream)
    all_st.imp_freq_config_st.imp_cfg_8.pad2 = PY_ZERO
    all_st.imp_freq_config_st.imp_cfg_8.ImpWinInd1Set0HalfLenOldFCPL = st.ImpWinInd1Set0HalfLenOldFCPL
    all_st.imp_freq_config_st.imp_cfg_8.pad3 = PY_ZERO
    all_st.imp_freq_config_st.imp_cfg_8.ImpWinInd1Set0HalfLenNewFCPL = st.ImpWinInd1Set0HalfLenNewFCPL
    all_st.imp_freq_config_st.imp_cfg_8.pad4 = PY_ZERO

    all_st.imp_freq_config_st.imp_cfg_9.ImpWinInd2Set0HalfLenOldFCPL = st.ImpWinInd2Set0HalfLenOldFCPL
    all_st.imp_freq_config_st.imp_cfg_9.pad1 = PY_ZERO
    all_st.imp_freq_config_st.imp_cfg_9.ImpWinInd2Set0HalfLenNewFCPL = st.ImpWinInd2Set0HalfLenNewFCPL
    all_st.imp_freq_config_st.imp_cfg_9.pad2 = PY_ZERO
    all_st.imp_freq_config_st.imp_cfg_9.ImpWinInd3Set0HalfLenOldFCPL = st.ImpWinInd3Set0HalfLenOldFCPL
    all_st.imp_freq_config_st.imp_cfg_9.pad3 = PY_ZERO
    all_st.imp_freq_config_st.imp_cfg_9.ImpWinInd3Set0HalfLenNewFCPL = st.ImpWinInd3Set0HalfLenNewFCPL
    all_st.imp_freq_config_st.imp_cfg_9.pad4 = PY_ZERO

# Symbol synchronization module assignment-----------------------#
    all_st.symbol_align_freq_config_st.recipoint_param = st.recipoint_param
    all_st.symbol_align_freq_config_st.NoiseNbiCarrierNum = st.NoiseNbiCarrierNum
    all_st.symbol_align_freq_config_st.pad = PY_ZERO

# NBI module assignment---------------------------#
    all_st.nbi_freq_config_st.NbiMaxPowScale = st.NbiMaxPowScale
    all_st.nbi_freq_config_st.pad = PY_ZERO
# Newly introduced in 20191010
    all_st.nbi_check_config.nbi_time_period = st.nbi_time_period
    all_st.nbi_check_config.nbi_check_min_level = st.nbi_check_min_level
    all_st.nbi_check_config.nf_use2_en = st.nf_use2_en
    all_st.nbi_check_config.nbi_use2_force_en = st.nbi_use2_force_en
    all_st.nbi_check_config.pad = PY_ZERO
    all_st.nbi_check_config.pad2 = PY_ZERO
# Low-power module assignment-------------------------#
    all_st.low_consume_freq_config_st.time_proc_delay = st.time_proc_delay
# Assigning other variables
    all_st.freqHPFMode = st.freqHPFMode
    all_st.freqLPFMode = st.freqLPFMode
    all_st.afe_config = st.afe_config
    all_st.pad = PY_ZERO
    all_st.max_frame_length = st.max_frame_len
    strs = ''
    i = 0
    while i < len(test_bin):
        x=test_bin[i]+(test_bin[i+1]<<8)+(test_bin[i+2]<<16)+(test_bin[i+3]<<24)
        strs = '%s%s,'%(strs,str(hex(x)))
        i=i+4
    return strs

#-----------------------------Manipulating file function definitions-----------------------------------
def get_freq_config_files(file_dir):
    L=[]
    i = 0
    c = 0
    for root, dirs, files in os.walk(file_dir):
        for file in files:
            if os.path.splitext(file)[1] == '.txt':
                    result =  re.findall('.+?\_[0-9]+.txt',file)
                    if result :
                        #print file
                        L.append(os.path.join(root, file))
    return L
#crc check
def my_crc32_check(strings):
    m_pdwCrc32Table = [0 for x in range(0,256)]
    dwPolynomial = 0xEDB88320;
    dwCrc = 0
    for i in range(0,255):
        dwCrc = i
        for j in [8,7,6,5,4,3,2,1]:
            if dwCrc & 1:
                dwCrc = (dwCrc >> 1) ^ dwPolynomial
            else:
                dwCrc >>= 1
        m_pdwCrc32Table[i] = dwCrc
    dwCrc32 = 0xFFFFFFFF
    for i in strings:
        b = ord(i)
        dwCrc32 = ((dwCrc32) >> 8) ^ m_pdwCrc32Table[(b) ^ ((dwCrc32) & 0x000000FF)]
    dwCrc32 = dwCrc32 ^ 0xFFFFFFFF
    return dwCrc32

# File CRC check
def crc32_check_freq_file(file):
    line_row = PY_ZERO
    num = PY_ZERO
    magic = ""
    mask = 0xffffffff
    with open(file,"r") as f:
        crc_num = PY_ZERO
        with open(''.join([os.path.split(file)[PY_ZERO],crc_tmp_name]),"w+") as nf:
            lines = f.readlines()
            i = PY_ZERO
            for li in lines:
                i += PY_ONE
            if i < crc_result_line:# If there are not enough lines in the file
                nf.close()
                f.close()
                delet_file_by_path(''.join([os.path.split(file)[PY_ZERO],crc_tmp_name]))# Delete file directory
                return False
            freq_num = PY_ZERO
            for li in lines:
                line_row += PY_ONE
                li = li.strip('\n')
                #print li
                if line_row == PY_ONE:
                    freq_num += int(li)# Get Band Number
                    #print freq_num
                if (line_row + PY_ONE)== firstMagicLine or (line_row + PY_ONE)== secondMagicLine:# Get previous number of devil numbers
                    num = int(li)
                if line_row == firstMagicLine or line_row == secondMagicLine:# Devil Digital Line
                    t = freq_num+ num
                    if int(li)-t != PY_ZERO:# Determine if the number of devil is the same
                        f.close()
                        nf.close()
                        delet_file_by_path(''.join([os.path.split(file)[PY_ZERO],crc_tmp_name]))
                        return False
                    #continue Devil numbers check together
                if line_row < crc_result_line-1:# Handling newlines
                    nf.write('%s\n'%str(li))
                elif line_row < crc_result_line:
                    nf.write(li)
                if line_row == crc_result_line:
                    crc_num = (li)
                    #print crc_num
            nf.close()
        with open(''.join([os.path.split(file)[PY_ZERO],crc_tmp_name]),"r") as nf:
            crc_res = '{:x}'.format(mask & my_crc32_check(nf.read()))+''# Change to hexadecimal data
            #print crc_res
        f.close()
        delet_file_by_path(''.join([os.path.split(file)[PY_ZERO],crc_tmp_name]))
        return crc_res == crc_num

def delet_file_by_path(path):
    if os.path.exists(path):
            os.remove(path)
            return
    raise FilepathException(path)

def write_all_target_str_header(target_p,all_str,num,length):
    target_path = target_p
    header_str = "#ifndef _PHY_FREQ_CFG_H_\n#define _PHY_FREQ_CFG_H_\n\n#define  FREQ_CFG_NUM  %s\n#define  ARRAY_LEN %s\n#define  PHY_FREQ_CFG_ST  %s\n#endif"%(str(num),str(length),all_str)
    with open(target_path,'w+') as hf:
        hf.write(header_str)
    hf.close()

def write_check_result_log(file_cfg,flag):
    target_path = os.path.join('..', 'mdm', 'phy', 'phy_freq_txt_cfg', 'check_log.txt') # Generated header file address
    tmp_time = '[%s]:'%time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time()))
    with open(target_path,'a+') as hf:
        if flag == True:
            hf.write('\n\n')
        else:
            hf.write('%s%s check failure \n'%(tmp_time,os.path.split(file_cfg)[1]))
    hf.close()

#-----Sort files in list
def sort_by_freq_num(file_list):
    new_list = []
    name_list = []
    for file in file_list:
        t = int(os.path.split(file)[1].split('_')[1].split('.')[0],10)
        if t > 0:
            name_list.append(t)
    name_list.sort(reverse = False)
    for name in name_list:
        #print (name)
        for file in file_list:
            num = os.path.split(file)[1].split('_')[1].split('.')[0]
            if num == str(name):
                new_list.append(file)
                break
    return new_list

def main_handle_freq_txt(path,target_path):
    data_path = target_path
    config_path = path
    count = 0# count
    i = 0
    bo_f  = False
    all_target_str = ''.join(['{','         \\','\n'])
    if os.path.isdir(config_path) == False:# Determine if the file directory exists
        return
    file_list = get_freq_config_files(config_path)# Get qualified txt files in the directory
    file_list = sort_by_freq_num(file_list)
    file_num = len(file_list)# Number of configuration files in the directory
    if file_num > 8:
        file_num = 8
    if file_num == PY_ZERO:
        all_target_str = '{0}'
        write_all_target_str_header(target_path,all_target_str,file_num,hex(sizeof(phy_all_freq_config_st)))
        return
    for config_file in file_list:#Traversing files
        count += PY_ONE# Count up
        if crc32_check_freq_file(config_file) == False:# crc check failed
            file_num = file_num - 1
            bo_f = True
            write_check_result_log(config_file,False)
            continue
        i = i+1
        tmp_st = init_tmp_st_from_file(config_file)#Read file to initialize temporary structure
        strs = init_target_st_from_tmp_st(tmp_st)#Data into strings
        #if i == file_num:
        all_target_str = ''.join([all_target_str ,(strs)])#Stitch the target strings together
        #else:
            #all_target_str = ''.join([all_target_str ,(strs),','])#Handle delimiters between different target strings
        all_target_str = ''.join([all_target_str , '\\', '\n'])
        if i == file_num:
            break;
    all_target_str = ''.join([all_target_str , '}'])
    write_all_target_str_header(target_path,all_target_str,hex(i),hex(sizeof(phy_all_freq_config_st)))
    if bo_f == True:
        write_check_result_log(config_file,True)
#target_path = '..\\..\\..\\mdm\\phy\\plc_freq\\phy_freq_cfg.h'#Generated header file address

# Path of the current file
pwd = os.getcwd()

# Parent path of the current file
father_path = os.path.pardir
target_path = os.path.join(father_path,'build_tmp/config/phy_freq_cfg.h')
path = os.path.join(father_path,'mdm/phy/phy_freq_txt_cfg')

main_handle_freq_txt(path,target_path)

