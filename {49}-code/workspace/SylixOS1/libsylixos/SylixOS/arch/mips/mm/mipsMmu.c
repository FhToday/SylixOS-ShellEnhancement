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
** 文   件   名: mipsMmu.c
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2015 年 09 月 01 日
**
** 描        述: MIPS 体系架构 MMU 驱动.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
/*********************************************************************************************************
  裁剪支持
*********************************************************************************************************/
#if LW_CFG_VMM_EN > 0
#include "mmu/mipsMmuCommon.h"
#include "arch/mips/common/mipsCpuProbe.h"
/*********************************************************************************************************
** 函数名称: archMmuInit
** 功能描述: 初始化 MMU
** 输　入  : pcMachineName  机器名称
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  archMmuInit (CPCHAR  pcMachineName)
{
    LW_MMU_OP  *pmmuop = API_VmmGetLibBlock();

    _DebugFormat(__LOGMESSAGE_LEVEL, "%s %s MMU initialization.\r\n", 
                 LW_CFG_CPU_ARCH_FAMILY, pcMachineName);

    mipsCpuProbe(pcMachineName);                                        /*  MIPS CPU 探测               */
    mipsMmuInit(pmmuop, pcMachineName);                                 /*  MIPS MMU 初始化             */
}

#endif                                                                  /*  LW_CFG_VMM_EN > 0           */
/*********************************************************************************************************
  END
*********************************************************************************************************/
