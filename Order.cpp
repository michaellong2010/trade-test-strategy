#include "stdafx.h"
#include "Order.h"


DWORD WINAPI ThreadProc( LPVOID lpParameter );
BOOL CALLBACK EnumChildProc( HWND hwnd, LPARAM lParam );
char g_Wnd_title[100];

int COrder::m_Account_count = 0;
HANDLE CCapitalOrder::g_hEvent_Account_Ready = NULL;
COrder::COrder() {
	g_hThread_Order = g_hWnd = NULL;
	g_ThreadID = 0;
	m_Account_index = m_Account_count++;
	memset ( &m_process_info, 0, sizeof ( PROCESS_INFORMATION ) );
}

COrder::~COrder() {
}

void COrder::Create_Order_UI () {

}

void COrder::SetUIVisibility ( BOOL visible )
{
	if ( g_hWnd != NULL )
		::ShowWindow ( g_hWnd, visible );
}

CCapitalOrder::~CCapitalOrder() {
	//delete m_pDlgOrder;
}

int CCapitalOrder::LoginAccount ( const CHAR *ID, const CHAR *Pass )
{
	DWORD dwWaitResult = -1;
	strID = ID;
	strPass = Pass;
	COPYDATASTRUCT cds;
	TLogin login_info;
	 
	//COrderTesterDlg *pOrder_cwnd = NULL;
	//pOrder_cwnd = ( COrderTesterDlg * ) CWnd::FromHandle ( g_hWnd );

	if ( m_pDlgOrder != NULL ) {
		//pOrder_cwnd->m_editID.SetWindowText ( ID );
		//pOrder_cwnd->m_editPassWord.SetWindowText ( Pass );
		//m_pDlgOrder->PostMessage ( WM_LOGIN, (WPARAM) strID.AllocSysString (), (LPARAM) strPass.AllocSysString () );
		strcpy ( login_info.LoginID, strID.GetString () );
		strcpy ( login_info.Password, strPass.GetString () );
		cds.cbData = sizeof ( TLogin );
		cds.lpData = & login_info;
		cds.dwData = LOGIN_COPYDATA;
		::SendMessage ( m_pDlgOrder->m_hWnd, WM_COPYDATA, (WPARAM) AfxGetMainWnd()->m_hWnd, (LPARAM) &cds );
		//::PostThreadMessage ( g_ThreadID, WM_LOGIN, (WPARAM) strID.AllocSysString (), (LPARAM) strPass.AllocSysString () );
		if ( ::GetCurrentThreadId () != this->g_ThreadID ) {
			dwWaitResult = ::WaitForSingleObject ( g_hEvent_Account_Ready, 10000 );
		}
	}
	if ( dwWaitResult == WAIT_TIMEOUT ) {
		SetUIVisibility ( SW_SHOW );
		return -1;
	}
	else
		if ( dwWaitResult == WAIT_OBJECT_0 ) {
			SetUIVisibility ( SW_SHOW );
			return 0;
		}

	return 0;
}

void CCapitalOrder::SetUIVisibility ( BOOL visible )
{
	COrder::SetUIVisibility ( visible );
}

