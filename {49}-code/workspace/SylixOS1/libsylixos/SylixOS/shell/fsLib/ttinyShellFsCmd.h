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
** 文   件   名: ttinyShellFsCmd.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2008 年 07 月 27 日
**
** 描        述: 系统内部命令定义 (文件系统相关).
*********************************************************************************************************/

#ifndef __TTINYSHELLFSCMD_H
#define __TTINYSHELLFSCMD_H

typedef struct fileNameNode {
    PCHAR fileName;
    PCHAR line;
    struct fileNameNode *next;
} FNode;

/*********************************************************************************************************
  裁剪控制
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
