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
** 文   件   名: arm64Backtrace.h
**
** 创   建   人: Wang.Xuan (王翾)
**
** 文件创建日期: 2018 年 06 月 29 日
**
** 描        述: ARM64 体系构架堆栈回溯.
*********************************************************************************************************/

#ifndef __ARM64_BACKTRACE_H
#define __ARM64_BACKTRACE_H

#include "sys/cdefs.h"

/*********************************************************************************************************
  Do not compiling with -fbounded-pointers!
*********************************************************************************************************/

#define BOUNDS_VIOLATED
#define CHECK_BOUNDS_LOW(ARG)       (ARG)
#define CHECK_BOUNDS_HIGH(ARG)      (ARG)
#define CHECK_1(ARG)                (ARG)
#define CHECK_1_NULL_OK(ARG)        (ARG)
#define CHECK_N(ARG, N)             (ARG)
#define CHECK_N_NULL_OK(ARG, N)     (ARG)
#define CHECK_STRING(ARG)           (ARG)
#define CHECK_SIGSET(SET)           (SET)
#define CHECK_SIGSET_NULL_OK(SET)   (SET)
#define CHECK_IOCTL(ARG, CMD)       (ARG)
#define CHECK_FCNTL(ARG, CMD)       (ARG)
#define CHECK_N_PAGES(ARG, NBYTES)  (ARG)
#define BOUNDED_N(PTR, N)           (PTR)
#define BOUNDED_1(PTR) BOUNDED_N    (PTR, 1)

/*********************************************************************************************************
  ARM64 栈帧结构
*********************************************************************************************************/

#define ADJUST_FRAME_POINTER(frame) (struct layout *)((char *)frame)

struct layout {
    void *__unbounded next;
    void *__unbounded return_address;
};

#endif                                                                  /*  __ARM64_BACKTRACE_H         */
/*********************************************************************************************************
  END
*********************************************************************************************************/
