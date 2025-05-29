/*
 * Copyright (c) CompanyNameMagicTag 2021-2021. All rights reserved..
 * Description: DFX interfaces
 */
/* *
 * Define the maximum limit of related items.
 */
#ifndef DIAG_DFX_MAC_VALUE_DEF_H
#define DIAG_DFX_MAC_VALUE_DEF_H

#define DFX_NETWORK_TOPO_ENTRY_MAX 8
#define DFX_STA_MAC_RPT_DISCOVER_PKT_NUM 4
#define DFX_RX_STATICS_ENTRY_MAX 16
#define DFX_TDMA_SCHEDULE_ENTRY_MAX 10
#define DFX_OFFLINE_STA_ENTRY_MAX 10
/* Maximum size of large packets that can be transferred: 5 KB */
#define DFX_MAX_BIG_PKT_SIZE 3072
#define DFX_NORMAL_PKT_SIZE_2K 2048
#define DFX_MIN_BIG_PKT_SIZE 2048
/* Number of STAs in the path information table */
#define DFX_ROUTER_TABLE_ENTRY_MAX 31
/* Size of the reporting structure of the path information table */
#define DFX_ROUTE_TABLE_PER_PACKET_SIZE 380
/* Number of STAs in the discovery list */
#define DFX_DISCOVER_LIST_ENTRY_MAX 19
/* Size of the structure reported in the discovery list */
#define DFX_DISCOVER_LIST_ENTRY_PACKET_SIZE 388
/* Number of sites in the simplified discovery list */
#define DFX_DISCOVER_LIST_ENTRY_SM_MAX 19
/* Size of the structure reported in the simplified discovery list */
#define DFX_SM_DISCOVER_LIST_ENTRY_PACKET_SIZE 380
/* Simplify the topology--Number of sites in the site information */
#define DFX_TOPO_STA_INFO_SM_MAX 31
/* Simplified topology: size of the structure for reporting site information */
#define DFX_SM_TOPO_STA_INFO_PACKET_SIZE 376
/* Simplify the topology--Version information Number of sites */
#define DFX_STA_VER_INFO_SM_MAX 8
/* Simplify the topology-size of the structure for reporting the version information */
#define DFX_SM_STA_VER_INFO_PACKET_SIZE 228
#define DFX_ASSOCIATED_STAS_ENTRY_MAX 48
/* Maintain and test the association status */
#define NM_ASSOC_DFX_RCV_BEACON 1             /* receive beacon. */
#define NM_ASSOC_DFX_RCV_REFUSE 2             /* Reject packets are received. */
#define NM_ASSOC_DFX_NOT_ALLOW_ASSOC_BY_BCN 4 /* The received beacon information does not allow association. */
/* Maximum number of members in the IP-MAC address mapping table at
 * the site when the IP address is used to connect to the UAC.
 */
#define PLC_NDM_IP_MAC_MAP_NUM_MAX 4
#define MAC_DIAG_CMD_QUERY_NETWORK_TOPO_ENTRY_NUM 8 /* Number of nodes in the topology        \
                                                     * reporting structure of the NDA channel \
                                                     */
#define DFX_NTB_SYNC_STA_CNT_MAX 4
#define DFX_NTB_EXCEPTION_CNT_MAX 8
#define DFX_NTB_SINGU_FREQERR_CNT_MAX 8
#define RX_STAT_NUM 16
/* Node search mode */
#define NM_NODE_FIND_MODE_STOP 0x00     /* disables node search. */
#define NM_NODE_FIND_MODE_ERR_LOCK 0x01 /* Search for the point that is locked incorrectly \
                                           and does not access the network. */
#define NM_NODE_FIND_MODE_LIST 0x02     /* Searches for the vertex in the MAC address list. */
/* Valid node lookup mode mask */
#define NM_NODE_FIND_MODE_MASK 0x03
#define NODE_FIND_MAC_LIST_NUM 8   /* supports the maximum number of MAC addresses \
                                    * that can be searched at the same time.       \
                                    */
