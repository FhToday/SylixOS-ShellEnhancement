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
** 文   件   名: sparcVfp.h
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2017 年 09 月 29 日
**
** 描        述: SPARC 体系架构 FPU 支持.
*********************************************************************************************************/

#ifndef __ARCH_SPARC_VFP_H
#define __ARCH_SPARC_VFP_H

PSPARC_FPU_OP   sparcVfpPrimaryInit(CPCHAR  pcMachineName, CPCHAR  pcFpuName);
VOID            sparcVfpSecondaryInit(CPCHAR  pcMachineName, CPCHAR  pcFpuName);

#endif                                                                  /*  __ARCH_SPARC_VFP_H          */
/*********************************************************************************************************
  END
*********************************************************************************************************/
