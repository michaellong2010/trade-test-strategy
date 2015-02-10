
// OrderTester.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "OrderTester.h"
#include "OrderTesterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD m_tid;
HANDLE m_thread_hnd;
DWORD WINAPI ThreadProc( LPVOID lpParameter );
// COrderTesterApp

BEGIN_MESSAGE_MAP(COrderTesterApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC( COrderTesterApp, CWinApp )
// COrderTesterApp construction

COrderTesterApp::~COrderTesterApp()
{
	if ( m_pFirstInstanceObj )
		delete m_pFirstInstanceObj;

	if ( m_pSecondInstanceObj )
		delete m_pSecondInstanceObj;
}

COrderTesterApp::COrderTesterApp()
{
	// support Restart Manager
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	m_pFirstInstanceObj = m_pSecondInstanceObj = NULL;
}


// The one and only COrderTesterApp object

//COrderTesterApp theApp;
CAboutDlg about_dlg;

// COrderTesterApp initialization
// The one and only CLimitSingleInstance object.
//CLimitSingleInstance g_SingleInstanceObj(TEXT("Global\\{05CA3573-B449-4e0b-83F5-7FD612E378E9}"));

BOOL COrderTesterApp::InitInstance()
{
	AfxOleInit();
	m_pFirstInstanceObj = new CLimitSingleInstance ( TEXT("Global\\{05CA3573-B449-4e0b-83F5-7FD612E378E9}") );
	m_nInstanceID = 0;
	if ( m_pFirstInstanceObj->IsAnotherInstanceRunning() ) {
		m_pSecondInstanceObj = new CLimitSingleInstance ( TEXT("Global\\{05CA3573-B449-4e0b-83F5-7FD612E378EA}") );	
		m_nInstanceID = 1;
		if ( m_pSecondInstanceObj->IsAnotherInstanceRunning() ) {
			TRACE ( "max  number of instance 2" );
			return FALSE;
		}
	}
    /*if (g_SingleInstanceObj.IsAnotherInstanceRunning())
       return FALSE;*/


	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains
	// any shell tree view or shell list view controls.
	//CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
//m_thread_hnd = ::CreateThread( NULL, 0, ThreadProc2, this, 0, &m_tid );
	COrderTesterDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Delete the shell manager created above.
	/*if (pShellManager != NULL)
	{
		delete pShellManager;
	}*/

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

HWND g_hWnd = NULL;
DWORD WINAPI ThreadProc2( LPVOID lpParameter ) {
	//INT_PTR nResponse = about_dlg.DoModal();
	BOOL bRet = FALSE;
	CAboutDlg *m_pdlgAbout = NULL;
	MSG msg;
	HACCEL hAccTable;

	m_pdlgAbout = new CAboutDlg ();
	m_pdlgAbout->Create ( CAboutDlg::IDD, NULL );
	g_hWnd = m_pdlgAbout->m_hWnd;
	m_pdlgAbout->ShowWindow(SW_SHOW);
	LONG g_WndProc = ::GetWindowLongPtr ( m_pdlgAbout->m_hWnd, GWL_WNDPROC );
	hAccTable = ::LoadAccelerators ( AfxGetInstanceHandle(), MAKEINTRESOURCE( IDR_ACCELERATOR1 ) );
	while( ( bRet = ::GetMessage( &msg, NULL, 0, 0 ) ) ) {
		if ( msg.message == WM_DESTROY )
			TRACE ( "WM_DESTROY\n" );
		else
			if ( msg.message == WM_CLOSE ) {
				TRACE ( "WM_CLOSE\n" );
				::DestroyWindow ( g_hWnd );
			}
			else
				if ( msg.message == WM_SYSCOMMAND ) {
					TRACE ( "WM_SYSCOMMAND\n" );
				}
				else
					if ( msg.message == WM_MESSAGE )
						TRACE ( "WM_MESSAGE\n" );
		if ( !( bRet = ::TranslateAccelerator ( g_hWnd, hAccTable, &msg ) ) ) {
			if ( ( bRet = ::TranslateMessage ( &msg ) ) == TRUE ) {
				TRACE ( "translate message is comsumed\n" );
				if ( msg.wParam != 0x0d && msg.wParam != 0x1b )
					if ( !::IsDialogMessage ( g_hWnd, &msg ) )
						::DispatchMessage ( &msg );
				//::CallWindowProc( (WNDPROC) g_WndProc, g_hWnd, WM_CHAR, msg.wParam, msg.lParam );
			}
			else {
				TRACE ( "translate message is not comsumed\n" );
				::DispatchMessage ( &msg );
			}
		}
	}
	return 0;
}

BOOL COrderTesterApp::PumpMessage() {
	return CWinThread::PumpMessage ();
}