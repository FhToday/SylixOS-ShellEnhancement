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
** 文   件   名: sparcAssert.c
**
** 创   建   人: Xu.Guizhou (徐贵洲)
**
** 文件创建日期: 2017 年 05 月 15 日
**
** 描        述: SPARC 体系构架断言.
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_PANIC
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
/*********************************************************************************************************
** 函数名称: archAssert
** 功能描述: 系统断言.
** 输　入  : pcCond         条件表达式
**           pcFunc         调用函数
**           pcFile         所在文件
**           iLine          文件行号
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID  archAssert (CPCHAR  pcCond, CPCHAR  pcFunc, CPCHAR  pcFile, INT  iLine)
{
#if LW_CFG_DEVICE_EN > 0
    fprintf(stderr, "condition: %s function: %s() in file: %s line: %d assert error!\n",
            pcCond, pcFunc, pcFile, iLine);
#endif

    panic("condition: %s function: %s() in file: %s line: %d assert error!\n",
          pcCond, pcFunc, pcFile, iLine);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
