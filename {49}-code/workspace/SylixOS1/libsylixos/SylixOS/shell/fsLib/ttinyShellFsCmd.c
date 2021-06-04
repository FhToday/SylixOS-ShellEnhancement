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
** ��   ��   ��: ttinyShellFsCmd.c
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2008 �� 07 �� 27 ��
**
** ��        ��: ϵͳ�ڲ������ (�ļ�ϵͳ���).

** BUG
2008.11.11  __tshellFsCmdCp() ������Ŀ¼�ļ���ΪԴ�ļ�.
2008.12.04  mv �ƶ��ļ�ʱ, ����ͬһ����, ��Ҫ���и���, ɾ������.
2009.02.25  touch ���������ļ���СΪ��.
2009.02.21  �޸��� ls �� attrib ��Էǵ�ǰĿ¼�Ĵ���.
2009.03.10  cp �ļ�ʱ��Ҫͬ���ļ�ʱ��.
2009.03.11  ls �� attrib ����Ķ��ļ��Ĵ�С��ӡ����.
2009.03.14  ����� df ָ���֧��.
2009.03.21  �� attrib �����Ϊ ll.
            cp ����Ļ����������ļ���С����Ӧ.
2009.05.19  mv Ҫ��ֻд��ʽ���ļ�.
2009.06.30  cp ������Ҫ�� O_TRUNC ���ļ�.
2009.07.03  ������ GCC ����ʱ�ľ���.
2009.07.04  ���� shfile ָ��, ����ִ��ָ���� shell ����ļ�.
2009.07.08  mv ����ʱ��Ŀ�����ʱ, ��Ҫ��ʾ.
2009.07.28  �� mv ʧ��ʱ, ֱ�ӽ��� cp �� rm ���ϲ���.
2009.08.27  ls �� ll ������ stat() ����ʱ, ʹ��Ĭ��������ʾ.
2009.09.01  ������ ls ll ����Բ���Ŀ¼��β���� / �ַ�ʱ���ֵĴ���.
2009.10.02  �޷���ȡ stat ���ļ�Ĭ��Ϊ�ַ������ļ�.
2009.11.18  ���� df ��ʾ.
2009.12.14  __tshellFsCmdLl() ֧�ִ�ӡ�޸�ʱ��.
            �� ls ����Ĵ�ӡ����.
            ���� mount ����.
2009.12.15  ���� symbol link �ļ�����.
            ���������ļ��ɹ��жϵ�һ�� bug.
2009.12.29  ll �����ļ����Ե�һ���ַ���ʾ�����꾡.
2010.09.10  ls ��� d_type != DT_UNKNOWN ʱ, ����Ҫ���� stat() ����.
2011.03.04  cat ����֧���м���.
2011.03.22  ls ���������ļ���ɫ����.
2011.03.27  ���� mkfifo ����.
2011.05.16  __tshellFsCmdMv() ʹ�� rename() �滻 ioctl(... FIOMOVE ...) ��ʽ. 
2011.06.03  ���� varload �� varsave ����.
2011.06.23  ʹ���µ�ɫ��ת��.
2011.08.07  __tshellFsCmdLl() ֧����ʾ�����ļ�Ŀ��Ĺ���.
2011.11.17  cat ����ܴ�ӡ�� reg �ļ�.
2012.03.25  cd �� ch ������Ҫ�� stat �ж�.
2012.10.20  ����һ�� tmpname ����.
2012.10.25  cd ����ʹ�� chdir() ʵ��.
2013.01.22  ll ������ļ������ߵ���ʾ, chmod �����ͨ�����֧��.
2013.04.01  ���� GCC 4.7.3 �������� warning.
2013.06.24  ����� vfat ����֧��.
2014.05.30  ����鿴�ļ��д�С������.
2014.10.10  cp ���Ŀ���ļ�����Ϊ��ԭʼ�ļ���ͬ�� mode.
2014.10.29  ls ��ʾ�ĵ��п��ͨ�� TIOCGWINSZ ��ȡ.
2014.12.08  ln ֧�� -f ѡ��.
*********************************************************************************************************/
#define  __SYLIXOS_STDIO
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
#include "../SylixOS/system/include/s_system.h"
#include "../SylixOS/shell/include/ttiny_shell.h"
#include "sys/ioctl.h"
/*********************************************************************************************************
  �ü�����
*********************************************************************************************************/
#if LW_CFG_SHELL_EN > 0 && LW_CFG_MAX_VOLUMES > 0
#include "../SylixOS/shell/ttinyShell/ttinyShell.h"
#include "../SylixOS/shell/ttinyShell/ttinyShellLib.h"
#include "../SylixOS/shell/ttinyShell/ttinyShellSysCmd.h"
#include "../SylixOS/shell/ttinyVar/ttinyVarLib.h"
#include "../SylixOS/fs/include/fs_fs.h"
#include "../SylixOS/shell/fsLib/ttinyShellFsCmd.h"
#if LW_CFG_POSIX_EN > 0
#include "fnmatch.h"
#endif                                                                  /*  LW_CFG_POSIX_EN > 0         */
/*********************************************************************************************************
  ��������
*********************************************************************************************************/
static INT      __tshellFsCmdCp(INT  iArgC, PCHAR  ppcArgV[]);
LW_API time_t   API_RootFsTime(time_t  *time);
/*********************************************************************************************************
** ��������: __tshellFsCmdCd
** ��������: ϵͳ���� "cd"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdCd (INT  iArgC, PCHAR  ppcArgV[])
{
    REGISTER INT    iError;
             CHAR   cPath[MAX_FILENAME_LENGTH];

    if (iArgC > 2) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (iArgC == 1) {
        if (API_TShellGetUserHome(getuid(), cPath, 
                                  MAX_FILENAME_LENGTH) == ERROR_NONE) {
            iError = cd(cPath);
        
        } else {
            iError = cd(PX_STR_ROOT);
        }
    
    } else {
        PCHAR   pcTail;
        
        if (ppcArgV[1][0] == '~') {
            pcTail = &ppcArgV[1][1];
            
            if (API_TShellGetUserHome(getuid(), cPath, 
                                      MAX_FILENAME_LENGTH) < ERROR_NONE) {
                fprintf(stderr, "can not get current user home directory!\n");
                return  (-ERROR_TSHELL_EUSER);
            }
            lib_strlcat(cPath, pcTail, MAX_FILENAME_LENGTH);
            iError = cd(cPath);
        
        } else {
            iError = cd(ppcArgV[1]);
        }
    }
    
    if (iError) {
        if (errno == EACCES) {
            fprintf(stderr, "insufficient permissions.\n");
        } else if (errno == ENOTDIR) {
            fprintf(stderr, "not a directory!\n");
        } else {
            fprintf(stderr, "cd: error: %s\n", lib_strerror(errno));
        }
        
        return  (iError);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdCh
** ��������: ϵͳ���� "ch"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdCh (INT  iArgC, PCHAR  ppcArgV[])
{
    REGISTER INT    iError;

    if (iArgC != 2) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    iError = chdir(ppcArgV[1]);
    if (iError) {
        if (errno == EACCES) {
            fprintf(stderr, "insufficient permissions.\n");
        } else if (errno == ENOTDIR) {
            fprintf(stderr, "not a directory!\n");
        } else {
            fprintf(stderr, "ch: error: %s\n", lib_strerror(errno));
        }
        return  (iError);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdPwd
** ��������: ϵͳ���� "pwd"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdPwd (INT  iArgC, PCHAR  ppcArgV[])
{
    CHAR   cFileName[MAX_FILENAME_LENGTH];
    
    ioDefPathGet(cFileName);
    lib_strcat(cFileName, "\n");
    printf(cFileName);
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdTmpname
** ��������: ϵͳ���� "tmpname"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdTmpname (INT  iArgC, PCHAR  ppcArgV[])
{
    char cBuf[L_tmpnam];
    
    printf("can mktmp as name: %s\n", tmpnam(cBuf));
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdMkdir
** ��������: ϵͳ���� "mkdir"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdMkdir (INT  iArgC, PCHAR  ppcArgV[])
{
    REGISTER INT    iError;
    
    if (iArgC != 2) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    iError = mkdir(ppcArgV[1], DEFAULT_DIR_PERM);
    if (iError) {
        if (errno == EACCES) {
            fprintf(stderr, "insufficient permissions.\n");
        } else {
            fprintf(stderr, "can not make this directory, error: %s\n", lib_strerror(errno));
        }
    }
    
    return  (iError);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdMkfifo
** ��������: ϵͳ���� "mkfifo"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdMkfifo (INT  iArgC, PCHAR  ppcArgV[])
{
    REGISTER INT    iError;
    
    if (iArgC != 2) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    iError = mkfifo(ppcArgV[1], 0);
    if (iError) {
        if (errno == EACCES) {
            fprintf(stderr, "insufficient permissions.\n");
        } else {
            fprintf(stderr, "can not make this fifo, error: %s\n", lib_strerror(errno));
        }
    }
    
    return  (iError);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdRmdir
** ��������: ϵͳ���� "rmdir"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdRmdir (INT  iArgC, PCHAR  ppcArgV[])
{
    REGISTER INT    iError;
    
    if (iArgC != 2) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    iError = rmdir(ppcArgV[1]);
    if (iError) {
        if (API_GetLastError() == ENOENT) {
            fprintf(stderr, "directory is not exist!\n");
        } else {
            fprintf(stderr, "can not remove directory, error: %s\n", lib_strerror(errno));
        }
    }
    
    return  (iError);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdRm
** ��������: ϵͳ���� "rm"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdRm (INT  iArgC, PCHAR  ppcArgV[])
{
    REGISTER INT    iError;
             PCHAR  pcFile;
             BOOL   bForce = LW_FALSE;
             BOOL   bBusy  = LW_FALSE;
    
    if (iArgC == 2) {
        pcFile = ppcArgV[1];

    } else if (iArgC > 2) {
        if (lib_strcmp(ppcArgV[1], "-f")) {
            fprintf(stderr, "option error!\n");
            return  (-ERROR_TSHELL_EPARAM);
        }
        bForce = LW_TRUE;
        pcFile = ppcArgV[2];

    } else {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }

    if (!bForce) {
        fisbusy(pcFile, &bBusy);
        if (bBusy) {
            fprintf(stderr, "the target file is being used!\n");
            return  (PX_ERROR);
        }
    }

    iError = unlink(pcFile);
    if (iError) {
        if (API_GetLastError() == ENOENT) {
            fprintf(stderr, "file is not exist!\n");
        } else {
            fprintf(stderr, "can not remove this file, error: %s\n", lib_strerror(errno));
        }
    }
    
    return  (iError);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdMv
** ��������: ϵͳ���� "mv"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdMv (INT  iArgC, PCHAR  ppcArgV[])
{
    REGISTER INT         iError = PX_ERROR;
    REGISTER INT         iFd;
             CHAR        cTemp[16];
             struct stat statGet;
    
    if (iArgC != 3) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    iFd = open(ppcArgV[1], O_RDONLY, 0);
    if (iFd < 0) {
        fprintf(stderr, "%s error: %s\n", ppcArgV[1], lib_strerror(errno));
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    iError = access(ppcArgV[2], 0);                                     /*  ���Ŀ���ļ��Ƿ����        */
    if (iError != PX_ERROR) {
__re_select:
        printf("destination file is exist, overwrite? (Y/N)\n");
        read(0, cTemp, 16);
        if ((cTemp[0] == 'N') ||
            (cTemp[0] == 'n')) {                                        /*  ������                      */
            goto    __error_handle;
        } else if ((cTemp[0] == 'Y') ||
                   (cTemp[0] == 'y')) {                                 /*  ����                        */
            if (stat(ppcArgV[2], &statGet)) {
                goto    __error_handle;
            }
            if (S_ISDIR(statGet.st_mode)) {                             /*  ��������Ŀ¼              */
                fprintf(stderr, "Error: %s is an existing directory!\n", ppcArgV[2]);
                goto    __error_handle;
            }
            if (unlink(ppcArgV[2]) != ERROR_NONE) {
                goto    __error_handle;
            }
        } else {
            goto    __re_select;                                        /*  ѡ�����                    */
        }
    }
    close(iFd);                                                         /*  �ر�Դ�ļ�                  */
    
    iError = rename(ppcArgV[1], ppcArgV[2]);
    if (iError < 0) {
        iError = __tshellFsCmdCp(3, ppcArgV);                           /*  ��Ҫ���п���ɾ��            */
        if (iError >= 0) {
            __tshellFsCmdRm(2, ppcArgV);                                /*  ɾ��Դ�ļ�                  */
        }
    }
    return  (iError);
    
__error_handle:
    close(iFd);
    return  (PX_ERROR);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdCat
