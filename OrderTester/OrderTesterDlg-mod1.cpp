
// OrderTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderTester.h"
#include "OrderTesterDlg.h"
//#include "afxdialogex.h"

#include "Functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

/*class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};*/

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CAboutDlg::PreTranslateMessage(MSG* pMsg) {
	return CDialog::PreTranslateMessage ( pMsg );
}

void CAboutDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	return CDialog::OnSysCommand ( nID, lParam );
}

void CAboutDlg::OnClose() {
	HWND m_Parent_hwnd = NULL, m_owner_hwnd = NULL, m_Quote_hwnd1 = NULL;
	m_Parent_hwnd = ::GetParent ( this->m_hWnd );
	m_owner_hwnd = ::GetWindow ( this->m_hWnd, GW_OWNER );

	//m_Quote_hwnd1 = ::FindWindowEx( NULL, this->m_hWnd, NULL, NULL );
	//this->EnableWindow ( false );
	//::SetParent ( this->m_hWnd, HWND_MESSAGE );
	CWnd::OnClose();
	return;
}

BOOL CAboutDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
	return CWnd::OnWndMsg ( message, wParam, lParam, pResult );
}

extern DWORD m_tid;
extern HANDLE m_thread_hnd;
LRESULT CAboutDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	DWORD tid = ::GetCurrentThreadId();

	if ( message == WM_CLOSE )
		TRACE ( "WM_CLOSE, %d\n", m_tid, tid );
	else
		if ( message == WM_DESTROY )
			TRACE ( "WM_DESTROY\n" );
		else
			if ( message == WM_SYSCOMMAND )
				TRACE ( "WM_SYSCOMMAND, %d, %d\n", wParam, SC_CLOSE );
			else
				if ( message == WM_CHAR )
					TRACE ( "WM_CHAR, %d %d\n", WM_CHAR, wParam );
				else
					if (message == WM_MESSAGE )
						TRACE ( "WM_MESSAGE\n" );
	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CAboutDlg::OnInitDialog() {
	HDESK m_desk_hwnd = ::GetThreadDesktop( ::GetCurrentThreadId() );
	return CDialog::OnInitDialog();
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// COrderTesterDlg dialog




COrderTesterDlg::COrderTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COrderTesterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void COrderTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TSACCOUNT, m_boxTSAccount);
	DDX_Control(pDX, IDC_COMBO_TFACCOUNT, m_boxTFAccount);
	DDX_Control(pDX, IDC_COMBO_OSACCOUNT, m_boxOSAccount);
	DDX_Control(pDX, IDC_COMBO_OFACCOUNT, m_boxOFAccount);
	DDX_Control(pDX, IDC_EDIT_ID, m_editID);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassWord);
	DDX_Control(pDX, IDC_LIST_REPORTS, m_listReports);
	DDX_Control(pDX, IDC_TAB_CONTROL, m_TabControl);
}

BEGIN_MESSAGE_MAP(COrderTesterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MESSAGE,&COrderTesterDlg::OnMessage)
	ON_BN_CLICKED(IDC_BUTTON_INIT, &COrderTesterDlg::OnBnClickedButtonInit)
	ON_CBN_SELCHANGE(IDC_COMBO_TSACCOUNT, &COrderTesterDlg::OnSelchangeComboTsaccount)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &COrderTesterDlg::OnBnClickedButtonClear)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_CONTROL, &COrderTesterDlg::OnTcnSelchangeTabControl)
	ON_NOTIFY(TCN_SELCHANGING, IDC_TAB_CONTROL, &COrderTesterDlg::OnTcnSelchangingTabControl)
	ON_CBN_SELCHANGE(IDC_COMBO_TFACCOUNT, &COrderTesterDlg::OnCbnSelchangeComboTfaccount)
	ON_CBN_SELCHANGE(IDC_COMBO_OSACCOUNT, &COrderTesterDlg::OnCbnSelchangeComboOsaccount)
	ON_CBN_SELCHANGE(IDC_COMBO_OFACCOUNT, &COrderTesterDlg::OnCbnSelchangeComboOfaccount)
