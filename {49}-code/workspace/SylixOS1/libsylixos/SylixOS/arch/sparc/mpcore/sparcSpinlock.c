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
** 文   件   名: sparcSpinlock.c
**
** 创   建   人: Xu.Guizhou (徐贵洲)
**
** 文件创建日期: 2017 年 05 月 15 日
**
** 描        述: SPARC 体系构架自旋锁驱动.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
/*********************************************************************************************************
  spinlock 状态
*********************************************************************************************************/
#if LW_CFG_SMP_EN > 0
/*********************************************************************************************************
  spinlock bug trace
*********************************************************************************************************/
#ifdef __LW_SPINLOCK_BUG_TRACE_EN
#define __LW_SPINLOCK_RECURSIVE_TRACE() \
        _BugFormat((psl->SL_ulCounter > 10), LW_TRUE, \
                   "spinlock RECURSIVE %lu!\r\n", psl->SL_ulCounter)
#else
#define __LW_SPINLOCK_RECURSIVE_TRACE()
#endif
/*********************************************************************************************************
** 函数名称: sparcSpinLock
** 功能描述: sparc spin lock
** 输　入  : psld       spinlock data 指针
**           pfuncPoll  循环等待时调用函数
**           pvArg      回调参数
** 输　出  : NONE
** 全局变量:
** 调用模块:
** 注  意  : 自旋结束时, 操作系统会调用内存屏障, 所以这里不需要调用.
*********************************************************************************************************/
static LW_INLINE VOID  sparcSpinLock (SPINLOCKTYPE *psld, VOIDFUNCPTR  pfuncPoll, PVOID  pvArg)
{
#if LW_CFG_CPU_ATOMIC_EN > 0
    UINT32          uiNewVal;
    SPINLOCKTYPE    sldVal;

    for (;;) {
        sldVal.SLD_uiLock = LW_ACCESS_ONCE(UINT32, psld->SLD_uiLock);
        uiNewVal          = sldVal.SLD_uiLock + (1 << LW_SPINLOCK_TICKET_SHIFT);
        __asm__ __volatile__ ("casa     [%2] 0xb, %3, %0"
                             : "=&r" (uiNewVal)
                             : "0" (uiNewVal), "r" (&psld->SLD_uiLock), "r" (sldVal)
                             : "memory");
        if (uiNewVal == sldVal.SLD_uiLock) {
            break;
        }
    }

    while (sldVal.SLD_usTicket != sldVal.SLD_usSvcNow) {
        if (pfuncPoll) {
            pfuncPoll(pvArg);
        }
        sldVal.SLD_usSvcNow = LW_ACCESS_ONCE(UINT16, psld->SLD_usSvcNow);
    }

#else
    UINT32  uiRes;

    for (;;) {
        __asm__ __volatile__("ldstub    [%1], %0"
                             : "=r" (uiRes)
                             : "r" (psld)
                             : "memory");
        if (uiRes == 0) {
            break;
        }
        if (pfuncPoll) {
            pfuncPoll(pvArg);
        }
    }
#endif                                                                  /*  LW_CFG_CPU_ATOMIC_EN > 0    */
}
/*********************************************************************************************************
** 函数名称: sparcSpinTryLock
** 功能描述: sparc spin trylock
** 输　入  : psld       spinlock data 指针
** 输　出  : 1: busy 0: ok
** 全局变量:
** 调用模块:
** 注  意  : 自旋结束时, 操作系统会调用内存屏障, 所以这里不需要调用.
*********************************************************************************************************/
static LW_INLINE UINT32  sparcSpinTryLock (SPINLOCKTYPE *psld)
{
#if LW_CFG_CPU_ATOMIC_EN > 0
    UINT32          uiNewVal;
    SPINLOCKTYPE    sldVal;

    if (psld->SLD_usTicket == psld->SLD_usSvcNow) {
        sldVal.SLD_uiLock = LW_ACCESS_ONCE(UINT32, psld->SLD_uiLock);
        uiNewVal          = sldVal.SLD_uiLock + (1 << LW_SPINLOCK_TICKET_SHIFT);
        __asm__ __volatile__ ("casa     [%2] 0xb, %3, %0"
                             : "=&r" (uiNewVal)
                             : "0" (uiNewVal), "r" (&psld->SLD_uiLock), "r" (sldVal)
                             : "memory");
        return  ((uiNewVal == sldVal.SLD_uiLock) ? 0 : 1);

    } else {
        return  (1);
    }

#else
    UINT32  uiRes;

    __asm__ __volatile__("ldstub    [%1], %0"
                         : "=r" (uiRes)
                         : "r" (psld)
                         : "memory");

    return  (uiRes);
#endif                                                                  /*  LW_CFG_CPU_ATOMIC_EN > 0    */
}
/*********************************************************************************************************
** 函数名称: sparcSpinUnlock
** 功能描述: sparc spin unlock
** 输　入  : psld       spinlock data 指针
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static LW_INLINE VOID  sparcSpinUnlock (SPINLOCKTYPE *psld)
{
#if LW_CFG_CPU_ATOMIC_EN > 0
    psld->SLD_usSvcNow++;
#else
    __asm__ __volatile__("stb   %%g0, [%0]" : : "r" (psld) : "memory");
#endif                                                                  /*  LW_CFG_CPU_ATOMIC_EN > 0    */
}
/*********************************************************************************************************
** 函数名称: archSpinInit
** 功能描述: 初始化一个 spinlock
** 输　入  : psl        spinlock 指针
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID  archSpinInit (spinlock_t  *psl)
{
    psl->SL_sltData.SLD_uiLock = 0;                                     /*  0: 未锁定状态  1: 锁定状态  */
    psl->SL_pcpuOwner          = LW_NULL;
    psl->SL_ulCounter          = 0;
    psl->SL_pvReserved         = LW_NULL;
    KN_SMP_WMB();
}
/*********************************************************************************************************
** 函数名称: archSpinDelay
** 功能描述: 等待事件
** 输　入  : NONE
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  archSpinDelay (VOID)
{
    volatile INT  i;

    for (i = 0; i < 3; i++) {
    }
}
/*********************************************************************************************************
** 函数名称: archSpinNotify
** 功能描述: 发送 spin 事件
** 输　入  : NONE
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID  archSpinNotify (VOID)
{
}
/*********************************************************************************************************
** 函数名称: archSpinLock
** 功能描述: spinlock 上锁
** 输　入  : psl        spinlock 指针
**           pcpuCur    当前 CPU
**           pfuncPoll  循环等待时调用函数
**           pvArg      回调参数
** 输　出  : 0: 没有获取
**           1: 正常加锁
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
INT  archSpinLock (spinlock_t  *psl, PLW_CLASS_CPU  pcpuCur, VOIDFUNCPTR  pfuncPoll, PVOID  pvArg)
{
    if (psl->SL_pcpuOwner == pcpuCur) {
        psl->SL_ulCounter++;
        __LW_SPINLOCK_RECURSIVE_TRACE();
        return  (1);                                                    /*  重复调用                    */
    }

    sparcSpinLock(&psl->SL_sltData, pfuncPoll, pvArg);

    psl->SL_pcpuOwner = pcpuCur;                                        /*  保存当前 CPU                */
    
    return  (1);                                                        /*  加锁成功                    */
}
/*********************************************************************************************************
** 函数名称: archSpinTryLock
** 功能描述: spinlock 试图上锁
** 输　入  : psl        spinlock 指针
**           pcpuCur    当前 CPU
** 输　出  : 0: 没有获取
**           1: 正常加锁
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
INT  archSpinTryLock (spinlock_t  *psl, PLW_CLASS_CPU  pcpuCur)
{
    if (psl->SL_pcpuOwner == pcpuCur) {
        psl->SL_ulCounter++;
        __LW_SPINLOCK_RECURSIVE_TRACE();
        return  (1);                                                    /*  重复调用                    */
    }

    if (sparcSpinTryLock(&psl->SL_sltData)) {
        return  (0);
    }

    psl->SL_pcpuOwner = pcpuCur;                                        /*  保存当前 CPU                */

    return  (1);                                                        /*  加锁成功                    */
}
/*********************************************************************************************************
** 函数名称: archSpinUnlock
** 功能描述: spinlock 解锁
** 输　入  : psl        spinlock 指针
**           pcpuCur    当前 CPU
** 输　出  : 0: 没有获取
**           1: 正常解锁
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
INT  archSpinUnlock (spinlock_t  *psl, PLW_CLASS_CPU  pcpuCur)
{
    if (psl->SL_pcpuOwner != pcpuCur) {
        return  (0);                                                    /*  没有权利释放                */
    }

    if (psl->SL_ulCounter) {
        psl->SL_ulCounter--;                                            /*  减少重复调用次数            */
        return  (1);
    }

    psl->SL_pcpuOwner = LW_NULL;                                        /*  没有 CPU 获取               */
    KN_SMP_WMB();

    sparcSpinUnlock(&psl->SL_sltData);                                  /*  汇编实现中带内存屏障        */

    return  (1);                                                        /*  解锁成功                    */
}
/*********************************************************************************************************
** 函数名称: archSpinLockRaw
** 功能描述: spinlock 上锁 (不进行重入判断)
** 输　入  : psl        spinlock 指针
** 输　出  : 0: 没有获取
**           1: 正常加锁
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
INT  archSpinLockRaw (spinlock_t  *psl)
{
    sparcSpinLock(&psl->SL_sltData, LW_NULL, LW_NULL);

    return  (1);                                                        /*  加锁成功                    */
}
/*********************************************************************************************************
** 函数名称: archSpinTryLockRaw
** 功能描述: spinlock 试图上锁 (不进行重入判断)
** 输　入  : psl        spinlock 指针
** 输　出  : 0: 没有获取
**           1: 正常加锁
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
INT  archSpinTryLockRaw (spinlock_t  *psl)
{
    if (sparcSpinTryLock(&psl->SL_sltData)) {
        return  (0);
    }

    return  (1);                                                        /*  加锁成功                    */
}
/*********************************************************************************************************
** 函数名称: archSpinUnlockRaw
** 功能描述: spinlock 解锁
** 输　入  : psl        spinlock 指针
** 输　出  : 0: 没有获取
**           1: 正常解锁
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
INT  archSpinUnlockRaw (spinlock_t  *psl)
{
    sparcSpinUnlock(&psl->SL_sltData);

    return  (1);                                                        /*  解锁成功                    */
}

#endif                                                                  /*  LW_CFG_SMP_EN               */
/*********************************************************************************************************
  END
*********************************************************************************************************/