** ��������: ϵͳ���� "cat"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdCat (INT  iArgC, PCHAR  ppcArgV[])
{
             BOOL           bLastLf = LW_TRUE;
    REGISTER INT            iError;
    REGISTER ssize_t        sstNum;
    REGISTER INT            iFd;
             CHAR           cBuffer[MAX_FILENAME_LENGTH];
             struct stat    statFile;
             
    if (iArgC != 2) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    iFd = open(ppcArgV[1], O_RDONLY, 0);
    if (iFd < 0) {
        if (errno == EACCES) {
            fprintf(stderr, "insufficient permissions.\n");
        } else {
            fprintf(stderr, "file error %s!\n", lib_strerror(errno));
        }
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    iError = fstat(iFd, &statFile);
    if (iError < 0) {
        fprintf(stderr, "file stat error!\n");
        close(iFd);
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (S_ISDIR(statFile.st_mode)) {
        fprintf(stderr, "file read error: Is a directory!\n");
        close(iFd);
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    API_ThreadCleanupPush((VOIDFUNCPTR)close, (PVOID)(LONG)iFd);
    
    do {
        sstNum = read(iFd, cBuffer, MAX_FILENAME_LENGTH);
        if (sstNum > 0) {
            write(1, cBuffer, (size_t)sstNum);
            // printf("\n%ld\n", sstNum);
            bLastLf = (cBuffer[sstNum - 1] == '\n') ? LW_TRUE : LW_FALSE;
        }
    } while (sstNum > 0);
    
    API_ThreadCleanupPop(LW_TRUE);
    
    if (!bLastLf) {
        printf("\n");
    }
    
    return  (ERROR_NONE);
}

/**
 *  pattern matching function
 *  return 0: match fails, 1: success
 */
static INT patternMatch(CPCHAR pattern, CPCHAR string) {
    INT pLen, sLen;
    pLen = lib_strlen(pattern);
    sLen = lib_strlen(string);

    if (pLen > sLen) {
        return 0;
    }

    INT i, j;
    for (i = 0; i < sLen; ++i) {
        INT boolean = 1;
        for (j = 0; j < pLen; ++j) {
            if (*(pattern + j) != *(string + i + j)) {
                boolean = 0;
                break;
            }
        }
        if (boolean) {
            return 1;
        }
    }
    return 0;
}

/**
 *  ͨ���ƥ��
 */
INT wildCard(char* pattern, char* string) {
    int m = lib_strlen(string);
    int n = lib_strlen(pattern);
    int dp[m + 1][n + 1];
    memset(dp, 0, sizeof(dp));
    dp[0][0] = 1;
    INT i;
    for (i = 1; i <= n; ++i) {
        if (pattern[i - 1] == '*') {
            dp[0][i] = 1;
        } else {
            break;
        }
    }
    INT j;
    for (i = 1; i <= m; ++i) {
        for (j = 1; j <= n; ++j) {
            if (pattern[j - 1] == '*') {
                dp[i][j] = dp[i][j - 1] | dp[i - 1][j];
            } else if (pattern[j - 1] == '?'
                    || string[i - 1] == pattern[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }
    return dp[m][n];
}

/**
 *  regular expression
 */
static INT match(CPCHAR s, CPCHAR p, INT i, INT j) {
    if (i == 0) {
        return 0;
    }
    if (p[j - 1] == '.') {
        return 1;
    }
    return s[i - 1] == p[j - 1];
}

static INT regex(CPCHAR pattern, CPCHAR string) {
    INT m = lib_strlen(string);
    INT n = lib_strlen(pattern);

    INT f[m + 1][n + 1];
    INT i, j;
    for (i = 0; i < m + 1; ++i) {
        for (j = 0; j < n + 1; ++j) {
            f[i][j] = 0;
        }
    }
    f[0][0] = 1;


    for (i = 0; i <= m; ++i) {
        for (j = 1; j <= n; ++j) {
            if (pattern[j - 1] == '*') {
                f[i][j] = f[i][j - 2];
                if (match(string, pattern, i, j - 1)) {
                    f[i][j] = f[i][j] || f[i - 1][j];
                }
            } else {
                if (match(string, pattern, i, j)) {
                    f[i][j] = f[i - 1][j - 1];
                }
            }
        }
    }

    return f[m][n];
}

/**
 *  grep, a string matching tool
 *  grep -args [string] [filename]
 */
static INT  __tshellFsCmdGrep (INT  iArgC, PCHAR  ppcArgV[])
{
    REGISTER INT            iError;
    REGISTER ssize_t        sstRdNum;
    REGISTER INT            iFd;
             PCHAR          pcBuffer = LW_NULL;
             CHAR           cTemp[128];
             size_t         stBuffer;
             struct stat    statFile;
             CPCHAR         pattern;
             INT            regexMode = 0;
             INT            recurMode = 0;
    REGISTER DIR            *pdir;
    REGISTER struct dirent  *pdirent;
             CHAR           cDirName[MAX_FILENAME_LENGTH] = "./";
             size_t         stDirLen = 1;
             INT            isDir = 0;   // �����Ƿ�Ϊָ��Ŀ¼


    if (iArgC <= 2) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }

    pattern = ppcArgV[iArgC - 2];

    FNode *head, *cur, *prev;   // ����洢
    head = (FNode *) __SHEAP_ALLOC(sizeof(FNode));
    prev = head;

    INT i;  // ����args�б�
    for (i = 1; i < iArgC - 2; ++i) {
        if (!lib_strcmp(ppcArgV[i], "-R")) {
            regexMode = 1;
        }
        if (!lib_strcmp(ppcArgV[i], "-r")) {
            recurMode = 1;
        }
    }

    /**
     *  ���һ�������Ƿ�ΪĿ¼
     */
    if (ppcArgV[iArgC - 1][0] == '.' || ppcArgV[iArgC - 1][0] == '/') {     // ָ��Ŀ¼
        lib_strcpy(cDirName, ppcArgV[iArgC - 1]);
        stDirLen = lib_strlen(cDirName);
        pdir = opendir(cDirName);
        if (stDirLen > 0) {
            if (cDirName[stDirLen - 1] != PX_DIVIDER) {
                cDirName[stDirLen++] = PX_DIVIDER;
            }
        }
        isDir = 1;
    }
    else {
        pdir = opendir(cDirName);   //��ǰĿ¼
    }

    if (!pdir) {
        if (errno == EACCES) {
            fprintf(stderr, "insufficient permissions.\n");
        } else {
            fprintf(stderr, "can not open dir! %s\n", lib_strerror(errno));
        }
    }
    else {

        pcBuffer = (PCHAR) __SHEAP_ALLOC((16 * LW_CFG_KB_SIZE)); /* ���ٻ������ڴ� */
        if (pcBuffer) {
            stBuffer = (16 * LW_CFG_KB_SIZE);
        } else {
            pcBuffer = cTemp;
            stBuffer = sizeof(cTemp);
        }

        if (recurMode) {
            PCHAR *dirList = (PCHAR*)__SHEAP_ALLOC(16 * MAX_FILENAME_LENGTH);
            INT dirCount = 0, dirNow = 0;
            do {
                pdirent = readdir(pdir);
                if (!pdirent) {
                    closedir(pdir);
                    if (dirNow < dirCount) {
                        lib_strcpy(cDirName, dirList[dirNow++]);
                        stDirLen = lib_strlen(cDirName);
                        cDirName[stDirLen++] = PX_DIVIDER;
                        cDirName[stDirLen] = '\0';
                        pdir = opendir(cDirName);
                    } else {
                        break;
                    }
                } else {
                    if (pdirent->d_type == DT_DIR) {
                        PCHAR newDir = (PCHAR)__SHEAP_ALLOC(MAX_FILENAME_LENGTH);
                        lib_strcpy(newDir, cDirName);
                        INT k;
                        for (k = 0; k < lib_strlen(pdirent->d_name); ++k) {
                            newDir[stDirLen + k] = pdirent->d_name[k];
                        }
                        newDir[stDirLen + k] = '\0';
                        dirList[dirCount] = (PCHAR)__SHEAP_ALLOC(MAX_FILENAME_LENGTH);
                        lib_strcpy(dirList[dirCount++], newDir);
                        continue;
                    }

                    if (!isDir && !wildCard(ppcArgV[iArgC - 1], pdirent->d_name)) {
                        continue;
                    }

                    CHAR path[MAX_FILENAME_LENGTH];
                    lib_strcpy(&path[0], cDirName);
                    lib_strcpy(&path[lib_strlen(cDirName)], pdirent->d_name);

                    iFd = open(path, O_RDONLY, 0);

                    if (iFd < 0) {
                        if (errno == EACCES) {
                            fprintf(stderr, "insufficient permissions.\n");
                        } else {
                            fprintf(stderr, "file error %s!\n",
                                    lib_strerror(errno));
                        }
                        return (-ERROR_TSHELL_EPARAM);
                    }

                    iError = fstat(iFd, &statFile);
                    if (iError < 0) {
                        fprintf(stderr, "file stat error!\n");
                        close(iFd);
                        return (-ERROR_TSHELL_EPARAM);
                    }

                    if (S_ISDIR(statFile.st_mode)) {
                        fprintf(stderr, "file read error: Is a directory!\n");
                        close(iFd);
                        return (-ERROR_TSHELL_EPARAM);
                    }

                    API_ThreadCleanupPush((VOIDFUNCPTR) close,
                            (PVOID) (LONG) iFd);

                    ssize_t sstNum = 0;
                    for (;;) {
                        sstRdNum = read(iFd, pcBuffer, stBuffer);
                        if (sstRdNum <= 0) {
                            break;
                        }
                        sstNum += sstRdNum;
                    }

                    PCHAR line = LW_NULL;
                    PCHAR ctr = pcBuffer;
                    size_t fileLen = 0;

                    for (;;) {
                        line = (PCHAR) __SHEAP_ALLOC(LW_CFG_KB_SIZE);
                        size_t strLen = 0;
                        PCHAR temp = ctr;
                        for (;; ++ctr) {
                            ++fileLen;
                            if (fileLen == sstNum || *ctr == '\n') {
                                ++ctr;
                                break;
                            }
                            ++strLen;
                        }

                        INT j;
                        for (j = 0; j < strLen; ++j) {
                            *(line + j) = *(temp + j);
                        }
                        *(line + strLen) = '\0';

                        if ((regexMode && regex(pattern, line)) || (!regexMode && patternMatch(pattern, line))) {
                            cur = (FNode *) __SHEAP_ALLOC(sizeof(FNode));
                            cur->fileName = (PCHAR) __SHEAP_ALLOC(
                                    sizeof(MAX_FILENAME_LENGTH));
                            lib_strcpy(cur->fileName, cDirName);
                            INT fNameLen = lib_strlen(pdirent->d_name);
                            INT k;
                            for (k = 0; k < fNameLen; ++k) {
                                cur->fileName[stDirLen + k] =
                                        pdirent->d_name[k];
                            }
                            cur->fileName[stDirLen + fNameLen] = '\0';
                            cur->line = line;
                            cur->next = NULL;
                            prev->next = cur;
                            prev = cur;
                        }

                        if (fileLen == sstNum) {
                            break;
                        }
                    }
                }
            } while (1);
        }
        else {

            do {
                pdirent = readdir(pdir);
                if (!pdirent) {
                    break;
                }
                else {
                    if (pdirent->d_type == DT_DIR) {
                        continue;
                    }
                    if (pdirent->d_type != DT_DIR) {
                        // ����Ŀ¼��һ���ǿɴ򿪿ɶ��ļ���

                        if (!isDir && !wildCard(ppcArgV[iArgC - 1], pdirent->d_name)) {
                            continue;
                        }

                        CHAR path[MAX_FILENAME_LENGTH];
                        lib_strcpy(&path[0], cDirName);
                        lib_strcpy(&path[lib_strlen(cDirName)], pdirent->d_name);

                        iFd = open(path, O_RDONLY, 0);

                        if (iFd < 0) {
                            if (errno == EACCES) {
                                fprintf(stderr, "insufficient permissions.\n");
                            } else {
                                fprintf(stderr, "file error %s!\n", lib_strerror(errno));
                            }
                            return (-ERROR_TSHELL_EPARAM);
                        }

                        iError = fstat(iFd, &statFile);
                        if (iError < 0) {
                            fprintf(stderr, "file stat error!\n");
                            close(iFd);
                            return (-ERROR_TSHELL_EPARAM);
                        }

                        if (S_ISDIR(statFile.st_mode)) {
                            fprintf(stderr, "file read error: Is a directory!\n");
                            close(iFd);
                            return (-ERROR_TSHELL_EPARAM);
                        }

                        API_ThreadCleanupPush((VOIDFUNCPTR)close, (PVOID)(LONG)iFd);

                        ssize_t sstNum = 0;
                        for (;;) {
                            sstRdNum = read(iFd, pcBuffer, stBuffer);
                            if (sstRdNum <= 0) {
                                break;
                            }
                            sstNum += sstRdNum;
                        }

                        PCHAR line = LW_NULL;
                        PCHAR ctr = pcBuffer;
                        size_t fileLen = 0;

                        for (;;) {
                            line = (PCHAR) __SHEAP_ALLOC(LW_CFG_KB_SIZE);
                            size_t strLen = 0;
                            PCHAR temp = ctr;
                            for (;; ++ctr) {
                                ++fileLen;
                                if (fileLen == sstNum || *ctr == '\n') {
                                    ++ctr;
                                    break;
                                }
                                ++strLen;
                            }

                            INT j;
                            for (j = 0; j < strLen; ++j) {
                                *(line + j) = *(temp + j);
                            }
                            *(line + strLen) = '\0';

                            if ((regexMode && regex(pattern, line)) || (!regexMode && patternMatch(pattern, line))) {
                                cur = (FNode *) __SHEAP_ALLOC(sizeof(FNode));
                                cur->fileName = (PCHAR)__SHEAP_ALLOC(sizeof(MAX_FILENAME_LENGTH));
                                lib_strcpy(cur->fileName, cDirName);
                                INT fNameLen = lib_strlen(pdirent->d_name);
                                INT k;
                                for (k = 0; k < fNameLen; ++k) {
                                    cur->fileName[stDirLen + k] = pdirent->d_name[k];
                                }
                                cur->fileName[stDirLen + fNameLen] = '\0';
                                cur->line = line;
                                cur->next = NULL;
                                prev->next = cur;
                                prev = cur;
                            }

                            if (fileLen == sstNum) {
                                break;
                            }
                        }
                    }
                }
            } while(1);

            closedir(pdir);

            if (stBuffer == (16 * LW_CFG_KB_SIZE)) {
                __SHEAP_FREE(pcBuffer);
            }
        }
    }

    if (head != prev) {
        for (cur = head->next; cur != NULL; cur = cur->next) {
            printf("%s\t %s\n", (*cur).fileName, (*cur).line);
        }
    }

    API_ThreadCleanupPop(LW_TRUE);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __checkIsTime
** ��������: ����ִ��Ƿ�Ϊʱ��
** �䡡��  : pcStr         �ַ���
**           time          ʱ��
** �䡡��  : 0  ��ʾת����ȷ
**           -1 ��ʾ����
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __checkIsTime (PCHAR  pcStr, time_t  *time)
{
    /*
     *  ����Ŀǰ������ʱ��
     */
    if (lib_strlen(pcStr) == 10) {
    }
    
    if (lib_strlen(pcStr) == 8) {
    }
    
    return  (-1);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdTouch
** ��������: ϵͳ���� "touch"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdTouch (INT  iArgC, PCHAR  ppcArgV[])
{
    REGISTER INT        iAflag = 0;
    REGISTER INT        iMflag = 0;
    REGISTER INT        iCflag = 0;
    
    REGISTER INT        i;
    REGISTER INT        iFileNameIndex = -1;
    
    REGISTER INT        iOc;
             time_t     timeNew;
             
    REGISTER INT        iFd;
    
    while ((iOc = getopt(iArgC, ppcArgV, "amc")) != -1) {
        switch (iOc) {
        case 'a':
            iAflag = 1;                                                 /*  ֻ�ı����ʱ��              */
            break;
        case 'm':
            iMflag = 1;                                                 /*  ֻ�ı��޸�ʱ��              */
            break;
        case 'c':
            iCflag = 1;                                                 /*  ���ļ������ڣ���������      */
            break;
        }
    }
    getopt_free();
    
    for (i = 1; i < iArgC; i++) {
        if (*ppcArgV[i] != '-') {
            if (__checkIsTime(ppcArgV[i], &timeNew)) {
                iFileNameIndex = i;
            }
        }
    }
    
    if (iFileNameIndex == -1) {
        fprintf(stderr, "file error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    (VOID)iAflag;                                                       /*  ��ʱ��ʹ��                  */
    (VOID)iMflag;
    
    if ((iCflag == 0) && (access(ppcArgV[iFileNameIndex], R_OK) < 0)) {
        iFd = open(ppcArgV[iFileNameIndex], O_WRONLY | O_CREAT | O_TRUNC, DEFAULT_FILE_PERM);
        if (iFd < 0) {
            if (errno == EACCES) {
                fprintf(stderr, "insufficient permissions.\n");
            } else {
                fprintf(stderr, "can not create file! error: %s\n", lib_strerror(errno));
            }
            return  (PX_ERROR);
        }
        close(iFd);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __fillWhite
** ��������: �������ɸ��ո�
** �䡡��  : stLen      �ո�ĳ���
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static VOID  __fillWhite (size_t  stLen)
{
    CHAR    cFmt[16];
    
    sprintf(cFmt, "%%-%zds", stLen);
    printf(cFmt, "");                                                   /*  ����ո�                    */
}
/*********************************************************************************************************
** ��������: __tshellFsCmdLs
** ��������: ϵͳ���� "ls"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdLs (INT  iArgC, PCHAR  ppcArgV[])
{
#define __TSHELL_BYTES_PERLINE          80                              /*  ���� 80 �ַ�                */
#define __TSHELL_BYTES_PERFILE          16                              /*  ÿ���ļ�����ʾ�񳤶�        */

             CHAR            cDirName[MAX_FILENAME_LENGTH] = ".";
             size_t          stDirLen = 1;
    
    REGISTER DIR            *pdir;
    REGISTER struct dirent  *pdirent;
             struct stat     statGet;
             struct winsize  winsz;
             
             INT             iError;
             
             size_t          stTotalLen = 0;
             size_t          stPrintLen;
             size_t          stPad;
             
    if ((iArgC > 1) && (lib_strcmp(ppcArgV[1], "-l") == 0)) {
        printf("you can use 'll' command.\n");
        return  (ERROR_NONE);
    }
    
    if ((iArgC == 2) && lib_strcmp(ppcArgV[1], ".")) {                  /*  ָ��Ŀ¼                    */
        lib_strcpy(cDirName, ppcArgV[1]);
        stDirLen = lib_strlen(cDirName);
        pdir     = opendir(cDirName);
        if (stDirLen > 0) {
            if (cDirName[stDirLen - 1] != PX_DIVIDER) {                 /*  ����Ŀ¼������ / ��β       */
                cDirName[stDirLen++] = PX_DIVIDER;                      /*  ���һ�� /                  */
            }
        }
    } else {
        pdir = opendir(cDirName);                                       /*  ��ǰĿ¼                    */
    }
    
    if (!pdir) {
        if (errno == EACCES) {
            fprintf(stderr, "insufficient permissions.\n");
        } else {
            fprintf(stderr, "can not open dir! %s\n", lib_strerror(errno));
        }
        return  (PX_ERROR);
    }
    
    if (ioctl(STD_OUT, TIOCGWINSZ, &winsz)) {                           /*  ��ô�����Ϣ                */
        winsz.ws_col = __TSHELL_BYTES_PERLINE;
    } else {
        winsz.ws_col = (unsigned short)ROUND_DOWN(winsz.ws_col, __TSHELL_BYTES_PERFILE);
    }
    
    do {
        pdirent = readdir(pdir);
        if (!pdirent) {
            break;
        
        } else {
            if ((pdirent->d_type == DT_UNKNOWN) ||                      /*  �޷���ȡ�ļ�����            */
                (pdirent->d_type == DT_REG)) {                          /*  REG �ļ���Ҫ��ȡ��ִ����Ϣ  */
                if ((stDirLen > 1) || 
                    ((stDirLen == 1) && (cDirName[0] == PX_ROOT))) {
                    lib_strcpy(&cDirName[stDirLen], pdirent->d_name);   /*  ����ָ��Ŀ¼                */
                    iError = stat(cDirName, &statGet);
                } else {
                    iError = stat(pdirent->d_name, &statGet);           /*  ʹ�õ�ǰĿ¼                */
                }
                if (iError < 0) {                                       /*  �豸�ļ�Ĭ��ʹ�����������  */
                    statGet.st_mode = S_IRUSR | S_IFREG;                /*  Ĭ������                    */
                }
            } else {
                statGet.st_mode = DTTOIF(pdirent->d_type);
            }
            
            API_TShellColorStart(pdirent->d_name, "", statGet.st_mode, STD_OUT);
            stPrintLen = printf("%-15s ", pdirent->d_name);             /*  ��ӡ�ļ���                  */
            if (stPrintLen > __TSHELL_BYTES_PERFILE) {
                stPad = ROUND_UP(stPrintLen, __TSHELL_BYTES_PERFILE)
                      - stPrintLen;                                     /*  �����������                */
                __fillWhite(stPad);
            } else {
                stPad = 0;
            }
            stTotalLen += stPrintLen + stPad;
            API_TShellColorEnd(STD_OUT);
            
            if (stTotalLen >= winsz.ws_col) {
                printf("\n");                                           /*  ����                        */
                stTotalLen = 0;
            }
        }
    } while (1);
    
    if (stTotalLen) {
        printf("\n");                                                   /*  ��������                    */
    }
    
    closedir(pdir);                                                     /*  �ر��ļ���                  */
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdCmp
** ��������: ϵͳ���� "cmp"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0:  �ļ���ͬ
**           1:  �ļ���ͬ
**           -1: ���ִ���
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdCmp (INT  iArgC, PCHAR  ppcArgV[])
{
             INT        iError = ERROR_NONE;
    REGISTER INT        iFdSrc;
    REGISTER INT        iFdDst = PX_ERROR;
    
    REGISTER ssize_t    sstRdSrcNum;
    REGISTER ssize_t    sstRdDstNum;
            
             CHAR       cTempSrc[128];
             CHAR       cTempDst[128];
             
             PCHAR      pcBufferSrc = LW_NULL;
             PCHAR      pcBufferDst = LW_NULL;
             
             size_t     stBuffer;
             
    struct   stat       statFile;
             off_t      oftSize;

    if (iArgC != 3) {
        fprintf(stderr, "parameter error!\n");
        return  (PX_ERROR);
    }
    if (lib_strcmp(ppcArgV[1], ppcArgV[2]) == 0) {                      /*  �ļ���ͬ                    */
        printf("file same!\n");
        return  (ERROR_NONE);
    }
    
    iFdSrc = open(ppcArgV[1], O_RDONLY);
    if (iFdSrc < 0) {
        fprintf(stderr, "can not open %s!\n", ppcArgV[1]);
        return  (PX_ERROR);
    }
    iFdDst = open(ppcArgV[2], O_RDONLY);
    if (iFdDst < 0) {
        close(iFdSrc);
        fprintf(stderr, "can not open %s!\n", ppcArgV[2]);
        return  (PX_ERROR);
    }
    
    iError = fstat(iFdSrc, &statFile);                                  /*  ���ȱȽ��ļ���С            */
    if (iError != ERROR_NONE) {
        fprintf(stderr, "%s get stat error!\n", ppcArgV[1]);
        goto    __error_handle;
    }
    oftSize = statFile.st_size;
    iError = fstat(iFdDst, &statFile);
    if (iError != ERROR_NONE) {
        fprintf(stderr, "%s get stat error!\n", ppcArgV[2]);
        goto    __error_handle;
    }
    if (oftSize != statFile.st_size) {                                  /*  �ļ���С��ͬ                */
        printf("file not same!\n");
        goto    __error_handle;
    }
    
    pcBufferSrc = (PCHAR)__SHEAP_ALLOC((16 * LW_CFG_KB_SIZE));          /*  ���ٱȽ��ڴ�                */
    pcBufferDst = (PCHAR)__SHEAP_ALLOC((16 * LW_CFG_KB_SIZE));
    if (!pcBufferSrc || !pcBufferDst) {
        if (pcBufferSrc) {
            __SHEAP_FREE(pcBufferSrc);                                  /*  ��������ͬʱ����ɹ�        */
        }
        if (pcBufferDst) {
            __SHEAP_FREE(pcBufferDst);
        }
        pcBufferSrc = cTempSrc;
        pcBufferDst = cTempDst;
        stBuffer    = sizeof(cTempSrc);
    } else {
        stBuffer    = (16 * LW_CFG_KB_SIZE);
    }
    
    for (;;) {                                                          /*  ��ʼ�Ƚ��ļ�                */
        sstRdSrcNum = read(iFdSrc, pcBufferSrc, stBuffer);
        sstRdDstNum = read(iFdDst, pcBufferDst, stBuffer);
        if (sstRdSrcNum != sstRdDstNum) {
            fprintf(stderr, "file read error!\n");
            goto    __error_handle;
        }
        if (sstRdSrcNum <= 0) {                                         /*  �ļ���ȡ���                */
            break;
        }
        if (lib_memcmp(pcBufferSrc, pcBufferDst, (UINT)sstRdSrcNum) != 0) {
            printf("file not same!\n");
            goto    __error_handle;
        }
    }
    close(iFdSrc);
    close(iFdDst);                                                      /*  �ر��ļ�                    */
    if (stBuffer == (16 * LW_CFG_KB_SIZE)) {
        __SHEAP_FREE(pcBufferSrc);
        __SHEAP_FREE(pcBufferDst);                                      /*  �ͷŻ�����                  */
    }
    printf("file same!\n");
    
    return  (ERROR_NONE);
    
__error_handle:
    close(iFdSrc);
    close(iFdDst);
    
    if (pcBufferSrc && (pcBufferSrc != cTempSrc)) {
        __SHEAP_FREE(pcBufferSrc);
    }
    if (pcBufferDst && (pcBufferDst != cTempDst)) {
        __SHEAP_FREE(pcBufferDst);
    }
    
    return  (PX_ERROR);
}
/*********************************************************************************************************
** ��������: __buildDstFileName
** ��������: ����Ŀ���ļ���
** �䡡��  : pcSrc         Դ�ļ�
**           pcDstDir      Ŀ��Ŀ¼
**           pcBuffer      �������
** �䡡��  : NONE
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static VOID __buildDstFileName (CPCHAR  pcSrc, CPCHAR  pcDstDir, PCHAR  pcBuffer)
{
    size_t  stDstDirLen   = lib_strlen(pcDstDir);
    PCHAR   pcSrcFileName = lib_rindex(pcSrc, PX_DIVIDER);

    if (pcSrcFileName == LW_NULL) {
        pcSrcFileName =  (PCHAR)pcSrc;
    } else {
        pcSrcFileName++;
    }

    lib_strlcpy(pcBuffer, pcDstDir, MAX_FILENAME_LENGTH - 1);

    if (pcBuffer[stDstDirLen - 1] != PX_DIVIDER) {
        pcBuffer[stDstDirLen]      = PX_DIVIDER;
        pcBuffer[stDstDirLen + 1]  = PX_EOS;
    }

    lib_strlcat(pcBuffer, pcSrcFileName, MAX_FILENAME_LENGTH);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdCp
** ��������: ϵͳ���� "cp"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdCp (INT  iArgC, PCHAR  ppcArgV[])
{
#define __LW_CP_BUF_SZ  (128 * LW_CFG_KB_SIZE)

             INT        iError = PX_ERROR;
    REGISTER INT        iFdSrc;
    REGISTER INT        iFdDst = PX_ERROR;
    
             CHAR       cDstFile[MAX_FILENAME_LENGTH] = "\0";
             PCHAR      pcDest;
             PCHAR      pcSrc;
             BOOL       bForce = LW_FALSE;

    REGISTER ssize_t    sstRdNum;
    REGISTER ssize_t    sstWrNum;
            
             CHAR       cTemp[128];
             PCHAR      pcBuffer = cTemp;
             size_t     stBuffer = sizeof(cTemp);
             size_t     stOptim;
             
    struct   stat       statFile;
    struct   stat       statDst;
    
             time_t     timeStart;
             time_t     timeEnd;
             time_t     timeDiff;

    if (iArgC == 3) {
        pcDest = ppcArgV[2];
        pcSrc  = ppcArgV[1];
    
    } else if (iArgC == 4) {
        if (ppcArgV[1][0] != '-') {
            fprintf(stderr, "option error!\n");
            return  (PX_ERROR);
        }
        if (lib_strchr(ppcArgV[1], 'f')) {                              /*  ǿ�и���                    */
            bForce = LW_TRUE;
        }
        pcDest = ppcArgV[3];
        pcSrc  = ppcArgV[2];
        
    } else {
        fprintf(stderr, "parameter error!\n");
        return  (PX_ERROR);
    }
    
    if (lib_strcmp(pcSrc, pcDest) == 0) {                               /*  �ļ��ظ�                    */
        fprintf(stderr, "parameter error!\n");
        return  (PX_ERROR);
    }
    
    iFdSrc = open(pcSrc, O_RDONLY, 0);                                  /*  ��Դ�ļ�                  */
    if (iFdSrc < 0) {
        fprintf(stderr, "can not open source file!\n");
        return  (PX_ERROR);
    }
    
    iError = fstat(iFdSrc, &statFile);                                  /*  ���Դ�ļ�����              */
    if (iError < 0) {
        goto    __error_handle;
    }
    if (S_ISDIR(statFile.st_mode)) {                                    /*  ���ܸ���Ŀ¼�ļ�            */
        fprintf(stderr, "can not copy directory!\n");
        errno  = EISDIR;
        iError = PX_ERROR;
        goto    __error_handle;
    }
    
    iError = stat(pcDest, &statDst);
    if (iError == ERROR_NONE) {
        if (S_ISDIR(statDst.st_mode)) {
            __buildDstFileName(pcSrc, pcDest, cDstFile);                /*  ����Ŀ���ļ�·��            */
        }
    }
    if (cDstFile[0] == PX_EOS) {
        lib_strlcpy(cDstFile, pcDest, MAX_FILENAME_LENGTH);
    }

    iError = stat(cDstFile, &statDst);                                  /*  ���Ŀ���ļ�����            */
    if (iError == ERROR_NONE) {                                         /*  Ŀ���ļ�����                */
        if ((statDst.st_dev == statFile.st_dev) &&
            (statDst.st_ino == statFile.st_ino)) {                      /*  Դ�ļ���Ŀ���ļ���ͬ        */
            close(iFdSrc);
            fprintf(stderr, "'%s' and '%s' are the same file!\n", pcSrc, cDstFile);
            return  (PX_ERROR);
        }
    }

    if (!bForce) {
        iError = access(cDstFile, 0);                                   /*  ���Ŀ���ļ��Ƿ����        */
        if (iError == ERROR_NONE) {
__re_select:
            printf("destination file is exist, overwrite? (Y/N)\n");
            read(0, cTemp, 128);
            if ((cTemp[0] == 'N') || (cTemp[0] == 'n')) {               /*  ������                      */
                iError = PX_ERROR;
                goto    __error_handle;
            
            } else if ((cTemp[0] != 'Y') && (cTemp[0] != 'y')) {        /*  ѡ�����                    */
                goto    __re_select;
            
            } else {                                                    /*  ѡ�񸲸�                    */
                unlink(cDstFile);                                       /*  ɾ��Ŀ���ļ�                */
            }
        }
    } else {
        iError = access(cDstFile, 0);                                   /*  ���Ŀ���ļ��Ƿ����        */
        if (iError == ERROR_NONE) {
            unlink(cDstFile);                                           /*  ɾ��Ŀ���ļ�                */
        }
    }
                                                                        /*  ����Ŀ���ļ�                */
    iFdDst = open(cDstFile, (O_WRONLY | O_CREAT | O_TRUNC), DEFFILEMODE);
    if (iFdDst < 0) {
        close(iFdSrc);
        fprintf(stderr, "can not open destination file!\n");
        return  (PX_ERROR);
    }
    
    stOptim = (UINT)__MIN(__LW_CP_BUF_SZ, statFile.st_size);            /*  ���㻺����                  */
    if (stOptim > 128) {
        pcBuffer = (PCHAR)__SHEAP_ALLOC(stOptim);                       /*  ���仺����                  */
        if (pcBuffer == LW_NULL) {
            pcBuffer =  cTemp;                                          /*  ʹ�þֲ���������            */
        
        } else {
            stBuffer =  stOptim;
        }
    }
    
    lib_time(&timeStart);                                               /*  ��¼��ʼʱ��                */
    
    for (;;) {                                                          /*  ��ʼ�����ļ�                */
        sstRdNum = read(iFdSrc, pcBuffer, stBuffer);
        if (sstRdNum > 0) {
            sstWrNum = write(iFdDst, pcBuffer, (size_t)sstRdNum);
            if (sstWrNum != sstRdNum) {                                 /*  д���ļ�����                */
                fprintf(stderr, "can not write destination file! error: %s\n", lib_strerror(errno));
                iError = PX_ERROR;
                break;
            }
        } else if (sstRdNum == 0) {                                     /*  �������                    */
            iError = ERROR_NONE;
            break;
        
        } else {
            iError = PX_ERROR;                                          /*  ��ȡ���ݴ���                */
            break;
        }
    }
    
    if (iError == ERROR_NONE) {
        lib_time(&timeEnd);                                             /*  ��¼����ʱ��                */
        timeDiff = timeEnd - timeStart;
        
        printf("copy complete. size:%lld(Bytes) time:%lld(s) speed:%lld(Bps)\n", 
               (INT64)statFile.st_size,
               (INT64)timeDiff,
               (INT64)(statFile.st_size / (timeDiff ? timeDiff : 1)));
    }
        
    if (pcBuffer != cTemp) {
        __SHEAP_FREE(pcBuffer);                                         /*  �ͷŻ���                    */
    }
    
__error_handle:
    close(iFdSrc);
    if (iFdDst >= 0) {
        fchmod(iFdDst, statFile.st_mode);                               /*  ����Ϊ��Դ�ļ���ͬ�� mode   */
        close(iFdDst);
    }
    
    if (iError == ERROR_NONE) {                                         /*  �������                    */
        struct utimbuf  utimbDst;
        
        utimbDst.actime  = statFile.st_atime;                           /*  �޸ĸ��ƺ��ļ���ʱ����Ϣ    */
        utimbDst.modtime = statFile.st_mtime;
        
        utime(cDstFile, &utimbDst);                                     /*  �����ļ�ʱ��                */
    }
    
    return  (iError);
}
/*********************************************************************************************************
** ��������: __tshellFsShowMode
** ��������: ��ʾ�ļ� mode ѡ��
** �䡡��  : pstat     �ļ�ѡ��
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static VOID __tshellFsShowMode (struct stat  *pstat)
{
    CHAR    cBuffer[11];
    
    if (S_ISLNK(pstat->st_mode)) {
        cBuffer[0] = 'l';
    
    } else if (S_ISDIR(pstat->st_mode)) {
        cBuffer[0] = 'd';
    
    } else if (S_ISCHR(pstat->st_mode)) {
        cBuffer[0] = 'c';
    
    } else if (S_ISBLK(pstat->st_mode)) {
        cBuffer[0] = 'b';
    
    } else if (S_ISSOCK(pstat->st_mode)) {
        cBuffer[0] = 's';
    
    } else if (S_ISFIFO(pstat->st_mode)) {
        cBuffer[0] = 'f';
    
    } else {
        cBuffer[0] = '-';
    }
    
    if (pstat->st_mode & S_IRUSR) {
        cBuffer[1] = 'r';
    } else {
        cBuffer[1] = '-';
    }
    
    if (pstat->st_mode & S_IWUSR) {
        cBuffer[2] = 'w';
    } else {
        cBuffer[2] = '-';
    }
    
    if (pstat->st_mode & S_IXUSR) {
        if (pstat->st_mode & S_ISUID) {
            cBuffer[3] = 's';
        } else {
            cBuffer[3] = 'x';
        }
    } else {
        if (pstat->st_mode & S_ISUID) {
            cBuffer[3] = 'S';
        } else {
            cBuffer[3] = '-';
        }
    }
    
    if (pstat->st_mode & S_IRGRP) {
        cBuffer[4] = 'r';
    } else {
        cBuffer[4] = '-';
    }
    
    if (pstat->st_mode & S_IWGRP) {
        cBuffer[5] = 'w';
    } else {
        cBuffer[5] = '-';
    }
    
    if (pstat->st_mode & S_IXGRP) {
        if (pstat->st_mode & S_ISGID) {
            cBuffer[6] = 's';
        } else {
            cBuffer[6] = 'x';
        }
    } else {
        if (pstat->st_mode & S_ISGID) {
            cBuffer[6] = 'S';
        } else {
            cBuffer[6] = '-';
        }
    }
    
    if (pstat->st_mode & S_IROTH) {
        cBuffer[7] = 'r';
    } else {
        cBuffer[7] = '-';
    }
    
    if (pstat->st_mode & S_IWOTH) {
        cBuffer[8] = 'w';
    } else {
        cBuffer[8] = '-';
    }
    
    if (pstat->st_mode & S_IXOTH) {
        cBuffer[9] = 'x';
    } else {
        cBuffer[9] = '-';
    }
    cBuffer[10] = PX_EOS;
    
    printf(cBuffer);
}
/*********************************************************************************************************
** ��������: __tshellFsShowFile
** ��������: ��ʾ�ļ���ϸ��Ϣ
** �䡡��  : pcFileName �ļ���
**           pcStat     �ļ���
**           pstat      �ļ�ѡ��
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static VOID __tshellFsShowFile (CPCHAR  pcFileName, PCHAR  pcStat, struct stat  *pstat)
{
    PCHAR           pcN;
    struct tm       tmBuf;
    CHAR            cTimeBuf[32];                                       /*  bigger than sizeof(ASCBUF)  */
    CHAR            cBuffer[MAX_FILENAME_LENGTH];
    
    __tshellFsShowMode(pstat);                                          /*  ��ʾ�ļ�����                */
            
    if (API_TShellGetUserName(pstat->st_uid,
                              cBuffer, sizeof(cBuffer))) {              /*  ����ļ������û���          */
        printf(" %-8d", pstat->st_uid);
    } else {
        printf(" %-8s", cBuffer);
    }
    
    if (API_TShellGetGrpName(pstat->st_gid,
                             cBuffer, sizeof(cBuffer))) {               /*  ����ļ������û���          */
        printf(" %-8d", pstat->st_gid);
    } else {
        printf(" %-8s", cBuffer);
    }
    
    lib_localtime_r(&pstat->st_mtime, &tmBuf);                          /*  ת��Ϊ tm ��ʽ              */
    lib_asctime_r(&tmBuf, cTimeBuf);                                    /*  �����ַ���                  */
    pcN = lib_index(cTimeBuf, '\n');
    if (pcN) {
        *pcN = PX_EOS;
    }
    
    printf(" %s", cTimeBuf);                                            /*  ��ӡ�޸�ʱ��                */
    
    if (S_ISDIR(pstat->st_mode)) {
        API_TShellColorStart(pcFileName, "", pstat->st_mode, STD_OUT);
        printf("           %s/\n", pcFileName);
        API_TShellColorEnd(STD_OUT);
                              
    } else if (S_ISLNK(pstat->st_mode)) {                               /*  �����ļ�                    */
        CHAR            cDstName[MAX_FILENAME_LENGTH] = "<unknown>";
        struct stat     statDst;
        ssize_t         sstLen;
        
        statDst.st_mode = 0;
        sstLen = readlink(pcStat, cDstName, MAX_FILENAME_LENGTH);
        if (sstLen >= 0) {
            cDstName[sstLen] = PX_EOS;                                  /*  ���������                  */
        }
        stat(cDstName, &statDst);
        API_TShellColorStart(pcFileName, cDstName, pstat->st_mode, STD_OUT);
        printf("           %s -> ", pcFileName);
        API_TShellColorStart(cDstName, "", statDst.st_mode, STD_OUT);
        printf("%s\n", cDstName);
        API_TShellColorEnd(STD_OUT);
    
    } else {
        if (pstat->st_size > (10 * (UINT64)LW_CFG_GB_SIZE)) {
            printf(" %6zdGB, ", (size_t)(pstat->st_size / LW_CFG_GB_SIZE));
        } else if (pstat->st_size > (10 * LW_CFG_MB_SIZE)) {
            printf(" %6zdMB, ", (size_t)(pstat->st_size / LW_CFG_MB_SIZE));
        } else if (pstat->st_size > (10 * LW_CFG_KB_SIZE)) {
            printf(" %6zdKB, ", (size_t)(pstat->st_size / LW_CFG_KB_SIZE));
        } else {
            printf(" %6zd B, ", (size_t)(pstat->st_size));
        }
        
        API_TShellColorStart(pcFileName, "", pstat->st_mode, STD_OUT);
        printf("%s\n", pcFileName);
        API_TShellColorEnd(STD_OUT);
    }
}
/*********************************************************************************************************
** ��������: __tshellFsCmdLl
** ��������: ϵͳ���� "ll"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdLl (INT  iArgC, PCHAR  ppcArgV[])
{
             PCHAR           pcStat;
             CHAR            cDirName[MAX_FILENAME_LENGTH] = ".";
             size_t          stDirLen = 1;
             
    REGISTER DIR            *pdir;
    REGISTER struct dirent  *pdirent;
             struct stat     statGet;
             
             INT             iError;
             INT             iItemNum = 0;
    
    if ((iArgC == 2) && lib_strcmp(ppcArgV[1], ".")) {                  /*  ָ��Ŀ¼                    */
        lib_strcpy(cDirName, ppcArgV[1]);
        if (stat(cDirName, &statGet) == ERROR_NONE) {
            if (!S_ISDIR(statGet.st_mode)) {                            /*  ����Ŀ¼                    */
                PCHAR   pcFile;
                _PathLastName(cDirName, &pcFile);
                __tshellFsShowFile(pcFile, pcFile, &statGet);
                iItemNum = 1;
                goto    __display_over;
            }
        }
        
        stDirLen = lib_strlen(cDirName);
        pdir     = opendir(cDirName);
        if (stDirLen > 0) {
            if (cDirName[stDirLen - 1] != PX_DIVIDER) {                 /*  ����Ŀ¼������ / ��β       */
                cDirName[stDirLen++] = PX_DIVIDER;                      /*  ���һ�� /                  */
            }
        }
    } else {
        pdir = opendir(cDirName);                                       /*  ��ǰĿ¼                    */
    }
    
    if (!pdir) {
        if (errno == EACCES) {
            fprintf(stderr, "insufficient permissions.\n");
        } else {
            fprintf(stderr, "can not open dir %s!\n", lib_strerror(errno));
        }
        return  (PX_ERROR);
    }
    
    do {
        pdirent = readdir(pdir);
        if (!pdirent) {
            break;
        
        } else {
            if ((stDirLen > 1) || 
                ((stDirLen == 1) && (cDirName[0] == PX_ROOT))) {
                lib_strcpy(&cDirName[stDirLen], pdirent->d_name);       /*  ����ָ��Ŀ¼                */
                pcStat = cDirName;
                iError = lstat(cDirName, &statGet);
            
            } else {
                pcStat = pdirent->d_name;
                iError = lstat(pdirent->d_name, &statGet);              /*  ʹ�õ�ǰĿ¼                */
            }
            if (iError < 0) {                                           /*  �豸�ļ�Ĭ��ʹ�����������  */
                statGet.st_dev     = 0;
                statGet.st_ino     = 0;
                statGet.st_mode    = 0666 | S_IFCHR;                    /*  Ĭ������                    */
                statGet.st_nlink   = 0;
                statGet.st_uid     = 0;
                statGet.st_gid     = 0;
                statGet.st_rdev    = 1;
                statGet.st_size    = 0;
                statGet.st_blksize = 0;
                statGet.st_blocks  = 0;
                statGet.st_atime   = API_RootFsTime(LW_NULL);           /*  Ĭ��ʹ�� root fs ��׼ʱ��   */
                statGet.st_mtime   = API_RootFsTime(LW_NULL);
                statGet.st_ctime   = API_RootFsTime(LW_NULL);
            }
            
            __tshellFsShowFile(pdirent->d_name, pcStat, &statGet);
            
            iItemNum++;
        }
    } while (1);
    
    closedir(pdir);
    
__display_over:
    printf("      total items: %d\n", iItemNum);
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __getDsize
** ��������: �ݹ��ȡ�ļ��е��ļ������ʹ�С
** �䡡��  : pcDirName     Ŀ¼��
**           stDirLen      ��ǰĿ¼������
**           pulFileCnt    �ļ�����
**           poftSize      �ܴ�С
** �䡡��  : NONE
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static VOID  __getDsize (PCHAR  pcDirName, size_t  stDirLen, ULONG  *pulFileCnt, off_t  *poftSize)
{
    DIR            *pdir;
    struct dirent  *pdirent;
    struct stat     statGet;
    
    pdir = opendir(pcDirName);
    if (pdir) {
        do {
            pdirent = readdir(pdir);
            if (pdirent) {
                bnprintf(pcDirName, MAX_FILENAME_LENGTH, stDirLen, "/%s", pdirent->d_name);
                if (lstat(pcDirName, &statGet) == ERROR_NONE) {
                    if (S_ISDIR(statGet.st_mode)) {
                        __getDsize(pcDirName, lib_strlen(pcDirName), pulFileCnt, poftSize);
                    
                    } else {
                        (*pulFileCnt) += 1;
                        (*poftSize)   += statGet.st_size;
                    }
                }
                pcDirName[stDirLen] = PX_EOS;
            }
        } while (pdirent);
        closedir(pdir);
    }
}
/*********************************************************************************************************
** ��������: __tshellFsCmdDsize
** ��������: ϵͳ���� "dsize"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : ERROR
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdDsize (INT  iArgC, PCHAR  ppcArgV[])
{
    ULONG       ulFiles = 0ul;
    off_t       oftSize = 0;
    struct stat statGet;
    CHAR        cDirName[MAX_FILENAME_LENGTH];
    
    if (iArgC < 2) {
        fprintf(stderr, "df arguments error, (dsize directory)\n");
        return  (PX_ERROR);
    }
    
    if (stat(ppcArgV[1], &statGet)) {
        fprintf(stderr, "can not get %s stat().\n", ppcArgV[1]);
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (S_ISDIR(statGet.st_mode)) {
        lib_strlcpy(cDirName, ppcArgV[1], MAX_FILENAME_LENGTH);
        printf("scanning...\n");
        __getDsize(cDirName, lib_strlen(cDirName), &ulFiles, &oftSize);
        printf("total file %lu size %llu bytes.\n", ulFiles, oftSize);
        
    } else {
        printf("total file 1 size %llu bytes.\n", statGet.st_size);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdDf
** ��������: ϵͳ���� "df"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdDf (INT  iArgC, PCHAR  ppcArgV[])
{
    static PCHAR   pcVolumeInfoHdr = \
                    "    VOLUME       TOTAL     FREE    USED RO            FS TYPE\n"
                    "-------------- --------- --------- ---- -- ---------------------------------\n";

    struct statfs       statfsGet;
           
           /*
            *  ������Ϣ
            */
           UINT64       ullFree;
           UINT64       ullTotal;
           
           /*
            *  ��ʾ�ñ���
            */
           ULONG        ulTotalDisp;
           ULONG        ulTotalPoint;
           ULONG        ulFreeDisp;
           ULONG        ulFreePoint;
           
           /*
            *  ��ʾ��λ�ı�ֵ
            */
           ULONG        ulTotalDiv;
           ULONG        ulFreeDiv;
           
           /*
            *  ��ʾ��λ
            */
           PCHAR        pcTotalUnit = "";
           PCHAR        pcFreeUnit  = "";
           
           /*
            *  �ļ�ϵͳ����
            */
           PCHAR        pcFsType = "unknown";
           
           /*
            *  ʹ�ðٷֱ�
            */
           INT          iUseagePercent;
           
           /*
            *  �Ƿ�Ϊֻ��
            */
           PCHAR        pcRo = "n";

    if (iArgC < 2) {
#if LW_CFG_OEMDISK_EN > 0
        API_OemDiskMountShow();
        printf("\n");
#endif                                                                  /*  LW_CFG_OEMDISK_EN > 0       */

#if (LW_CFG_MAX_VOLUMES > 0) && (LW_CFG_YAFFS_EN > 0)
        API_YaffsDevMountShow();
        printf("\n");
#endif                                                                  /*  (LW_CFG_MAX_VOLUMES > 0)    */
                                                                        /*  (LW_CFG_YAFFS_EN > 0)       */
#if (LW_CFG_BLKRAW_EN > 0) && (LW_CFG_MAX_VOLUMES > 0) && (LW_CFG_MOUNT_EN > 0)
        API_MountShow();
#endif                                                                  /*  LW_CFG_MOUNT_EN             */
        return  (ERROR_NONE);
    }
    
    if (statfs(ppcArgV[1], &statfsGet) < 0) {
        if (errno == EACCES) {
            fprintf(stderr, "insufficient permissions.\n");
        } else {
            perror("volume state error");
        }
        return  (PX_ERROR);
    }
    
    ullFree  = ((UINT64)statfsGet.f_bfree  * statfsGet.f_bsize);
    ullTotal = ((UINT64)statfsGet.f_blocks * statfsGet.f_bsize);
    
    if (ullTotal > (2ul * LW_CFG_GB_SIZE)) {
        ulTotalDiv  = LW_CFG_GB_SIZE;
        pcTotalUnit = "GB";
    } else if (ullTotal > (2ul * LW_CFG_MB_SIZE)) {
        ulTotalDiv  = LW_CFG_MB_SIZE;
        pcTotalUnit = "MB";
    } else {
        ulTotalDiv  = LW_CFG_KB_SIZE;
        pcTotalUnit = "KB";
    }
    
    if (ullFree > (2ul * LW_CFG_GB_SIZE)) {
        ulFreeDiv  = LW_CFG_GB_SIZE;
        pcFreeUnit = "GB";
    } else if (ullFree > (2ul * LW_CFG_MB_SIZE)) {
        ulFreeDiv  = LW_CFG_MB_SIZE;
        pcFreeUnit = "MB";
    } else {
        ulFreeDiv  = LW_CFG_KB_SIZE;
        pcFreeUnit = "KB";
    }
    
    ulTotalDisp  = (ULONG)(ullTotal / ulTotalDiv);
    if (ullTotal % ulTotalDiv) {
        ulTotalPoint = (ULONG)(ullTotal / (UINT64)(ulTotalDiv / 100));
        ulTotalPoint = (ulTotalPoint % 100);
    } else {
        ulTotalPoint = 0;
    }
    
    ulFreeDisp   = (ULONG)(ullFree / ulFreeDiv);
    if (ullFree % ulFreeDiv) {
        ulFreePoint = (ULONG)(ullFree / (UINT64)(ulFreeDiv / 100));
        ulFreePoint = (ulFreePoint % 100);
    } else {
        ulFreePoint = 0;
    }
    
    {
        INT  iFd = open(ppcArgV[1], O_RDONLY, 0);
        if (iFd >= 0) {
            ioctl(iFd, FIOFSTYPE, (LONG)&pcFsType);
            close(iFd);
        }
    }
    
    printf(pcVolumeInfoHdr);
    
    if (statfsGet.f_flag & ST_RDONLY) {
        pcRo = "y";
    }
    
    if (ullFree > ullTotal) {
        printf("%-14s %4lu.%02lu%-2s   unknown ---%% %-2s %s\n", ppcArgV[1], 
               ulTotalDisp, ulTotalPoint, 
               pcTotalUnit, pcRo, pcFsType);

    } else if (ullTotal == 0) {
        printf("%-14s %4lu.%02lu%-2s %4lu.%02lu%-2s %3d%% %-2s %s\n", ppcArgV[1],
               0ul, 0ul, "KB", 0ul, 0ul, "KB",
               100, pcRo, pcFsType);
    
    } else {
        UINT64      ullUseBlocks = (UINT64)(statfsGet.f_blocks - statfsGet.f_bfree);
    
        ullUseBlocks *= 100;
        iUseagePercent = (INT)(ullUseBlocks / statfsGet.f_blocks);
        printf("%-14s %4lu.%02lu%-2s %4lu.%02lu%-2s %3d%% %-2s %s\n", ppcArgV[1], 
               ulTotalDisp, ulTotalPoint, pcTotalUnit, 
               ulFreeDisp,  ulFreePoint,  pcFreeUnit, 
               iUseagePercent, pcRo, pcFsType);
    }
           
    return  (0);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdChmod
** ��������: ϵͳ���� "chmod"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdChmod (INT  iArgC, PCHAR  ppcArgV[])
{
    REGISTER size_t     stLen;
             INT        i;
             INT        iNewMode = DEFAULT_FILE_PERM;

    if (iArgC != 3) {
        fprintf(stderr, "chmod arguments error, (chmod newmode filename)\n");
        return  (PX_ERROR);
    }
    
    stLen = lib_strnlen(ppcArgV[1], 5);
    if (stLen > 4) {
        fprintf(stderr, "newmode error, (eg. 777 ... octal)\n");
        return  (PX_ERROR);
    }
    
    for (i = 0; i < stLen; i++) {                                       /*  ������ 8���Ƶ�              */
        if ((ppcArgV[1][i] > '7') || (ppcArgV[1][i] < '0')) {
            fprintf(stderr, "newmode error, (eg. 777 ... octal)\n");
            return  (PX_ERROR);
        }
    }
    
    sscanf(ppcArgV[1], "%o", &iNewMode);
    
    if (lib_strchr(ppcArgV[2], '*') ||
        lib_strchr(ppcArgV[2], '?')) {                                  /*  ���� shell ͨ���           */

#if LW_CFG_POSIX_EN > 0
        DIR             *pdir;
        struct dirent   *pdirent;
        CHAR             cName[MAX_FILENAME_LENGTH];
        size_t           stDirLen;
        PCHAR            pcTail;
        INT              iRet;
        
        pcTail = lib_strrchr(ppcArgV[2], PX_DIVIDER);
        if (pcTail) {
            stDirLen = pcTail - ppcArgV[2];
            lib_memcpy(cName, ppcArgV[2], stDirLen);
            cName[stDirLen] = PX_EOS;
            pcTail++;                                                   /*  ָ���ļ�������              */
        
        } else {
            stDirLen = 1;
            lib_strcpy(cName, ".");                                     /*  ��ǰĿ¼                    */
            pcTail = ppcArgV[2];                                        /*  ָ���ļ�������              */
        }
        
        pdir = opendir(cName);
        if (!pdir) {
            fprintf(stderr, "can not open dir %s error: %s\n", cName, lib_strerror(errno));
            return  (PX_ERROR);
        }
        
        pdirent = readdir(pdir);
        while (pdirent) {
            iRet = fnmatch(pcTail, pdirent->d_name, FNM_PATHNAME);
            if (iRet == ERROR_NONE) {
                bnprintf(cName, MAX_FILENAME_LENGTH, stDirLen, "/%s", pdirent->d_name);
                i = chmod(cName, iNewMode);                             /*  ת���ļ�ģʽ                */
                if (i < 0) {
                    if (errno == EACCES) {
                        fprintf(stderr, "%s insufficient permissions.\n", 
                                pdirent->d_name);
                    } else {
                        fprintf(stderr, "%s can not change file mode, error: %s\n", 
                                pdirent->d_name, lib_strerror(errno));
                    }
                }
            }
            pdirent = readdir(pdir);
        }
        closedir(pdir);
#else
        printf("sylixos do not have fnmatch().\n");
        return  (PX_ERROR);
#endif
    } else {
        i = chmod(ppcArgV[2], iNewMode);                                /*  ת���ļ�ģʽ                */
        if (i < 0) {
            if (errno == EACCES) {
                fprintf(stderr, "insufficient permissions.\n");
            } else {
                perror("can not change file mode error");
            }
            return  (PX_ERROR);
        }
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdMkfs
** ��������: ϵͳ���� "mkfs"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdMkfs (INT  iArgC, PCHAR  ppcArgV[])
{
    REGISTER INT    iError;
    
    if (iArgC != 2) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    printf("now format media, please wait...\n");
    iError = mkfs(ppcArgV[1]);
    if (iError) {
        if (errno == EACCES) {
            fprintf(stderr, "insufficient permissions.\n");
        } else {
            fprintf(stderr, "can not format media : error: %s\n", lib_strerror(errno));
        }
        return  (-errno);
    }
    printf("disk format ok.\n");
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdShfile
** ��������: ϵͳ���� "shfile"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdShfile (INT  iArgC, PCHAR  ppcArgV[])
{
    FILE  *fileShell;
    PCHAR  pcCmd       = LW_NULL;
    PCHAR  pcCmdBuffer = LW_NULL;                                       /*  �����                    */
    
    if (iArgC != 2) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    fileShell = fopen(ppcArgV[1], "r");                                 /*  �� shell �ļ�             */
    if (fileShell == LW_NULL) {
        if (errno == EACCES) {
            fprintf(stderr, "insufficient permissions.\n");
        } else {
            fprintf(stderr, "can not open %s: %s\n", ppcArgV[1], lib_strerror(errno));
        }
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    pcCmdBuffer = (PCHAR)__SHEAP_ALLOC(LW_CFG_SHELL_MAX_COMMANDLEN + 1);/*  ���������                */
    if (pcCmdBuffer == LW_NULL) {
        fclose(fileShell);
        fprintf(stderr, "system low memory.\n");
        return  (PX_ERROR);
    }
    
    do {
        pcCmd = fgets(pcCmdBuffer, LW_CFG_SHELL_MAX_COMMANDLEN, 
                      fileShell);                                       /*  ���һ��ָ��                */
        if (pcCmd) {
            API_TShellExec(pcCmd);                                      /*  ִ��ָ������                */
        }
    } while (pcCmd);
    
    __SHEAP_FREE(pcCmdBuffer);                                          /*  �ͷŻ���                    */
    fclose(fileShell);
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: nameMatch
** ��������: �ж��ļ����Ƿ��������Ҫ��
** �䡡��  : require       �ļ���Ҫ��
**           fileName      �ļ���
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
INT nameMatch(PCHAR require, PCHAR fileName) {
    INT m = lib_strlen(fileName);
    INT n = lib_strlen(require);
    INT dp[m + 1][n + 1];
    INT i, j;
    for (i = 0; i <= m; i++) {
        for (j = 0; j <= n; j++) {
            dp[i][j] = 0;
        }
    }
    dp[0][0] = 1;
    for (i = 1; i <= n; i++) {
        if (require[i - 1] == '*') {
            dp[0][i] = 1;
        } else {
            break;
        }
    }
    for (i = 1; i <= m; i++) {
        for (j = 1; j <= n; j++) {
            if (require[j - 1] == '*') {
                dp[i][j] = dp[i][j - 1] | dp[i - 1][j];
            } else if (require[j - 1] == '?'
                    || fileName[i - 1] == require[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
        }
    }
    return dp[m][n];
}
/*********************************************************************************************************
** ��������: typeMatch
** ��������: �ж��ļ������Ƿ��������Ҫ��
** �䡡��  : require       �ļ�����Ҫ��
**           fileName      �ļ�����
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
INT typeMatch(unsigned char require, unsigned char type) {
    if (require == 16 || require == type) {
        return 1;
    } else {
        return 0;
    }
}
/*********************************************************************************************************
** ��������: linksMatch
** ��������: �ж��ļ�Ӳ���������Ƿ��������Ҫ��
** �䡡��  : require1      �ļ�Ӳ����Ҫ��1
**           require2      �ļ�Ӳ����Ҫ��2
**           linksNum      �ļ�Ӳ��������
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
INT linksMatch(INT require1, INT require2, INT linksCount) {
    if (require1 == 0)
        return 1;
    else if (require1 == 1 && linksCount > require2)
        return 1;
    else if (require1 == 2 && linksCount < require2)
        return 1;
    else if (require1 == 3 && linksCount == require2)
        return 1;
    else
        return 0;
}
/*********************************************************************************************************
** ��������: sizeMatch
** ��������: �ж��ļ���С�Ƿ��������Ҫ��
** �䡡��  : require1      �ļ���СҪ��1
**           require2      �ļ���СҪ��2
**           fileSize      �ļ���С
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
INT sizeMatch(INT require1, INT64 require2, INT64 fileSize) {
    if (require1 == 0)
        return 1;
    else if (require1 == 1 && fileSize > require2)
        return 1;
    else if (require1 == 2 && fileSize < require2)
        return 1;
    else if (require1 == 3 && fileSize == require2)
        return 1;
    else
        return 0;
}
/*********************************************************************************************************
** ��������: isEmpty
** ��������: �ж��Ƿ�Ϊ���ļ�����ļ���
** �䡡��  : require       �Ƿ���ҪΪ��
**           type          �ļ�����
**           pDirName      �ļ���
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
INT isEmpty(INT require, unsigned char type, CHAR pDirName[MAX_FILENAME_LENGTH]) {
    if (require == 0)
        return 1;
    if (type == DT_DIR) {
        REGISTER DIR *pdir;
        REGISTER struct dirent *pdirent;
        pdir = opendir(pDirName);
        pdirent = readdir(pdir);
        if (!pdirent)
            return 1;
    } else {
        struct stat statGet;
        stat(pDirName, &statGet);
        if (statGet.st_size == 0)
            return 1;
    }
    return 0;
}
/*********************************************************************************************************
** ��������: mtimeMatch
** ��������: �ж��Ƿ�����޸�ʱ���Ƿ���ϱ�׼
** �䡡��  : require1
**           require2
**           mtime
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
INT mtimeMatch (INT require1, INT64 require2, INT64 mtime) {
    time_t now = API_RootFsTime(LW_NULL);
    if (require1 == 0) return 1;
    else if (require1 == 1 && now - mtime > require2) return 1;
    else if (require1 == 2 && now - mtime <= require2) return 1;
    else return 0;
}
/*********************************************************************************************************
** ��������: chmodMatch
** ��������: �ж��Ƿ�Ȩ���Ƿ���ϱ�׼
** �䡡��  : require
**           fileMode
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
INT chmodMatch (INT require, INT fileMode) {
    if (require == 512 || require == fileMode) return 1;
    return 0;
}
/*********************************************************************************************************
** ��������: __tshellFsCmdFind
** ��������: ϵͳ���� "find"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  :
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
static INT __tshellFsCmdFind(INT iArgC, PCHAR ppcArgU[]) {
#define __TSHELL_BYTES_PERLINE          80                              /*  ���� 80 �ַ�                */
#define __TSHELL_BYTES_PERFILE          16                              /*  ÿ���ļ�����ʾ�񳤶�        */

    CHAR cDirName[MAX_FILENAME_LENGTH] = ".";
    size_t stDirLen = 1;

    REGISTER DIR *pdir;
    REGISTER struct dirent *pdirent;
    struct stat statGet;
    struct winsize winsz;

    CHAR pDirName[MAX_FILENAME_LENGTH];
    INT linksRequire1 = 0;
    INT linksRequire2 = 0;
    INT sizeRequire1 = 0;
    INT64 sizeRequire2 = 0;
    INT mtimeRequire1 = 0;
    INT64 mtimeRequire2 = 0;
    INT chmodRequire = 512;
    unsigned char typeRequire = 16;
    PCHAR nameRequire = "*";
    INT needEmpty = 0;
    CHAR dirList[500][MAX_FILENAME_LENGTH];
    INT dirCount = 0;
    INT dirNow = 0;

    if (iArgC < 2) {
        fprintf(stderr, "argument error.\n");
        return (-ERROR_TSHELL_EPARAM);
    }
    CPCHAR ops[6] = { "-name", "-type", "-links", "-empty", "-size", "-perm" };
    CPCHAR timeOps[2] = { "-mtime", "-mmin" };
    BOOL isOp = LW_FALSE;
    INT i;
    for (i = 0; i < 6; i++) {
        if (!lib_strcmp(ppcArgU[1], ops[i]))
            isOp = LW_TRUE;
        else if (!lib_strcmp(ppcArgU[1], timeOps[0])
                || !lib_strcmp(ppcArgU[1], timeOps[1]))
            isOp = LW_TRUE;
    }
    for (i = 0; i < iArgC; i++) {
        if (!lib_strcmp(ppcArgU[i], ops[0])) {
            i++;
            if (i == iArgC) {
                fprintf(stderr, "argument error.\n");
                return (-ERROR_TSHELL_EPARAM);
            }
            nameRequire = ppcArgU[i];
        } else if (!lib_strcmp(ppcArgU[i], ops[1])) {
            i++;
            if (i == iArgC || lib_strlen(ppcArgU[i]) != 1) {
                fprintf(stderr, "argument error.\n");
                return (-ERROR_TSHELL_EPARAM);
            }
            switch (ppcArgU[i][0]) {
            case 'b':
                typeRequire = DT_BLK;
                break;
            case 'c':
                typeRequire = DT_CHR;
                break;
            case 'd':
                typeRequire = DT_DIR;
                break;
            case 'p':
                typeRequire = DT_FIFO;
                break;
            case 'l':
                typeRequire = DT_LNK;
                break;
            case 'f':
                typeRequire = DT_REG;
                break;
            default:
                fprintf(stderr, "argument error.\n");
                return (-ERROR_TSHELL_EPARAM);
            }
        } else if (!lib_strcmp(ppcArgU[i], ops[2])) {
            i++;
            if (i == iArgC) {
                fprintf(stderr, "argument error.\n");
                return (-ERROR_TSHELL_EPARAM);
            }
            INT idx = lib_strlen(ppcArgU[i]) - 1;
            INT ex = 1;
            while (idx > 0) {
                if (ppcArgU[i][idx] < '0' || ppcArgU[i][idx] > '9') {
                    fprintf(stderr, "argument error.\n");
                    return (-ERROR_TSHELL_EPARAM);
                }
                linksRequire2 += (ppcArgU[i][idx] - '0') * ex;
                ex *= 10;
                idx--;
            }
            if (ppcArgU[i][0] != '+' && ppcArgU[i][0] != '-'
                    && (ppcArgU[i][idx] < '0' || ppcArgU[i][idx] > '9')) {
                fprintf(stderr, "argument error.\n");
                return (-ERROR_TSHELL_EPARAM);
            } else if (ppcArgU[i][0] == '+')
                linksRequire1 = 1;
            else if (ppcArgU[i][0] == '-')
                linksRequire1 = 2;
            else {
                linksRequire1 = 3;
                linksRequire2 += (ppcArgU[i][0] - '0') * ex;
            }
        } else if (!lib_strcmp(ppcArgU[i], ops[3]))
            needEmpty = 1;
        else if (!lib_strcmp(ppcArgU[i], ops[4])) {
            i++;
            if (i == iArgC) {
                fprintf(stderr, "argument error.\n");
                return (-ERROR_TSHELL_EPARAM);
            }
            INT idx = lib_strlen(ppcArgU[i]) - 1;
            INT ex = 1, ex2 = 1;
            if (ppcArgU[i][idx] == 'k') {
                ex2 *= 1000;
                idx--;
            } else if (ppcArgU[i][idx] == 'm') {
                ex2 *= 1000000;
                idx--;
            }
            while (idx > 0) {
                if (ppcArgU[i][idx] < '0' || ppcArgU[i][idx] > '9') {
                    fprintf(stderr, "argument error.\n");
                    return (-ERROR_TSHELL_EPARAM);
                }
                sizeRequire2 += (ppcArgU[i][idx] - '0') * ex;
                ex *= 10;
                idx--;
            }
            if (ppcArgU[i][0] != '+' && ppcArgU[i][0] != '-'
                    && (ppcArgU[i][idx] < '0' || ppcArgU[i][idx] > '9')) {
                fprintf(stderr, "argument error.\n");
                return (-ERROR_TSHELL_EPARAM);
            } else if (ppcArgU[i][0] == '+')
                sizeRequire1 = 1;
            else if (ppcArgU[i][0] == '-')
                sizeRequire1 = 2;
            else {
                sizeRequire1 = 3;
                sizeRequire2 += (ppcArgU[i][0] - '0') * ex;
            }
            sizeRequire2 *= ex2;
        } else if (!lib_strcmp(ppcArgU[i], ops[5])) {
            i++;
            if (i == iArgC || lib_strlen(ppcArgU[i]) != 3) {
                fprintf(stderr, "argument error.\n");
                return (-ERROR_TSHELL_EPARAM);
            }
            INT j, ex = 64;
            chmodRequire = 0;
            for (j = 0; j < 3; j++) {
                if (ppcArgU[i][j] < '0' || ppcArgU[i][j] > '7') {
                    fprintf(stderr, "argument error.\n");
                    return (-ERROR_TSHELL_EPARAM);
                }
                chmodRequire += (ppcArgU[i][j] - '0') * ex;
                ex /= 8;
            }
        } else if (!lib_strcmp(ppcArgU[i], timeOps[0])
                || !lib_strcmp(ppcArgU[i], timeOps[1])) {
            i++;
            if (i == iArgC) {
                fprintf(stderr, "argument error.\n");
                return (-ERROR_TSHELL_EPARAM);
            }
            INT idx = lib_strlen(ppcArgU[i]) - 1;
            INT ex = 1;
            while (idx > 0) {
                if (ppcArgU[i][idx] < '0' || ppcArgU[i][idx] > '9') {
                    fprintf(stderr, "argument error.\n");
                    return (-ERROR_TSHELL_EPARAM);
                }
                mtimeRequire2 += (ppcArgU[i][idx--] - '0') * ex;
                ex *= 10;
            }
            if (ppcArgU[i][0] == '+')
                mtimeRequire1 = 1;
            else if (ppcArgU[i][0] == '-')
                mtimeRequire1 = 2;
            else {
                fprintf(stderr, "argument error.\n");
                return (-ERROR_TSHELL_EPARAM);
            }
            if (!lib_strcmp(ppcArgU[i - 1], timeOps[0]))
                mtimeRequire2 *= 86400;
            else
                mtimeRequire2 *= 60;
        }
    }
    if (isOp) {
        cDirName[stDirLen++] = PX_DIVIDER;
        pdir = opendir(cDirName); /*  ��ǰĿ¼                    */
    } else { /*  ָ��Ŀ¼                    */
        lib_strcpy(cDirName, ppcArgU[1]);
        stDirLen = lib_strlen(cDirName);
        pdir = opendir(cDirName);
        if (stDirLen > 0) {
            if (cDirName[stDirLen - 1] != PX_DIVIDER) /*  ����Ŀ¼������ / ��β       */
                cDirName[stDirLen++] = PX_DIVIDER; /*  ���һ�� /                  */
        }
    }

    if (!pdir) {
        if (errno == EACCES)
            fprintf(stderr, "insufficient permissions.\n");
        else
            fprintf(stderr, "can not open dir! %s\n", lib_strerror(errno));
        return (PX_ERROR);
    }

    if (ioctl(STD_OUT, TIOCGWINSZ, &winsz))
        winsz.ws_col = __TSHELL_BYTES_PERLINE;
    else
        winsz.ws_col = (unsigned short) ROUND_DOWN(winsz.ws_col,
                __TSHELL_BYTES_PERFILE);

    do {
        pdirent = readdir(pdir);
        if (!pdirent) {
            closedir(pdir);
            if (dirNow < dirCount) {
                lib_strcpy(cDirName, dirList[dirNow++]);
                stDirLen = lib_strlen(cDirName);
                cDirName[stDirLen++] = PX_DIVIDER;
                cDirName[stDirLen] = '\0';
                pdir = opendir(cDirName);
            } else
                break;
        } else {
            if ((stDirLen > 1)
                    || ((stDirLen == 1) && (cDirName[0] == PX_ROOT))) {
                lib_strcpy(&pDirName[0], cDirName);
                lib_strcpy(&pDirName[stDirLen], pdirent->d_name); /*  ����ָ��Ŀ¼                */
                stat(pDirName, &statGet);
            } else {
                stat(pdirent->d_name, &statGet); /*  ʹ�õ�ǰĿ¼                */
            }

            if (pdirent->d_type == DT_DIR) {
                lib_strcpy(&dirList[dirCount][0], cDirName);
                lib_strcpy(&dirList[dirCount][stDirLen], pdirent->d_name);
                dirCount++;
            }

            PCHAR fileName = pdirent->d_name;
            unsigned char fileType = pdirent->d_type;
            INT64 fileSize = statGet.st_size;
            INT64 mtime = statGet.st_mtime;
            INT linksCount = statGet.st_nlink;
            INT fileMode = statGet.st_mode & 511;
            if (nameMatch(nameRequire, fileName)
                    && typeMatch(typeRequire, fileType)
                    && linksMatch(linksRequire1, linksRequire2, linksCount)
                    && sizeMatch(sizeRequire1, sizeRequire2, fileSize)
                    && isEmpty(needEmpty, fileType, pDirName)
                    && mtimeMatch(mtimeRequire1, mtimeRequire2, mtime)
                    && chmodMatch(chmodRequire, fileMode)) {
                printf(pDirName);
                printf("\n");
            }
        }
    } while (1);
    return (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdMount
** ��������: ϵͳ���� "mount"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
#if LW_CFG_MOUNT_EN > 0

static INT  __tshellFsCmdMount (INT  iArgC, PCHAR  ppcArgV[])
{
    PCHAR       pcType   = LW_NULL;
    PCHAR       pcDev    = LW_NULL;
    PCHAR       pcFs     = LW_NULL;
    PCHAR       pcOption = LW_NULL;

    INT         iC;
    INT         iOptInd;
    
    if (iArgC < 3 || (iArgC == 4)) {
        fprintf(stderr, "option error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    while ((iC = getopt(iArgC, ppcArgV, "t:o:")) != EOF) {
        switch (iC) {
        
        case 't':
            pcType = optarg;
            break;
            
        case 'o':
            pcOption = optarg;
            break;
        }
    }
    
    iOptInd = optind;
    
    getopt_free();
    
    if (iOptInd > (iArgC - 2)) {
        fprintf(stderr, "option error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    pcDev = ppcArgV[iOptInd];
    pcFs  = ppcArgV[iOptInd + 1];
    
    if (API_MountEx(pcDev, pcFs, pcType, pcOption) != ERROR_NONE) {
        fprintf(stderr, "mount error, error: %s\n", lib_strerror(errno));
        return  (PX_ERROR);
    } else {
        return  (ERROR_NONE);
    }
}
/*********************************************************************************************************
** ��������: __tshellFsCmdUmount
** ��������: ϵͳ���� "umount"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdUmount (INT  iArgC, PCHAR  ppcArgV[])
{
    if (iArgC != 2) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    if (API_Unmount(ppcArgV[1]) == ERROR_NONE) {
        return  (ERROR_NONE);
    } else {
        fprintf(stderr, "umount error, error: %s\n", lib_strerror(errno));
        return  (PX_ERROR);
    }
}
/*********************************************************************************************************
** ��������: __tshellFsCmdRemount
** ��������: ϵͳ���� "remount"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
#if LW_CFG_OEMDISK_EN > 0

static INT  __tshellFsCmdRemount (INT  iArgC, PCHAR  ppcArgV[])
{
    INT             iBlkFd;
    PLW_OEMDISK_CB  poemd = LW_NULL;

    if (iArgC != 2) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    iBlkFd = open(ppcArgV[1], O_RDONLY);
    if (iBlkFd < 0) {
        fprintf(stderr, "can not open %s error: %s!\n", ppcArgV[1], lib_strerror(errno));
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (ioctl(iBlkFd, LW_BLKD_CTRL_OEMDISK, &poemd) || !poemd) {
        close(iBlkFd);
        fprintf(stderr, "can not get AUTO-Mount information!\n");
        return  (PX_ERROR);
    }
    close(iBlkFd);
    
    if (API_OemDiskRemount(poemd)) {
        fprintf(stderr, "Remount fail: %s!\n", lib_strerror(errno));
        return  (PX_ERROR);
    }
    
    return  (ERROR_NONE);
}

#endif                                                                  /*  LW_CFG_OEMDISK_EN > 0       */
/*********************************************************************************************************
** ��������: __tshellFsCmdShowmount
** ��������: ϵͳ���� "showmount"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdShowmount (INT  iArgC, PCHAR  ppcArgV[])
{
#if LW_CFG_OEMDISK_EN > 0
    API_OemDiskMountShow();
    printf("\n");
#endif                                                                  /*  LW_CFG_OEMDISK_EN > 0       */

#if (LW_CFG_MAX_VOLUMES > 0) && (LW_CFG_YAFFS_EN > 0)
    API_YaffsDevMountShow();
    printf("\n");
#endif                                                                  /*  (LW_CFG_MAX_VOLUMES > 0)    */
                                                                        /*  (LW_CFG_YAFFS_EN > 0)       */
    API_MountShow();
    
    return  (ERROR_NONE);
}

#endif                                                                  /*  LW_CFG_MOUNT_EN > 0         */
/*********************************************************************************************************
** ��������: __tshellFsCmdLn
** ��������: ϵͳ���� "ln"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
static INT  __tshellFsCmdLn (INT  iArgC, PCHAR  ppcArgV[])
{
    if (iArgC == 3) {
        if (ppcArgV[1][0] == '-') {
            fprintf(stderr, "arguments error!\n");
            return  (-ERROR_TSHELL_EPARAM);
        }
        if (symlink(ppcArgV[1], ppcArgV[2]) != ERROR_NONE) {
            fprintf(stderr, "symlink error: %s\n", lib_strerror(errno));
        }
    } else if (iArgC == 4) {
        if (ppcArgV[1][0] != '-') {
            fprintf(stderr, "arguments error!\n");
            return  (-ERROR_TSHELL_EPARAM);
        }
        if (!lib_strchr(ppcArgV[1], 's')) {
            fprintf(stderr, "must use -s to create a symbol link.\n");
            return  (-ERROR_TSHELL_EPARAM);
            
        } else {
            if (lib_strchr(ppcArgV[1], 'f')) {
                struct stat  statGet;
                if (lstat(ppcArgV[3], &statGet) == ERROR_NONE) {
                    if (S_ISLNK(statGet.st_mode)) {
                        unlink(ppcArgV[3]);
                    }
                }
            }
            if (symlink(ppcArgV[2], ppcArgV[3]) != ERROR_NONE) {
                fprintf(stderr, "symlink error: %s\n", lib_strerror(errno));
            }
        }
    } else {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdDosfslabel
** ��������: ϵͳ���� "dosfslabel"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
#if LW_CFG_FATFS_EN > 0

static INT  __tshellFsCmdDosfslabel (INT  iArgC, PCHAR  ppcArgV[])
{
    INT  iFd;
    INT  iError;
    CHAR cLabel[MAX_FILENAME_LENGTH];
    
    if (iArgC < 2) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }

    if (iArgC == 2) {                                                   /*  ��ȡ���                    */
        iFd = open(ppcArgV[1], O_RDONLY);
    } else {
        iFd = open(ppcArgV[1], O_RDWR);                                 /*  ���þ��                    */
    }
    
    if (iFd < 0) {
        fprintf(stderr, "can not open device, error: %s\n", lib_strerror(errno));
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (iArgC == 2) {
        iError = ioctl(iFd, FIOLABELGET, cLabel);
        if (iError < ERROR_NONE) {
            fprintf(stderr, "can not get label, error: %s\n", lib_strerror(errno));
        } else {
            printf("%s\n", cLabel);
        }
    } else {
        iError = ioctl(iFd, FIOLABELSET, ppcArgV[2]);
        if (iError < ERROR_NONE) {
            fprintf(stderr, "can not set label, error: %s\n", lib_strerror(errno));
        }
    }
    
    close(iFd);
    
    return  (iError);
}

#endif                                                                  /*  LW_CFG_FATFS_EN > 0         */
/*********************************************************************************************************
** ��������: __tshellFsCmdFdisk
** ��������: ϵͳ���� "fdisk"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
#if LW_CFG_OEMDISK_EN > 0

static INT  __tshellFsCmdFdisk (INT  iArgC, PCHAR  ppcArgV[])
{
    CHAR              cInput[512];
    LW_OEMFDISK_PART  fdpInfo[4];
    UINT              uiNPart;
    size_t            stAlign, stNum;
    struct stat       statGet;
    PCHAR             pcBlkFile;
    UINT              i, uiPct, uiTotalPct = 0;
    CHAR              cActive, cChar, *pcStr;
    INT               iCnt, iType;

    if (iArgC < 2) {
        fprintf(stderr, "too few arguments!\n");
        return  (-ERROR_TSHELL_EPARAM);

    } else if (iArgC == 2) {
        return  (oemFdiskShow(ppcArgV[1]));

    } else {
        if (lib_strcmp(ppcArgV[1], "-f")) {
            fprintf(stderr, "you must use '-f' to make disk partition!\n");
            return  (-ERROR_TSHELL_EPARAM);
        }

        pcBlkFile = ppcArgV[2];
    }

    if (stat(pcBlkFile, &statGet)) {
        fprintf(stderr, "can not get block device status: %s.\n", lib_strerror(errno));
        return  (-ERROR_TSHELL_EPARAM);
    }

    if (!S_ISBLK(statGet.st_mode)) {
        fprintf(stderr, "%s is not a block device.\n", pcBlkFile);
        return  (-ERROR_TSHELL_EPARAM);
    }

    printf("block device %s total size: %llu (MB)\n", pcBlkFile, (statGet.st_size >> 20));

__input_num:
    printf("please input how many partition(s) you want to make (1 ~ 4) : ");
    fflush(stdout);
    fpurge(stdin);
    if (scanf("%d", &uiNPart) != 1) {
        goto    __input_num;
    }

    if ((uiNPart < 1) || (uiNPart > 4)) {
        printf("the number must be 1 ~ 4\n");
        goto    __input_num;
    }

__input_align:
    printf("please input how many bytes align (4K 8K ...) : ");
    fflush(stdout);
    fpurge(stdin);
    if (scanf("%zu", &stAlign) != 1) {
        goto    __input_align;
    }

    if ((stAlign < 4096) || (stAlign & (stAlign - 1))) {
        printf("the number must be 4096 at least and must the n-th power of 2\n");
        goto    __input_align;
    }

    for (i = 0; i < uiNPart; i++) {
__input_size:
        printf("please input the partition %d size percentage(%%) or capacity(M) 0 means all left space : ", i);
        fflush(stdout);
        fpurge(stdin);
        for (pcStr = cInput, stNum = 0;
             ((cChar = getchar()) != '\n') || stNum >= sizeof(cInput) - 1;
             stNum++) {                                                 /*  ��������ַ���              */
            if (cChar == EOF) {
                break;
            } else {
                *pcStr++ = cChar;
            }
        }
        if ((stNum <= 0) || (stNum >= sizeof(cInput))) {
            goto    __input_size;                                       /*  �������                    */
        }
        *pcStr = PX_EOS;

        if (sscanf(cInput, "%u", &uiPct) != 1) {                        /*  ��ȡ����                    */
            goto    __input_size;
        }

        if (lib_strchr(cInput, 'M') || lib_strchr(cInput, 'm')) {       /*  ��������                    */
            fdpInfo[i].FDP_ucSzPct  = 101;                              /*  ʹ����������                */
            fdpInfo[i].FDP_ulMBytes = uiPct;                            /*  TODO: �ж�����Խ��          */

        } else {                                                        /*  ��������                    */
            if (uiPct > 100) {
                printf("the partition size percentage(%%) must be 1 ~ 100\n");
                goto    __input_size;
            }

            if (uiPct == 0) {
                uiTotalPct  = 100;
            } else {
                uiTotalPct += uiPct;
            }

            if (uiTotalPct > 100) {
                printf("the partition size percentage seriously error (bigger than 100%%)!\n");
                return  (-ERROR_TSHELL_EPARAM);
            }

            fdpInfo[i].FDP_ucSzPct = (UINT8)uiPct;
        }

__input_active:
        printf("is this partition active(y/n) : ");
        fflush(stdout);
        fpurge(stdin);
        do {
            cActive = (CHAR)getchar();
        } while ((cActive == '\r') || (cActive == '\n'));
        if ((cActive != 'y') &&
            (cActive != 'Y') &&
            (cActive != 'n') &&
            (cActive != 'N')) {
            printf("please use y or n\n");
            goto    __input_active;
        }

        if ((cActive == 'y') || (cActive == 'Y')) {
            fdpInfo[i].FDP_bIsActive = LW_TRUE;
        } else {
            fdpInfo[i].FDP_bIsActive = LW_FALSE;
        }

__input_type:
        printf("please input the file system type\n");
        printf("1: FAT   2: TPSFS   3: LINUX   4: RESERVED\n");
        fpurge(stdin);
        if ((scanf("%d", &iType) != 1) || ((iType < 1) || (iType > 4))) {
            printf("please use 1 2 3 or 4\n");
            goto    __input_type;
        }

        switch (iType) {

        case 1:
            fdpInfo[i].FDP_ucPartType = LW_DISK_PART_TYPE_WIN95_FAT32;
            break;

        case 2:
            fdpInfo[i].FDP_ucPartType = LW_DISK_PART_TYPE_TPS;
            break;

        case 3:
            fdpInfo[i].FDP_ucPartType = LW_DISK_PART_TYPE_NATIVE_LINUX;
            break;
        
        case 4:
            fdpInfo[i].FDP_ucPartType = LW_DISK_PART_TYPE_RESERVED;
            break;
            
        default:
            fdpInfo[i].FDP_ucPartType = LW_DISK_PART_TYPE_TPS;
            break;
        }
    }

    printf("making partition...\n");
    iCnt = oemFdisk(pcBlkFile, fdpInfo, uiNPart, stAlign);
    if (iCnt <= ERROR_NONE) {
        fprintf(stderr, "make partition error: %s\n", lib_strerror(errno));
        return  (PX_ERROR);
    }
    oemFdiskShow(pcBlkFile);

    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdClrGpt
** ��������: ϵͳ���� "clrgpt"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
static INT  __tshellFsCmdClrGpt (INT  iArgC, PCHAR  ppcArgV[])
{
    INT               iFdBlk;
    ULONG             ulSecSize;
    struct stat       statGet;
    PCHAR             pcBlkFile;
    PVOID             pvBuffer;
    
    if (iArgC == 2) {
        pcBlkFile = ppcArgV[1];
    } else if ((iArgC == 4) && !lib_strcmp(ppcArgV[1], "-s")) {
        pcBlkFile = ppcArgV[3];
    } else {
        fprintf(stderr, "too few arguments!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    iFdBlk = open(pcBlkFile, O_RDWR);
    if (iFdBlk < 0) {
        fprintf(stderr, "can not open: %s error: %s!\n", pcBlkFile, lib_strerror(errno));
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (fstat(iFdBlk, &statGet)) {
        close(iFdBlk);
        fprintf(stderr, "can not get block device status: %s.\n", lib_strerror(errno));
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (!S_ISBLK(statGet.st_mode)) {
        close(iFdBlk);
        fprintf(stderr, "%s is not a block device.\n", pcBlkFile);
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (iArgC == 4) {
        ulSecSize = lib_atol(ppcArgV[2]);
        if ((ulSecSize < 512) || (ulSecSize > (16 * LW_CFG_KB_SIZE))) {
            close(iFdBlk);
            fprintf(stderr, "sector size must >= 512 && <= 16K Bytes!\n");
            return  (-ERROR_TSHELL_EPARAM);
        }
        
    } else {
        if (ioctl(iFdBlk, LW_BLKD_GET_SECSIZE, &ulSecSize) < 0) {
            close(iFdBlk);
            fprintf(stderr, "command 'LW_BLKD_GET_SECSIZE' error: %s!\n", lib_strerror(errno));
            return  (-ERROR_TSHELL_EPARAM);
        }
    }
    
    pvBuffer = __SHEAP_ALLOC((size_t)ulSecSize);
    if (!pvBuffer) {
        close(iFdBlk);
        fprintf(stderr, "system low memory!\n");
        return  (PX_ERROR);
    }
    lib_bzero(pvBuffer, (size_t)ulSecSize);
    
    if (pwrite(iFdBlk, pvBuffer, (size_t)ulSecSize, (off_t)ulSecSize) != ulSecSize) {
        close(iFdBlk);
        fprintf(stderr, "can not write data to block, error: %s!\n", lib_strerror(errno));
        return  (PX_ERROR);
    }
    
    fsync(iFdBlk);
    close(iFdBlk);
    
    return  (ERROR_NONE);
}
/*********************************************************************************************************
** ��������: __tshellFsCmdMkGrub
** ��������: ϵͳ���� "mkgrub"
** �䡡��  : iArgC         ��������
**           ppcArgV       ������
** �䡡��  : 0
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
#ifdef LW_CFG_CPU_ARCH_X86

static INT  __tshellFsCmdMkGrub (INT  iArgC, PCHAR  ppcArgV[])
{
    if (iArgC < 2) {
        fprintf(stderr, "arguments error!\n");
        return  (-ERROR_TSHELL_EPARAM);
    }
    
    if (oemGrub(ppcArgV[1]) < ERROR_NONE) {
        fprintf(stderr, "make grub boot program error: %s\n", lib_strerror(errno));
        return  (PX_ERROR);
    }
    
    printf("make grub boot program ok.\n");
    
    return  (ERROR_NONE);
}

#endif                                                                  /*  LW_CFG_CPU_ARCH_X86         */
#endif                                                                  /*  LW_CFG_OEMDISK_EN > 0       */
/*********************************************************************************************************
 ** ��������: __tshellFsCmdSed
 ** ��������: ϵͳ���� "cat"
 ** �䡡��  : iArgC         ��������
 **           ppcArgV       ������
 ** �䡡��  : 0
 ** ȫ�ֱ���:
 ** ����ģ��:
 *********************************************************************************************************/
static INT __tshellFsCmdSed(INT iArgC, PCHAR ppcArgV[]) {
    if (iArgC == 2) {  //command: sed filename
        FILE *fp;
        char str[100];
        fp = fopen(ppcArgV[1], "r");
        if (fp == NULL) {
            return 1;
        }
        while (1) {
            if (fgets(str, 100, fp) != NULL) {
                printf("%s", str);

            } else {
                break;
            }
        }
        fclose(fp);
    } else if (iArgC == 3) {
        char *filename = ppcArgV[2];
        char *addressin = ppcArgV[1];
        char address[100];
        strcpy(address, addressin);
        int f, t;
        char c;
        char t_c;
        char s[100];

        if (sscanf(address, "'%d%c\%s'", &f, &c, s) == 3
                && (c == 'i' || c == 'a' || c == 'c')) { // sed '3a\text' filename //'3i\text' //'3c\text'
            int l = strlen(s);
            s[l - 1] = '\0';
            //printf( "%d\n%c\n%s\n",f, c,s);
            sed_aic(f, c, s, filename);

        } else if (sscanf(address, "'%d,%d%c'", &f, &t, &c) == 3) {
            //printf( "%d\n%d\n%c\n",f, t,c);
            if (c == 'p') { // ��ӡ����
                sed_p3(f, t, filename); // sed '3,5p' filename
            } else if (c == 'd') { // ��ӡ����
                sed_d3(f, t, filename); // sed '3,5d' filename
            }
        } else if (sscanf(address, "'%d,%c%c'", &f, &t_c, &c) == 3) { // sed '3,$d' filename
            //printf( "%d\n%c\n%c\n",f, t_c,c);
            if (c == 'd') { // ��ӡ����
                if (t_c == '$')
                    sed_d$(f, filename); // sed '3,$d' filename
            }
        } else if (sscanf(address, "'%d%c'", &f, &c) == 2) {
            //printf( "%d\n%c\n",f, c);
            if (c == 'p') { // ��ӡ����
                sed_p2(f, filename); // sed '3p' filename
            } else if (c == 'd') { // ��ӡ����
                sed_d2(f, filename); // sed '3d' filename
            } else if (c == 'q') { // ��ӡ����
                sed_q2(f, filename); // sed '3q' filename
            }
        } else {

            char fc, lc;
            char fs[100];
            char ls[100];
            int slash_count = 0;
            int slash_index[3] = { -1 };

            int i = 0;
            while (i < strlen(address)) {
                if (address[i] == '/') {
                    slash_index[slash_count] = i;
                    slash_count++;
                }
                i++;
            }
            if (slash_count == 3) { // '/ / /'
                if (slash_index[0] == 2
                        && slash_index[2] == strlen(address) - 3) { //sed 's/test/trial/p' filename 's/l/g/g' 's/l/g/p' 's/l/g/2'
                    fc = address[1];
                    strncpy(fs, address + slash_index[0] + 1,
                            slash_index[1] - slash_index[0] - 1);
                    fs[slash_index[1] - slash_index[0] - 1] = '\0';
                    strncpy(ls, address + slash_index[1] + 1,
                            slash_index[2] - slash_index[1] - 1);
                    ls[slash_index[2] - slash_index[1] - 1] = '\0';
                    lc = address[slash_index[2] + 1];
                    //printf("fc:%c\nfs:%s\nls:%s\nlc:%c\n",fc,fs, ls,lc);
                    if (fc == 's') {
                        sed_s(fs, ls, lc, filename); //sed 's/test/trial/p' filename
                    }
                } else if (slash_index[0] == 2
                        && slash_index[2] == strlen(address) - 2) { //sed 's/test/trial/' filename
                    fc = address[1];
                    strncpy(fs, address + slash_index[0] + 1,
                            slash_index[1] - slash_index[0] - 1);
                    fs[slash_index[1] - slash_index[0] - 1] = '\0';
                    strncpy(ls, address + slash_index[1] + 1,
                            slash_index[2] - slash_index[1] - 1);
                    ls[slash_index[2] - slash_index[1] - 1] = '\0';
                    //printf("fc:%c\nfs:%s\nls:%s",fc,fs, ls);
                    if (fc == 's') {
                        sed_s(fs, ls, '\0', filename);
                    } else if (fc == 'y') {
                        sed_y3(fs, ls, filename);
                    }
                }
            }
        }
    } else if (iArgC == 4) {
        char *filename = ppcArgV[3];
        char *addressin = ppcArgV[1];
        char address[100];
        strcpy(address, addressin);
        int f, t;
        char f_c;
        char c;
        //printf("%s\n",filename);
        if (address[strlen(address) - 1] == 'w') { //sed '2,3w fileb' filea//sed '2w fileb' filea
            char *wfile = ppcArgV[2];
            int i = 0;
            //printf("%s\n",wfile);
            while (i < strlen(wfile)) {
                //printf("%c\n",wfile[i]);
                if (wfile[i] == '\'') {
                    wfile[i] = '\0';
                }
                i++;
            }
            //printf("%s\n",wfile);
            if (sscanf(address, "'%d,%d%c", &f, &t, &c) == 3) {
                //printf( "%d\n%d\n%c\n%s\n",f, t,c,wfile);
                sed_w4(f, t, wfile, filename);
            } else if (sscanf(address, "'%d%c", &f, &c) == 2) {
                //printf( "%d\n%c\n%s\n",f, c , wfile);
                sed_w3(f, wfile, filename);
            }
        } else if (address[strlen(address) - 1] == 'r') { // sed '3r fileb' filea// sed '$r fileb' filea
            char *wfile = ppcArgV[2];
            int i = 0;
            //printf("%s\n",wfile);
            while (i < strlen(wfile)) {
                //printf("%c\n",wfile[i]);
                if (wfile[i] == '\'') {
                    wfile[i] = '\0';
                }
                i++;
            }
            //printf("%s\n",wfile);
            if (sscanf(address, "'%d%c", &f, &c) == 2) {
                //printf("d\n");
                sed_r3(f, wfile, filename);
            } else if (sscanf(address, "'%c%c", &f_c, &c) == 2) {
                //printf("%c\n",f_c);
                //if(f_c == '"$"'){
                //printf("$\n");
                sed_r$(wfile, filename);
                //}

            }

        }
    }
    return 0;
}
void sed_aic(int f, char c, char *s, char *filename) {
    // sed '3a\text' filename //'3i\text' //'3c\text'
    FILE *fp;
    int count = 0;
    char str[100];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    while (1) {
        if (fgets(str, 100, fp) != NULL) {
            count++;
            if (c == 'i') {
                if (f == count) {
                    printf("%s\n", s);
                }
            }
            if (c != 'c') {
                printf("%s", str);
            } else {
                if (f == count) {
                    printf("%s\n", s);
                } else {
                    printf("%s", str);
                }
            }
            if (c == 'a') {
                if (f == count) {
                    printf("%s\n", s);
                }
            }
        } else {
            break;
        }
    }
    fclose(fp);
}

void sed_p3(int f, int t, char *filename) {      // command: sed '3,5p' filename
    // command: sed '3,5p' filename
    FILE *fp;
    int count = 0;
    char str[100];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    while (1) {
        if (fgets(str, 100, fp) != NULL) {
            count++;
            printf("%s", str);
            if (count <= t && count >= f) {
                printf("%s", str);
            }
        } else {
            break;
        }
    }
    fclose(fp);
}

void sed_p2(int f, char *filename) { // sed '3p' filename
    // sed '3p' filename
    FILE *fp;
    int count = 0;
    char str[100];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    while (1) {
        if (fgets(str, 100, fp) != NULL) {
            count++;
            printf("%s", str);
            if (count == f) {
                printf("%s", str);
            }
        } else {
            break;
        }
    }
    fclose(fp);
}

void sed_s(char *fs, char *ls, char lc, char * filename) {
    //printf("test function input:\n%s\n%s\n",fs,ls);
    //sed 's/test/trial/p' filename
    //sed 's/test/trial/g' filename
    //sed 's/test/trial/2' filename
    FILE *fp;
    int count = 0;
    char str[100];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    if (lc == '\0') { //sed 's/l/g/'
        BOOL one = FALSE;
        while (1) {
            if (fgets(str, 100, fp) != NULL) {
                count++;
                if (!one) {
                    if (strrpc_1(str, fs, ls)) {
                        one = TRUE;
                    }
                }
                printf("%s", str);
            } else {
                break;
            }
        }
    } else if (lc == 'g') {
        while (1) {
            if (fgets(str, 100, fp) != NULL) {
                count++;
                strrpc(str, fs, ls);
                printf("%s", str);
            } else {
                break;
            }
        }
    } else if (lc == 'p') {
        while (1) {
            if (fgets(str, 100, fp) != NULL) {
                count++;
                if (strrpc_1(str, fs, ls)) {
                    printf("%s", str);
                }
            } else {
                break;
            }
        }
    } else if (lc <= '9' && lc >= '2') {
        int number = lc - 48;
        while (1) {
            if (fgets(str, 100, fp) != NULL) {
                count++;
                int i = 0;
                while (i < number) {
                    strrpc_1(str, fs, ls);
                    i++;
                }

                i = 0;
                while (i < number - 1) {
                    strrpc_1(str, ls, fs);
                    i++;
                }
                printf("%s", str);
            } else {
                break;
            }
        }
    }

    fclose(fp);
}

void sed_d3(int f, int t, char *filename) {
    // sed '3,5d' filename
    FILE *fp;
    int count = 0;
    char str[100];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    while (1) {
        if (fgets(str, 100, fp) != NULL) {
            count++;
            if (count < f || count > t) {
                printf("%s", str);
            }
        } else {
            break;
        }
    }
    fclose(fp);
}
void sed_d2(int f, char *filename) {
    // sed '3d' filename
    FILE *fp;
    int count = 0;
    char str[100];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    while (1) {
        if (fgets(str, 100, fp) != NULL) {
            count++;
            if (count != f) {
                printf("%s", str);
            }
        } else {
            break;
        }
    }
    fclose(fp);
}
void sed_d$(int f, char *filename) {
    // sed '3d' filename
    FILE *fp;
    int count = 0;
    char str[100];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    while (1) {
        if (fgets(str, 100, fp) != NULL) {
            count++;
            if (count < f) {
                printf("%s", str);
            }
        } else {
            break;
        }
    }
    fclose(fp);
}

void sed_y3(char *fs, char *ls, char *filename) {
    int l = strlen(fs);
    FILE *fp;
    int count = 0;
    char str[100];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    while (1) {
        if (fgets(str, 100, fp) != NULL) {
            count++;
            int i = 0;
            while (i < strlen(str)) {
                int j = 0;
                while (j < l) {
                    if (str[i] == fs[j]) {
                        str[i] = ls[j];
                        break;
                    }
                    j++;
                }
                i++;
            }
            printf("%s", str);
        } else {
            break;
        }
    }
    fclose(fp);
}

void sed_w4(int f, int t, char *wfile, char *filename) { //sed '2,3w fileb' filea
    FILE *fp;
    int count = 0;
    char str[100];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    FILE *fw;
    fw = fopen(wfile, "w");
    if (fw == NULL) {
        return;
    }
    while (1) {
        if (fgets(str, 100, fp) != NULL) {
            count++;
            if (count <= t && count >= f) {
                printf("%s", str);
                fwrite(str, strlen(str), 1, fw);
            }
        } else {
            break;
        }
    }
    fclose(fp);
    fclose(fw);
}

void sed_w3(int f, char *wfile, char *filename) { //sed '2w fileb' filea
    FILE *fp;
    int count = 0;
    char str[100];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    FILE *fw;
    fw = fopen(wfile, "w");
    if (fw == NULL) {
        return;
    }
    while (1) {
        if (fgets(str, 100, fp) != NULL) {
            count++;
            if (count == f) {
                printf("%s", str);
                fwrite(str, strlen(str), 1, fw);
            }
        } else {
            break;
        }
    }
    fclose(fp);
    fclose(fw);
}

void sed_r3(int f, char *wfile, char *filename) {
    FILE *fp;
    int count = 0;
    char str[100];
    char str2[100];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    FILE *fw;
    fw = fopen(wfile, "r");
    if (fw == NULL) {
        return;
    }
    while (1) {
        if (fgets(str, 100, fp) != NULL) {
            count++;
            printf("%s", str);
            if (count == f) {
                while (1) {
                    if (fgets(str2, 100, fw) != NULL) {
                        count++;
                        printf("%s", str2);
                    } else {
                        break;
                    }
                }

            }
        } else {
            break;
        }
    }
    fclose(fp);
    fclose(fw);
}
void sed_r$(char *wfile, char *filename) {
    printf("%s %s\n", wfile, filename);
    FILE *fp;
    int count = 0;
    char str[100];
    char str2[100];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    FILE *fw;
    fw = fopen(wfile, "r");
    if (fw == NULL) {
        return;
    }
    while (1) {
        if (fgets(str, 100, fp) != NULL) {
            count++;
            printf("%s", str);
        } else {
            break;
        }
    }
    while (1) {
        if (fgets(str2, 100, fw) != NULL) {
            count++;
            printf("%s", str2);
        } else {
            break;
        }
    }
    fclose(fp);
    fclose(fw);
}
void sed_q2(int f, char *filename) {
    // sed '3d' filename
    FILE *fp;
    int count = 0;
    char str[100];
    fp = fopen(filename, "r");
    if (fp == NULL) {
        return;
    }
    while (1) {
        if (fgets(str, 100, fp) != NULL) {
            if (count < f) {
                printf("%s", str);
            } else {
                break;
            }
            count++;
        } else {
            break;
        }
    }
    fclose(fp);
}
BOOL strrpc_1(char *str, char *oldstr, char *newstr) {
    char bstr[strlen(str)]; //ת��������
    memset(bstr, 0, sizeof(bstr));
    BOOL one = FALSE;
    int i = 0;
    while (i < strlen(str)) {
        if (!one && !strncmp(str + i, oldstr, strlen(oldstr))) { //����Ŀ���ַ���
            strcat(bstr, newstr);
            i += strlen(oldstr) - 1;
            one = TRUE;
        } else {
            strncat(bstr, str + i, 1); //����һ�ֽڽ�������

        }
        i++;
    }

    strcpy(str, bstr);
    return one;
}
char *strrpc(char *str, char *oldstr, char *newstr) {
    char bstr[strlen(str)]; //ת��������
    memset(bstr, 0, sizeof(bstr));
    int i = 0;
    while (i < strlen(str)) {
        if (!strncmp(str + i, oldstr, strlen(oldstr))) { //����Ŀ���ַ���
            strcat(bstr, newstr);
            i += strlen(oldstr) - 1;
        } else {
            strncat(bstr, str + i, 1); //����һ�ֽڽ�������
        }
        i++;
    }

    strcpy(str, bstr);
    return str;
}

/*********************************************************************************************************
** ��������: __tshellFsCmdInit
** ��������: ��ʼ���ļ�ϵͳ���
** �䡡��  : NONE
** �䡡��  : NONE
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
VOID  __tshellFsCmdInit (VOID)
{
    API_TShellKeywordAdd("sed", __tshellFsCmdSed);
    API_TShellFormatAdd("sed", "command");
    API_TShellHelpAdd("sed", "some sed command");

    API_TShellKeywordAdd("find", __tshellFsCmdFind);
    API_TShellFormatAdd("find", " find pathname -options");
    API_TShellHelpAdd("find", "find files by options\n");

    API_TShellKeywordAdd("cd", __tshellFsCmdCd);
    API_TShellFormatAdd("cd", " path");
    API_TShellHelpAdd("cd", "set current working path\n");
    
    API_TShellKeywordAdd("ch", __tshellFsCmdCh);
    API_TShellFormatAdd("ch", " dir");
    API_TShellHelpAdd("ch", "change dir\n");
    
    API_TShellKeywordAdd("pwd", __tshellFsCmdPwd);
    API_TShellFormatAdd("pwd", " ");
    API_TShellHelpAdd("pwd", "print working directory\n");
    
    API_TShellKeywordAdd("df", __tshellFsCmdDf);
    API_TShellFormatAdd("df", " volume name");
    API_TShellHelpAdd("df", "display volume useage information\n");
    
    API_TShellKeywordAdd("tmpname", __tshellFsCmdTmpname);
    API_TShellHelpAdd("tmpname", "calculate a temp file name.\n");
    
    API_TShellKeywordAdd("mkdir", __tshellFsCmdMkdir);
    API_TShellFormatAdd("mkdir", " directory");
    API_TShellHelpAdd("mkdir", "make a new directory\n");
    
    API_TShellKeywordAdd("mkfifo", __tshellFsCmdMkfifo);
    API_TShellFormatAdd("mkfifo", " [fifo name]");
    API_TShellHelpAdd("mkfifo", "make a new fifo\n");
    
    API_TShellKeywordAdd("rmdir", __tshellFsCmdRmdir);
    API_TShellFormatAdd("rmdir", " directory");
    API_TShellHelpAdd("rmdir", "remove a directory\n");
    
    API_TShellKeywordAdd("rm", __tshellFsCmdRm);
    API_TShellFormatAdd("rm", " [-f] file name");
    API_TShellHelpAdd("rm", "remove a file\n");
    
    API_TShellKeywordAdd("mv", __tshellFsCmdMv);
    API_TShellFormatAdd("mv", " SRC file name, DST file name");
    API_TShellHelpAdd("mv", "move a file\n");
    
    API_TShellKeywordAdd("cat", __tshellFsCmdCat);
    API_TShellFormatAdd("cat", " file name");
    API_TShellHelpAdd("cat", "display file\n");
    
    /**
     * add grep cmd
     */
    API_TShellKeywordAdd("grep", __tshellFsCmdGrep);
    API_TShellFormatAdd("grep", " [-args] [pattern] [file name]");
    API_TShellHelpAdd("grep", "search files for a string or regular expression\n");

    API_TShellKeywordAdd("cp", __tshellFsCmdCp);
    API_TShellFormatAdd("cp", " src file name dst file name");
    API_TShellHelpAdd("cp", "copy file\n");
    
    API_TShellKeywordAdd("cmp", __tshellFsCmdCmp);
    API_TShellFormatAdd("cmp", " [file one] [file two]");
    API_TShellHelpAdd("cmp", "compare two file\n");
    
    API_TShellKeywordAdd("touch", __tshellFsCmdTouch);
    API_TShellFormatAdd("touch", " [-amc] file name");
    API_TShellHelpAdd("touch", "touch a file\n");
    
    API_TShellKeywordAdd("ls", __tshellFsCmdLs);
    API_TShellFormatAdd("ls", " [path name]");
    API_TShellHelpAdd("ls", "list file(s)\n");
    
    API_TShellKeywordAdd("ll", __tshellFsCmdLl);
    API_TShellFormatAdd("ll", " [path name]");
    API_TShellHelpAdd("ll", "get file(s) attrib\n");
    
    API_TShellKeywordAdd("dsize", __tshellFsCmdDsize);
    API_TShellFormatAdd("dsize", " [path name]");
    API_TShellHelpAdd("dsize", "get directory size\n");
    
    API_TShellKeywordAdd("chmod", __tshellFsCmdChmod);
    API_TShellFormatAdd("chmod", " newmode filename");
    API_TShellHelpAdd("chmod", "change file attrib\n"
                               "eg. chmod 777 file\n");
    
    API_TShellKeywordAdd("mkfs", __tshellFsCmdMkfs);
    API_TShellFormatAdd("mkfs", " media name");
    API_TShellHelpAdd("mkfs", "make a fs format in a disk(format disk).\n");
    
    API_TShellKeywordAdd("shfile", __tshellFsCmdShfile);
    API_TShellFormatAdd("shfile", " shell file");
    API_TShellHelpAdd("shfile", "execute a shell file.\n");
    
#if LW_CFG_MOUNT_EN > 0
    API_TShellKeywordAdd("mount", __tshellFsCmdMount);
    API_TShellFormatAdd("mount", " [-t fstype] [-o option] [blk dev] [mount path]");
    API_TShellHelpAdd("mount",  "mount a volume.\n"
                                "eg. mount /dev/blk/sata0 /mnt/hdd0\n"
                                "    mount -t vfat /dev/blk/sata0 /mnt/hdd0\n"
                                "    mount -t tpsfs /dev/blk/sata0 /mnt/hdd0\n"
                                "    mount -t romfs /dev/blk/rom0 /mnt/rom0\n"
                                "    mount -t romfs /root/romfile /mnt/rom1\n"
                                "    mount -t iso9660 /root/cd.iso /mnt/cdrom\n"
                                "    mount -t ramfs 100000 /mnt/ram\n"
                                "    mount -t nfs -o ro 192.168.0.123:/nfstest /mnt/nfs\n"
                                "-o \n"
                                "ro (read-only file system)\n"
                                "rw (read-write file system)\n");
                                
    API_TShellKeywordAdd("umount", __tshellFsCmdUmount);
    API_TShellFormatAdd("umount", " [mount path]");
    API_TShellHelpAdd("umount",  "unmount a volume.\n"
                                 "eg. mount /mnt/usb\n");
                                 
#if LW_CFG_OEMDISK_EN > 0
    API_TShellKeywordAdd("remount", __tshellFsCmdRemount);
    API_TShellFormatAdd("remount", " [/dev/blk/*]");
    API_TShellHelpAdd("remount",  "auto remount a block device.\n"
                                 "eg. remount /dev/blk/hdd-0\n");
#endif                                                                  /*  LW_CFG_OEMDISK_EN > 0       */

    API_TShellKeywordAdd("showmount", __tshellFsCmdShowmount);
    API_TShellHelpAdd("showmount",  "show all mount point.\n");
#endif                                                                  /*  LW_CFG_MOUNT_EN > 0         */

    API_TShellKeywordAdd("ln", __tshellFsCmdLn);
    API_TShellFormatAdd("ln", " [-s | -f] [actualpath] [sympath]");
    API_TShellHelpAdd("ln",   "create a symbol link file (must use -s).\n"
                              "eg. ln -s /tmp/dir /dir\n");

#if LW_CFG_FATFS_EN > 0
    API_TShellKeywordAdd("dosfslabel", __tshellFsCmdDosfslabel);
    API_TShellFormatAdd("dosfslabel", " [[vol newlabel] [vol]]");
    API_TShellHelpAdd("dosfslabel",   "get or set volumn label.\n"
                                      "eg. dosfslabel /usb/ms0\n"
                                      "    dosfslabel /usb/ms1 newlabel\n");
#endif                                                                  /*  LW_CFG_FATFS_EN > 0         */

#if LW_CFG_OEMDISK_EN > 0
    API_TShellKeywordAdd("fdisk", __tshellFsCmdFdisk);
    API_TShellFormatAdd("fdisk", " [-f] [block I/O device]");
    API_TShellHelpAdd("fdisk",   "show or make disk partition table\n"
                                 "eg. fdisk /dev/blk/udisk0\n"
                                 "    fdisk -f /dev/blk/sata0\n");
                                 
    API_TShellKeywordAdd("clrgpt", __tshellFsCmdClrGpt);
    API_TShellFormatAdd("clrgpt", " [-s [sector size]] [block I/O device]");
    API_TShellHelpAdd("clrgpt",   "clear GPT infomation\n"
                                  "eg. clrgpt /dev/blk/udisk0\n"
                                  "    clrgpt -s 512 /dev/blk/sata0\n");

#ifdef LW_CFG_CPU_ARCH_X86
    API_TShellKeywordAdd("mkgrub", __tshellFsCmdMkGrub);
    API_TShellFormatAdd("mkgrub", " [block I/O device]");
    API_TShellHelpAdd("mkgrub",   "make disk grub boot program\n");
#endif                                                                  /*  LW_CFG_CPU_ARCH_X86         */
#endif                                                                  /*  LW_CFG_OEMDISK_EN > 0       */
}

#endif                                                                  /*  LW_CFG_SHELL_EN > 0         */
                                                                        /*  LW_CFG_MAX_VOLUMES > 0      */
/*********************************************************************************************************
  END
*********************************************************************************************************/
