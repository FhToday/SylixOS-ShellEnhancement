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
** 文   件   名: ThreadIsLock.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 10 月 28 日
**
** 描        述: 获取任务是否锁定的调度器.

** BUG
2008.01.20  返回值为 ULONG 表明锁的嵌套深度.
2008.01.20  使用局部锁代替原有的全局调度器锁.
2013.07.18  兼容 SMP 系统.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
/*********************************************************************************************************
** 函数名称: API_ThreadIsLock
** 功能描述: 获取任务是否锁定的调度器, 如果锁定, 则任务如果不主动退出就绪态, 则会一直运行下去. 
** 输　入  : NONE
** 输　出  : 没有锁定返回 0, 其他表示被锁定.
** 全局变量: 
** 调用模块: 
** 注  意  : 此 API 仅对当前 CPU 生效, 并不会影响其他 CPU 调度
                                           API 函数
*********************************************************************************************************/
LW_API  
ULONG  API_ThreadIsLock (VOID)
{
    PLW_CLASS_TCB   ptcbCur;

    if (!LW_SYS_STATUS_IS_RUNNING()) {                                  /*  系统必须已经启动            */
        _ErrorHandle(ERROR_KERNEL_NOT_RUNNING);
        return  (1);
    }
    
    LW_TCB_GET_CUR_SAFE(ptcbCur);
    
    return  (__THREAD_LOCK_GET(ptcbCur));
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
