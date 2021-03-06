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
** 文   件   名: riscvBacktrace.c
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2018 年 03 月 20 日
**
** 描        述: RISC-V 体系构架堆栈回溯 (来源于 glibc).
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
/*********************************************************************************************************
  Only GCC support now.
*********************************************************************************************************/
#ifdef   __GNUC__
#include "riscvBacktrace.h"
/*********************************************************************************************************
  This implementation assumes a stack layout that matches the defaults
  used by gcc's `__builtin_frame_address' and `__builtin_return_address'
  (FP is the frame pointer register):

        +-----------------+     +-----------------+
  FP -> | previous FP     |---->| previous FP     |---->...
        |                 |     |                 |
        | return address  |     | return address  |
        +-----------------+     +-----------------+
*********************************************************************************************************/
/*********************************************************************************************************
  Get some notion of the current stack.  Need not be exactly the top
  of the stack, just something somewhere in the current frame.
*********************************************************************************************************/
#ifndef CURRENT_STACK_FRAME
#define CURRENT_STACK_FRAME         ({ char __csf; &__csf; })
#endif
/*********************************************************************************************************
  By default we assume that the stack grows downward.
*********************************************************************************************************/
#ifndef INNER_THAN
#define INNER_THAN                  <
#endif
/*********************************************************************************************************
  By default, the frame pointer is just what we get from gcc.
*********************************************************************************************************/
#ifndef FIRST_FRAME_POINTER
#define FIRST_FRAME_POINTER         __builtin_frame_address(0)
#endif
/*********************************************************************************************************
** 函数名称: getEndStack
** 功能描述: 获得堆栈结束地址
** 输　入  : NONE
** 输　出  : 堆栈结束地址
** 全局变量:
** 调用模块:
*********************************************************************************************************/
static PVOID  getEndStack (VOID)
{
    PLW_CLASS_TCB  ptcbCur;

    LW_TCB_GET_CUR_SAFE(ptcbCur);

    return  ((PVOID)ptcbCur->TCB_pstkStackTop);
}
/*********************************************************************************************************
** 函数名称: backtrace
** 功能描述: 获得当前任务调用栈
** 输　入  : array     获取数组
**           size      数组大小
** 输　出  : 获取的数目
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API 
int  backtrace (void **array, int size)
{
    struct layout *current;
    void *__unbounded top_frame;
    void *__unbounded top_stack;
    void *__unbounded end_stack;
    int cnt = 0;

    asm volatile ("move %0, s0" : "=r"(top_frame));

    top_stack = CURRENT_STACK_FRAME;
    end_stack = getEndStack();

    /*
     * We skip the call to this function, it makes no sense to record it.
     */
    current = (struct layout *)((unsigned long)top_frame - 2 * sizeof(unsigned long));

    while (cnt < size) {
        if (((void *)current INNER_THAN top_stack) || (!((void *)current INNER_THAN end_stack))) {
            /*
             * This means the address is out of range.  Note that for the
             * toplevel we see a frame pointer with value NULL which clearly is
             * out of range.
             */
            break;
        }

        if (current->return_address == 0) {
            break;
        }

        array[cnt++] = current->return_address;

        current = (struct layout *)((unsigned long)(current->next) - 2 * sizeof(unsigned long));
    }

    return  (cnt);
}

#endif                                                                  /*  __GNUC__                    */
/*********************************************************************************************************
  END
*********************************************************************************************************/
