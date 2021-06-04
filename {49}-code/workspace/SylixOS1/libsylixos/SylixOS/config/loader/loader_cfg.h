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
** 文   件   名: loader_cfg.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2009 年 07 月 09 日
**
** 描        述: 装载器配置文件。
*********************************************************************************************************/

#ifndef __LOADER_CFG_H
#define __LOADER_CFG_H

/*********************************************************************************************************
*                                        boot loader 服务 (推荐使用 uboot 或者其他专业 bootloader)
*
* 依存关系: 1: I/O系统
            2: shell系统
*********************************************************************************************************/

#define LW_CFG_BOOTLOADER_EN                    1                       /*  是否需要提供 bootloader 服务*/

/*********************************************************************************************************
*                                        module loader 服务 (.so .ko)
*
* 依存关系: 1: I/O系统
            2: shell系统
            3: symbol服务
            4: 信号系统
*********************************************************************************************************/

#define LW_CFG_MODULELOADER_EN                  1                       /*  是否需要提供模块装载服务    */
#define LW_CFG_MODULELOADER_GCOV_EN             1                       /*  内核模块代码覆盖率分析接口  */
#define LW_CFG_MODULELOADER_ATEXIT_EN           0                       /*  是否允许内核模块 atexit()   */
#define LW_CFG_MODULELOADER_FPUCHK_EN           0                       /*  是否强行检查 FPU 设置兼容性 */
#define LW_CFG_MODULELOADER_TEXT_RO_EN          0                       /*  TEXT 段写保护               */
#define LW_CFG_MODULELOADER_RMMOD_ATREBOOT_EN   0                       /*  重启时卸载所有模块          */
#define LW_CFG_MODULELOADER_STRONGSYM_FIRST_EN  0                       /*  搜索模块符号时是否优先强符号*/
                                                                        /*  建议为 0 与其他操作系统一致 */
/*********************************************************************************************************
*                                        Trusted computing
*
* 依存关系: 1: module loader
*********************************************************************************************************/

#define LW_CFG_TRUSTED_COMPUTING_EN             0                       /*  可信计算模块认证接口        */

#endif                                                                  /*  __LOADER_CFG_H              */
/*********************************************************************************************************
  END
*********************************************************************************************************/
