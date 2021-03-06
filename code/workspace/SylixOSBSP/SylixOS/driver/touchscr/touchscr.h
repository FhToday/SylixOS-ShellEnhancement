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
** 文   件   名: touchscr.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2014 年 04 月 29 日
**
** 描        述: 2440 触摸屏接口
*********************************************************************************************************/

#ifndef __TOUCHSCR_H
#define __TOUCHSCR_H

/*********************************************************************************************************
  API
*********************************************************************************************************/
INT  tsDrv(void);
INT  tsDevCreate(PCHAR  pcName);

#endif                                                                  /*  __TOUCHSCR_H                */
/*********************************************************************************************************
  END
*********************************************************************************************************/