#define NODE_FIND_MAC_LIST_SIZE 48 /* Length of the MAC address list = Number of supported addresses x 6 */

#define MAX_NEIGHBOR_SUM 15       /* Maximum number of neighboring networks, excluding the local network */
#define QUERY_MAX_NEIGHBOR_SUM 30 /* Maximum number of neighboring networks involved in the query interface, \
                                   * excluding the local network                                             \
                                   */
/*
 * Category reported to DBK
 */
#define TYPE_DEBUG_RELEASE 0x02 /* Indicates that the DBK R&D commissioning version and \
                                   onsite maintenance version coexist. */
#define TYPE_DEBUG 0x00         /* Indicates the DBK R&D commissioning version. */
#define TYPE_RELEASE 0x01       /* Indicates the onsite maintenance version of the DBK. */
/* Inter-layer message ID (0x1000~0x5000)
 * Common service query command ID. The value ranges from 0X1000 to 0X1BFF.
 */

#define ID_DIAG_CMD_QUERY_ID_SEG_START 0x1000
#define ID_DIAG_CMD_QUERY_ID_SEG_END 0x1BFF
#define ID_DIAG_CMD_QUERY_NETWORK_CONVERGENCE_TIME 0x1000  /* time consumed for querying the \
                                                              self-networking of a site */
#define ID_DIAG_CMD_QUERY_NETWORK_TOPO 0x1001              /* querying the network topology table */
#define ID_DIAG_CMD_QUERY_ROUTER_TABLE 0x1002              /* Query routing table information */
#define ID_DIAG_CMD_QUERY_TX_STATICS 0X1003                /* Query Transmitter Statistics */
#define ID_DIAG_CMD_QUERY_RX_STATICS 0X1004                /* Query Receiver Statistics */
#define ID_DIAG_CMD_QUERY_RSB_STATICS_PK 0X1005            /* Query Reassembly Module Statistics */
#define ID_DIAG_CMD_QUERY_NETWORK_TOPO_SM 0X1006           /* Query Simplified Topology-Site Information \
                                                            * (P1901 Remotely Added)                     \
                                                            */
#define ID_DIAG_CMD_QUERY_ALL_STA_INFO 0X1007              /* queries the number of site \
                                                              proxy changes and offline times. */
#define ID_DIAG_CMD_QUERY_DEVICE_FREQ_INFO 0X1008          /* Query BTS Frequency Band Information */
#define ID_DIAG_CMD_QUERY_NETWORK_STATE_INFO 0X1009        /* Query the CCO-side networking information or \
                                                            * STA short association information.           \
                                                            */
#define ID_DIAG_CMD_QUERY_NEIGHBOR_NETWORK 0x100A          /* Query neighboring network information */
#define ID_DIAG_CMD_QUERY_NETWORK_ATTRIBUTE 0x100B         /* Query Site Network Attribute */
#define ID_DIAG_CMD_QUERY_REQUEST_JOIN_FAILED_COUNT 0x100C /* Query Site Association Failures */
#define ID_DIAG_CMD_QUERY_JOIN_NETWORK_EXPIRED 0x100D      /* queries the time required for site association. */
#define ID_DIAG_CMD_QUERY_ASSOCIATED_STAS 0x100E           /* queries the list of objects associated with a site. */
#define ID_DIAG_CMD_QUERY_DISCOVER_LIST 0x100F             /* queries the site discovery list. */
#define ID_DIAG_CMD_QUERY_TOPO_VER_SM 0x1010               /* Query the simplified topology-version information \
                                                            * (p1901 remote adding)                             \
                                                            */
#define ID_DIAG_CMD_QUERY_PHS_STAT 0x1011                  /* Query the Statistics of the Phase \
                                                              Identification Module. */
#define ID_DIAG_CMD_QUERY_NDM_CONNECT_STAT 0x1012          /* Queries the connection information about the meter \
                                                            * reading controller (disabled).                     \
                                                            */
