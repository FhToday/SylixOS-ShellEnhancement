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
** 文   件   名: mipsFpuEmu.c
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2017 年 12 月 1 日
**
** 描        述: MIPS FPU 模拟源文件.
*********************************************************************************************************/
#define __SYLIXOS_KERNEL
#include "SylixOS.h"
#include <linux/compat.h>
#include "mipsFpuEmu.h"
/*********************************************************************************************************
  全局变量定义
*********************************************************************************************************/
struct mips_fpu_emulator_stats  _G_mipsFpuEmuStats;                         /*  FPU 模拟统计信息        */
/*********************************************************************************************************
  END
*********************************************************************************************************/
