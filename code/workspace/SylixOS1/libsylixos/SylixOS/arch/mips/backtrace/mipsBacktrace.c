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
** 文   件   名: mipsBacktrace.c
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2013 年 12 月 09 日
**
** 描        述: MIPS 体系架构堆栈回溯 (来源于 glibc).
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
#include "stdlib.h"
#include "system/signal/signalPrivate.h"
/*********************************************************************************************************
  Only GCC support now.
*********************************************************************************************************/
#ifdef   __GNUC__
#include "mipsBacktrace.h"
/*********************************************************************************************************
  指令定义
*********************************************************************************************************/
#if LW_CFG_CPU_WORD_LENGHT == 32
#define ADDUI_SP_INST           0x27bd0000                              /*  ADDIU SP                    */
#define SW_RA_INST              0xafbf0000                              /*  SW RA                       */
#define LONGLOG                 2
#else
#define ADDUI_SP_INST           0x67bd0000                              /*  DADDIU SP                   */
#define SW_RA_INST              0xffbf0000                              /*  SD RA                       */
#define LONGLOG                 3
#endif                                                                  /*  LW_CFG_CPU_WORD_LENGHT == 32*/
#define JR_RA_INST              0x03e00008

#define INST_OP_MASK            0xffff0000
#define INST_OFFSET_MASK        0x0000ffff

#define IS_UNALIGN_RA(p)        ((addr_t)(p) & 0x3)
#define IS_UNALIGN_SP(p)        ((addr_t)(p) & (ARCH_REG_SIZE - 1))
/*********************************************************************************************************
** 函数名称: getEndStack
** 功能描述: 获得堆栈结束地址
** 输　入  : NONE
** 输　出  : NONE
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
    MIPS_INSTRUCTION  *addr;
    unsigned long     *ra;
    unsigned long     *sp;
    unsigned long     *fp;
    unsigned long     *end_stack;
    unsigned long     *low_stack;
    MIPS_INSTRUCTION   inst;
    unsigned int       stack_size;
    unsigned int       ra_offset;
    unsigned int       cnt;

    if (!array || (size < 0)) {
        return  (-1);
    }

    __asm__ __volatile__("move %0, $ra\n"                               /*  获得 RA 和 SP               */
                         "move %1, $sp\n"
                         :"=r"(ra), "=r"(low_stack));

    cnt = 0;
    array[cnt++] = ra;

    end_stack = getEndStack();                                          /*  获得堆栈结束地址            */

    stack_size = 0;
    for (addr = (MIPS_INSTRUCTION *)backtrace; ; addr++) {              /*  从 backtrace 函数向下找     */
        inst = *addr;
        if ((inst & INST_OP_MASK) == ADDUI_SP_INST) {                   /*  找到类似 ADDUI SP, SP, -40  */
                                                                        /*  的堆栈开辟指令              */
            stack_size = abs((short)(inst & INST_OFFSET_MASK));         /*  取出 backtrace 函数堆栈大小 */
            if (stack_size != 0) {
                break;
            }

        } else if (inst == JR_RA_INST) {                                /*  遇上了 JR RA 指令           */
            return  (cnt);                                              /*  直接返回                    */
        }
    }

    sp = (unsigned long *)((unsigned long)low_stack + stack_size);      /*  调用者的 SP                 */
    if (IS_UNALIGN_SP(sp)) {
        return  (cnt);
    }

    if (sp[-1] != ((unsigned long)ra)) {                                /*  backtrace 保存的 RA 不对    */
        return  (cnt);
    }

    fp = (unsigned long *)sp[-2];
    if (fp != sp) {                                                     /*  backtrace 保存的 FP 不对    */
        return  (cnt);                                                  /*  或 Release 版本没有保存 FP  */
    }

    if ((sp >= end_stack) || (sp <= low_stack)) {                       /*  SP 不合法                   */
        return  (cnt);
    }

    for (; cnt < size; ) {                                              /*  backtrace                   */

        ra_offset  = 0;
        stack_size = 0;

        for (addr = (MIPS_INSTRUCTION *)ra;                             /*  在返回的位置向上找          */
             stack_size == 0;                                           /*  堆栈开辟指令是函数的第一条  */
             addr--) {                                                  /*  指令, 找到 stack_size 便跳出*/

            inst = *addr;
            switch (inst & INST_OP_MASK) {

            case SW_RA_INST:                                            /*  遇上类似 SW RA, 4(SP) 的指令*/
                ra_offset = abs((short)(inst & INST_OFFSET_MASK));      /*  获得保存 RA 时的偏移量      */
                break;

            case ADDUI_SP_INST:                                         /*  找到类似 ADDUI SP, SP, -40  */
                                                                        /*  的堆栈开辟指令              */
                stack_size = abs((short)(inst & INST_OFFSET_MASK));     /*  取出该函数堆栈大小          */
                break;

            default:
                break;
            }
        }

        if (!ra_offset) {                                               /*  叶子函数, 没有保存 RA       */
            break;                                                      /*  无法继续分析                */
        }

        ra = (unsigned long *)sp[ra_offset >> LONGLOG];                 /*  取出保存的 RA               */
        if (ra == 0 || IS_UNALIGN_RA(ra)) {                             /*  最后一层无返回函数          */
            break;
        }

        array[cnt++] = ra;

        fp = (unsigned long *)sp[(ra_offset >> LONGLOG) - 1];           /*  取出保存的 FP               */

        sp = (unsigned long *)((unsigned long)sp + stack_size);         /*  计算调用者的 SP             */

        if (fp != sp) {                                                 /*  计算的 SP 与保存的 FP 不相同*/
            sp = fp;                                                    /*  以 FP 为准                  */
        }

        if (IS_UNALIGN_SP(sp)) {
            break;
        }

        if ((sp >= end_stack) || (sp <= low_stack)) {                   /*  SP 不合法                   */
            break;
        }
    }

    return  (cnt);
}

#endif                                                                  /*  __GNUC__                    */
/*********************************************************************************************************
  END
*********************************************************************************************************/
