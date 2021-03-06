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
** 文   件   名: threadpool.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 03 月 20 日
**
** 描        述: 这是线程池内部头文件

** BUG
2007.04.08  加入了对裁剪的宏支持
*********************************************************************************************************/

#ifndef __THREADPOOL_H
#define __THREADPOOL_H

/*********************************************************************************************************
  INTERNAL FUNCTION
*********************************************************************************************************/

#if LW_CFG_THREAD_POOL_EN > 0 && LW_CFG_MAX_THREAD_POOLS > 0

PLW_CLASS_THREADPOOL  _Allocate_ThreadPool_Object(VOID);
VOID                  _Free_ThreadPool_Object(PLW_CLASS_THREADPOOL    pthreadpool);

/*********************************************************************************************************
  INLINE FUNCTION
*********************************************************************************************************/

static LW_INLINE INT  _ThreadPool_Index_Invalid (UINT16    usIndex)
{
    return  (usIndex >= LW_CFG_MAX_THREAD_POOLS);
}

#endif                                                                  /*  LW_CFG_THREAD_POOL_EN > 0   */
                                                                        /*  LW_CFG_MAX_THREAD_POOLS > 0 */
#endif                                                                  /*  __THREADPOOL_H              */
/*********************************************************************************************************
  END
*********************************************************************************************************/
