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
** 文   件   名: arch_mmu.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2013 年 05 月 04 日
**
** 描        述: ARM 内存管理相关.
**
** BUG:
2015.04.01 将 VMM 页面尺寸相关的配置移动到这里.
*********************************************************************************************************/

#ifndef __ARM_ARCH_MMU_H
#define __ARM_ARCH_MMU_H

/*********************************************************************************************************
  L4 微内核虚拟机 MMU
*********************************************************************************************************/

#define LW_CFG_VMM_L4_HYPERVISOR_EN           0                         /*  是否使用 L4 虚拟机 MMU      */

/*********************************************************************************************************
  是否需要内核超过 3 级页表支持
*********************************************************************************************************/

#define LW_CFG_VMM_PAGE_4L_EN                 0                         /*  是否需要 4 级页表支持       */

/*********************************************************************************************************
  虚拟内存页表相关配置
*********************************************************************************************************/

#if LW_CFG_CPU_PHYS_ADDR_64BIT > 0
#define LW_CFG_VMM_PAGE_SHIFT                 12                        /*  2^12 = 4096                 */
#define LW_CFG_VMM_PAGE_SIZE                  (1ul << LW_CFG_VMM_PAGE_SHIFT)
#define LW_CFG_VMM_PAGE_MASK                  (~(LW_CFG_VMM_PAGE_SIZE - 1))

#define LW_CFG_VMM_PTE_SHIFT                  LW_CFG_VMM_PAGE_SHIFT
#define LW_CFG_VMM_PTE_SIZE                   LW_CFG_VMM_PAGE_SIZE
#define LW_CFG_VMM_PTE_MASK                   (0x1fful << LW_CFG_VMM_PTE_SHIFT)

#define LW_CFG_VMM_PMD_SHIFT                  21                        /*  2^21 = 2MB                  */
#define LW_CFG_VMM_PMD_SIZE                   (1ul << LW_CFG_VMM_PMD_SHIFT)
#define LW_CFG_VMM_PMD_MASK                   (0x1fful << LW_CFG_VMM_PMD_SHIFT)

#define LW_CFG_VMM_PGD_SHIFT                  30                        /*  2^30 = 1GB                  */
#define LW_CFG_VMM_PGD_SIZE                   (1ul << LW_CFG_VMM_PGD_SHIFT)
#define LW_CFG_VMM_PGD_MASK                   (0x3ul << LW_CFG_VMM_PGD_SHIFT)

#else
#define LW_CFG_VMM_PAGE_SHIFT                 12                        /*  2^12 = 4096                 */
#define LW_CFG_VMM_PAGE_SIZE                  (1ul << LW_CFG_VMM_PAGE_SHIFT)
#define LW_CFG_VMM_PAGE_MASK                  (~(LW_CFG_VMM_PAGE_SIZE - 1))

#define LW_CFG_VMM_PMD_SHIFT                  20                        /*  NO PMD same as PGD          */
#define LW_CFG_VMM_PMD_SIZE                   (1ul << LW_CFG_VMM_PMD_SHIFT)
#define LW_CFG_VMM_PMD_MASK                   (~(LW_CFG_VMM_PMD_SIZE - 1))

#define LW_CFG_VMM_PGD_SHIFT                  20                        /*  2^20 = 1MB                  */
#define LW_CFG_VMM_PGD_SIZE                   (1ul << LW_CFG_VMM_PGD_SHIFT)
#define LW_CFG_VMM_PGD_MASK                   (~(LW_CFG_VMM_PGD_SIZE - 1))
#endif                                                                  /*  LW_CFG_CPU_PHYS_ADDR_64BIT  */

/*********************************************************************************************************
  内存分组数量
*********************************************************************************************************/
#ifdef __SYLIXOS_KERNEL

#define LW_CFG_VMM_ZONE_NUM                   8                         /*  物理分区数                  */
#define LW_CFG_VMM_VIR_NUM                    8                         /*  虚拟分区数                  */

/*********************************************************************************************************
  MMU 转换条目类型
*********************************************************************************************************/
#if !defined(__ASSEMBLY__) && !defined(ASSEMBLY)

#if LW_CFG_CPU_PHYS_ADDR_64BIT > 0
typedef UINT64  LW_PGD_TRANSENTRY;                                      /*  页目录类型                  */
typedef UINT64  LW_PMD_TRANSENTRY;                                      /*  中间页目录类型              */
typedef UINT64  LW_PTE_TRANSENTRY;                                      /*  页表条目类型                */

#else                                                                   /*  LW_CFG_ARM_LPAE > 0         */
typedef UINT32  LW_PGD_TRANSENTRY;                                      /*  页目录类型                  */
typedef UINT32  LW_PMD_TRANSENTRY;                                      /*  中间页目录类型              */
typedef UINT32  LW_PTE_TRANSENTRY;                                      /*  页表条目类型                */
#endif                                                                  /*  LW_CFG_CPU_PHYS_ADDR_64BIT  */

#endif
#endif                                                                  /*  __SYLIXOS_KERNEL            */
#endif                                                                  /*  __ARM_ARCH_MMU_H            */
/*********************************************************************************************************
  END
*********************************************************************************************************/
