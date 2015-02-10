
// QuoteTester.cpp : 定義應用程式的類別行為。
//

#include "stdafx.h"
#include "QuoteTester.h"
#include "QuoteTesterDlg_New_UI.h"
#include "test_dlg.h"
#include "psapi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*DWORD g_hThreadID;
HANDLE g_hThread_Order;
DWORD WINAPI ThreadProc1( LPVOID lpParameter );
HWND g_hWnd = NULL;*/
// CQuoteTesterApp

BEGIN_MESSAGE_MAP(CQuoteTesterApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


/*20150109 added by michael*/
int CQuoteTesterApp::Close_OrderTester ( )
{
	// Get the list of process identifiers.

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;

    if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
    {
        return 1;
    }


    // Calculate how many process identifiers were returned.

    cProcesses = cbNeeded / sizeof(DWORD);
	HANDLE hProcess;
	HMODULE hMod;
	TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");
	string TerminateMe = "OrderTester.exe";

	for ( i = 0; i < cProcesses; i++ )
	{
		if( aProcesses[i] != 0 )
		{
			//PrintProcessNameAndID( aProcesses[i] );
			hProcess = OpenProcess( PROCESS_TERMINATE | PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, aProcesses[i] );
			if (NULL != hProcess ) {
				if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) ) {
					GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName)/sizeof(TCHAR) );
					if ( TerminateMe == szProcessName ) {
						::TerminateProcess ( hProcess, 0 );
						TRACE ( szProcessName );
					}
				}
			}
			CloseHandle( hProcess );
		}
	}
	return 0;
}
// CQuoteTesterApp 建構

CQuoteTesterApp::CQuoteTesterApp()
{
	// 支援重新啟動管理員
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此加入建構程式碼，
	// 將所有重要的初始設定加入 InitInstance 中
}


// 僅有的一個 CQuoteTesterApp 物件

CQuoteTesterApp theApp;


// CQuoteTesterApp 初始設定

BOOL CQuoteTesterApp::InitInstance()
{
	// 假如應用程式資訊清單指定使用 ComCtl32.dll 6 (含) 以後版本，
	// 來啟動視覺化樣式，在 Windows XP 上，則需要 InitCommonControls()。
	// 否則任何視窗的建立都將失敗。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 設定要包含所有您想要用於應用程式中的
	// 通用控制項類別。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 建立殼層管理員，以防對話方塊包含
	// 任何殼層樹狀檢視或殼層清單檢視控制項。
	//CShellManager *pShellManager = new CShellManager;

	// 標準初始設定
	// 如果您不使用這些功能並且想減少
	// 最後完成的可執行檔大小，您可以
	// 從下列程式碼移除不需要的初始化常式，
	// 變更儲存設定值的登錄機碼
	// TODO: 您應該適度修改此字串
	// (例如，公司名稱或組織名稱)
	SetRegistryKey(_T("本機 AppWizard 所產生的應用程式"));

	Close_OrderTester();
	CQuoteTesterDlg_New_UI dlg;
	m_pMainWnd = &dlg;
	//g_hThread_Order = ::CreateThread( NULL, 0, ThreadProc1, this, 0, &g_hThreadID );
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置於使用 [確定] 來停止使用對話方塊時
		// 處理的程式碼
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置於使用 [取消] 來停止使用對話方塊時
		// 處理的程式碼
		/*::PostThreadMessage ( dlg.m_Order_operator1.g_ThreadID, WM_DESTROY, 0, 0 );
		::PostThreadMessage ( dlg.m_Order_operator1.g_ThreadID, WM_QUIT, 0, 0 );
		::WaitForSingleObject ( dlg.m_Order_operator1.g_hThread_Order, INFINITE );
		::PostThreadMessage ( dlg.m_Order_operator2.g_ThreadID, WM_DESTROY, 0, 0 );
		::PostThreadMessage ( dlg.m_Order_operator2.g_ThreadID, WM_QUIT, 0, 0 );
		::WaitForSingleObject ( dlg.m_Order_operator2.g_hThread_Order, INFINITE );*/
	}
	if (  dlg.m_Order_operator1.m_process_info.hProcess != NULL )
		TerminateProcess (  dlg.m_Order_operator1.m_process_info.hProcess, 0 );
	if (  dlg.m_Order_operator2.m_process_info.hProcess != NULL )
		TerminateProcess (  dlg.m_Order_operator2.m_process_info.hProcess, 0 );

	// 刪除上面所建立的殼層管理員。
	/*if (pShellManager != NULL)
	{
		delete pShellManager;
	}*/

	// 因為已經關閉對話方塊，傳回 FALSE，所以我們會結束應用程式，
	// 而非提示開始應用程式的訊息。
	return FALSE;
}

/*DWORD WINAPI ThreadProc1( LPVOID lpParameter ) {
	BOOL bRet = FALSE;
	MSG msg;	
	HACCEL hAccTable;
	Ctest_dlg *m_pdlgAbout = NULL;

	m_pdlgAbout = new Ctest_dlg ();
	m_pdlgAbout->Create ( Ctest_dlg::IDD, NULL );
	g_hWnd = m_pdlgAbout->m_hWnd;
	m_pdlgAbout->ShowWindow(SW_SHOW);

	while ( ( bRet = ::GetMessage ( &msg, NULL, 0, 0 ) ) ) {
		if ( ( bRet = ::TranslateMessage ( &msg ) ) == TRUE ) {
			if ( msg.message == WM_KEYUP )
				TRACE ( "WM_SYSCOMMAND\n" );		
		}
		else {
		
		}
		if ( msg.message == WM_CHAR )
			TRACE ( "WM_CHAR\n" );
		else
			if ( msg.message == WM_CLOSE )
				TRACE ( "WM_CLOSE\n" );
			else
				if ( msg.message == WM_DESTROY )
					TRACE ( "WM_DESTROY\n" );
		//::TranslateMessage ( &msg );
		::DispatchMessage ( &msg );
	}
	return 0;
}*/
