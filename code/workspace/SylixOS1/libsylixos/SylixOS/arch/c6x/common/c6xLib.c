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
** 文   件   名: c6xLib.c
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2017 年 03 月 17 日
**
** 描        述: c6x 体系构架内部库.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
#include "stdlib.h"
/*********************************************************************************************************
** 函数名称: archPageCopy
** 功能描述: 拷贝一个页面
** 输　入  : pvTo      目标
**           pvFrom    源
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  archPageCopy (PVOID  pvTo, PVOID  pvFrom)
{
#if defined(_TMS320C6400_PLUS)
    archC64PlusMemcpy(pvTo, pvFrom, LW_CFG_VMM_PAGE_SIZE);
#else
    REGISTER INT      i;
    REGISTER UINT64  *pu64To   = (UINT64 *)pvTo;
    REGISTER UINT64  *pu64From = (UINT64 *)pvFrom;

    for (i = 0; i < (LW_CFG_VMM_PAGE_SIZE >> 3); i += 16) {
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
        *pu64To++ = *pu64From++;
    }
#endif                                                                  /*  !defined(_TMS320C6400_PLUS) */
}
/*********************************************************************************************************
** 函数名称: archReboot
** 功能描述: 系统重新启动
** 输　入  : iRebootType       重启类型
**           ulStartAddress    启动开始地址
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  archReboot (INT  iRebootType, addr_t  ulStartAddress)
{
    bspReboot(iRebootType, ulStartAddress);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
