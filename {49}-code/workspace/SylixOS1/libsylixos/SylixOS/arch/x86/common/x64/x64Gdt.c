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
** 文   件   名: x64Gdt.c
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2018 年 01 月 09 日
**
** 描        述: x64 体系构架 GDT.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
/*********************************************************************************************************
** 函数名称: x86GdtInit
** 功能描述: 初始化 GDT
** 输　入  : NONE
** 输　出  : ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  x86GdtInit (VOID)
{
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: x86TssInit
** 功能描述: 初始化 TSS
** 输　入  : NONE
** 输　出  : ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  x86TssInit (VOID)
{
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: x86GdtSecondaryInit
** 功能描述: Secondary CPU 初始化 GDT
** 输　入  : NONE
** 输　出  : ERROR CODE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  x86GdtSecondaryInit (VOID)
{
    return  (ERROR_NONE);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
