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
** 文   件   名: bspSmp.c
**
** 创   建   人: Han.hui (韩辉)
**
** 文件创建日期: 2018 年 9 月 20 日
**
** 描        述: 处理器需要为 SylixOS 提供的功能支持.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
/*********************************************************************************************************
** 函数名称: bspCpuLogic2Physical
** 功能描述: 获得逻辑 CPU 的物理 ID
** 输  入  : ulCPUId      目标 CPU
** 输  出  : 物理 ID
** 全局变量:
** 调用模块:
*********************************************************************************************************/
#if LW_CFG_CPU_ARCH_SMT > 0

LW_WEAK ULONG  bspCpuLogic2Physical (ULONG  ulCPUId)
{
    return  (ulCPUId);
}

#endif                                                                  /*  LW_CFG_CPU_ARCH_SMT > 0     */
/*********************************************************************************************************
  END
*********************************************************************************************************/
