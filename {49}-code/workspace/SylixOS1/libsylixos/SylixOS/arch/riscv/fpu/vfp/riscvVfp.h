/*********************************************************************************************************
**
**                                    �й�������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: riscvVfp.h
**
** ��   ��   ��: Jiao.JinXing (������)
**
** �ļ���������: 2018 �� 03 �� 20 ��
**
** ��        ��: RISC-V ��ϵ�ܹ� FPU ֧��.
*********************************************************************************************************/

#ifndef __ARCH_RISCV_VFP_H
#define __ARCH_RISCV_VFP_H

PRISCV_FPU_OP   riscvVfpPrimaryInit(CPCHAR  pcMachineName, CPCHAR  pcFpuName);
VOID            riscvVfpSecondaryInit(CPCHAR  pcMachineName, CPCHAR  pcFpuName);

#endif                                                                  /*  __ARCH_RISCV_VFP_H          */
/*********************************************************************************************************
  END
*********************************************************************************************************/