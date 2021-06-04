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
** 文   件   名: lwip_telnet.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2009 年 05 月 06 日
**
** 描        述: lwip telnet server 工具.
*********************************************************************************************************/

#ifndef __LWIP_TELNET_H
#define __LWIP_TELNET_H

/*********************************************************************************************************
  裁剪控制
*********************************************************************************************************/
#if (LW_CFG_NET_EN > 0) && (LW_CFG_NET_TELNET_EN > 0) && (LW_CFG_SHELL_EN > 0)

LW_API VOID  API_INetTelnetInit(const PCHAR  pcPtyStartName);           /*  初始化 telnet 服务器        */
LW_API INT   API_INetTelnetBindDev(UINT  uiIndex);                      /*  设置 telnet 绑定设备        */

#define inetTelnetInit          API_INetTelnetInit
#define inetTelnetBindDev       API_INetTelnetBindDev

#endif                                                                  /*  (LW_CFG_NET_EN > 0)         */
                                                                        /*  (LW_CFG_NET_TELNET_EN > 0)  */
                                                                        /*  (LW_CFG_SHELL_EN > 0)       */
#endif                                                                  /*  __LWIP_TELNET_H             */
/*********************************************************************************************************
  END
*********************************************************************************************************/