END_MESSAGE_MAP()


// COrderTesterDlg message handlers

BOOL COrderTesterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	m_TabControl.InsertItem( 0, _T("STOCK (證券)"));
	m_TabControl.InsertItem( 1, _T("FUTURE (期貨)"));
	m_TabControl.InsertItem( 2, _T("OPTION (選擇權)"));
	m_TabControl.InsertItem( 3, _T("FOREIGNSTOCK (複委託)"));
	m_TabControl.InsertItem( 4, _T("FUTURESTOPLOSS (期貨停損)"));
	m_TabControl.InsertItem( 5, _T("OPTIONSTOPLOSS (選擇權停損)"));
	m_TabControl.InsertItem( 6, _T("OVERSEAFUTURE (海外期貨)"));
	m_TabControl.InsertItem( 7, _T("OVERSEAOPTION (海外選擇權)"));

	m_StockDlg.Create( IDD_DIALOG_STOCK, GetDlgItem( IDC_TAB_CONTROL));
	m_FutureDlg.Create( IDD_DIALOG_FUTURE, GetDlgItem( IDC_TAB_CONTROL));
	m_OptionDlg.Create( IDD_DIALOG_OPTION, GetDlgItem( IDC_TAB_CONTROL));
	m_ForeignStockDlg.Create( IDD_DIALOG_FOREIGNSTOCK, GetDlgItem( IDC_TAB_CONTROL));
	m_FutureStopLossDlg.Create( IDD_DIALOG_FUTURESTOPLOSS, GetDlgItem( IDC_TAB_CONTROL));
	m_OptionStopLossDlg.Create( IDD_DIALOG_OPTIONSTOPLOSS, GetDlgItem( IDC_TAB_CONTROL));
	m_OverseaFutureDlg.Create( IDD_DIALOG_OVERSEAFUTURE, GetDlgItem( IDC_TAB_CONTROL));
	m_OverseaOptionDlg.Create( IDD_DIALOG_OVERSEAOPTION, GetDlgItem( IDC_TAB_CONTROL));

	CRect rs;
	m_TabControl.GetClientRect( rs);
	rs.top+=40;
	rs.bottom-=4;
	rs.left+=4;
	rs.right-=4;

	m_StockDlg.MoveWindow(rs);
	m_FutureDlg.MoveWindow(rs);
	m_OptionDlg.MoveWindow(rs);
	m_ForeignStockDlg.MoveWindow(rs);
	m_FutureStopLossDlg.MoveWindow(rs);
	m_OptionStopLossDlg.MoveWindow(rs);
	m_OverseaFutureDlg.MoveWindow(rs);
	m_OverseaOptionDlg.MoveWindow(rs);

	m_TabControl.SetCurSel(0);
	m_StockDlg.ShowWindow(TRUE);

	m_bRegister = false;

	HWND m_Quote_hwnd = ::FindWindow(NULL,_T("QuoteTester"));	
	HWND m_Parent = NULL, m_Quote_hwnd1 = NULL;
	m_Parent = ::GetParent ( m_Quote_hwnd );
	m_Quote_hwnd1 = ::FindWindowEx( NULL, m_Quote_hwnd, NULL, NULL );
	BOOL m_ischild = FALSE;
	m_ischild = ::IsChild( ::GetDesktopWindow() , m_Quote_hwnd );
	//::SendMessage ( ::GetWindow(m_Quote_hwnd, GW_CHILD), WM_CLOSE, 0, 0 );	
	//::SendMessage(m_Quote_hwnd1, WM_SYSCOMMAND, SC_CLOSE, 0);
	//::PostThreadMessage ( m_tid, WM_CLOSE, 0, 0 );
	/*HWND m_msg_only_hwnd = NULL;
	if ( m_Quote_hwnd )
		m_msg_only_hwnd = ::SetParent( ::GetWindow(m_Quote_hwnd,GW_CHILD), HWND_MESSAGE );
	::CloseWindow ( m_Quote_hwnd );*/
	//if ( ::GetParent( about_dlg.m_hWnd ) == NULL )
		//::SetParent( about_dlg.m_hWnd, this->m_hWnd );
	HDESK m_desk_hwnd = ::GetThreadDesktop( ::GetCurrentThreadId() );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COrderTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
		/*CAboutDlg *m_pdlgAbout = NULL;
		m_pdlgAbout = new CAboutDlg ();
		m_pdlgAbout->Create ( CAboutDlg::IDD, GetDesktopWindow() );
		m_pdlgAbout->ShowWindow(SW_SHOW);*/
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void COrderTesterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR COrderTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT COrderTesterDlg::OnMessage(WPARAM wParam,LPARAM lParam)
{
	BSTR bstrData = (BSTR)lParam;

	CString strReport( bstrData);
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	
	pDlg->AddReport( strReport);

	SysFreeString(bstrData);

	return 0;
}