#define ID_DIAG_CMD_QUERY_VIOLENCE_STA 0x1013              /* Queries the CCO-side rejection list (not in use). */
#define ID_DIAG_CMD_QUERY_DISCOVER_LIST_SM 0x1016          /* simplified discovery list */
#define ID_DIAG_CMD_QUERY_SECURITY_INFO 0x101a             /* protocol security statistics */
#define ID_DIAG_CMD_QUERY_FREQUENCY_STATUS 0x101b          /* Indicates whether a network-wide frequency band \
                                                            * change frame is received.                       \
                                                            */
#define ID_DIAG_CMD_CCO_MAC_MULTYNET_QUERY 0x101e          /* Query SNID (Different Behaviors Between STA and CCO) */
#define ID_DIAG_CMD_CCO_MAC_MULTYNET_SET 0x101f            /* multi-network setting command. Set the frequency band and
                                                            * frequency switching time. If the STA and CCO are
                                                            * in different frequency switching states
                                                            */
#define ID_DIAG_CMD_MAC_FREQ_SET 0x1020
/* Query the frequency band, whether the handover is in progress, and the remaining time of the handover progress.
 * The STA and CCO are different.
 */
#define ID_DIAG_CMD_MAC_FREQ_QUERY 0x1021
/* Whether to enable the status query of the whitelist and whether to add the meter archive to the whitelist */
#define ID_DIAG_CMD_CCO_MAC_WHITE_STA_QUERY 0x1022
#define ID_DIAG_CMD_CCO_MAC_WHITE_STA_SET 0x1023     /* whitelist configuration */
#define ID_DIAG_CMD_CCO_MAC_CLR 0x1024               /* clear reject list */
#define ID_DIAG_CMD_QUERY_CVG_STATICS_PK 0x1025      /* convergence and distribution packet statistics */
#define ID_DIAG_CMD_QUERY_LEAVE_IND_STAT_INFO 0x1026 /* statistics offline indication packet */
#define ID_DIAG_CMD_SOFT_RESET 0x1027                /* queries the soft restart information of a site. */
#define ID_DIAG_CMD_PROXY_NUM_QUERY 0x1028           /* Query the current number of proxies. */
#define ID_DIAG_CMD_CHILD_SUM_QUERY 0x1029           /* Query the number of substations of a site. */
#define ID_DIAG_CMD_NM_PACKET_STAT_QUERY 0x102a      /* NMS packet count */
#define ID_DIAG_CMD_NM_ASSOC_REQ_STAT_QUERY 0x102b   /* association request packet statistics */
#define ID_DIAG_CMD_NM_ASSOC_CNF_STAT_QUERY 0x102c   /* association response packet statistics */
#define ID_DIAG_CMD_STA_PROXY_CHANGE_INFO 0x102e     /* STA obtains the maintainable and testable \
                                                      * information about proxy changes.          \
                                                      */
/* Obtain the maintainability and testability information about
network management association and proxy request processing. */
#define ID_DIAG_CMD_ASSOC_INFO 0x102f
#define ID_DIAG_CMD_QUERY_FC_PB_FAULT_INFO 0x1031 /* Obtain the statistics of FC and PB mismatch. */
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM) || defined(PRODUCT_CFG_DBK)
#define ID_DIAG_CMD_GET_NDM_INFO 0x1035    /* Obtains the working mode and TEI of the UAC. */
#define ID_DIAG_CMD_SET_NDM_ATTR 0x1036    /* Sets the TEI of the NMR. */
#define ID_DIAG_CMD_GET_NDM_NV_INFO 0x1083 /* Obtains the working mode and TEI of the NV item of the NDM. */
#endif
#define ID_DIAG_CMD_LED_STATUS_INFO 0x1037              /* queries the LED indicator status. */
#define ID_DIAG_CMD_QUERY_RX_DIAGNOSTIC 0x1038          /* Query the Diagnostic Statistics of the Receiver */
#define ID_DIAG_CMD_QUERY_TX_DIAGNOSTIC 0x1039          /* Query the Diagnostic Statistics of the Transmitter */
#define ID_DIAG_CMD_QUERY_COMM_RATE_INFO 0x1040         /* Query and Report Communication Rate Information */
#define ID_DIAG_CMD_QUERY_CHOSEN_STA_DISLIST 0x1041     /* Query the sending list of a specified site. */
#define ID_DIAG_CMD_QUERY_CHOSEN_STA_TOPOINFO 0x1042    /* Query topology information of a specified site */
#define ID_DIAG_CMD_QUERY_VERSION_INFO 0x1043           /* Query the version information and packet injection NV \
                                                         * configuration information of the site.                \
                                                         */
