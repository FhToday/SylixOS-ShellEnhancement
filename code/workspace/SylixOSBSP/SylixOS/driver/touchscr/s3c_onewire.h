/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                SyliXOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: s3c_onewire.h
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2014 年 05 月 11 日
**
** 描        述: 一线触屏设备驱动.
*********************************************************************************************************/
#ifndef S3C_ONEWIRE_H__
#define S3C_ONEWIRE_H__

/*********************************************************************************************************
  一线触屏设备 ioctl 驱动应当实现命令定义
*********************************************************************************************************/
#define S3C_ONEWIRE_TS_IOCTL_CMD        200
#define S3C_ONEWIRE_TS_SET_PRIO         (S3C_ONEWIRE_TS_IOCTL_CMD + 1)  /*  设置扫描线程优先级          */
#define S3C_ONEWIRE_TS_SET_BACKLIGHT    (S3C_ONEWIRE_TS_IOCTL_CMD + 2)  /*  设置背光亮度                */
#define S3C_ONEWIRE_TS_GET_PRIO         (S3C_ONEWIRE_TS_IOCTL_CMD + 3)  /*  获得扫描线程优先级          */
#define S3C_ONEWIRE_TS_GET_BACKLIGHT    (S3C_ONEWIRE_TS_IOCTL_CMD + 4)  /*  获得背光亮度                */
/*********************************************************************************************************
  一线触屏信息
*********************************************************************************************************/
typedef struct __S3C_ONEWIRE_TS {
    UCHAR           ucTimerNum;
    UINT            uiOneWireGpio;
    UINT64          ulPclk;
    ULONG           ulTimerIntVector;
} S3C_ONEWIRE_TS;
/*********************************************************************************************************
** 函数名称: s3cOneWireTsDrv
** 功能描述: 安装一线触屏驱动程序
** 输  入  : NONE
** 输  出  : 错误码
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  s3cOneWireTsDrv(VOID);
/*********************************************************************************************************
** 函数名称: s3cOneWireTsCreate
** 功能描述: 创建一线触屏设备
** 输  入  : pcName             设备名
**           pTs                一线触屏信息
** 输  出  : 错误码
** 全局变量:
** 调用模块:
*********************************************************************************************************/
INT  s3cOneWireTsDevCreate(PCHAR  pcName, S3C_ONEWIRE_TS  *pTs);

#endif                                                                  /*  S3C_ONEWIRE_H__             */
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
