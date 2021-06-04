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
** 文   件   名: lib_isinf.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2009 年 03 月 02 日
**
** 描        述: 浮点相关运算
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
#include "../SylixOS/system/include/s_system.h"
/*********************************************************************************************************
** 函数名称: lib_isnan
** 功能描述: 
** 输　入  : 
** 输　出  : 
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
#if LW_KERN_FLOATING > 0

int  lib_isnan (double  dX)
{
    return  (__ARCH_DOUBLE_ISNAN(dX));
}
/*********************************************************************************************************
** 函数名称: lib_isinf
** 功能描述: 
** 输　入  : 
** 输　出  : 
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
int  lib_isinf (double  dX)
{
    return  (__ARCH_DOUBLE_ISINF(dX));
}

#endif                                                                  /*  LW_KERN_FLOATING            */
/*********************************************************************************************************
  END
*********************************************************************************************************/