#define ID_DIAG_CMD_QUERY_BACKUP_MAC 0x1045             /* Query the backup MAC address. */
#define ID_DIAG_CMD_SET_BACKUP_MAC 0x1046               /* Set the backup MAC address. */
#define ID_DIAG_CMD_NDM_FOLLOW_STAT_STRU 0x1047         /* Query the Follow-up Information About the Patrol Data \
                                                         * Collection of the Reader                              \
                                                         */
#define ID_DIAG_CMD_SET_NDM_FOLLOW 0x1048               /* Sets the data acquisition follow-up function of \
                                                         * the meter reading controller.                   \
                                                         */
#define ID_DIAG_CMD_QUERY_MAX_VIOLENCE_STA 0x1049       /* Set the number of records in the rejection list to 200. */
#define ID_DIAG_CMD_MAC_GET_WHITELIST 0x104a            /* Query whitelist */
#define ID_DIAG_CMD_MAC_REPORT_WHITELIST_CHANGED 0x104b /* Report whitelist changes. */
#define ID_DIAG_CMD_SET_LOCK_NETWORK 0x1058             /* Locks the network. */
#define ID_DIAG_CMD_SET_UNLOCK_NETWORK 0x105a           /* unlocks the network. */
#define ID_DIAG_CMD_NDM_QUERY_P2P_INFO 0x1059           /* Query P2P Statistics */
#define ID_DIAG_CMD_NDM_QUERY_CIU_STATE 0x1060          /* Queries the CIU status. */
#define ID_DIAG_CMD_QUERY_STA_INFO 0x1061               /* Query workstation information          \
                                                         * (supplement to the 101b query command) \
                                                         */
#define ID_DIAG_CMD_QUERY_CONNECTED_GRAPH_INFO 0x1062   /* Query Site Connection Information */
#define ID_DIAG_CMD_QUERY_ZC_DFX_INFO 0x1065            /* Query the maintenance and test information \
                                                         * collected at 00:00.                        \
                                                         */
#define ID_DIAG_CMD_AD_COLLECT_DFX_INFO 0x1066          /* AD acquisition, maintainability and testability */
/*
 * locks or unlocks the maintainability and testability
 * information of the STA.
 */
#define ID_DIAG_CMD_TF_LOCK_STA_OM 0x106f
#define ID_DIAG_CMD_QUERY_PHASE_IDENTIFY_RESULT_REQ 0X1077 /* DBK Query the phase identification result */
#define ID_DIAG_CMD_PI_CCO_STATIC 0X1078                   /* Maintainability and testability \
                                                              of CCO phase identification */
#define ID_DIAG_CMD_PI_CCO_BASE_NTB 0x1079                 /* phase identification CCO-side \
                                                              reference NTB data reporting */
#define ID_DIAG_CMD_ASSOC_DFX_INFO 0x107A                  /* sites added to the network for \
                                                              maintainability and testability */
#define ID_DIAG_CMD_MULTI_NETWORK_ASSOC_DFX_INFO 0x107B    /* Multi-Network Association \
                                                              Maintainability and Testability */
#define ID_DIAG_CMD_SERIAL_NUM_CHANGE_DFX_INFO 0x107C      /* networking serial number, restart, \
                                                            * maintainability and testability    \
                                                            */
#define ID_DIAG_CMD_PI_STA_STATIC 0X1082                   /* Maintainability and testability \
                                                              of STA phase identification */
