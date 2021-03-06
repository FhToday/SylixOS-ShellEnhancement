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
** 文   件   名: loader_vpthread.c
**
** 创   建   人: Han.hui (韩辉)
**
** 文件创建日期: 2016 年 11 月 23 日
**
** 描        述: 模块对 VPROCESS 进程内线程管理.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
/*********************************************************************************************************
  裁剪支持
*********************************************************************************************************/
#if LW_CFG_MODULELOADER_EN > 0
#include "../include/loader_lib.h"
/*********************************************************************************************************
** 函数名称: vprocThreadAdd
** 功能描述: 将一个线程加入进程
** 输　入  : pvVProc    进程控制块指针
**           ptcb       线程控制块
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
VOID  vprocThreadAdd (PVOID   pvVProc, PLW_CLASS_TCB  ptcb)
{
    LW_LD_VPROC  *pvproc = (LW_LD_VPROC *)pvVProc;
    
    if (pvproc) {
        __KERNEL_ENTER();                                               /*  进入内核                    */
        if (_LIST_LINE_IS_NOTLNK(&ptcb->TCB_lineProcess)) {
            _List_Line_Add_Tail(&ptcb->TCB_lineProcess, &pvproc->VP_plineThread);
        }
        __KERNEL_EXIT();                                                /*  退出内核                    */
    }
}
/*********************************************************************************************************
** 函数名称: vprocThreadDelete
** 功能描述: 将一个线程从进程表中删除
** 输　入  : pvVProc    进程控制块指针
**           ptcb       线程控制块
** 输　出  : NONE
** 全局变量:
** 调用模块:
** 注  意  : 由于此函数会删除链表结构, 所以必须使用 VP_LOCK 保证安全.
*********************************************************************************************************/
VOID  vprocThreadDelete (PVOID   pvVProc, PLW_CLASS_TCB  ptcb)
{
    LW_LD_VPROC  *pvproc = (LW_LD_VPROC *)pvVProc;

    if (pvproc) {
        LW_VP_LOCK(pvproc);                                             /*  锁定当前进程                */
        __KERNEL_ENTER();                                               /*  进入内核                    */
        if (!_LIST_LINE_IS_NOTLNK(&ptcb->TCB_lineProcess)) {
            _List_Line_Del(&ptcb->TCB_lineProcess, &pvproc->VP_plineThread);
        }
        __KERNEL_EXIT();                                                /*  退出内核                    */
        LW_VP_UNLOCK(pvproc);                                           /*  解锁当前进程                */
    }
}
/*********************************************************************************************************
** 函数名称: vprocThreadNum
** 功能描述: 统计进程内线程个数
** 输　入  : pid        进程 ID
**           pulNum     统计个数
** 输　出  : ERROR or OK
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  vprocThreadNum (pid_t  pid, ULONG  *pulNum)
{
    LW_LD_VPROC     *pvproc;
    PLW_LIST_LINE    plineTemp;
    PLW_CLASS_TCB    ptcb;
    
    if (!pulNum) {
        _ErrorHandle(EINVAL);
        return  (PX_ERROR);
    }
    
    pvproc = vprocGet(pid);
    if (!pvproc) {
        return  (PX_ERROR);
    }
    
    __KERNEL_ENTER();                                                   /*  进入内核                    */
    for (plineTemp  = pvproc->VP_plineThread;
         plineTemp != LW_NULL;
         plineTemp  = _list_line_get_next(plineTemp)) {

        ptcb = _LIST_ENTRY(plineTemp, LW_CLASS_TCB, TCB_lineProcess);
        if (ptcb->TCB_iDeleteProcStatus) {
            continue;                                                   /*  已经在删除过程中            */
        }
        (*pulNum)++;
    }
    __KERNEL_EXIT();                                                    /*  退出内核                    */
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: vprocThreadKill
** 功能描述: 杀死进程内的除主线程外的所有线程
** 输　入  : pvVProc        进程控制块指针
**           ptcbExcp       除此之外
** 输　出  : NONE
** 全局变量:
** 调用模块:
** 注  意  : 由于此函数中间将释放系统控制权, 所以必须使用 VP_LOCK 保证安全.
*********************************************************************************************************/
VOID  vprocThreadKill (PVOID  pvVProc, PLW_CLASS_TCB  ptcbExcp)
{
    LW_LD_VPROC      *pvproc = (LW_LD_VPROC *)pvVProc;
    PLW_LIST_LINE     plineTemp;
    PLW_CLASS_TCB     ptcb;
    
    LW_VP_LOCK(pvproc);                                                 /*  锁定当前进程                */
    for (plineTemp  = pvproc->VP_plineThread;
         plineTemp != LW_NULL;
         plineTemp  = _list_line_get_next(plineTemp)) {
        
        ptcb = _LIST_ENTRY(plineTemp, LW_CLASS_TCB, TCB_lineProcess);
        if (ptcb->TCB_iDeleteProcStatus) {
            continue;                                                   /*  已经在删除过程中            */
        }
        if (ptcb != ptcbExcp) {                                         /*  ptcbCur 为主线程            */
            _excJobAdd((VOIDFUNCPTR)kill, (PVOID)ptcb->TCB_ulId, (PVOID)SIGKILL, 0, 0, 0, 0);
        }
    }
    LW_VP_UNLOCK(pvproc);                                               /*  解锁当前进程                */
}
/*********************************************************************************************************
** 函数名称: vprocThreadTraversal
** 功能描述: 遍历进程内的所有线程 (为了提高效率此函数会锁定 KERNEL)
** 输　入  : pid            进程 id
**           pfunc          回调函数
**           pvArg[0 ~ 5]   遍历函数参数
** 输　出  : ERROR or OK
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  vprocThreadTraversal (pid_t          pid,
                           VOIDFUNCPTR    pfunc,
                           PVOID          pvArg0,
                           PVOID          pvArg1,
                           PVOID          pvArg2,
                           PVOID          pvArg3,
                           PVOID          pvArg4,
                           PVOID          pvArg5)
{
    LW_LD_VPROC      *pvproc;
    PLW_LIST_LINE     plineTemp;
    PLW_CLASS_TCB     ptcb;

    __KERNEL_ENTER();                                                   /*  进入内核                    */
    pvproc = vprocGet(pid);
    if (!pvproc) {
        __KERNEL_EXIT();                                                /*  退出内核                    */
        return  (PX_ERROR);
    }

    for (plineTemp  = pvproc->VP_plineThread;
         plineTemp != LW_NULL;
         plineTemp  = _list_line_get_next(plineTemp)) {

        ptcb = _LIST_ENTRY(plineTemp, LW_CLASS_TCB, TCB_lineProcess);
        if (ptcb->TCB_iDeleteProcStatus) {
            continue;                                                   /*  已经在删除过程中            */
        }
        pfunc(ptcb, pvArg0, pvArg1, pvArg2, pvArg3, pvArg4, pvArg5);
    }
    __KERNEL_EXIT();                                                    /*  退出内核                    */

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** 函数名称: vprocThreadSigaction
** 功能描述: 设置进程内所有任务的 sigaction
** 输　入  : pvVProc        进程控制块指针
**           pfunc          设置函数
**           iSigIndex      信号下标
**           psigactionNew  信号句柄
** 输　出  : ERROR or OK
** 全局变量:
** 调用模块:
*********************************************************************************************************/
#if LW_CFG_SIGNAL_EN > 0