void __stdcall OnBSTRCallBack( BSTR bstrData)
{
	//SendMessage(FindWindow(NULL,_T("OrderTester")),WM_MESSAGE,0,(long)bstrData);
	TRACE("Run in thread: %x\n", GetCurrentThreadId());

	TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	CString strMsg(bstrData);

	PostMessage(FindWindow(NULL,_T("OrderTester")),WM_MESSAGE,0,(long)strMsg.AllocSysString());
}

void __stdcall OnAccountCallBack( BSTR bstrData)
{
	TRACE("Run in thread: %x\n", GetCurrentThreadId());

	TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	CString strAccount( bstrData);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	pDlg->AddAccount( strAccount);

	pDlg->AddReport( strAccount);

}

void __stdcall OnAccountWithLoginIDCallBack( BSTR bstrLogInID, BSTR bstrData)
{
	TRACE("Run in thread: %x\n", GetCurrentThreadId());

	TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	CString strID(bstrLogInID);
	CString strAccount( bstrData);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	pDlg->AddAccount( strAccount);

	pDlg->AddReport( strID + _T("   ") + strAccount);

}

void __stdcall OnOrderAsyncCallBack( DWORD dwThreadID, int nCode, BSTR bstrMessage)
{
	CString strMsg;
	strMsg.Format( _T("ThradID: %d  Code: %d  Message: %s"),dwThreadID,nCode,CString(bstrMessage));

	
	SendMessage(FindWindow(NULL,_T("OrderTester")),WM_MESSAGE,0,(long)strMsg.AllocSysString());

	/*COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	pDlg->AddReport( strMsg );*/
}

void __stdcall OnExecutionReportCallBack ( BSTR bstrData)
{
	CString strMsg;
	strMsg.Format( _T("current ThradID: %d  main ThreadID: %d  Message: %s"),::GetCurrentThreadId ( ), AfxGetApp()->m_nThreadID, CString( bstrData ));


	SendMessage(FindWindow(NULL,_T("OrderTester")),WM_MESSAGE,0,(long)strMsg.AllocSysString());
}