#define ID_DIAG_CMD_QUERY_TL_INFO 0x1088                   /* Query Transport Layer Information */
#define ID_DIAG_CMD_QUERY_MESH_RT_INFO 0x1089              /* query the information of mesh route  */
#define ID_DIAG_CMD_COORDINATE_CONFLICT_INFO 0x1090        /* STA multi-network conflict information */
#define ID_DIAG_CMD_TF_CROSSTALK_RESULT_REQ 0x1094         /* DBK Query the non-station information              \
                                                            * in the transformer district identification result. \
                                                            */
#define ID_DIAG_CMD_QUERY_APP_VER_INFO 0x1095              /* Query the version information of the application layer. */
#define ID_DIAG_CMD_QUERY_MESH_RT_INFO_BY_MAC 0x1096
#define ID_DIAG_CMD_STA_QUERY_SIGNAL_QUALITY_TO_PROXY 0x109b
#define ID_DIAG_CMD_CCO_QUERY_SIGNAL_QUALITY_TO_PROXY 0x109c
#define ID_DIAG_CMD_CCO_RT_OPTI_POLICY 0x109d
#define ID_DIAG_CMD_CCO_RT_OPTI_POLICY_TEST 0x109e
#define ID_DIAG_CMD_QUERY_GT_NTB_RESET_INFO 0x10a7
#define ID_DIAG_CMD_TX_SEND_QUEUE_INFO 0x10aa
#define ID_DIAG_CMD_SLOT_UPDATE_INFO 0x10ab
#define ID_DIAG_CMD_QUERY_NTB_STAT_V200 0x10ac         /* Queries the statistics of the NTB module. */
#define ID_DIAG_CMD_QUERY_NTB_FREQERR_STAT_V200 0x10ad /* Query NTB's Frequency Deviation Statistics. */
#if defined(PRODUCT_CFG_PRODUCT_TYPE_NDM)
#define ID_DIAG_CMD_REPORT_NDM_CAPTURE 0x1a90    /* NDM report capture message */
#define ID_DIAG_CMD_SET_NDM_CAPURE_SWITCH 0x1a91 /* NDM set capture switch */
#define ID_DIAG_CMD_SET_NDM_ACCESS_INFO 0x1a92   /* DBK send access info */
#define ID_DIAG_CMD_STOP_CAPTURE 0x1a94          /* DBK stop capture */
#endif
#define ID_DIAG_CMD_QUERY_NETWORK_ATTRIBUTE_V200 0x10bb /* Query the Network Attributes of a Site (V200) */
#define ID_DIAG_CMD_MULTI_NETWORK_RCV_INFO_V200 0x10be  /* V200 multi-network packet receiving query */
#define ID_DIAG_CMD_SET_TRANSMIT_POWER_V200 0x10C0      /* sets the transmit power. */
#define ID_DIAG_CMD_DELAYED_RESTART_V200 0x10C1         /* network-wide delayed restart */
#define ID_DIAG_CMD_COMMON_TEMP_COMMAND 0x10C2          /* common temporary commands */
#define ID_DIAG_CMD_REPORT_PB_STAT 0x10C3
#define ID_DIAG_CMD_GET_TOP_NEIGHBOR 0x10c4           /* Query the best neighbor of the site */
#define ID_DIAG_CMD_STA_COLLECT_RESULT_SIMPLE 0x10c6  /* Query the brief SNR statistics. */
#define ID_DIAG_CMD_NET_BEST_NODE_STATISTICS 0x10c7   /* Query the best node on a single network. */
#define ID_DIAG_CMD_NET_SHORT_CYCLE_STATISTICS 0x10c8 /* Query Single-Network Short-Period Statistics */
#define ID_DIAG_CMD_CCO_SET_NODE_FIND_PARA 0x10ca     /* CCO Set node search parameters. */
#define ID_DIAG_CMD_STA_FAST_CHANGE_STAT 0x10cb       /* Query the Fast Convergence Record of a Site */
#define ID_DIAG_CMD_STA_SNR_CTRL_STAT 0x10cd          /* Query the SNR policy control status.  */
#define ID_DIAG_CMD_QUERY_LOCK_INFO 0x10cf            /* Query lock information */
#define ID_DIAG_CMD_QUERY_NEAR_STA_DATA 0x10d4        /* Query the information about the nearby sites collected \
                                                       * by the site.                                           \
                                                       */
