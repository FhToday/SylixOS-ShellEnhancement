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
** ��   ��   ��: system_cfg.h
**
** ��   ��   ��: Han.Hui (����)
**
** �ļ���������: 2006 �� 12 �� 12 ��
**
** ��        ��: ����ϵͳ���������ļ���
*********************************************************************************************************/

#ifndef __SYSTEM_CFG_H
#define __SYSTEM_CFG_H

/*********************************************************************************************************
*                               �ؼ�����������
*********************************************************************************************************/

#define LW_CFG_MAX_DRIVERS                      64      /*  ϵͳʹ�õ������������              < 65535 */
#define LW_CFG_MAX_FILES                        200     /*  ϵͳ�ļ�������������                < 65536 */

#define LW_CFG_MAX_THREAD_POOLS                 2       /*  ϵͳ����������̳߳�������          < 65535 */
#define LW_CFG_MAX_EXCEMSGS                     20      /*  ϵͳ�쳣��Ϣ������������          < 65536 */
#define LW_CFG_MAX_LOGMSGS                      64      /*  ϵͳ��־��Ϣ������������          < 65536 */

#define LW_CFG_MAX_DMA_CHANNELS                 64      /*  ϵͳ�� DMA ͨ������                 < 65535 */
#define LW_CFG_MAX_DMA_LISTNODES                64      /*  ϵͳ DMA ������ҵ���е�����, ����������,    */
                                                        /*  ϵͳ��ʹ�ö�̬�ڴ����ڵ�                  */

#define LW_CFG_MAX_SIGQUEUE_NODES               100     /*  ϵͳ�����źŵȴ��������ڵ���              */

#define LW_CFG_MAX_GPIOS                        256     /*  ϵͳ�� GPIO ����                            */

/*********************************************************************************************************
*                                     DMA ���ܹ���
*
* �����ϵ: 1: �źŷ��� (��Ҫ t_except �����߳�֧��)
*********************************************************************************************************/

#define LW_CFG_DMA_EN                           1       /*  �Ƿ�����ϵͳ�ṩ�� DMA ��֧��               */

/*********************************************************************************************************
*                                     �̳߳ع��ܹ���
*
* �����ϵ: 1: �����ź���
*********************************************************************************************************/

#define LW_CFG_THREAD_POOL_EN                   1       /*  �Ƿ������̳߳ع��ܹ���                      */

/*********************************************************************************************************
*                   �źŹ��� (ϵͳ��Ҫϵͳ��Ҫ����һ������֧���źŴ���)
*
* �����ϵ: 1: ��Ϣ����
*********************************************************************************************************/

#define LW_CFG_SIGNAL_EN                        1       /*  �Ƿ�����ϵͳʹ���ź�                        */

/*********************************************************************************************************
*    POSIX ��ʱ�����丽������, �����Ҫ BSD Net/3 ����֧������Ҫͬʱ��ͨ�źŷ����posix ��ʱ������
*
* �����ϵ: 1: �źŷ���
            2: �ں˶�ʱ������
*********************************************************************************************************/

#define LW_CFG_PTIMER_EN                        1       /*  �Ƿ���������ϵͳ��ͨ posix ��ʱ������       */
#define LW_CFG_PTIMER_AUTO_DEL_EN               0       /*  ���߳�ɾ��ʱ, ���������� posix ��ʱ���Զ�   */
                                                        /*  ��ɾ��, ʹ�ñ�ѡ��Ϻķ���Դ, �����Լ�����  */
                                                        /*  ɾ���� posix ��ʱ��API:timer_delete()ɾ��   */

/*********************************************************************************************************
*                                           I/O ϵͳ
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: �����ź���
            4: RTC ֧��
*********************************************************************************************************/

#define LW_CFG_DEVICE_EN                        1       /*  �Ƿ�����ϵͳ�豸����                        */
                                                        
                                                        /*  ��������Ϊ 0 ʹ�÷ּ������豸               */
