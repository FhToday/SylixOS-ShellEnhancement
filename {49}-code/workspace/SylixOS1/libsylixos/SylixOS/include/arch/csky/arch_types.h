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
** ��   ��   ��: arch_types.h
**
** ��   ��   ��: Wang.Xuan (���Q)
**
** �ļ���������: 2018 �� 05 �� 07 ��
**
** ��        ��: C-SKY �������.
*********************************************************************************************************/

#ifndef __CSKY_ARCH_TYPES_H
#define __CSKY_ARCH_TYPES_H

/*********************************************************************************************************
  C++ �� void ������ typedef
*********************************************************************************************************/

#ifdef __cplusplus
#define VOID     void
#else
typedef          void               VOID;                               /*  void   ����                 */
#endif                                                                  /*  __cplusplus                 */

/*********************************************************************************************************
  ����������������Ͷ���
*********************************************************************************************************/

typedef          int                BOOL;                               /*  ������������                */

typedef          void              *PVOID;                              /*  void * ����                 */
typedef const    void              *CPVOID;                             /*  const void  *               */

typedef          char               CHAR;                               /*  8 λ�ַ�����                */
typedef unsigned char               UCHAR;                              /*  8 λ�޷����ַ�����          */
typedef unsigned char              *PUCHAR;                             /*  8 λ�޷����ַ�����ָ��      */
typedef          char              *PCHAR;                              /*  8 λ�ַ�ָ�����            */
typedef const    char              *CPCHAR;                             /*  const char  *               */

typedef unsigned char               BYTE;                               /*  8 λ�ֽڱ���                */
typedef unsigned char              *PBYTE;                              /*  8 λ�ֽڱ���ָ��            */

typedef          long               LONG;                               /*  32/64 λ������              */
typedef unsigned long               ULONG;                              /*  32/64 λ�޷���������        */

typedef          int                INT;                                /*  ��������� int              */
typedef unsigned int                UINT;                               /*  ��������� unsigned int     */
typedef signed   int                SINT;                               /*  ��������� signed   int     */

/*********************************************************************************************************
  ע��: INT8 ������ signed ��. ���� int8_t ���ܱ�֤�� signed ��.
        �ܶ������Ĭ�� char �� unsigned. �����������ǿ�ƽ� INT8 ����Ϊ signed ��.
*********************************************************************************************************/

typedef signed   char               INT8;                               /*  char                        */
typedef unsigned char               UINT8;                              /*  8  λ�޷���������           */
typedef signed   char               SINT8;                              /*  8  λ�з���������           */

typedef          short              INT16;                              /*  short                       */
typedef unsigned short              UINT16;                             /*  16 λ�޷���������           */
typedef signed   short              SINT16;                             /*  16 λ�з���������           */

typedef          int                INT32;                              /*  long or int                 */
typedef unsigned int                UINT32;                             /*  32 λ�޷���������           */
typedef signed   int                SINT32;                             /*  32 λ�з���������           */

typedef          long long          INT64;                              /*  long long                   */
typedef unsigned long long          UINT64;                             /*  64 λ�޷���������           */
typedef signed   long long          SINT64;                             /*  64 λ�з���������           */

/*********************************************************************************************************
  �����ͱ�����������ϵͳ�����жϵ�������.
*********************************************************************************************************/

typedef volatile unsigned int       INTREG;                             /*  ���崦�����жϼĴ���        */

#endif                                                                  /*  __CSKY_ARCH_TYPES_H         */
/*********************************************************************************************************
  END
*********************************************************************************************************/