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
** 文   件   名: fsCommon.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2009 年 07 月 05 日
**
** 描        述: 文件系统接口公共服务部分(这个部分加入的晚了, 导致很多文件系统公共行为没有很好的封装
                                          在以后的版本中, 将会慢慢改进).
*********************************************************************************************************/

#ifndef __FSCOMMON_H
#define __FSCOMMON_H

/*********************************************************************************************************
  文件系统函数声明
*********************************************************************************************************/
#if LW_CFG_MAX_VOLUMES > 0

INT      __fsRegister(CPCHAR   pcName, 
                      FUNCPTR  pfuncCreate, 
                      FUNCPTR  pfuncCheck,
                      FUNCPTR  pfuncProb);                              /*  注册文件系统                */
FUNCPTR  __fsCreateFuncGet(CPCHAR       pcName, 
                           PLW_BLK_DEV  pblkd, 
                           UINT8        ucPartType);                    /*  获得文件系统创建函数        */
UINT8    __fsPartitionProb(PLW_BLK_DEV  pblkd);                         /*  特殊分区探测                */

VOID     __fsDiskLinkCounterAdd(PLW_BLK_DEV  pblkd);                    /*  物理连接计数操作            */
VOID     __fsDiskLinkCounterDec(PLW_BLK_DEV  pblkd);
UINT     __fsDiskLinkCounterGet(PLW_BLK_DEV  pblkd);

#endif                                                                  /*  (LW_CFG_MAX_VOLUMES > 0)    */

/*********************************************************************************************************
  通用函数声明
*********************************************************************************************************/

INT      __fsCheckFileName(CPCHAR  pcName);                             /*  文件名正确性检查            */

#endif                                                                  /*  __FSCOMMON_H                */
/*********************************************************************************************************
  END
*********************************************************************************************************/