#define LW_CFG_PATH_VXWORKS                     0       /*  �Ƿ�ʹ�� VxWorks ���ݵ� I/O �豸Ŀ¼�ṹ    */
                                                        /*  VxWorks �������豸������ͳһ�ĵ�������      */
                                                        /*  Ϊ 0 ʱ: SylixOS ���ļ�ϵͳ���ּ������豸   */
                                                        /*  ��ϸ������ doc/FAQ �����ĵ�               */
                                                        
                                                        /*  ���鲻Ҫ�Ķ�������������                    */
#define LW_CFG_PATH_MAX                         512     /*  ���Ŀ¼������                            */
#define LW_CFG_DIR_MAX                          256     /*  �����Ŀ¼����                              */

/*********************************************************************************************************
*                                     ���� Linux �¼���ɵȴ��첽�¼�����
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
            4: �źŷ���
            5: ��ʱ��
*********************************************************************************************************/

#define LW_CFG_SIGNALFD_EN                      1       /*  �Ƿ���� signalfd ����ģ��                  */
#define LW_CFG_TIMERFD_EN                       1       /*  �Ƿ���� timerfd ����ģ��                   */
#define LW_CFG_EVENTFD_EN                       1       /*  �Ƿ���� eventfd ����ģ��                   */

/*********************************************************************************************************
*                                     ��· I/O ����֧������
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_SELECT_EN                        1       /*  �Ƿ�֧�� select pselect ��I/O��·���ù���   */
#define LW_CFG_SELECT_INTER_EN                  1       /*  select ϵ�к����Ƿ�֧�ֱ��źŴ��           */
#define LW_CFG_EPOLL_EN                         1       /*  �Ƿ�֧�ֲ��� linux epoll ������ϵͳ         */

/*********************************************************************************************************
*                                     ϵͳ���ܷ���
*
* �����ϵ: 1: ��Ϣ����
            2: loader
*********************************************************************************************************/

#define LW_CFG_SYSPERF_EN                       1       /*  �Ƿ�����ϵͳ���ܷ���                        */

/*********************************************************************************************************
*                                     ϵͳ/Ӧ�ñ�����Ϣ�ݴ�
*
* �����ϵ: 1: �豸����
*********************************************************************************************************/

#define LW_CFG_CDUMP_EN                         1       /*  �Ƿ�����ϵͳ/Ӧ�ñ�����Ϣ�ݴ�               */
#define LW_CFG_CDUMP_BUF_SIZE                   2048    /*  �����С                                    */
#define LW_CFG_CDUMP_CALL_STACK_DEPTH           20      /*  ����ջ���                                  */

/*********************************************************************************************************
*                                      I/O ϵͳ���֧��
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_INOTIFY_EN                       1       /*  �Ƿ�֧�� linux ϵͳ���ݵ� inotify ����      */
#define LW_CFG_INOTIFY_MAX_QUEUE_EVENTS         256     /*  ����¼����г���                            */
#define LW_CFG_INOTIFY_MAX_INSTANCES            128     /*  �����Դ������ٸ� inotify ʵ��             */
#define LW_CFG_INOTIFY_MAX_WATCHES              2048    /*  inotify ���� watcher ����                 */

/*********************************************************************************************************
*                                      GPIO ��׼�ӿ�
* �����ϵ: 1: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_GPIO_EN                          1       /*  �Ƿ�ʹ�ܱ�׼�� GPIO ����ӿ�                */

/*********************************************************************************************************
*                                      ͼ�λ���ʾ�豸
* �����ϵ: 1: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_GRAPH_EN                         1       /*  �Ƿ�ʹ��ͼ�λ��豸ģ��                      */

/*********************************************************************************************************
*                                      �ַ����ܵ�ͨ�Ź���
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
*********************************************************************************************************/
	
#define LW_CFG_SPIPE_EN                         1       /*  �Ƿ�����ϵͳ�ṩ�ַ����ܵ�ͨ��֧��          */

#ifdef __SYLIXOS_LITE                                   /*  pipe �� mkfifo �����Ĺܵ���С               */
#define LW_CFG_SPIPE_DEFAULT_SIZE               (8 * 1024)
#else                                                   /*  ע��: ����С�� PIPE_BUF ���� (limits.h)     */
#define LW_CFG_SPIPE_DEFAULT_SIZE               (32 * 1024)
#endif

