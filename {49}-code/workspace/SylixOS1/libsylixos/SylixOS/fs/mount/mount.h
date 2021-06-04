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
** 文   件   名: mount.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2008 年 09 月 26 日
**
** 描        述: mount 挂载库, 这是 SylixOS 第二类块设备挂载方式.
                 详细见 blockIo.h (不推荐使用! 可能存在问题)
*********************************************************************************************************/

#ifndef __MOUNT_H
#define __MOUNT_H

/*********************************************************************************************************
  裁剪宏
*********************************************************************************************************/
#if (LW_CFG_MAX_VOLUMES > 0) && (LW_CFG_MOUNT_EN > 0)

LW_API VOID     API_MountInit(VOID);
LW_API INT      API_Mount(CPCHAR  pcDevName, CPCHAR  pcVolName, CPCHAR  pcFileSystem);
LW_API INT      API_MountEx(CPCHAR  pcDevName, CPCHAR  pcVolName, CPCHAR  pcFileSystem, CPCHAR  pcOption);
LW_API INT      API_Unmount(CPCHAR  pcVolName);
LW_API VOID     API_MountShow(VOID);

#endif                                                                  /*  LW_CFG_MAX_VOLUMES > 0      */
                                                                        /*  LW_CFG_MOUNT_EN > 0         */
#endif                                                                  /*  __MOUNT_H                   */
/*********************************************************************************************************
  END
*********************************************************************************************************/
