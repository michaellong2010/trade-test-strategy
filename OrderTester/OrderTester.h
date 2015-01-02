
// OrderTester.h : main header file for the PROJECT_NAME application
//
#include "LimitSingleInstance.h"

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// COrderTesterApp:
// See OrderTester.cpp for the implementation of this class
//

class COrderTesterApp : public CWinApp
{
	DECLARE_DYNAMIC( COrderTesterApp )
public:
	COrderTesterApp();
	~COrderTesterApp();
// Overrides
public:
	virtual BOOL InitInstance();
	virtual BOOL PumpMessage();     // low level message pump
	/*20141231 added by michael
	limit app instance number*/
	int m_nInstanceID;
	CLimitSingleInstance *m_pFirstInstanceObj, *m_pSecondInstanceObj;

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern COrderTesterApp theApp;