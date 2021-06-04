/*********************************************************************************************************
**
**                                    �й������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: ttinyShellFsCmd.h
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2008 �� 07 �� 27 ��
**
** ��        ��: ϵͳ�ڲ������ (�ļ�ϵͳ���).
*********************************************************************************************************/

#ifndef __TTINYSHELLFSCMD_H
#define __TTINYSHELLFSCMD_H

typedef struct fileNameNode {
    PCHAR fileName;
    PCHAR line;
    struct fileNameNode *next;
} FNode;

/*********************************************************************************************************
  �ü�����
*********************************************************************************************************/
#if LW_CFG_SHELL_EN > 0

VOID  __tshellFsCmdInit(VOID);

void sed_p3(int i1,int i2, char * string);
void sed_p2(int i1, char * string);
void sed_d2(int f, char *filename);
void sed_d$(int f, char *filename);

void sed_d3(int i, int i1, char *string);

void sed_s(char *fs, char *ls, char fc, char * filename);

void sed_aic(int f, char c, char *s, char *filename);

void sed_y3(char *fs, char *ls, char *filename);

void sed_w4(int f, int t, char *wfile, char *filename);

void sed_w3(int f, char *wfile, char *filename);

void sed_r3(int f, char *wfile, char *filename);

void sed_r$(char *wfile, char *filename);

void sed_q2(int f, char *filename);

BOOL strrpc_1(char *str,char *oldstr,char *newstr);
char *strrpc(char *str,char *oldstr,char *newstr);

#endif                                                                  /*  LW_CFG_SHELL_EN > 0         */
#endif                                                                  /*  __TTINYSHELLFSCMD_H         */
/*********************************************************************************************************
  END
*********************************************************************************************************/
