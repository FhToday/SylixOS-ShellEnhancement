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
** 文   件   名: _CpuLib.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2014 年 01 月 10 日
**
** 描        述: CPU 操作库.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
/*********************************************************************************************************
** 函数名称: _CpuActive
** 功能描述: 将 CPU 设置为激活状态 (进入内核且关闭中断状态下被调用)
** 输　入  : pcpu      CPU 结构
** 输　出  : ERROR or OK
** 全局变量: 
** 调用模块: 
** 注  意  : 必须保证 pcpu 当前执行线程有一个有效的 TCB 例如 _K_tcbDummyKernel 或者其他.
*********************************************************************************************************/
INT  _CpuActive (PLW_CLASS_CPU   pcpu)
{
    if (LW_CPU_IS_ACTIVE(pcpu)) {
        return  (PX_ERROR);
    }
    
    pcpu->CPU_ulStatus |= LW_CPU_STATUS_ACTIVE;
    
#if (LW_CFG_SMP_EN > 0) && (LW_CFG_CPU_ARCH_SMT > 0)
    LW_PHYCPU_GET(pcpu->CPU_ulPhyId)->PHYCPU_uiLogic++;
#endif
    KN_SMP_MB();
    
    _CandTableUpdate(pcpu);                                             /*  更新候选线程                */

    pcpu->CPU_ptcbTCBCur  = LW_CAND_TCB(pcpu);
    pcpu->CPU_ptcbTCBHigh = LW_CAND_TCB(pcpu);
    
#if (LW_CFG_SMP_EN > 0) && (LW_CFG_CPU_ARCH_SMT > 0)
    if (pcpu->CPU_ptcbTCBCur->TCB_usIndex >= LW_NCPUS) {
        LW_PHYCPU_GET(pcpu->CPU_ulPhyId)->PHYCPU_uiNonIdle++;           /*  运行的非 Idle 任务          */
    }
#endif
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: _CpuInactive
** 功能描述: 将 CPU 设置为非激活状态 (进入内核且关闭中断状态下被调用)
** 输　入  : pcpu      CPU 结构
** 输　出  : ERROR or OK
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
#if LW_CFG_SMP_CPU_DOWN_EN > 0

INT  _CpuInactive (PLW_CLASS_CPU   pcpu)
{
    INT             i;
    ULONG           ulCPUId;
    PLW_CLASS_CPU   pcpuOther;
    PLW_CLASS_TCB   ptcbOther;
    PLW_CLASS_TCB   ptcb;
    BOOL            bRotIdle;

    if (!LW_CPU_IS_ACTIVE(pcpu)) {
        return  (PX_ERROR);
    }
    
    ulCPUId = LW_CPU_GET_ID(pcpu);
    ptcb    = LW_CAND_TCB(pcpu);
    
    pcpu->CPU_ulStatus &= ~LW_CPU_STATUS_ACTIVE;
    
#if LW_CFG_CPU_ARCH_SMT > 0
    LW_PHYCPU_GET(pcpu->CPU_ulPhyId)->PHYCPU_uiLogic--;
#endif
    KN_SMP_MB();
    
#if LW_CFG_CPU_ARCH_SMT > 0
    if (pcpu->CPU_ptcbTCBCur->TCB_usIndex >= LW_NCPUS) {
        LW_PHYCPU_GET(pcpu->CPU_ulPhyId)->PHYCPU_uiNonIdle--;           /*  运行的非 Idle 任务          */
    }
#endif
    
    _CandTableRemove(pcpu);                                             /*  移除候选执行线程            */
    LW_CAND_ROT(pcpu) = LW_FALSE;                                       /*  清除优先级卷绕标志          */

    pcpu->CPU_ptcbTCBCur  = LW_NULL;
    pcpu->CPU_ptcbTCBHigh = LW_NULL;
    
    if (ptcb->TCB_bCPULock) {                                           /*  此任务设置了亲和度          */
        return  (ERROR_NONE);
    }
    
    bRotIdle = LW_FALSE;
    
    LW_CPU_FOREACH_ACTIVE_EXCEPT (i, ulCPUId) {                         /*  CPU 必须是激活状态          */
        pcpuOther = LW_CPU_GET(i);
        ptcbOther = LW_CAND_TCB(pcpuOther);
        
        if (LW_CPU_ONLY_AFFINITY_GET(pcpuOther)) {                      /*  仅运行亲和度任务            */
            continue;
        }

        if (!LW_CAND_ROT(pcpuOther) && 
            (LW_PRIO_IS_EQU(LW_PRIO_LOWEST, 
                            ptcbOther->TCB_ucPriority))) {              /*  运行 idle 任务且无标注      */
            LW_CAND_ROT(pcpuOther) = LW_TRUE;
            bRotIdle               = LW_TRUE;
            break;                                                      /*  只标注一个 CPU 即可         */
        }
    }
    
    if (!bRotIdle) {
        LW_CPU_FOREACH_ACTIVE_EXCEPT (i, ulCPUId) {                     /*  CPU 必须是激活状态          */
            pcpuOther = LW_CPU_GET(i);
            ptcbOther = LW_CAND_TCB(pcpuOther);
            
            if (LW_CPU_ONLY_AFFINITY_GET(pcpuOther)) {                  /*  仅运行亲和度任务            */
                continue;
            }

            if (LW_PRIO_IS_HIGH(ptcb->TCB_ucPriority,
                                ptcbOther->TCB_ucPriority)) {
                LW_CAND_ROT(pcpuOther) = LW_TRUE;
            }
        }
    }
    
    _CpuBogoMipsClear(ulCPUId);                                         /*  清除之前计算的 BogoMIPS     */
    
    return  (ERROR_NONE);
}

#endif                                                                  /*  LW_CFG_SMP_CPU_DOWN_EN > 0  */
/*********************************************************************************************************
** 函数名称: _CpuGetCur
** 功能描述: 获取当前 CPU 控制块 (驱动程序在中断状态下被调用)
** 输　入  : NONE
** 输　出  : 当前 CPU 控制块
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
#if defined(__SYLIXOS_ARM_ARCH_M__)

PLW_CLASS_CPU  _CpuGetCur (VOID)
{
    return  (LW_CPU_GET_CUR());
}

#endif                                                                  /*  __SYLIXOS_ARM_ARCH_M__      */
/*********************************************************************************************************
** 函数名称: _CpuGetNesting
** 功能描述: 获取 CPU 当前中断嵌套值
** 输　入  : NONE
** 输　出  : 中断嵌套值
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
ULONG  _CpuGetNesting (VOID)
{
#if LW_CFG_SMP_EN > 0
    INTREG          iregInterLevel;
    ULONG           ulNesting;
    
    iregInterLevel = KN_INT_DISABLE();                                  /*  关闭中断                    */
    ulNesting      = LW_CPU_GET_CUR()->CPU_ulInterNesting;
    KN_INT_ENABLE(iregInterLevel);                                      /*  打开中断                    */
    
    return  (ulNesting);
#else
    return  (LW_CPU_GET_CUR()->CPU_ulInterNesting);
#endif                                                                  /*  LW_CFG_SMP_EN > 0           */
}
/*********************************************************************************************************
** 函数名称: _CpuGetMaxNesting
** 功能描述: 获取 CPU 最大中断嵌套值
** 输　入  : NONE
** 输　出  : 中断嵌套值
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
ULONG  _CpuGetMaxNesting (VOID)
{
#if LW_CFG_SMP_EN > 0
    INTREG          iregInterLevel;
    ULONG           ulNesting;
    
    iregInterLevel = KN_INT_DISABLE();                                  /*  关闭中断                    */
    ulNesting      = LW_CPU_GET_CUR()->CPU_ulInterNestingMax;
    KN_INT_ENABLE(iregInterLevel);                                      /*  打开中断                    */
    
    return  (ulNesting);
