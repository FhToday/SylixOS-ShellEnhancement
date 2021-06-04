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
** 文   件   名: posix_opt.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2013 年 03 月 14 日
**
** 描        述: 对 POSIX 标准兼容库.
*********************************************************************************************************/

#ifndef __POSIX_OPT_H
#define __POSIX_OPT_H

/*********************************************************************************************************
 Job control is supported. 
*********************************************************************************************************/

#define _POSIX_JOB_CONTROL                  1

/*********************************************************************************************************
 Processes have a saved set-user-ID and a saved set-group-ID. 
*********************************************************************************************************/

#define _POSIX_SAVED_IDS                    1

/*********************************************************************************************************
 Synchronizing file data is supported. 
*********************************************************************************************************/

#define _POSIX_SYNCHRONIZED_IO              1

/*********************************************************************************************************
 The fsync function is present. 
*********************************************************************************************************/

#define _POSIX_FSYNC                        200809L

/*********************************************************************************************************
 Mapping of files to memory is supported. 
*********************************************************************************************************/

#define _POSIX_MAPPED_FILES                 200809L

/*********************************************************************************************************
 Locking of all memory could be supported in future. 
*********************************************************************************************************/

#define _POSIX_MEMLOCK                      0

/*********************************************************************************************************
 Locking of ranges of memory is supported. 
*********************************************************************************************************/

#define _POSIX_MEMLOCK_RANGE                200809L

/*********************************************************************************************************
 Setting of memory protections is supported. 
*********************************************************************************************************/

#define _POSIX_MEMORY_PROTECTION            200809L

/*********************************************************************************************************
 File supports O_SYNC 
*********************************************************************************************************/

#define _POSIX_SYNC_IO                      1

/*********************************************************************************************************
 POSIX threads interface. 
*********************************************************************************************************/

#define _POSIX_THREADS                      1
#define _POSIX_THREAD_PRIORITY_SCHEDULING   1
#define _POSIX_THREAD_ATTR_STACKSIZE        1
#define _POSIX_THREAD_ATTR_STACKADDR        1
#define _POSIX_SEMAPHORES                   1

/*********************************************************************************************************
 We have the reentrant functions described in POSIX. 
*********************************************************************************************************/

#define _POSIX_REENTRANT_FUNCTIONS          1
#define _POSIX_THREAD_SAFE_FUNCTIONS        200809L

/*********************************************************************************************************
 Real-time signals are supported. 
*********************************************************************************************************/

#define _POSIX_REALTIME_SIGNALS             1

/*********************************************************************************************************
 Asynchronous I/O supported. 
*********************************************************************************************************/

#define _POSIX_ASYNCHRONOUS_IO              1

/*********************************************************************************************************
 Alternative name for Unix98. 
*********************************************************************************************************/

#define _LFS_ASYNCHRONOUS_IO                _POSIX_ASYNCHRONOUS_IO

/*********************************************************************************************************
 The LFS support in asynchronous I/O is also available. 
*********************************************************************************************************/

#define _LFS64_ASYNCHRONOUS_IO              _POSIX_ASYNCHRONOUS_IO

/*********************************************************************************************************
 The rest of the LFS is also available. 
*********************************************************************************************************/

#define _LFS_LARGEFILE                      1
#define _LFS64_LARGEFILE                    1
#define _LFS64_STDIO                        1

/*********************************************************************************************************
 POSIX.4 shared memory objects are supported (using regular files). 
*********************************************************************************************************/

#define _POSIX_SHARED_MEMORY_OBJECTS        _POSIX_MAPPED_FILES

/*********************************************************************************************************
 CPU-time clocks support needs to be checked at runtime. 
*********************************************************************************************************/

#define _POSIX_CPUTIME                      0

/*********************************************************************************************************
 Clock support in threads must be also checked at runtime. 
*********************************************************************************************************/

#define _POSIX_THREAD_CPUTIME               0

/*********************************************************************************************************
 GNU libc provides regular expression handling. 
*********************************************************************************************************/

#define _POSIX_REGEXP                       1

/*********************************************************************************************************
 Reader/Writer locks are available. 
*********************************************************************************************************/

#define _POSIX_READER_WRITER_LOCKS          1

/*********************************************************************************************************
 We have a POSIX shell. 
*********************************************************************************************************/

#define _POSIX_SHELL                        1

/*********************************************************************************************************
 We cannot support the Timeouts option without _POSIX_THREADS. 
*********************************************************************************************************/

#define _POSIX_TIMEOUTS                     1

/*********************************************************************************************************
 The `spawn' function family is supported. 
*********************************************************************************************************/

#define _POSIX_SPAWN                        200809L

/*********************************************************************************************************
 We do have POSIX timers. 
*********************************************************************************************************/

#define _POSIX_TIMERS                       1

/*********************************************************************************************************
 The barrier functions are not available. 
*********************************************************************************************************/

#define _POSIX_BARRIERS                     1

/*********************************************************************************************************
 POSIX message queues available. 
*********************************************************************************************************/

#define _POSIX_MESSAGE_PASSING              1

/*********************************************************************************************************
 Thread process-shared synchronization is supported. 
*********************************************************************************************************/

#define _POSIX_THREAD_PROCESS_SHARED        1

/*********************************************************************************************************
 The monotonic clock available. 
*********************************************************************************************************/

#define _POSIX_MONOTONIC_CLOCK              1

/*********************************************************************************************************
 The clock selection interfaces are available. 
*********************************************************************************************************/

#define _POSIX_CLOCK_SELECTION              200809L

/*********************************************************************************************************
 Advisory information interfaces could be available in future. 
*********************************************************************************************************/

#define _POSIX_ADVISORY_INFO                0

/*********************************************************************************************************
 IPv6 support is available. 
*********************************************************************************************************/

#define _POSIX_IPV6                         200809L

/*********************************************************************************************************
 Raw socket support is available. 
*********************************************************************************************************/

#define _POSIX_RAW_SOCKETS                  200809L

/*********************************************************************************************************
 We have at least one terminal. 
*********************************************************************************************************/

#define _POSIX2_CHAR_TERM                   200809L

/*********************************************************************************************************
 Neither process nor thread sporadic server interfaces is available. 
*********************************************************************************************************/

#undef _POSIX_SPORADIC_SERVER
#undef _POSIX_THREAD_SPORADIC_SERVER

/*********************************************************************************************************
 trace.h is not available. 
*********************************************************************************************************/

#undef _POSIX_TRACE
#undef _POSIX_TRACE_EVENT_FILTER
#undef _POSIX_TRACE_INHERIT
#undef _POSIX_TRACE_LOG

/*********************************************************************************************************
 Typed memory objects are not available. 
*********************************************************************************************************/

#undef _POSIX_TYPED_MEMORY_OBJECTS

/*********************************************************************************************************
 Support for priority inheritance or protection.
*********************************************************************************************************/

#define _POSIX_THREAD_PRIO_INHERIT          1
#define _POSIX_THREAD_PRIO_PROTECT          1

#endif
/*********************************************************************************************************
  END
*********************************************************************************************************/