INT  vprocThreadSigaction (PVOID  pvVProc, VOIDFUNCPTR  pfunc, INT  iSigIndex, 
                           const struct sigaction  *psigactionNew)
{
    LW_LD_VPROC    *pvproc = (LW_LD_VPROC *)pvVProc;
    PLW_LIST_LINE   plineTemp;
    PLW_CLASS_TCB   ptcb;
    PLW_CLASS_TCB   ptcbCur;
    
    if (!pvproc) {
        _ErrorHandle(ESRCH);
        return  (PX_ERROR);
    }
    
    LW_TCB_GET_CUR_SAFE(ptcbCur);
    
    __KERNEL_ENTER();                                                   /*  进入内核                    */
    for (plineTemp  = pvproc->VP_plineThread;
         plineTemp != LW_NULL;
         plineTemp  = _list_line_get_next(plineTemp)) {
         
        ptcb = _LIST_ENTRY(plineTemp, LW_CLASS_TCB, TCB_lineProcess);
        if (ptcb->TCB_iDeleteProcStatus) {
            continue;                                                   /*  已经在删除过程中            */
        }
        if (ptcb != ptcbCur) {
            pfunc(ptcb, iSigIndex, psigactionNew);
        }
    }
    __KERNEL_EXIT();                                                    /*  退出内核                    */

    return  (ERROR_NONE);
}

