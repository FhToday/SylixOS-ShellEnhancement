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
** 文   件   名: inlObject.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2006 年 12 月 17 日
**
** 描        述: 这是系统对象函数。

** BUG
2007.11.21  修改注释.
2012.12.07  修正 _ObjectClassOK() 对于全局对象解析的错误.
            global 的判断不再放在句柄中.
*********************************************************************************************************/

#ifndef __INLOBJECT_H
#define __INLOBJECT_H

/*********************************************************************************************************
  _ObjectGetIndex
*********************************************************************************************************/

static LW_INLINE UINT16 _ObjectGetIndex (LW_OBJECT_ID    ulId)
{
    return  ((UINT16)(ulId & 0x0000ffff));
}

/*********************************************************************************************************
  _ObjectGetNode
*********************************************************************************************************/

static LW_INLINE ULONG _ObjectGetNode (LW_OBJECT_ID    ulId)
{
	return  ((ULONG)((ulId >> 16) & 0x03ff));
}

/*********************************************************************************************************
  _ObjectGetClass
*********************************************************************************************************/

static LW_INLINE ULONG _ObjectGetClass (LW_OBJECT_ID    ulId)
{
    return  ((ULONG)((ulId >> 26) & 0x3f));
}

/*********************************************************************************************************
  _ObjectClassOK
*********************************************************************************************************/

static LW_INLINE INT   _ObjectClassOK (LW_OBJECT_ID    ulId, UINT8    ucClass)
{
    ulId = ((ulId >> 26) & 0x3f);
    
    return  (ulId == ucClass);
}

/*********************************************************************************************************
  _ObjectCloseId
*********************************************************************************************************/

static LW_INLINE ULONG _ObjectCloseId (LW_OBJECT_ID    *pulId)
{
    *pulId = 0;
    
    return  (0);
}

#endif                                                                  /*  __INLOBJECT_H               */
/*********************************************************************************************************
  END
*********************************************************************************************************/
