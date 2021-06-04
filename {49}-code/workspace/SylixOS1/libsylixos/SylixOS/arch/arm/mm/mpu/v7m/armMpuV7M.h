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
** 文   件   名: armMpuV7M.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2017 年 11 月 08 日
**
** 描        述: ARM 体系构架 MPU 驱动.
*********************************************************************************************************/

#ifndef __ARMMPUV7M_H
#define __ARMMPUV7M_H

VOID  armMpuV7MInit(CPCHAR  pcMachineName, const ARM_MPU_REGION  mpuregion[]);
PVOID armMpuV7MDmaAlloc(size_t  stSize);
PVOID armMpuV7MDmaAllocAlign(size_t  stSize, size_t  stAlign);
VOID  armMpuV7MDmaFree(PVOID  pvDmaMem);

#endif                                                                  /*  __ARMMPUV7M_H               */
/*********************************************************************************************************
  END
*********************************************************************************************************/