#else
    return  (LW_CPU_GET_CUR()->CPU_ulInterNestingMax);
#endif                                                                  /*  LW_CFG_SMP_EN > 0           */
}
/*********************************************************************************************************
** 函数名称: _CpuSetSchedAffinity
** 功能描述: 设置指定 CPU 为强亲和度调度模式. (非 0 号 CPU, 进入内核并关中断被调用)
** 输　入  : ulCPUId       CPU ID 
**           bEnable       是否使能为强亲和度模式
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
#if LW_CFG_SMP_EN > 0

VOID  _CpuSetSchedAffinity (ULONG  ulCPUId, BOOL  bEnable)
{
    PLW_CLASS_CPU   pcpu;
    
    pcpu = LW_CPU_GET(ulCPUId);
    if (LW_CPU_ONLY_AFFINITY_GET(pcpu) == bEnable) {
        return;                                                         /*  不需要进行任何处理          */
    }
    
    LW_CPU_ONLY_AFFINITY_SET(pcpu, bEnable);
    LW_CAND_ROT(pcpu) = LW_TRUE;                                        /*  需要尝试调度                */
    
    if ((LW_CPU_GET_CUR_ID() != ulCPUId) &&
        (LW_CPU_GET_IPI_PEND(ulCPUId) & LW_IPI_SCHED_MSK) == 0) {
        _SmpSendIpi(ulCPUId, LW_IPI_SCHED, 0, LW_TRUE);                 /*  产生核间中断                */
    }
}
/*********************************************************************************************************
** 函数名称: _CpuGetSchedAffinity
** 功能描述: 获取指定 CPU 是否为强亲和度调度模式. (进入内核并关中断被调用)
** 输　入  : ulCPUId       CPU ID 
**           pbEnable      是否使能为强亲和度模式
** 输　出  : ERROR
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID  _CpuGetSchedAffinity (ULONG  ulCPUId, BOOL  *pbEnable)
{
    PLW_CLASS_CPU   pcpu;
    
    pcpu = LW_CPU_GET(ulCPUId);
    *pbEnable = LW_CPU_ONLY_AFFINITY_GET(pcpu);
}

#endif                                                                  /*  LW_CFG_SMP_EN > 0           */
/*********************************************************************************************************
  END
*********************************************************************************************************/
