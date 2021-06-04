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
** 文   件   名: mpi_inline.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 04 月 10 日
**
** 描        述: 这是系统多处理器公用内联函数库
*********************************************************************************************************/

#ifndef __MPI_INLINE_H
#define __MPI_INLINE_H

/*********************************************************************************************************
  地址偏移量加基地址
*********************************************************************************************************/

static LW_INLINE PVOID  _Addresses_Add_Offset (PVOID  pvBase, size_t  stOffset)
{
    return  ((PVOID)((PCHAR)pvBase + stOffset));
}

/*********************************************************************************************************
  地址偏移量减基地址
*********************************************************************************************************/

static LW_INLINE PVOID  _Addresses_Subtract_Offset (PVOID  pvBase, size_t  stOffset)
{
    return  ((PVOID)((PCHAR)pvBase - stOffset));
}

/*********************************************************************************************************
  计算两个地址的差值
*********************************************************************************************************/

static LW_INLINE size_t  _Addresses_Subtract (PVOID  pvLeft, PVOID  pvRight)
{
    return  ((size_t)((PCHAR)pvLeft - (PCHAR)pvRight));
}

/*********************************************************************************************************
  检查某地址是否在相关范围以内
*********************************************************************************************************/

static LW_INLINE BOOL  _Addresses_Is_In_Range (PVOID  pvAddress,
                                               PVOID  pvBase,
                                               PVOID  pvLimit)
{
    return  ((pvAddress >= pvBase) && (pvAddress <= pvLimit));
}

#endif                                                                  /*  __MPI_INLINE_H              */
/*********************************************************************************************************
  END
*********************************************************************************************************/