#endif                                                                  /*  LW_CFG_SIGNAL_EN > 0        */
/*********************************************************************************************************
** 函数名称: vprocThreadAffinity
** 功能描述: 设置进程内所有线程调度的 CPU 集合
** 输　入  : pvVProc       进程控制块指针
**           stSize        CPU 掩码集内存大小
**           pcpuset       CPU 掩码
** 输　出  : ERROR or OK
** 全局变量:
** 调用模块:
*********************************************************************************************************/
#if LW_CFG_SMP_EN > 0

INT  vprocThreadAffinity (PVOID  pvVProc, size_t  stSize, const PLW_CLASS_CPUSET  pcpuset)
{
    LW_LD_VPROC    *pvproc = (LW_LD_VPROC *)pvVProc;
    PLW_LIST_LINE   plineTemp;
    PLW_CLASS_TCB   ptcb, ptcbCur;

    if (!pvproc) {
        _ErrorHandle(ESRCH);
        return  (PX_ERROR);
    }

    LW_TCB_GET_CUR_SAFE(ptcbCur);

    LW_VP_LOCK(pvproc);                                                 /*  锁定当前进程                */
    for (plineTemp  = pvproc->VP_plineThread;
         plineTemp != LW_NULL;
         plineTemp  = _list_line_get_next(plineTemp)) {
    
        ptcb = _LIST_ENTRY(plineTemp, LW_CLASS_TCB, TCB_lineProcess);
        if (__THREAD_LOCK_GET(ptcb)) {                                  /*  外部没有锁定此任务          */
            continue;                                                   /*  线程被锁定                  */
        }

        if (ptcb == ptcbCur) {
            __KERNEL_ENTER();                                           /*  进入内核                    */
            _ThreadSetAffinity(ptcb, stSize, pcpuset);
            __KERNEL_EXIT();                                            /*  退出内核                    */

        } else if (ptcb->TCB_iDeleteProcStatus == LW_TCB_DELETE_PROC_NONE) {
            __KERNEL_ENTER();                                           /*  进入内核                    */
            _ThreadStop(ptcb);
            __KERNEL_EXIT();                                            /*  退出内核 (可能产生调度)     */

            __KERNEL_ENTER();                                           /*  进入内核                    */
            _ThreadSetAffinity(ptcb, stSize, pcpuset);
            _ThreadContinue(ptcb, LW_FALSE);
            __KERNEL_EXIT();                                            /*  退出内核                    */
        }
    }
    LW_VP_UNLOCK(pvproc);                                               /*  解锁当前进程                */
    
    return  (ERROR_NONE);
}

#endif                                                                  /*  LW_CFG_SMP_EN > 0           */
#endif                                                                  /*  LW_CFG_MODULELOADER_EN > 0  */
/*********************************************************************************************************
  END
*********************************************************************************************************/