extern HWND g_hWnd;
void COrderTesterDlg::OnBnClickedButtonInit()
{
	// TODO: Add your control notification handler code here
	try
	{
		CString strAccount;
		m_editID.GetWindowText(strAccount);

		CString strPassWord;
		m_editPassWord.GetWindowText(strPassWord);

		int nCode = SKOrderLib_Initialize( (LPCSTR)CStringA(strAccount), (LPCSTR)CStringA(strPassWord) );

		if ( nCode == 0)
		{
			if( m_bRegister == false )
			{
				RegisterOnAccountCallBack( (long)OnAccountCallBack );
				RegisterOnOrderAsyncReportCallBack( (long)OnOrderAsyncCallBack);
				RegisterOnAccountWithLoginIDCallBack( (long)OnAccountWithLoginIDCallBack );
				RegisterOnRealBalanceReportCallBack( (long)OnBSTRCallBack );
				RegisterOnOpenInterestCallBack( (long)OnBSTRCallBack );
				RegisterOnStopLossReportCallBack( (long)OnBSTRCallBack );
				RegisterOnOverseaFuturesCallBack( (long)OnBSTRCallBack );
				RegisterOnOverseaFutureOpenInterestCallBack( (long)OnBSTRCallBack );
				RegisterOnOverseaOptionsCallBack( (long)OnBSTRCallBack );
				RegisterOnFutureRightCallBack( (long)OnBSTRCallBack );
				RegisterOnCertCanceledCallBack( (long)OnBSTRCallBack );
				RegisterOnExecutionReportCallBack ( ( long ) OnExecutionReportCallBack );


				m_bRegister = true;
			}

			GetUserAccount();

			MessageBox(_T("SUCCESS"));

			int nCert = SKOrderLib_ReadCertByID( (LPTSTR)(LPCTSTR)strAccount );

			if( nCert == 0 )
				MessageBox(_T("READ CERT SUCCESS"));
			else
				MessageBox(_T("READ CERT FALE"));
		}
	}
	catch ( char* pMsg)
	{
		MessageBox( CString(pMsg), _T("Initialize Error"), MB_ICONASTERISK|MB_TOPMOST);
	}
	catch (...)
	{
	
	}
	//::SendMessage ( m_hWnd, WM_SYSCOMMAND, IDM_ABOUTBOX, 0 );
	//::PostMessage ( g_hWnd, WM_CLOSE, 0, 0 );
	//::PostThreadMessage ( m_tid, WM_CLOSE, 0, 0 );
	CString strMsg = "knight";
	::PostMessage(this->m_hWnd,WM_MESSAGE,0,(long)strMsg.AllocSysString());
}

vector<CString> COrderTesterDlg::DivideByComma( CString strBuf)
{
   vector<CString> Values;

   CString strTemp;

   while ( true)
   {
       strTemp = strBuf.Find(_T(","))==-1?
           strBuf:strBuf.Left( strBuf.Find(_T(",")));

       //if ( !strTemp.IsEmpty())
           Values.push_back( strTemp);

       if ( strBuf.Find(_T(",")) == -1)
           break;

       strBuf = strBuf.Mid( strBuf.Find(_T(","))+1, strBuf.GetLength()-1);
   }

   return Values;
}

void COrderTesterDlg::AddAccount(const CString& strAccount)
{
	vector<CString> Items = DivideByComma( strAccount);

	if ( Items.size() >= 6)
	{
		CString strFullAccount = Items[1] + Items[3];

		if ( Items[0] == "TS")
		{
			if ( m_boxTSAccount.FindStringExact( -1, strFullAccount) == CB_ERR)
			{
				m_boxTSAccount.AddString( strFullAccount);
			}
		}
		else if ( Items[0] == _T("TF"))
		{
			if ( m_boxTFAccount.FindStringExact( -1, strFullAccount) == CB_ERR)
			{
				m_boxTFAccount.AddString( strFullAccount);
			}
		}
		else if ( Items[0] == _T("OF"))//海期
		{
			if ( m_boxOFAccount.FindStringExact( -1, strFullAccount) == CB_ERR)
			{
				m_boxOFAccount.AddString( strFullAccount);
			}
		}
		else if ( Items[0] == _T("OS"))//複委託帳號(美股、港股下單使用)
		{
			if ( m_boxOSAccount.FindStringExact( -1, strFullAccount) == CB_ERR)
			{
				m_boxOSAccount.AddString( strFullAccount);
			}
		}
	}
}


void COrderTesterDlg::AddReport(const CString& strReports)
{
	m_listReports.AddString( strReports);

	CString    str;
	CSize      sz;
	static int dx = 0;
	TEXTMETRIC tm;
	CDC*       pDC = m_listReports.GetDC();
	CFont*     pFont = m_listReports.GetFont();

	// Select the listbox font, save the old font
	CFont* pOldFont = pDC->SelectObject(pFont);
	// Get the text metrics for avg char width
	pDC->GetTextMetrics(&tm); 

	sz = pDC->GetTextExtent( strReports);

	// Add the avg width to prevent clipping
	sz.cx += tm.tmAveCharWidth;

	if ( sz.cx > dx)
		dx = sz.cx;

	// Select the old font back into the DC
	pDC->SelectObject(pOldFont);
	m_listReports.ReleaseDC(pDC);

	// Set the horizontal extent so every character of all strings 
	// can be scrolled to.
	m_listReports.SetHorizontalExtent(dx);
}


