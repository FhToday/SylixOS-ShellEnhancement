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
** 文   件   名: px_pthread_np.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2013 年 05 月 04 日
**
** 描        述: pthread 扩展兼容库.
*********************************************************************************************************/

#ifndef __PX_PTHREAD_NP_H
#define __PX_PTHREAD_NP_H

#include "SylixOS.h"                                                    /*  操作系统头文件              */

/*********************************************************************************************************
  裁剪支持
*********************************************************************************************************/
#if (LW_CFG_POSIX_EN > 0) && (LW_CFG_POSIXEX_EN > 0)

#include "px_pthread.h"

#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

/*********************************************************************************************************
  pthread NULL attr method (PTHREAD_NULL_ATTR_METHOD_USE_INHERIT is default method)
*********************************************************************************************************/
#define PTHREAD_NULL_ATTR_METHOD_USE_INHERIT        0                   /*  inherit stack size and prio */
#define PTHREAD_NULL_ATTR_METHOD_USE_DEFSETTING     1                   /*  use default attr            */

LW_API int  pthread_null_attr_method_np(int  method, int *old_method);
LW_API int  pthread_verify_np(pthread_t thread);

LW_API int  pthread_start_np(pthread_t  thread);
LW_API int  pthread_wakeup_np(pthread_t  thread, int  timeout_only);

LW_API int  pthread_attr_get_np(pthread_t  thread, pthread_attr_t *pattr);
LW_API int  pthread_getattr_np(pthread_t thread, pthread_attr_t *pattr);

LW_API int  pthread_setname_np(pthread_t  thread, const char  *name);
LW_API int  pthread_getname_np(pthread_t  thread, char  *name, size_t len);

LW_API int  pthread_setaffinity_np(pthread_t  thread, size_t setsize, const cpu_set_t *set);
LW_API int  pthread_getaffinity_np(pthread_t  thread, size_t setsize, cpu_set_t *set);

LW_API void pthread_key_cleanup_np(int destroy);

/*********************************************************************************************************
  获得线程列表
*********************************************************************************************************/
struct pthread_info {
    pthread_t  tid;
    char      *name;
    size_t     size;
};

struct pthread_list {
    unsigned int         total;                                         /*  进程中线程的总数            */
    unsigned int         get_cnt;                                       /*  本次获取的线程句柄数        */
    unsigned int         pool_cnt;                                      /*  缓冲区中可保存的线程 ID 个数*/
    struct pthread_info *pool;                                          /*  线程句柄缓冲区              */
};

#if LW_CFG_MODULELOADER_EN > 0
LW_API int  pthread_list_np(struct pthread_list *list);
#endif

#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif                                                                  /*  LW_CFG_POSIX_EN > 0         */
                                                                        /*  LW_CFG_POSIXEX_EN > 0       */
#endif                                                                  /*  __PX_PTHREAD_NP_H           */
/*********************************************************************************************************
  END
*********************************************************************************************************/