#define LW_CFG_SPIPE_MULTI_EN                   1       /*  �Ƿ���Ӧ�ö���̷߳���, ����߳̽���ͬһ�ܵ�*/
                                                        /*  ���û�п���Ϊ 0 ���Ч��, ����Ӧ�ú��ټ�   */
                                                        /*  Ϊ�˼�����, ����Ĭ������Ϊ 1                */
/*********************************************************************************************************
*                                      �ܵ�ͨ�Ź���
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: ��Ϣ����
            4: I/Oϵͳ
*********************************************************************************************************/
	
#define LW_CFG_PIPE_EN                          1       /*  �Ƿ�����ϵͳ�ṩ�ܵ�ͨ��֧��                */

/*********************************************************************************************************
*                                      �б߽���Ϣ�豸
* �����ϵ: 1: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_BMSG_EN                          1       /*  �Ƿ�����ϵͳ�ṩ�߽���Ϣ�豸֧��            */

/*********************************************************************************************************
*                                      �ź����豸
* �����ϵ: 1: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_SEMFD_EN                         1       /*  ���ź����ļ���, ��֧�� select �� I/O ����   */

/*********************************************************************************************************
*                                      �ڴ��豸����
* �����ϵ: 1: �������ź���
            2: �����ź���
            4: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_MEMDEV_EN                        1       /*  �Ƿ�����ϵͳ�ṩ�ڴ��豸֧��                */

/*********************************************************************************************************
*                                      CAN �豸����
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_CAN_EN                           1       /*  �Ƿ�����ϵͳ�ṩ CAN �豸����               */

/*********************************************************************************************************
*                                      BUZZER �豸����
* �����ϵ: 1: �������ź���
            2: ��Ϣ����
            3: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_BUZZER_EN                        1       /*  �Ƿ�����ϵͳ BUZZER �豸����                */

/*********************************************************************************************************
*                                      SDCARD �豸���� (ϵͳ������ SD ����ģ��)
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
            4: disk cache
*********************************************************************************************************/

#define LW_CFG_SDCARD_EN                        1       /*  �Ƿ�����ϵͳ�ṩ SD ���豸����              */
                                                        /*  ϵͳ���ṩ SD ����׼�豸ģ��                */
#define LW_CFG_SDCARD_SDIO_EN                   1       /*  �Ƿ������ṩ SDIO Э��֧��                  */
                                                        /*  ��������SD���豸����ģ��                    */
#define LW_CFG_SDCARD_CRC_EN                    1       /*  �Ƿ����� SD �� CRC ����                     */
                                                        
/*********************************************************************************************************
*                                      PCI �豸����
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_PCI_EN                           1       /*  �Ƿ�����ϵͳ�ṩ PCI �豸����               */
#define LW_CFG_PCI_64                           1       /*  PCI 64 bit ��ַ֧�� (����Ϊ 1)              */

/*********************************************************************************************************
*                                      USB �����豸����
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_USB_EN                           1       /*  �Ƿ�����ϵͳ�ṩ USB �����豸����           */

/*********************************************************************************************************
*                                      ATA �豸����
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_ATA_EN                           1       /*  �Ƿ�����ϵͳ�ṩ ATA �豸����               */
#define LW_CFG_ATA_MAX_CTLS                     2       /*  ��� IDE ����������                         */

/*********************************************************************************************************
*                                      AHCI �豸����
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
            4: PCI�豸
            5: shell
*********************************************************************************************************/

#define LW_CFG_AHCI_EN                          1       /*  �Ƿ�����ϵͳ�ṩ AHCI �����豸����          */
#define LW_CFG_AHCI_TRIM_EN                     1       /*  �Ƿ�����ϵͳʹ�� TRIM ����                  */
#define LW_CFG_AHCI_CACHE_SIZE_MB               1       /*  SATA ���ļ�ϵͳ CACHE ��С, ��λ: MB        */

/*********************************************************************************************************
*                                      NVMe �豸����
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
            4: PCI�豸
            5: shell
            6: vmm
*********************************************************************************************************/

