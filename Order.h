#include <resource.h>
#include <OrderTesterDlg.h>

#pragma once


class COrder {
public:
	COrder();
	~COrder();
	
	HANDLE g_hThread_Order;
	HWND g_hWnd;
	DWORD g_ThreadID;
	CStringA strID, strPass;
/*20141227 added by michael*/
	static int m_Account_count;
	int m_Account_index;

	virtual void Create_Order_UI ();
	void SetUIVisibility ( BOOL visible );
/*20141230 added by michael*/
	PROCESS_INFORMATION m_process_info;
protected:
	//void SetLoginIDPassword ();
	CWnd *m_pOrder_cwnd;
};

class CCapitalOrder : public COrder {
public:
	CCapitalOrder();
	~CCapitalOrder();
	virtual void Create_Order_UI ();
	void SetUIVisibility ( BOOL visible );
	virtual int LoginAccount ( const CHAR *ID, const CHAR *Pass );

	COrderTesterDlg *m_pDlgOrder;
/*20150102 added by name*/
	static HANDLE g_hEvent_Account_Ready;
protected:
};