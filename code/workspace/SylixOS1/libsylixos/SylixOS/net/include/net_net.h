/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: net_net.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2009 年 05 月 06 日
**
** 描        述: sylixos inet.
*********************************************************************************************************/

#ifndef __NET_NET_H
#define __NET_NET_H

/*********************************************************************************************************
  网络配置
*********************************************************************************************************/
#include "../SylixOS/config/net/net_tools_cfg.h"                        /*  网络工具配置                */
/*********************************************************************************************************
  网络功能
*********************************************************************************************************/
#ifdef __SYLIXOS_KERNEL
#include "../lwip/lwip_sylixos.h"                                       /*  lwip sylixos 移植           */
#endif                                                                  /*  __SYLIXOS_KERNEL            */
/*********************************************************************************************************
  网络工具
*********************************************************************************************************/
#include "../lwip/tools/ping/lwip_ping.h"                               /*  ping 工具                   */
#include "../lwip/tools/ping6/lwip_ping6.h"                             /*  ping6 工具                  */
#include "../lwip/tools/netbios/lwip_netbios.h"                         /*  简易 netbios 名字服务器     */
#include "../lwip/tools/ftp/lwip_ftp.h"                                 /*  ftp 服务                    */
#include "../lwip/tools/tftp/lwip_tftp.h"                               /*  tftp 服务                   */
#include "../lwip/tools/telnet/lwip_telnet.h"                           /*  telnet server 工具          */
#include "../lwip/tools/nat/lwip_nat.h"                                 /*  NAT 网络工具                */
#include "../lwip/tools/vpn/lwip_vpn.h"                                 /*  虚拟以太网 VPN 工具         */
#include "../lwip/tools/npf/lwip_npf.h"                                 /*  网络数据包过滤器            */
#include "../lwip/tools/qos/lwip_qos.h"                                 /*  网络 QoS                    */
#include "../lwip/tools/vlan/lwip_vlan.h"                               /*  VLAN 工具                   */

#endif                                                                  /*  __NET_NET_H                 */
/*********************************************************************************************************
  END
*********************************************************************************************************/
