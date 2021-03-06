/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: mipsVfp32.h
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2015 年 11 月 17 日
**
** 描        述: MIPS 体系架构 VFP32 支持.
*********************************************************************************************************/

#ifndef __ARCH_MIPSVFP32_H
#define __ARCH_MIPSVFP32_H

#include "../mipsFpu.h"

PMIPS_FPU_OP  mipsVfp32PrimaryInit(CPCHAR    pcMachineName, CPCHAR  pcFpuName);
VOID          mipsVfp32SecondaryInit(CPCHAR  pcMachineName, CPCHAR  pcFpuName);

UINT32        mipsVfp32GetFCSR(VOID);
VOID          mipsVfp32SetFCSR(UINT32  uiFCSR);

#endif                                                                  /*  __ARCH_MIPSVFP32_H          */
/*********************************************************************************************************
  END
*********************************************************************************************************/