#define LW_CFG_NVME_EN                          1       /*  �Ƿ�����ϵͳ�ṩ NVMe �����豸����          */
#define LW_CFG_NVME_TRIM_EN                     1       /*  �Ƿ�����ϵͳʹ�� TRIM ����                  */
#define LW_CFG_NVME_CACHE_SIZE_MB               1       /*  NVMe ���ļ�ϵͳ CACHE ��С, ��λ: MB        */

/*********************************************************************************************************
*                               SIO �豸���� (TTY �� PTY �豸)
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_SIO_DEVICE_EN                    1       /*  �Ƿ�����ϵͳ����ͬ���豸                    */
#define LW_CFG_SIO_TERMINAL_NOT_7_BIT           1       /*  tty �ն˲�ʹ�þ���� 7 bit ģʽ (��������)  */
                                                        /*  �� OPT_TERMINAL ������ OPT_7_BIT ѡ��       */
/*********************************************************************************************************
*                                        �����ն��豸����
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
            4: TTY �豸֧�� (SIO)
*********************************************************************************************************/

#define LW_CFG_PTY_DEVICE_EN                    1       /*  �Ƿ�����ϵͳ���������ն��豸                */

/*********************************************************************************************************
*                                   �����ڴ��豸���� (���ڽ��̼�ͨ��)
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
            4: VMM �����ڴ����
*********************************************************************************************************/

#define LW_CFG_SHM_DEVICE_EN                    1       /*  �Ƿ��������̼�ͨ�Ź����ڴ������豸          */

/*********************************************************************************************************
*                                      FIO LIB ��֧��
* �����ϵ: 1: �������ź���
            2: �����ź���
            3: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_FIO_LIB_EN                       1       /*  �Ƿ�����ϵͳ�ṩ FIO LIB ��                 */
#define LW_CFG_FIO_STDIN_LINE_EN                1       /*  stdin  ��ʼ��ʱ�Ƿ�ʹ�� line buffer ģʽ    */
#define LW_CFG_FIO_STDOUT_LINE_EN               1       /*  stdout ��ʼ��ʱ�Ƿ�ʹ�� line buffer ģʽ    */
#define LW_CFG_FIO_STDERR_LINE_EN               1       /*  stderr ��ʼ��ʱ�Ƿ�ʹ�� line buffer ģʽ    */

/*********************************************************************************************************
*                                        ��־������
* �����ϵ: 1: ��Ϣ����
            2: �����ڴ����
            3: I/Oϵͳ
*********************************************************************************************************/

#define LW_CFG_LOG_LIB_EN                       1       /*  �Ƿ�����ϵͳ�ṩ��־������                  */
#define LW_CFG_LOG_MSG_LEN_MAX                  1024    /*  printk �����Ϣ����                         */

/*********************************************************************************************************
*                                      ���Ĺ�����֧��
* �����ϵ: 1: �����ź���
*********************************************************************************************************/

#define LW_CFG_POWERM_EN                        1       /*  �Ƿ�����ϵͳ�ṩ���Ĺ���������              */
#define LW_CFG_POWERM_OPTION                    LW_OPTION_THREAD_STK_CHK
                                                        /*  ���Ĺ����������߳�ѡ��                      */

/*********************************************************************************************************
*                                      �Ȳ����Ϣ��ѭ�����֧��
* �����ϵ: 1: �����ź���
            2: ��Ϣ����
*********************************************************************************************************/

#define LW_CFG_HOTPLUG_EN                       1       /*  �Ƿ�����ϵͳ�ṩ�Ȳ����Ϣ֧��              */
#define LW_CFG_HOTPLUG_OPTION                   LW_OPTION_THREAD_STK_CHK
                                                        /*  �Ȳ����Ϣ�����߳�ѡ��                      */
#define LW_CFG_HOTPLUG_MAX_MSGS                 8       /*  ��󲢷���Ϣ����(��ϵͳ�ɻ������Ϣ����)    */

#define LW_CFG_HOTPLUG_DEV_DEFAULT_BUFSIZE      4096    /*  /dev/hotplug �豸Ĭ�ϻ�������С             */

#endif                                                  /*  __SYSTEM_CFG_H                              */
/*********************************************************************************************************
  END
*********************************************************************************************************/