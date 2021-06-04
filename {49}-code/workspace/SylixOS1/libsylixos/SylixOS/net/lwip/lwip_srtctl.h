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
** 文   件   名: lwip_srtctl.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2017 年 12 月 08 日
**
** 描        述: ioctl 源路由表支持.
*********************************************************************************************************/

#ifndef __LWIP_SRTCTL_H
#define __LWIP_SRTCTL_H

/*********************************************************************************************************
  裁剪控制
*********************************************************************************************************/
#if LW_CFG_NET_EN > 0 && LW_CFG_NET_ROUTER > 0 && LW_CFG_NET_BALANCING > 0

INT  __srtIoctlInet(INT  iFamily, INT  iCmd, PVOID  pvArg);

#endif                                                                  /*  LW_CFG_NET_EN > 0           */
                                                                        /*  LW_CFG_NET_ROUTER > 0       */
                                                                        /*  LW_CFG_NET_BALANCING > 0    */
#endif                                                                  /*  __LWIP_SRTCTL_H             */
/*********************************************************************************************************
  END
*********************************************************************************************************/
