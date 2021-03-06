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
** 文   件   名: config.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2007 年 12 月 09 日
**
** 描        述: S3C2440 配置.
*********************************************************************************************************/

#ifndef __CONFIG_H
#define __CONFIG_H

/*********************************************************************************************************
  包含顶层配置
*********************************************************************************************************/

#include "../../config.h"

/*********************************************************************************************************
  外部晶振定义 (两种外部晶振频率可供选择 12.0000 MHz 和 16.9344 MHz)
*********************************************************************************************************/
#ifndef ASSEMBLY

#define __FREQ_12_0000MHZ       0                                       /*  12.0000 MHz                 */
#define __FREQ_16_9344MHZ       1                                       /*  16.9344 MHz                 */

/*********************************************************************************************************
  系统 CPU PLL 主频选择 (晶振为 12.0000 MHz)
*********************************************************************************************************/

#define __MPLL_FREQ_271_MHZ     2                                       /*  271.50 MHz                  */
#define __MPLL_FREQ_304_MHZ     3                                       /*  304.00 MHz                  */
#define __MPLL_FREQ_405_MHZ     4                                       /*  405.00 MHz                  */
#define __MPLL_FREQ_532_MHZ     5                                       /*  532.00 MHz                  */

/*********************************************************************************************************
  系统 CPU PLL 主频选择 (晶振为 16.9344 MHz)
*********************************************************************************************************/

#define __MPLL_FREQ_266_MHZ     2                                       /*  266.72 MHz                  */
#define __MPLL_FREQ_296_MHZ     3                                       /*  296.35 MHz                  */
#define __MPLL_FREQ_399_MHZ     4                                       /*  399.65 MHz                  */
#define __MPLL_FREQ_530_MHZ     5                                       /*  530.61 MHz                  */
#define __MPLL_FREQ_533_MHZ     6                                       /*  533.43 MHz                  */

/*********************************************************************************************************
  系统 USB PLL 主频选择
*********************************************************************************************************/

#define __UPLL_FREQ_48_MHZ      0                                       /*  48 MHz                      */
#define __UPLL_FREQ_96_MHZ      1                                       /*  96 MHz                      */

/*********************************************************************************************************
  AHB 总线与 APB 总线速度参数定义 (FCLK:CPU Freq, HCLK:AHB Freq, PCLK:APB Freq)
*********************************************************************************************************/

#define __AHBCLK_DIV2           2                                       /*  FCLK / HCLK = 2             */
#define __AHBCLK_DIV3           3                                       /*  FCLK / HCLK = 3             */
#define __AHBCLK_DIV4           4                                       /*  FCLK / HCLK = 4             */
#define __AHBCLK_DIV6           6                                       /*  FCLK / HCLK = 6             */
#define __AHBCLK_DIV8           8                                       /*  FCLK / HCLK = 8             */

#define __APBCLK_DIV1           1                                       /*  HCLK / PCLK = 1             */
#define __APBCLK_DIV2           2                                       /*  HCLK / PCLK = 2             */

/*********************************************************************************************************
  用户配置外部晶振频率 (__FREQ_12_0000MHZ or __FREQ_16_9344MHZ)
*********************************************************************************************************/

#define INPUT_FREQ              __FREQ_12_0000MHZ                       /*  外部振荡器频率 12.0000 MHz  */

#define MPLL_FREQ               __MPLL_FREQ_405_MHZ                     /*  选择 MPLL 主频为 405.00 MHz */
#define UPLL_FREQ               __UPLL_FREQ_48_MHZ                      /*  选择 UPLL 主频为 48 MHz     */

#define AHB_DIV                 __AHBCLK_DIV4                           /*  AHB 总线频率为 101.00 MHz   */
#define APB_DIV                 __APBCLK_DIV2                           /*  APB 总线频率为  50.00 MHz   */

/*********************************************************************************************************
  平台配置
*********************************************************************************************************/

#define MINI2440_PACKET         1
#undef MICRO2440_PACKET

/*********************************************************************************************************
  系统头文件
*********************************************************************************************************/

#include "2440addr.h"                                                   /*  寄存器定义                  */
#include "targetInit.h"                                                 /*  目标系统相关                */

#endif                                                                  /*  ASSEMBLY                    */
#endif                                                                  /*  __CONFIG_H                  */
/*********************************************************************************************************
  END
*********************************************************************************************************/