void COrderTesterDlg::OnSelchangeComboTsaccount()
{
	// TODO: Add your control notification handler code here
	CString strText;

	m_boxTSAccount.GetLBText( m_boxTSAccount.GetCurSel(), strText);

	m_StockDlg.SetAccount( strText);	

}

void COrderTesterDlg::OnCbnSelchangeComboTfaccount()
{
	// TODO: Add your control notification handler code here
	CString strText;
	m_boxTFAccount.GetLBText( m_boxTFAccount.GetCurSel(), strText);

	m_FutureDlg.SetAccount(strText);
	m_OptionDlg.SetAccount(strText);
	m_FutureStopLossDlg.SetAccount(strText);
	m_OptionStopLossDlg.SetAccount(strText);
}


void COrderTesterDlg::OnCbnSelchangeComboOsaccount()
{
	// TODO: Add your control notification handler code here
	
	CString strText;
	m_boxOSAccount.GetLBText( m_boxOSAccount.GetCurSel(), strText);
	
	m_ForeignStockDlg.SetAccount(strText);
}


void COrderTesterDlg::OnCbnSelchangeComboOfaccount()
{
	// TODO: Add your control notification handler code here

	CString strText;
	m_boxOFAccount.GetLBText( m_boxOFAccount.GetCurSel(), strText);
	
	m_OverseaFutureDlg.SetAccount(strText);
	m_OverseaOptionDlg.SetAccount(strText);
}

void COrderTesterDlg::OnBnClickedButtonClear()
{
	// TODO: Add your control notification handler code here

	m_listReports.ResetContent();
}


void COrderTesterDlg::OnTcnSelchangeTabControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int nCurrent = m_TabControl.GetCurSel();

	switch ( nCurrent)
	{
		case 0: m_StockDlg.ShowWindow( TRUE); break;
		case 1: m_FutureDlg.ShowWindow( TRUE); break;
		case 2: m_OptionDlg.ShowWindow( TRUE); break;
		case 3: m_ForeignStockDlg.ShowWindow( TRUE); break;
		case 4: m_FutureStopLossDlg.ShowWindow( TRUE); break;
		case 5: m_OptionStopLossDlg.ShowWindow( TRUE); break;
		case 6: m_OverseaFutureDlg.ShowWindow( TRUE); break;
		case 7: m_OverseaOptionDlg.ShowWindow( TRUE); break;
	}

	*pResult = 0;
}


void COrderTesterDlg::OnTcnSelchangingTabControl(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	int nCurrent = m_TabControl.GetCurSel();

	switch ( nCurrent)
	{
		case 0: m_StockDlg.ShowWindow( FALSE); break;
		case 1: m_FutureDlg.ShowWindow( FALSE); break;
		case 2: m_OptionDlg.ShowWindow( FALSE); break;
		case 3: m_ForeignStockDlg.ShowWindow( FALSE); break;
		case 4: m_FutureStopLossDlg.ShowWindow( FALSE); break;
		case 5: m_OptionStopLossDlg.ShowWindow( FALSE); break;
		case 6: m_OverseaFutureDlg.ShowWindow( FALSE); break;
		case 7: m_OverseaOptionDlg.ShowWindow( FALSE); break;
	}

	*pResult = 0;
}

BOOL COrderTesterDlg::PreTranslateMessage(MSG* pMsg) {
	return CDialog::PreTranslateMessage ( pMsg );
}

void CAboutDlg::PostNcDestroy ( )
{
	CDialog::PostNcDestroy();
	delete this;
}

void CAboutDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	// TODO: Add your message handler code here
}
