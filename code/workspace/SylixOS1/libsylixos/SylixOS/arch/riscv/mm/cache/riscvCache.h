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
** 文   件   名: riscvCache.h
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2018 年 04 月 12 日
**
** 描        述: RISC-V 体系构架 CACHE 驱动.
*********************************************************************************************************/

#ifndef __ARCH_RISCV_CACHE_H
#define __ARCH_RISCV_CACHE_H

VOID  riscvCacheInit(LW_CACHE_OP  *pcacheop,
                     CACHE_MODE    uiInstruction,
                     CACHE_MODE    uiData,
                     CPCHAR        pcMachineName);
VOID  riscvCacheReset(CPCHAR  pcMachineName);

#endif                                                                  /*  __ARCH_RISCV_CACHE_H        */
/*********************************************************************************************************
  END
*********************************************************************************************************/
