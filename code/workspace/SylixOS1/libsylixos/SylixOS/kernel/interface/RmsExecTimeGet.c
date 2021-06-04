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
** ��   ��   ��: RmsExecTimeGet.c
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2007 �� 01 �� 11 ��
**
** ��        ��: ��õ�ǰ����ӵ��� API_RmsPeriod() ������Ŀǰִ�е�ʱ�䣬��λΪ: Tick.

** BUG
2007.11.04  ���� _DebugHandle() ����.
2008.05.18  ʹ�� __KERNEL_ENTER() ���� ThreadLock();
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
/*********************************************************************************************************
** ��������: API_RmsExecTimeGet
** ��������: ��õ�ǰ����ӵ��� API_RmsPeriod() ������Ŀǰִ�е�ʱ�䣬��λΪ: Tick.
** �䡡��  : 
**           ulId                          RMS ���
**           pulExecTime                   ����ʱ��
** �䡡��  : 
** ȫ�ֱ���: 
** ����ģ��: 
                                           API ����
                                           
                                       (�������ж��е���)
*********************************************************************************************************/
#if (LW_CFG_RMS_EN > 0) && (LW_CFG_MAX_RMSS > 0)

LW_API  
ULONG   API_RmsExecTimeGet (LW_OBJECT_HANDLE  ulId, ULONG  *pulExecTime)
{
             INTREG         iregInterLevel;
    REGISTER PLW_CLASS_RMS  prms;
    REGISTER UINT16         usIndex;
    
    usIndex = _ObjectGetIndex(ulId);
    
    if (LW_CPU_GET_CUR_NESTING()) {                                     /*  �������ж��е���            */
        _DebugHandle(__ERRORMESSAGE_LEVEL, "called from ISR.\r\n");
        _ErrorHandle(ERROR_KERNEL_IN_ISR);
        return  (ERROR_KERNEL_IN_ISR);
    }
    
#if LW_CFG_ARG_CHK_EN > 0
    if (!pulExecTime) {
        _DebugHandle(__ERRORMESSAGE_LEVEL, "pulExecTime invalidate.\r\n");
        _ErrorHandle(ERROR_KERNEL_BUFFER_NULL);
        return  (ERROR_KERNEL_BUFFER_NULL);
    }
    
    if (!_ObjectClassOK(ulId, _OBJECT_RMS)) {                           /*  �������ͼ��                */
        _DebugHandle(__ERRORMESSAGE_LEVEL, "RMS handle invalidate.\r\n");
        _ErrorHandle(ERROR_KERNEL_HANDLE_NULL);
        return  (ERROR_KERNEL_HANDLE_NULL);
    }
    
    if (_Rms_Index_Invalid(usIndex)) {                                  /*  �������������              */
        _DebugHandle(__ERRORMESSAGE_LEVEL, "RMS handle invalidate.\r\n");
        _ErrorHandle(ERROR_KERNEL_HANDLE_NULL);
        return  (ERROR_KERNEL_HANDLE_NULL);
    }
    
    iregInterLevel = __KERNEL_ENTER_IRQ();                              /*  �����ں�                    */
    if (_Rms_Type_Invalid(usIndex)) {
        __KERNEL_EXIT_IRQ(iregInterLevel);                              /*  �˳��ں�                    */
        _DebugHandle(__ERRORMESSAGE_LEVEL, "RMS handle invalidate.\r\n");
        _ErrorHandle(ERROR_RMS_NULL);
        return  (ERROR_RMS_NULL);
    }
#else
    iregInterLevel = __KERNEL_ENTER_IRQ();                              /*  �����ں�                    */
#endif

    prms = &_K_rmsBuffer[usIndex];
    
    if (prms->RMS_ucStatus != LW_RMS_ACTIVE) {                          /*  ״̬����                    */
        __KERNEL_EXIT_IRQ(iregInterLevel);                              /*  �˳��ں�                    */
        _ErrorHandle(ERROR_RMS_STATUS);
        return  (ERROR_RMS_STATUS);
    }
    
    *pulExecTime = _RmsGetExecTime(prms);                               /*  �������ʱ��                */
    
    __KERNEL_EXIT_IRQ(iregInterLevel);                                  /*  �˳��ں�                    */

    return  (ERROR_NONE);
}

#endif                                                                  /*  (LW_CFG_RMS_EN > 0)         */
                                                                        /*  (LW_CFG_MAX_RMSS > 0)       */
/*********************************************************************************************************
  END
*********************************************************************************************************/