CCapitalOrder::CCapitalOrder() {
	DWORD dwWaitResult = -1;

	if ( g_hEvent_Account_Ready == NULL )
		g_hEvent_Account_Ready = ::CreateEvent ( NULL, FALSE, FALSE, "Global\\OrderAccountReady" );

	//g_hThread_Order = ::CreateThread( NULL, 0, ThreadProc, this, 0, &g_ThreadID );
	m_pDlgOrder = NULL;
	TCHAR exe_App_name[] = _T("\"C:\\Program Files\\WinMerge\\WinMergeU.exe\"");
	HWND m_ProcessMainHwnd;
	DWORD m_tid, m_pid;

	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof( si );
	
	if( !CreateProcess( NULL,   // No module name (use command line)
		//argv[1],        // Command line
		_T ( "\"J:\\2014 VC project\\Trade_Test\\Debug\\OrderTester.exe\"" ) ,
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		NULL,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi )           // Pointer to PROCESS_INFORMATION structure
		) 
	{
		TRACE( "CreateProcess failed (%d).\n", GetLastError() );
		//return -2;
	}
	else {
		g_hThread_Order = pi.hThread;
		g_ThreadID = pi.dwThreadId;
		m_process_info = pi;
		TRACE ( RUNTIME_CLASS( COrderTesterDlg )->m_lpszClassName );
		dwWaitResult = ::WaitForSingleObject ( g_hEvent_Account_Ready, 10000 );
		if ( dwWaitResult == WAIT_OBJECT_0 ) {			
			m_ProcessMainHwnd = ::FindWindow ( RUNTIME_CLASS( COrderTesterDlg )->m_lpszClassName, NULL );
			m_ProcessMainHwnd = ::FindWindow ( NULL, "OrderTester" );
			m_tid = ::GetWindowThreadProcessId ( m_ProcessMainHwnd, &m_pid );
			while ( ( m_ProcessMainHwnd != NULL ) && ( pi.dwThreadId != m_tid || pi.dwProcessId != m_pid ) )
				m_ProcessMainHwnd = ::FindWindowEx ( NULL, m_ProcessMainHwnd, NULL, NULL );
			if ( pi.dwThreadId == m_tid && pi.dwProcessId == m_pid ) {
				this->m_pDlgOrder = ( COrderTesterDlg * ) CWnd::FromHandle ( m_ProcessMainHwnd );
				this->g_hWnd = m_ProcessMainHwnd;
				::EnumChildWindows ( g_hWnd, EnumChildProc, 100 );
			}
		}
	}
}

/*BOOL CALLBACK EnumChildProc(          HWND hwnd,
    LPARAM lParam
);*/
BOOL CALLBACK EnumChildProc( HWND hwnd, LPARAM lParam )
{
	CWnd *pCWnd = NULL;

	pCWnd = CWnd::FromHandle ( hwnd );
	if ( lParam == 100 ) {
		/*::GetWindowText ( hwnd, g_Wnd_title, 100 ) ;
		TRACE ( g_Wnd_title );
		TRACE ( "\n" );*/		
		if ( pCWnd && pCWnd->GetDlgCtrlID () == IDC_LIST_REPORTS )
			::EnableWindow ( hwnd, TRUE );
		else
			::EnableWindow ( hwnd, FALSE );
		return TRUE;
	}
	else
		if ( lParam == 101 ) {
			return TRUE;
		}
	return FALSE;
}


void CCapitalOrder::Create_Order_UI () {
	COrder::Create_Order_UI ();

	//COrderTesterDlg *m_pDlgOrder = NULL;
	m_pDlgOrder = new COrderTesterDlg ();
	m_pDlgOrder->Create ( COrderTesterDlg::IDD );
	m_pDlgOrder->ShowWindow( SW_SHOW );
	g_hWnd = m_pDlgOrder->m_hWnd;
	m_pDlgOrder->SetDlgCtrlID ( IDD_ORDERTESTER_DIALOG );
	TRACE ( "%d", m_pDlgOrder->GetDlgCtrlID( ) );

	/*BOOL EnumChildWindows(          HWND hWndParent,
    WNDENUMPROC lpEnumFunc,
    LPARAM lParam
);*/

	::EnumChildWindows ( g_hWnd, EnumChildProc, 100 );
}

DWORD WINAPI ThreadProc( LPVOID lpParameter ) {
	CCapitalOrder *m_pOrder_operator;
	BOOL bRet = FALSE;
	MSG msg;

	m_pOrder_operator = ( CCapitalOrder * ) lpParameter;
	m_pOrder_operator->Create_Order_UI ();

	while ( ( bRet = ::GetMessage ( &msg, NULL, 0, 0 ) ) ) {
		if ( msg.message == WM_SYSCOMMAND )
			TRACE ( "WM_SYSCOMMAND\n" );
		else
			if ( msg.message == WM_CLOSE )
				TRACE ( "WM_CLOSE\n" );
			else
				if ( msg.message == WM_DESTROY ) {
					::DestroyWindow ( m_pOrder_operator->g_hWnd );
					TRACE ( "WM_DESTROY\n" );
				}
				else
					if ( msg.message == WM_TIMER )
						TRACE ( "time out\n" );
		//::TranslateMessage ( &msg );
		if ( ( bRet = ::TranslateMessage ( &msg ) ) == TRUE ) {
			TRACE ( "%x %x\n", msg.message, msg.wParam );
		}
		else {
		::DispatchMessage ( &msg );
		}
	}
	return 0;
}