
// QuoteTester.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CQuoteTesterApp:
// �аѾ\��@�����O�� QuoteTester.cpp
//

class CQuoteTesterApp : public CWinApp
{
public:
	CQuoteTesterApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CQuoteTesterApp theApp;