
// QuoteTester.cpp : �w�q���ε{�������O�欰�C
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
// CQuoteTesterApp �غc

CQuoteTesterApp::CQuoteTesterApp()
{
	// �䴩���s�Ұʺ޲z��
	//m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �b���[�J�غc�{���X�A
	// �N�Ҧ����n����l�]�w�[�J InitInstance ��
}


// �Ȧ����@�� CQuoteTesterApp ����

CQuoteTesterApp theApp;


// CQuoteTesterApp ��l�]�w

BOOL CQuoteTesterApp::InitInstance()
{
	// ���p���ε{����T�M����w�ϥ� ComCtl32.dll 6 (�t) �H�᪩���A
	// �ӱҰʵ�ı�Ƽ˦��A�b Windows XP �W�A�h�ݭn InitCommonControls()�C
	// �_�h����������إ߳��N���ѡC
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �]�w�n�]�t�Ҧ��z�Q�n�Ω����ε{������
	// �q�α�����O�C
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// �إߴ߼h�޲z���A�H����ܤ���]�t
	// ����߼h���˵��δ߼h�M���˵�����C
	//CShellManager *pShellManager = new CShellManager;

	// �зǪ�l�]�w
	// �p�G�z���ϥγo�ǥ\��åB�Q���
	// �̫᧹�����i�����ɤj�p�A�z�i�H
	// �q�U�C�{���X�������ݭn����l�Ʊ`���A
	// �ܧ��x�s�]�w�Ȫ��n�����X
	// TODO: �z���ӾA�׭ק惡�r��
	// (�Ҧp�A���q�W�٩β�´�W��)
	SetRegistryKey(_T("���� AppWizard �Ҳ��ͪ����ε{��"));

	Close_OrderTester();
	CQuoteTesterDlg_New_UI dlg;
	m_pMainWnd = &dlg;
	//g_hThread_Order = ::CreateThread( NULL, 0, ThreadProc1, this, 0, &g_hThreadID );
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �b����m��ϥ� [�T�w] �Ӱ���ϥι�ܤ����
		// �B�z���{���X
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �b����m��ϥ� [����] �Ӱ���ϥι�ܤ����
		// �B�z���{���X
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

	// �R���W���ҫإߪ��߼h�޲z���C
	/*if (pShellManager != NULL)
	{
		delete pShellManager;
	}*/

	// �]���w�g������ܤ���A�Ǧ^ FALSE�A�ҥH�ڭ̷|�������ε{���A
	// �ӫD���ܶ}�l���ε{�����T���C
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
