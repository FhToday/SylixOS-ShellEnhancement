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
** 文   件   名: samsungi2c.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 09 月 19 日
**
** 描        述: S3C2440 I2C 驱动(总线驱动部分)
*********************************************************************************************************/

#ifndef __SAMSUNGI2C_H
#define __SAMSUNGI2C_H

/*********************************************************************************************************
** Function name:           i2cBusFuns
** Descriptions:            初始化 i2c 总线并获取操作函数集
** input parameters:        NONE
** output parameters:       NONE
** Returned value:          总线操作函数集
*********************************************************************************************************/
PLW_I2C_FUNCS  i2cBusFuns(VOID);

#endif                                                                  /*  __SAMSUNGI2C_H              */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
