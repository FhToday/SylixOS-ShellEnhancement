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
** 文   件   名: arch_compiler.h
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2016 年 06 月 25 日
**
** 描        述: x86 编译器相关.
*********************************************************************************************************/

#ifndef __X86_ARCH_COMPILER_H
#define __X86_ARCH_COMPILER_H

/*********************************************************************************************************
  fast variable
*********************************************************************************************************/

#ifndef REGISTER
#define	REGISTER                register                                /*  寄存器变量                  */
#endif

/*********************************************************************************************************
  inline function
*********************************************************************************************************/

#ifdef __GNUC__
#define LW_INLINE               inline                                  /*  内联函数定义                */
#define LW_ALWAYS_INLINE        __attribute__((always_inline))
#else
#define LW_INLINE               __inline
#define LW_ALWAYS_INLINE        __inline
#endif

/*********************************************************************************************************
  weak function
*********************************************************************************************************/

#ifdef __SYLIXOS_KERNEL
#ifdef __GNUC__
#define LW_WEAK                 __attribute__((weak))                   /*  弱符号                      */
#else
#define LW_WEAK
#endif
#endif                                                                  /*  __SYLIXOS_KERNEL            */

/*********************************************************************************************************
  shared object function call prepare
*********************************************************************************************************/

#define LW_SOFUNC_PREPARE(func)

/*********************************************************************************************************
  编译器结构缩排相关配置
*********************************************************************************************************/

#define LW_STRUCT_PACK_FIELD(x)     x
#define LW_STRUCT_PACK_STRUCT       __attribute__((packed))
#define LW_STRUCT_PACK_BEGIN                                            /*  单字节缩排结构体            */
#define LW_STRUCT_PACK_END                                              /*  结束单字节缩排结构体        */

/*********************************************************************************************************
  编译器 ABI 是否允许内核使用浮点类型
*********************************************************************************************************/

#if LW_CFG_CPU_WORD_LENGHT == 64
#define LW_KERN_FLOATING            0
#else
#define LW_KERN_FLOATING            1                                   /*  是否允许内核使用浮点类型    */
#endif

#endif                                                                  /*  __X86_ARCH_COMPILER_H       */
/*********************************************************************************************************
  END
*********************************************************************************************************/
