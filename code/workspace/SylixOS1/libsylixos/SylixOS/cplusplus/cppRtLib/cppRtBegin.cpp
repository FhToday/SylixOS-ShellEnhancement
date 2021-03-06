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
** 文   件   名: cppRtBegin.cpp
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2011 年 03 月 08 日
**
** 描        述: 操作系统平台 C++ run time 全局对象构建与析构操作库. 
*********************************************************************************************************/
#include "SylixOS.h"
/*********************************************************************************************************
  C 环境函数
*********************************************************************************************************/
extern "C" {
/*********************************************************************************************************
  说明:
  
  编译器(gcc)编译 C++ 工程时, 如果存在全局对象, 那么全局对象的构建函数指针会放在可执行 elf 文件的 .ctors 
  节区(section), 析构函数会放在可执行 elf 文件的 .dtors 节区, 一般标准 gcc 库会引出四个符号:
   
  __CTOR_LIST__
  __CTOR_END__
  __DTOR_LIST__
  __DTOR_END__
   
  其中 __CTOR_LIST__ 表示所有的全局对象构造函数指针数组的首地址, 起始指针为 0xFFFFFFFF, 之后的每一项为
  一个构造函数的入口, 直到 __CTOR_END__ 为止, __CTOR_END__ 指向的函数指针为 0x00000000
  
  其中 __DTOR_LIST__ 表示所有的全局对象析构函数指针数组的首地址, 起始指针为 0xFFFFFFFF, 之后的每一项为
  一个析构函数的入口, 直到 __DTOR_END__ 为止, __DTOR_END__ 指向的函数指针为 0x00000000
  
  一下代码就实现了这个 4 个符号类似的定义. 这样系统就可以在运行用户程序之前, 初始化 C++ 环境, 运行全局
  对象的构造函数, 在系统 reboot 时, 运行系统的析构函数.
  
  如果要让这些符号处于对应 .ctors 和 .dtors 节区指定的位置, 则需要在连接文件加入一下代码:
  
  .ctors :
  {
      KEEP (*cppRtBegin*.o(.ctors))
      KEEP (*(.preinit_array))
      KEEP (*(.init_array))
      KEEP (*(SORT(.ctors.*)))
      KEEP (*(.ctors))
      KEEP (*cppRtEnd*.o(.ctors))
  }
  
  .dtors :
  {
      KEEP (*cppRtBegin*.o(.dtors))
      KEEP (*(.fini_array))
      KEEP (*(SORT(.dtors.*)))
      KEEP (*(.dtors))
      KEEP (*cppRtEnd*.o(.dtors))
  }
  
  以上链接脚本, 将需要的符号定义到了 .ctors .dtors 节区对应的位置 (分别定义到了这两个节区的首尾)
  (其中 .init_array 和 .fini_array 分别是构建和析构具有静态存储时限的对象)
  
  注意:
  
  由于操作系统是在调用用户之前, 就运行了全局对象构造函数, 此时并没有进入多任务环境, 所以对象的构造函数一定
  要足够的简单, 一般仅是用来初始化对象的属性和一些基本数据结构, 更多的操作可以在类中加入专门的初始化方法
  来实现.
*********************************************************************************************************/
/*********************************************************************************************************
  C++ 全局对象构建与析构函数表 (为了和一些编译器不产生冲突, 这里使用 SylixOS 自带的符号)
*********************************************************************************************************/
#ifndef LW_CFG_CPU_ARCH_C6X
#ifdef __GNUC__
static VOIDFUNCPTR __LW_CTOR_LIST__[1] __attribute__((section(".ctors"))) = { (VOIDFUNCPTR)-1 };
static VOIDFUNCPTR __LW_DTOR_LIST__[1] __attribute__((section(".dtors"))) = { (VOIDFUNCPTR)-1 };
#endif                                                                  /*  __GNUC__                    */
#endif                                                                  /*  !LW_CFG_CPU_ARCH_C6X        */
/*********************************************************************************************************
** 函数名称: __cppRtDoCtors
** 功能描述: C++ 运行全局对象构造函数
** 输　入  : NONE
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID  __cppRtDoCtors (VOID)
{
#ifndef LW_CFG_CPU_ARCH_C6X
#ifdef __GNUC__
    volatile VOIDFUNCPTR    *ppfunc;
    
    for (ppfunc = __LW_CTOR_LIST__ + 1;  *ppfunc != LW_NULL;  ppfunc++) {
        if ((*ppfunc) != (VOIDFUNCPTR)-1) {                             /*  可能 C 库还有一个起始符号   */
            (*ppfunc)();
        }
    }
#endif                                                                  /*  __GNUC__                    */
#else
#define PINIT_BASE      __TI_INITARRAY_Base
#define PINIT_LIMIT     __TI_INITARRAY_Limit

    extern __attribute__((weak)) __far volatile VOIDFUNCPTR const PINIT_BASE[];
    extern __attribute__((weak)) __far volatile VOIDFUNCPTR const PINIT_LIMIT[];

    if (PINIT_BASE != PINIT_LIMIT) {
        ULONG  i = 0;
        while (&(PINIT_BASE[i]) != PINIT_LIMIT) {
            PINIT_BASE[i++]();
        }
    }
#endif                                                                  /*  !LW_CFG_CPU_ARCH_C6X        */
}
/*********************************************************************************************************
** 函数名称: __cppRtDoDtors
** 功能描述: C++ 运行全局对象析构函数
** 输　入  : NONE
** 输　出  : NONE
** 全局变量: 
** 调用模块: 
*********************************************************************************************************/
VOID  __cppRtDoDtors (VOID)
{
#ifndef LW_CFG_CPU_ARCH_C6X
#ifdef __GNUC__
    volatile VOIDFUNCPTR    *ppfunc;
    
    for (ppfunc = __LW_DTOR_LIST__ + 1;  *ppfunc != LW_NULL;  ppfunc++);/*  首先需要运行最后一个        */
    ppfunc--;
    
    while (ppfunc > __LW_DTOR_LIST__) {
        if ((*ppfunc) != (VOIDFUNCPTR)-1) {                             /*  可能 C 库还有一个起始符号   */
            (*ppfunc)();
        }
        ppfunc--;
    }
#endif                                                                  /*  __GNUC__                    */
#endif                                                                  /*  !LW_CFG_CPU_ARCH_C6X        */
}
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
