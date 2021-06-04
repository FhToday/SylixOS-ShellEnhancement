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
** 文   件   名: ppcExc.h
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2015 年 12 月 15 日
**
** 描        述: PowerPC 体系构架异常处理.
*********************************************************************************************************/

#ifndef __ARCH_PPCEXC_H
#define __ARCH_PPCEXC_H

VOID  archDecrementerInit(ULONG    ulVector,
                          BOOL     bPreemptive,
                          UINT32   uiDecValue);

#endif                                                                  /*  __ARCH_PPCEXC_H             */
/*********************************************************************************************************
  END
*********************************************************************************************************/
