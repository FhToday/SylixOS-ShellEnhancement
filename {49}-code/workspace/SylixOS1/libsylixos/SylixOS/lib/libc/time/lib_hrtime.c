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
** 文   件   名: lib_hrtime.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2014 年 07 月 04 日
**
** 描        述: 系统高精度时间.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
#include "../SylixOS/system/include/s_system.h"
#if LW_CFG_POSIX_EN > 0
#include "sys/times.h"
#endif                                                                      /*  LW_CFG_POSIX_EN > 0     */
/*********************************************************************************************************
** 函数名称: lib_gethrtime
** 功能描述: 获得系统精确时间
** 输　入  : NONE
** 输　出  : 以 ns 为单位的精确时间
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
#if LW_CFG_RTC_EN > 0

hrtime_t  lib_gethrtime (VOID)
{
    struct timespec  tm;
    
    lib_clock_gettime(CLOCK_MONOTONIC, &tm);
    
    return  ((((hrtime_t)tm.tv_sec) * __TIMEVAL_NSEC_MAX) + tm.tv_nsec);
}
/*********************************************************************************************************
** 函数名称: lib_gethrvtime
** 功能描述: 获得用户进程精确时间
** 输　入  : NONE
** 输　出  : 以 ns 为单位的精确时间
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
hrtime_t  lib_gethrvtime (VOID)
{
#if LW_CFG_POSIX_EN > 0
    struct tms tmsBuf;
    ULONG      ulTicks;
    
    times(&tmsBuf);
    
    ulTicks = (ULONG)tmsBuf.tms_utime + tmsBuf.tms_stime;
    
    return  ((hrtime_t)ulTicks * (__TIMEVAL_NSEC_MAX / LW_TICK_HZ));
#else
    return  (0);
#endif                                                                  /*  LW_CFG_POSIX_EN > 0         */
}

#endif                                                                  /*  LW_CFG_RTC_EN > 0           */
/*********************************************************************************************************
  END
*********************************************************************************************************/