#define ID_DIAG_CMD_QUERY_NEAR_NET_INFO 0x10d5        /* Query the nearby network information \
                                                         collected by the site. */
#define ID_DIAG_CMD_SET_BELONG_NETWORK 0x10d6         /* Set the home network of the site. */
#define ID_DIAG_CMD_CLEAR_BELONG_NETWORK 0x10d7       /* clear the home network of the site. */
#define ID_DIAG_CMD_GET_BELONG_CHANGE_REOCRDS 0x10da  /* obtains the ownership change record. */
#define ID_DIAG_CMD_QUERY_S_SPEC_INFO 0x10df          /* Query SPC Information */
#define ID_DIAG_CMD_QUERY_FAST_SEND_INFO 0x10e0       /* fast sending statistics */
#define ID_DIAG_CMD_QUERY_RCV_BT_CHL_INFO  0x10f0
#define ID_DIAG_CMD_MAC_PHYSICAL_TEST_MODE 0x1A01
#define ID_DIAG_CMD_MAC_BUFF_INFO 0x1a02
/* Recorder service class ID [0x1c00 ~ 0x1c80) */
#define ID_DIAG_CMD_NDM_CTRL_BUSI_ID_SEG_START 0x1c00
#define ID_DIAG_CMD_NDM_CTRL_BUSI_ID_SEG_END 0x1c80
#define ID_DIAG_CMD_NDM_CTRL_ROUTE_LOCK 0x1C00   /* route lockout */
#define ID_DIAG_CMD_NDM_CTRL_ROUTE_UNLOCK 0x1C01 /* route unlock */
#define ID_DIAG_CMD_NDM_CTRL_ROUTE_CONFIG 0x1C02 /* static route configuration */
#define ID_DIAG_CMD_NDM_CTRL_TRACE 0x1C03        /* trace command */
#define ID_DIAG_CMD_NDM_CTRL_PING 0x1C04         /* ping command */
#define ID_DIAG_CMD_NDM_CTRL_SEARCH_DEV 0x1C05   /* device search */
#define ID_DIAG_CMD_NDM_NETWORK_SEARCH 0x3408    /* network searching command */
/* Other services [0x1c81~0x1ca0) */
/* Used for the NCR channel to return ACK each time a site is reached. */
#define ID_DIAG_CMD_NDM_CTRL_PIPE_ACK 0x1c82
#define ID_DIAG_CMD_QUERY_CMD_NOT_SUPPORT 0x1c83
#define ID_DIAG_CMD_MAC_HANDLE_CONNECT_IND 0x1C86
#define ID_DIAG_CMD_MAC_HANDLE_CONNECT_CNF 0x1C87
/* Reserved [0x1ca1~0x2000]
 * The preceding commands are query commands.
 * Command line switch: 0x1C00~0x1c0F
 * DIAG commands related to the NCR (0x1C10~0X1CFF)
 * START: unsolicited report class and storage class ID (0x2900~0x3000)
 */
#define ID_DIAG_CMD_NOTIFY_MAC_SPEED_LOW 0x2900
#define ID_DIAG_CMD_REPORT_TXRX_STATIC_SPEED 0x2901
#define ID_DIAG_CMD_REPORT_JOIN_FAILED 0x2902
#define ID_DIAG_CMD_REPORT_NETWORK_ATTRIBUTE 0x2905
#define ID_DIAG_CMD_RECORD_SPEED_ABNORMITY 0x2922
/* END Unsolicited report class and storage class ID
 * Unsolicited report class ID: 0x2900??x3000
 * Event ID (0x1000, 0x5000)
 */
#define ID_DIAG_EVENT_NO_SACK_BROADCAST 0x1001
#define ID_DIAG_EVENT_NOTIFY_MAC_RX_SPEED_LOW 0x1002
#define ID_DIAG_EVENT_NOTIFY_MAC_TX_SPEED_LOW 0x1003
#endif