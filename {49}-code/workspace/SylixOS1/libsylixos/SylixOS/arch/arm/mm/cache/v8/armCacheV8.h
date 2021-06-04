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
** 文   件   名: armCacheV8.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2016 年 05 月 01 日
**
** 描        述: ARMv8 体系构架 CACHE 驱动.
*********************************************************************************************************/

#ifndef __ARMCACHEV8_H
#define __ARMCACHEV8_H

VOID  armCacheV8Init(LW_CACHE_OP *pcacheop,
                     CACHE_MODE   uiInstruction, 
                     CACHE_MODE   uiData, 
                     CPCHAR       pcMachineName);
                      
VOID  armCacheV8Reset(CPCHAR  pcMachineName);

#endif                                                                  /*  __ARMCACHEV8_H              */
/*********************************************************************************************************
  END
*********************************************************************************************************/
