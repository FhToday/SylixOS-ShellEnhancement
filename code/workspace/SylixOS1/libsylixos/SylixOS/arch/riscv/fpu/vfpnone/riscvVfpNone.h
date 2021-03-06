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
** 文   件   名: riscvVfpNone.h
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2018 年 03 月 20 日
**
** 描        述: RISC-V 体系架构无 VFP 支持.
*********************************************************************************************************/

#ifndef __ARCH_RISCV_VFP_NONE_H
#define __ARCH_RISCV_VFP_NONE_H

PRISCV_FPU_OP  riscvVfpNonePrimaryInit(CPCHAR  pcMachineName, CPCHAR  pcFpuName);
VOID           riscvVfpNoneSecondaryInit(CPCHAR  pcMachineName, CPCHAR  pcFpuName);

#endif                                                                  /*  __ARCH_RISCV_VFP_NONE_H     */
/*********************************************************************************************************
  END
*********************************************************************************************************/
