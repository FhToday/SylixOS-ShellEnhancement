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
** ��   ��   ��: lib_strcasestr.c
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2018 �� 11 �� 20 ��
**
** ��        ��: ��
*********************************************************************************************************/
#include "../SylixOS/kernel/include/k_kernel.h"
/*********************************************************************************************************
** ��������: lib_strcasestr
** ��������: 
** �䡡��  : 
** �䡡��  : 
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
PCHAR  lib_strcasestr (CPCHAR  cpcS1, CPCHAR  cpcS2)
{
    CHAR   c, sc;
    size_t len;

    if ((c = *cpcS2++) != 0) {
        c = lib_tolower((unsigned char)c);
        len = lib_strlen(cpcS2);
        do {
            do {
                if ((sc = *cpcS1++) == 0) {
                    return  (LW_NULL);
                }
            } while ((char)lib_tolower((unsigned char)sc) != c);
        } while (lib_strncasecmp(cpcS1, cpcS2, len) != 0);
        cpcS1--;
    }
    
    return  ((char *)cpcS1);
}
/*********************************************************************************************************
  END
*********************************************************************************************************/