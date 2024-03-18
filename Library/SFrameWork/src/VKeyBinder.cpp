/*
 * FILE: VKeyBinder.cpp
 *
 * DESCRIPTION: class representing the binding between input key and the message
 *
 * CREATED BY: Hedi, 2002/8/1
 *
 * HISTORY:
 *
 * Copyright (c) 2002-2008 Archosaur Studio, All Rights Reserved.	
 */

#include "VKeyBinder.h"

VKeyBinder::VKeyBinder()
{
	m_nKeyBindNum		= 0;
	m_pKeyBindTable		= NULL;

}

VKeyBinder::~VKeyBinder()
{
}

bool VKeyBinder::Init()
{
	m_nKeyBindNum = 1;

	KEYBIND * pNewBindTable;
	pNewBindTable = (KEYBIND *) realloc(m_pKeyBindTable, sizeof(KEYBIND) * m_nKeyBindNum);
	if( NULL == pNewBindTable )
		return false;

	m_pKeyBindTable = pNewBindTable;

	memset(&m_pKeyBindTable[0], 0, sizeof(KEYBIND));
	m_pKeyBindTable[0].vkey[0]		= VK_ESCAPE;
	m_pKeyBindTable[0].dwMsgID		= GAMEMSG_EXIT;
	return true;
}

bool VKeyBinder::Release()
{
	if( m_pKeyBindTable )
	{
		free(m_pKeyBindTable);
		m_pKeyBindTable = NULL;
	}

	return true;
}

bool VKeyBinder::GetKeyBind(int index, KEYBIND * pKeyBind)
{
	if( index < m_nKeyBindNum )
	{
		*pKeyBind = m_pKeyBindTable[index];
		return true;
	}

	return false;
}