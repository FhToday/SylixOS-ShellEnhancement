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
** 文   件   名: armCacheV7M.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2017 年 11 月 08 日
**
** 描        述: ARMv7 Cortex-Mx 体系构架 CACHE 驱动.
*********************************************************************************************************/

#ifndef __ARMCACHEV7M_H
#define __ARMCACHEV7M_H

VOID  armCacheV7MInit(LW_CACHE_OP *pcacheop,
                      CACHE_MODE   uiInstruction,
                      CACHE_MODE   uiData,
                      CPCHAR       pcMachineName);

VOID  armCacheV7MReset(CPCHAR  pcMachineName);

#endif                                                                  /*  __ARMCACHEV7M_H             */
/*********************************************************************************************************
  END
*********************************************************************************************************/
