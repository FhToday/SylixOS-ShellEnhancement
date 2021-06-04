/*********************************************************************************************************
**
**                                    �й�������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: armParam.c
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2016 �� 04 �� 27 ��
**
** ��        ��: ARM ��ϵ������������.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
#include "armParam.h"
/*********************************************************************************************************
  ��������
*********************************************************************************************************/
static ARM_PARAM    armParam = { LW_TRUE, LW_FALSE };
/*********************************************************************************************************
** ��������: archKernelParam
** ��������: ARM ��ϵ����������������.
** �䡡��  : pcParam       ��������
**                         unalign=yes      �Ƿ�֧�ַǶ������
** �䡡��  : NONE
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
VOID  archKernelParam (CPCHAR  pcParam)
{
    if (lib_strncmp(pcParam, "unalign=", 8) == 0) {                     /*  �Ƿ�֧�ַǶ������          */
        if (pcParam[8] == 'n') {
            armParam.AP_bUnalign = LW_FALSE;
        } else {
            armParam.AP_bUnalign = LW_TRUE;
        }
    
#if LW_CFG_SMP_EN > 0
    } else if (lib_strncmp(pcParam, "sldepcache=", 11) == 0) {
        if (pcParam[11] == 'n') {
            armParam.AP_bSLDepCache = LW_FALSE;
        } else {
            armParam.AP_bSLDepCache = LW_TRUE;
            __ARCH_SPIN_BYPASS();
        }
#endif                                                                  /*  LW_CFG_SMP_EN > 0           */
    }
}
/*********************************************************************************************************
** ��������: archKernelParamGet
** ��������: ��ȡ��������.
** �䡡��  : NONE
** �䡡��  : ����
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
ARM_PARAM  *archKernelParamGet (VOID)
{
    return  (&armParam);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/