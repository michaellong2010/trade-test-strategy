
// OrderTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderTester.h"
#include "OrderTesterDlg.h"
#include <Account.h>
//#include "afxdialogex.h"

#include "Functions.h"
#include <comutil.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD swat;
CString g_strWndTitle;
HWND g_hWnd_OrderTesterDlg = NULL;
int COrderTesterDlg::m_Account_count = 0;
VOID CALLBACK TimerProc1( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime );
HANDLE g_hEvent_FutureRight_Ready = NULL, g_hEvent_OpenInterest_Ready = NULL;
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

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// COrderTesterDlg dialog
IMPLEMENT_DYNAMIC(COrderTesterDlg, CDialog)


BOOL COrderTesterDlg::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult) {
	return CWnd::OnWndMsg ( message, wParam, lParam, pResult );
}

LRESULT COrderTesterDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	
	TLogin *plogin_info;
	TOrder_info *porder_info;
	CString str1, str2, strMsg;
	SYSTEMTIME ti;
	double new_free_margin = 0, ticker_margin = 0, pip_value = 0, keep_margin = 0, base_margin = 0;
	string symbol;
	int sync_lots = 0;
	vector<CString> Items;

	::GetLocalTime ( &ti );
	if ( message == WM_CLOSE )
		return TRUE;
	else
		if ( message == WM_TIMER )
			return TRUE;
		else
			if ( message == WM_COPYDATA ) {
				COPYDATASTRUCT cds;
				memcpy ( &cds, ( void * ) lParam, sizeof ( COPYDATASTRUCT ) );
				if ( cds.dwData == LOGIN_COPYDATA ) {
					plogin_info = ( TLogin * ) cds.lpData;
					str2 = plogin_info->Password;
					str1 = plogin_info->LoginID;
					::SendMessage ( this->m_hWnd, WM_LOGIN, (WPARAM) str1.AllocSysString (), (LPARAM) str2.AllocSysString () );
				}
				else
					if ( cds.dwData == ORDER_COPYDATA ) {
						strMsg.Format(_T("send order to capital at time: %d: %d: %d: %d "), ti.wHour, ti.wMinute, ti.wSecond, ti.wMilliseconds );
						AddReport ( strMsg );
						porder_info = ( TOrder_info * ) cds.lpData;
						symbol = porder_info->ticker_symbol;
						//pip_value = mMap_perpip_value [ symbol ];
						ticker_margin = mMap_origin_margin [ symbol ];
						//keep_margin = mMap_keep_margin [ symbol ];
						//this->margin
						//this->equity
						//new_free_margin = free_margin - ticker_margin;
						if ( margin > equity )
							base_margin =  margin;
						else
							base_margin =  equity;
						sync_lots = porder_info->lots;
						if ( mMap_open_interest.count ( symbol ) ) {
							if ( mMap_open_interest [ symbol ] > 0 )
								m_nlong_position = mMap_open_interest [ symbol ];
							else
								m_nshort_position = mMap_open_interest [ symbol ];
						}
						else {
							m_nlong_position = m_nshort_position = 0;
						}
						::SetWindowText ( m_FutureDlg.GetDlgItem ( IDC_EDIT_STOCKNO )->m_hWnd, porder_info->ticker_symbol );

						( ( CComboBox * ) m_FutureDlg.GetDlgItem ( IDC_COMBO_ORDERTYPE ) )->SetCurSel( 0 );
						( ( CComboBox * ) m_FutureDlg.GetDlgItem ( IDC_COMBO_DAYTRADE ) )->SetCurSel( 0 );						
						if ( porder_info->exit_tick == -1 ) {
							sprintf ( str1.GetBufferSetLength ( 100 ), "%lf",  porder_info->open_price );
							::SetWindowText ( m_FutureDlg.GetDlgItem ( IDC_EDIT_PRICE )->m_hWnd, str1 );

							if ( porder_info->position_type == OP_BUY ) {
								if ( ( m_nshort_position - m_nlong_position ) >= 0 ) {
									( ( CComboBox * ) m_FutureDlg.GetDlgItem ( IDC_COMBO_BUYSELL ) )->SetCurSel( 0 );
									sync_lots = m_nshort_position - m_nlong_position + sync_lots;
									new_free_margin = free_margin + ticker_margin * ( m_nshort_position - m_nlong_position - sync_lots );
								}
								else
									if ( ( m_nlong_position - m_nshort_position ) >= sync_lots ) {
										( ( CComboBox * ) m_FutureDlg.GetDlgItem ( IDC_COMBO_BUYSELL ) )->SetCurSel( 1 );
										sync_lots =  m_nlong_position - m_nshort_position - sync_lots;
										new_free_margin = free_margin + ticker_margin * sync_lots;
									}
									/*else
										if ( ( m_nlong_position - m_nshort_position ) < sync_lots ) {
											( ( CComboBox * ) m_FutureDlg.GetDlgItem ( IDC_COMBO_BUYSELL ) )->SetCurSel( 0 );
											sync_lots = m_nshort_position - m_nlong_position + sync_lots;
										}*/
							}
							else
								if ( porder_info->position_type == OP_SHORT ) {
									if ( ( m_nlong_position - m_nshort_position ) >= 0 ) {
										( ( CComboBox * ) m_FutureDlg.GetDlgItem ( IDC_COMBO_BUYSELL ) )->SetCurSel( 1 );
										sync_lots = m_nlong_position - m_nshort_position + sync_lots;
										new_free_margin = free_margin + ticker_margin * ( m_nlong_position - m_nshort_position - sync_lots );
									}
									else
										if ( ( m_nshort_position - m_nlong_position ) >= sync_lots ) {
											( ( CComboBox * ) m_FutureDlg.GetDlgItem ( IDC_COMBO_BUYSELL ) )->SetCurSel( 0 );
											sync_lots = m_nshort_position - m_nlong_position - sync_lots;
											new_free_margin = free_margin + ticker_margin * sync_lots;
										}
										/*else
											if ( ( m_nshort_position - m_nlong_position ) < sync_lots ) {
												( ( CComboBox * ) m_FutureDlg.GetDlgItem ( IDC_COMBO_BUYSELL ) )->SetCurSel( 1 );
												sync_lots = m_nlong_position - m_nshort_position + sync_lots;
											}*/
								}
						}
						else
							if ( porder_info->exit_tick > 0 ) {
								sprintf ( str1.GetBufferSetLength ( 100 ), "%lf",  porder_info->close_price );
								::SetWindowText ( m_FutureDlg.GetDlgItem ( IDC_EDIT_PRICE )->m_hWnd, str1 );

								if ( porder_info->position_type == OP_SHORT ) {
									if ( ( m_nshort_position - m_nlong_position ) >= 0 ) {
										( ( CComboBox * ) m_FutureDlg.GetDlgItem ( IDC_COMBO_BUYSELL ) )->SetCurSel( 0 );
										sync_lots = m_nshort_position - m_nlong_position;
									}
									else
										if ( ( m_nshort_position - m_nlong_position ) < 0 ) {
											( ( CComboBox * ) m_FutureDlg.GetDlgItem ( IDC_COMBO_BUYSELL ) )->SetCurSel( 1 );
											sync_lots = m_nlong_position - m_nshort_position;
										}
								}
								else
									if ( porder_info->position_type == OP_BUY ) {
										if ( ( m_nlong_position - m_nshort_position ) >= 0 ) {
											( ( CComboBox * ) m_FutureDlg.GetDlgItem ( IDC_COMBO_BUYSELL ) )->SetCurSel( 1 );
											sync_lots = m_nlong_position - m_nshort_position;
										}
										else
											if ( ( m_nlong_position - m_nshort_position ) < 0 ) {
												( ( CComboBox * ) m_FutureDlg.GetDlgItem ( IDC_COMBO_BUYSELL ) )->SetCurSel( 0 );
												sync_lots = m_nshort_position - m_nlong_position;
											}
									}
								new_free_margin = free_margin + ticker_margin * sync_lots;
							}
						//sprintf ( str1.GetBufferSetLength( 100 ), "%d",  porder_info->lots );
						sprintf ( str1.GetBufferSetLength( 100 ), "%d",  sync_lots );
						::SetWindowText ( m_FutureDlg.GetDlgItem ( IDC_EDIT_QTY )->m_hWnd, str1 );
						if ( porder_info->lots != sync_lots )
							TRACE ( "lots: %d, sync_lots: %d\n", porder_info->lots, sync_lots );
						//if ( base_margin > 0 && new_free_margin > 0 && 100 * ( new_free_margin / base_margin ) > 30.0 )
						m_FutureDlg.OnBnClickedButtonSendfutureorder ( );
					}
			}
			else
				if ( message == WM_REFRESH_FUTURE_RIGHT ) {
					strMsg = ( BSTR ) wParam;
					Items = DivideByComma ( strMsg );
					margin = _tstof( Items [ 0 ] );
					equity = _tstof( Items [ 6 ] );
					free_margin = _tstof( Items [ 18 ] );
					return TRUE;
				}
				else
					if ( message == WM_REFRESH_OPEN_INTEREST ) {
						strMsg = ( BSTR ) wParam;
						Items = DivideByComma ( strMsg );
						if ( Items.size ( ) > 1 ) {
							symbol = Items [ 2 ].GetString ( );
							if ( Items [ 3 ] == "B" ) {
								m_nlong_position = _ttoi( Items [ 4 ] );
								m_nshort_position = 0;
								mMap_open_interest [ symbol ] = m_nlong_position;
							}
							else
								if ( Items [ 3 ] == "S" ) {
									m_nshort_position = _ttoi( Items [ 4 ] );
									m_nlong_position = 0;
									mMap_open_interest [ symbol ] = m_nshort_position * -1;
								}
								else {
									m_nlong_position = m_nshort_position = 0;
								}
						}
						else {
							mMap_open_interest.clear ();
						}
						return TRUE;
					}
		//if ( message == WM_LOGIN )
			//return FALSE;
		//else
			return CDialog::WindowProc ( message, wParam, lParam );
}

COrderTesterDlg::COrderTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(COrderTesterDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Account_index = m_Account_count++;
	CWinApp *ptheApp = AfxGetApp();
	COrderTesterApp *pOrderTesterApp = NULL;
	if ( ptheApp->IsKindOf ( RUNTIME_CLASS( COrderTesterApp ) ) ) {
		//if ( ( ( COrderTesterApp * ) ptheApp )->m_pFirstInstanceObj != NULL )
		pOrderTesterApp = ( COrderTesterApp * ) ptheApp;
		m_Account_index = pOrderTesterApp->m_nInstanceID;
	}
	m_get_TF_accounts = 0;
	m_strAccount = "";
	//g_hEvent_Account_Ready = ::CreateEvent ( NULL, FALSE, FALSE, NULL );
	g_hEvent_Account_Ready = ::OpenEvent ( EVENT_ALL_ACCESS, FALSE, "Global\\OrderAccountReady" );
	if ( ! g_hEvent_Account_Ready )
		::MessageBox ( NULL, "open \"g_hEvent_Account_Ready\" fail", "kkk", MB_OK );
	TRACE ( "%x\n", &swat );
	char buf [ 100 ];
	sprintf ( buf, "%x %x %x %x", &swat, ::GetCurrentProcessId(), ::GetCurrentThreadId ( ), &m_Account_index );
	m_WndOwnerThreadID = ::GetCurrentThreadId ( );
	//::MessageBox ( NULL, buf, "kkk", MB_OK );

	margin = free_margin = equity = 0;
	mMap_origin_margin.insert ( pair <string, double> ( "TX00", 83000 ) );
	mMap_keep_margin.insert ( pair <string, double> ( "TX00", 64000 ) );
	mMap_perpip_value.insert ( pair <string, double> ( "TX00", 200 ) );
	mMap_origin_margin.insert ( pair <string, double> ( "MTX00", 20750 ) );
	mMap_keep_margin.insert ( pair <string, double> ( "MTX00", 16000 ) );
	mMap_perpip_value.insert ( pair <string, double> ( "MTX00", 50 ) );
	m_nlong_position = m_nshort_position = 0;
	g_hEvent_FutureRight_Ready = ::CreateEvent ( NULL, FALSE, FALSE, NULL );
	g_hEvent_OpenInterest_Ready = ::CreateEvent ( NULL, FALSE, FALSE, NULL );
	isTimer_start = FALSE;
}

COrderTesterDlg::~COrderTesterDlg ( )
{
	if ( g_hEvent_Account_Ready )
		::CloseHandle ( g_hEvent_Account_Ready );
	::CloseHandle ( g_hEvent_FutureRight_Ready );
	::CloseHandle ( g_hEvent_OpenInterest_Ready );
	::KillTimer ( this->m_hWnd, 1993 );
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
	ON_WM_NCDESTROY()
	ON_MESSAGE ( WM_LOGIN, &COrderTesterDlg::OnLogin )
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
	m_StockDlg.SetDlgCtrlID ( IDD_DIALOG_STOCK );
	m_FutureDlg.Create( IDD_DIALOG_FUTURE, GetDlgItem( IDC_TAB_CONTROL));
	m_FutureDlg.SetDlgCtrlID ( IDD_DIALOG_FUTURE );
	m_OptionDlg.Create( IDD_DIALOG_OPTION, GetDlgItem( IDC_TAB_CONTROL));
	m_OptionDlg.SetDlgCtrlID ( IDD_DIALOG_OPTION );
	m_ForeignStockDlg.Create( IDD_DIALOG_FOREIGNSTOCK, GetDlgItem( IDC_TAB_CONTROL));
	m_ForeignStockDlg.SetDlgCtrlID ( IDD_DIALOG_FOREIGNSTOCK );
	m_FutureStopLossDlg.Create( IDD_DIALOG_FUTURESTOPLOSS, GetDlgItem( IDC_TAB_CONTROL));
	m_FutureStopLossDlg.SetDlgCtrlID ( IDD_DIALOG_FUTURESTOPLOSS );
	m_OptionStopLossDlg.Create( IDD_DIALOG_OPTIONSTOPLOSS, GetDlgItem( IDC_TAB_CONTROL));
	m_OptionStopLossDlg.SetDlgCtrlID ( IDD_DIALOG_OPTIONSTOPLOSS );
	m_OverseaFutureDlg.Create( IDD_DIALOG_OVERSEAFUTURE, GetDlgItem( IDC_TAB_CONTROL));
	m_OverseaFutureDlg.SetDlgCtrlID ( IDD_DIALOG_OVERSEAFUTURE );
	m_OverseaOptionDlg.Create( IDD_DIALOG_OVERSEAOPTION, GetDlgItem( IDC_TAB_CONTROL));
	m_OverseaOptionDlg.SetDlgCtrlID ( IDD_DIALOG_OVERSEAOPTION );

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

/*20150102 added by michael*/
	g_strWndTitle.Preallocate ( 100 );
	::GetWindowText ( this->m_hWnd, g_strWndTitle.GetBufferSetLength ( 100 ), 100 );
	char buf [ 100 ];
	sprintf ( buf, "%x", this->m_hWnd );
	//::MessageBox ( NULL, buf, "kkk", MB_OK );
	if ( g_hEvent_Account_Ready )
		::SetEvent ( g_hEvent_Account_Ready );
	g_hWnd_OrderTesterDlg = this->m_hWnd;
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void COrderTesterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
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
	
	/*20141230 modified by michael
	In our app，COrderTesterDlg instance belong to worker thread of app rather than main thread，our main thread own CQuoteTester instance，only take use AfxGetMainWnd( ) when get main window in main thread*/
	//COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	
	//pDlg->AddReport( strReport);
	AddReport( strReport);

	SysFreeString(bstrData);

	return 0;
}

void __stdcall OnBSTRCallBack( BSTR bstrData)
{
	//SendMessage(FindWindow(NULL,_T("OrderTester")),WM_MESSAGE,0,(long)bstrData);
	TRACE("Run in thread: %x\n", GetCurrentThreadId());

	TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	CString strMsg(bstrData);

	PostMessage(FindWindow(NULL,  ( LPCSTR) g_strWndTitle ),WM_MESSAGE,0,(long)strMsg.AllocSysString());
	//::PostMessage ( AfxGetMainWnd ( )->m_hWnd, WM_MESSAGE, 0, ( long ) strMsg.AllocSysString ( ) );
}

/*20141229 added by michael*/
void __stdcall OnNotifyFutureRightCallBack ( BSTR bstrData )
{
	//COrderTesterDlg* pDlg = (COrderTesterDlg*) CWnd::FromHandle ( /*FindWindow(NULL,  ( LPCSTR) g_strWndTitle )*/g_hWnd_OrderTesterDlg );
	CString strMsg ( bstrData );
	vector<CString> Items;
	if ( strMsg.Find ( _T ( "##" ) ) != -1 ) {
		::SetEvent ( g_hEvent_FutureRight_Ready );
	}
	else {
/*parse future right information*/
		//Items = pDlg->DivideByComma ( strMsg );
		/*pDlg->margin = _tstof( Items [ 0 ] );
		pDlg->equity = _tstof( Items [ 6 ] );
		pDlg->free_margin = _tstof( Items [ 18 ] );*/
		::PostMessage ( FindWindow( NULL,  ( LPCSTR) g_strWndTitle ), WM_REFRESH_FUTURE_RIGHT, ( WPARAM ) bstrData, 0 );
	}
	OnBSTRCallBack ( bstrData );
}

void __stdcall OnNotifyOpenInterestCallBack ( BSTR bstrData )
{
	//COrderTesterDlg* pDlg = (COrderTesterDlg*) CWnd::FromHandle ( /*FindWindow(NULL,  ( LPCSTR) g_strWndTitle )*/g_hWnd_OrderTesterDlg );
	CString strMsg ( bstrData );
	vector<CString> Items;
	if ( strMsg.Find ( _T ( "##" ) ) != -1 ) {
		::SetEvent ( g_hEvent_OpenInterest_Ready );		
	}
	else {
/*parse open interest information*/
		/*Items = pDlg->DivideByComma ( strMsg );
		if ( Items.size ( ) > 1 ) {
			if ( Items [ 3 ] == "B" )
				pDlg->m_nlong_position = _ttoi( Items [ 4 ] );
			else
				if ( Items [ 3 ] == "S" )
					pDlg->m_nshort_position = _ttoi( Items [ 4 ] );
				else {
					pDlg->m_nlong_position = pDlg->m_nshort_position = 0;
				}
		}*/
		//pDlg->m_nlong_position =
		::PostMessage ( FindWindow( NULL,  ( LPCSTR) g_strWndTitle ), WM_REFRESH_OPEN_INTEREST, ( WPARAM ) bstrData, 0 );
	}
	//pDlg->m_nlong_position = pDlg->m_nshort_position = 5;
	OnBSTRCallBack ( bstrData );
}

/*BOOL CALLBACK EnumThreadWndProc(          HWND hwnd,
    LPARAM lParam
);
*/
BOOL CALLBACK EnumThreadWndProc ( HWND hwnd, LPARAM lParam )
{
	CWnd *m_pCwnd = NULL;
	COrderTesterDlg* pDlg = NULL;
	CString strAccount( ( BSTR ) lParam);
	if ( lParam != NULL ) {
		//pstrAccount = ( CString * ) lParam;
		if ( hwnd != NULL ) {
			m_pCwnd = CWnd::FromHandle ( hwnd );
			TRACE ( "%x   %d\n", hwnd, m_pCwnd->GetDlgCtrlID () );
			if ( /*m_pCwnd->GetDlgCtrlID () == IDD_ORDERTESTER_DIALOG*/m_pCwnd->IsKindOf ( RUNTIME_CLASS( COrderTesterDlg ) ) ) {
				pDlg = ( COrderTesterDlg * ) m_pCwnd;
				pDlg->AddAccount( strAccount );
				pDlg->AddReport( strAccount );
				return TRUE;
			}
		}
	}
	return FALSE;
}

BOOL IsMainWndAsOrderTesterDlg ( COrderTesterDlg **ppDlg )
{
	CWnd *pCWnd = NULL;

	pCWnd = ::AfxGetMainWnd ();

	if ( pCWnd != NULL && pCWnd->IsKindOf ( RUNTIME_CLASS( COrderTesterDlg ) ) ) {
		*ppDlg = ( COrderTesterDlg * ) pCWnd;
		return TRUE;
	}
	else {
		*ppDlg = NULL;
		return FALSE;
	}
}

void __stdcall OnAccountCallBack( BSTR bstrData)
{
	TRACE("Run in thread: %x\n", GetCurrentThreadId());

	TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	CString strAccount( bstrData);

	//COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	COrderTesterDlg* pDlg = NULL;
	if ( IsMainWndAsOrderTesterDlg ( &pDlg) ) {
		if ( ::GetCurrentThreadId ( ) == pDlg->m_WndOwnerThreadID ) {
			pDlg->AddAccount( strAccount);
			pDlg->AddReport( strAccount);
		}
		else {
			TRACE ( "maybe cause thread unsafe access\n" );
		}
	}
	else {
		::EnumThreadWindows ( ::GetCurrentThreadId ( ), EnumThreadWndProc, (LPARAM) bstrData );
	}
}

void __stdcall OnAccountWithLoginIDCallBack( BSTR bstrLogInID, BSTR bstrData)
{
	TRACE("Run in thread: %x\n", GetCurrentThreadId());

	TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	CString strID(bstrLogInID);
	CString strAccount( bstrData);

	//COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	COrderTesterDlg* pDlg = NULL;
	if ( IsMainWndAsOrderTesterDlg ( &pDlg) ) {
		if ( ::GetCurrentThreadId ( ) == pDlg->m_WndOwnerThreadID ) {
			pDlg->AddAccount( strAccount);
			pDlg->AddReport( strID + _T("   ") + strAccount);
		}
		else {
			TRACE("file:%s function:%s line:%d\n", __FILE__, __FUNCTION__, __LINE__);
			TRACE ( "maybe cause thread unsafe access\n" );
		}
	}
	else {
		::EnumThreadWindows ( ::GetCurrentThreadId ( ), EnumThreadWndProc, (LPARAM) bstrData );
	}

}

void __stdcall OnOrderAsyncCallBack( DWORD dwThreadID, int nCode, BSTR bstrMessage)
{
	CString strMsg;
	strMsg.Format( _T( "ThradID: %d  Code: %d  Message: %s" ),dwThreadID,nCode,CString(bstrMessage));


	//TRACE ( "%x %x\n", ::GetProcessIdOfThread( ::GetCurrentThread ( ) ), ::GetProcessId ( AfxGetApp()->m_hInstance ) );
	TRACE ( "%s\n", g_strWndTitle );
	SendMessage(FindWindow(NULL, ( LPCSTR) g_strWndTitle ),WM_MESSAGE,0,(long)strMsg.AllocSysString());
	/*COrderTesterDlg* pDlg = NULL;
	if ( IsMainWndAsOrderTesterDlg ( &pDlg) ) {
		SendMessage( pDlg->m_hWnd, WM_MESSAGE, 0, ( long ) strMsg.AllocSysString ( ) );
	}
	else
		TRACE ( "There no idea to located CorderTesterDlg\n" );*/

	/*COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	pDlg->AddReport( strMsg );*/
}


void COrderTesterDlg::OnBnClickedButtonInit()
{
	// TODO: Add your control notification handler code here
	try
	{
		CString strAccount;
		DWORD dwWaitResult = -1;
		CFutureDlg  *pFutureDlg = NULL;
		m_editID.GetWindowText(strAccount);

		CString strPassWord;
		m_editPassWord.GetWindowText(strPassWord);

		int nCode = SKOrderLib_Initialize( (LPCSTR)CStringA(strAccount), (LPCSTR)CStringA(strPassWord) );

		if ( nCode == 0 || nCode == SK_WARNING_LOGIN_ALREADY )
		{
			if( m_bRegister == false && nCode == 0 )
			{
				RegisterOnAccountCallBack( (long)OnAccountCallBack );
				RegisterOnOrderAsyncReportCallBack( (long)OnOrderAsyncCallBack);
				RegisterOnAccountWithLoginIDCallBack( (long)OnAccountWithLoginIDCallBack );
				RegisterOnRealBalanceReportCallBack( (long)OnBSTRCallBack );
				//RegisterOnOpenInterestCallBack( (long)OnBSTRCallBack );
				RegisterOnOpenInterestCallBack( (long) OnNotifyOpenInterestCallBack );
				RegisterOnStopLossReportCallBack( (long)OnBSTRCallBack );
				RegisterOnOverseaFuturesCallBack( (long)OnBSTRCallBack );
				RegisterOnOverseaFutureOpenInterestCallBack( (long)OnBSTRCallBack );
				RegisterOnOverseaOptionsCallBack( (long)OnBSTRCallBack );
				//RegisterOnFutureRightCallBack( (long)OnBSTRCallBack );
				RegisterOnFutureRightCallBack( (long) OnNotifyFutureRightCallBack );
				RegisterOnCertCanceledCallBack( (long)OnBSTRCallBack );
				RegisterOnExecutionReportCallBack ( (long)OnBSTRCallBack );

				m_bRegister = true;
			}

			m_get_TF_accounts = 0;
			GetUserAccount();

			//MessageBox(_T("SUCCESS"));

			int nCert = SKOrderLib_ReadCertByID( (LPTSTR)(LPCTSTR)strAccount );

			if( nCert == 0 ) {
				//MessageBox(_T("READ CERT SUCCESS"));
				pFutureDlg = ( ( CFutureDlg  * ) m_TabControl.GetDlgItem ( IDD_DIALOG_FUTURE ) );
				do {
					pFutureDlg->OnBnClickedButtonFutureright ( );
				} while ( pFutureDlg->nCode == SK_ERROR_QUERY_IN_PROCESSING );
				if ( pFutureDlg->nCode == 0 )
					dwWaitResult = ::WaitForSingleObject ( g_hEvent_FutureRight_Ready, 10000 );
				do {
					pFutureDlg->OnBnClickedButtonOpeninterest ( );
				} while ( pFutureDlg->nCode == SK_ERROR_QUERY_IN_PROCESSING );
				if ( pFutureDlg->nCode == 0 )
					dwWaitResult = ::WaitForSingleObject ( g_hEvent_OpenInterest_Ready, 10000 );
				if ( g_hEvent_Account_Ready )
					::SetEvent ( g_hEvent_Account_Ready );
				if ( isTimer_start == FALSE )
					::SetTimer ( this->m_hWnd, 1993, 15000, TimerProc1 );
				isTimer_start = TRUE;
			}
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

/*VOID CALLBACK TimerProc(          HWND hwnd,
    UINT uMsg,
    UINT_PTR idEvent,
    DWORD dwTime
);*/
VOID CALLBACK TimerProc1( HWND hwnd, UINT uMsg, UINT_PTR idEvent, DWORD dwTime )
{
	DWORD dwWaitResult = -1;
	TRACE ( "%x\n", ::GetCurrentThreadId ( ) );
	CFutureDlg *pFutureDlg = ( CFutureDlg  * )  ( (COrderTesterDlg*) AfxGetMainWnd() )->m_TabControl.GetDlgItem ( IDD_DIALOG_FUTURE );

	do {
		pFutureDlg->OnBnClickedButtonFutureright ( );
	} while ( pFutureDlg->nCode == SK_ERROR_QUERY_IN_PROCESSING );
	if ( pFutureDlg->nCode == 0 )
		dwWaitResult = ::WaitForSingleObject ( g_hEvent_FutureRight_Ready, 10000 );
	do {
		pFutureDlg->OnBnClickedButtonOpeninterest ( );
	} while ( pFutureDlg->nCode == SK_ERROR_QUERY_IN_PROCESSING );
	if ( pFutureDlg->nCode == 0 )
		dwWaitResult = ::WaitForSingleObject ( g_hEvent_OpenInterest_Ready, 10000 );

}

void COrderTesterDlg::AddAccount(const CString& strAccount)
{
	vector<CString> Items = DivideByComma( strAccount);
	CString wnd_caption;

	if ( Items.size() >= 6)
	{
		CString strFullAccount = Items[1] + Items[3];

		if ( Items[0] == "TS")
		{
			if ( m_boxTSAccount.FindStringExact( -1, ( LPCTSTR ) strFullAccount) == CB_ERR)
			{
				m_boxTSAccount.AddString( strFullAccount);
			}
		}
		else if ( Items[0] == _T("TF"))
		{
			if ( m_boxTFAccount.FindStringExact( -1, strFullAccount) == CB_ERR)
			{
				m_get_TF_accounts++;
				m_boxTFAccount.AddString( strFullAccount);
				if ( m_Account_index == ( m_get_TF_accounts - 1 ) ) {
					m_strAccount = strFullAccount;
					this->GetDlgItem ( IDC_EDIT_ID )->GetWindowText ( wnd_caption );
					this->SetWindowText ( wnd_caption + "---" + m_strAccount );
					g_strWndTitle.ReleaseBuffer ( 100 );
					::GetWindowText ( this->m_hWnd, g_strWndTitle.GetBufferSetLength ( 100 ), 100 );
					m_boxTFAccount.SetCurSel ( m_Account_index );
					/*20141230 added by michael
					need to fire combobox change event via sending a message*/
					SendMessage ( WM_COMMAND, MAKEWPARAM ( m_boxTFAccount.GetDlgCtrlID(), CBN_SELCHANGE ), LPARAM( ( HWND ) m_boxTFAccount.m_hWnd ) );
					
					//::SetTimer ( this->m_hWnd, 1993, 15000, TimerProc1 );
					//::SetEvent ( g_hEvent_Account_Ready );
				}
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
	TRACE("Run in thread: %x\n", GetCurrentThreadId());

	TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
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

void COrderTesterDlg::PostNcDestroy() 
{	
    CDialog::PostNcDestroy();
    //delete this;
}

void COrderTesterDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();
}

BOOL CALLBACK EnumChildProc1( HWND hwnd, LPARAM lParam )
{
	CWnd *m_pCwnd = NULL;
	if ( lParam == 200 ) {
		if ( hwnd != NULL ) {
			m_pCwnd = CWnd::FromHandle ( hwnd );
			TRACE ( "%x   %d\n", hwnd, m_pCwnd->GetDlgCtrlID () );
		}
		return TRUE;
	}
	return FALSE;
}

void COrderTesterDlg::Switch_TabControl ( int NewPage )
{
	int nCurrent = m_TabControl.GetCurSel();
	if ( nCurrent != NewPage ) {
		m_TabControl.SetCurSel ( NewPage );

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

		nCurrent = NewPage;
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
	}
}

LRESULT COrderTesterDlg::OnLogin ( WPARAM wParam, LPARAM lParam )
{
	CStringA strID ( ( BSTR ) wParam ) , strPass  ( ( BSTR ) lParam );

	this->m_editID.SetWindowText ( ( LPCSTR ) strID );
	this->m_editPassWord.SetWindowText ( ( LPCSTR ) strPass );
	OnBnClickedButtonInit();
	SysFreeString ( ( BSTR ) wParam );
	SysFreeString ( ( BSTR ) lParam );
	//HWND m_stockdlg_hwnd = ::FindWindowEx ( this->m_hWnd, ::GetWindow ( this->m_TabControl.m_hWnd, GW_CHILD ), NULL, NULL );
	//HWND m_hwnd = ::FindWindow ( NULL, "police" );
	//::EnumChildWindows ( this->m_TabControl.m_hWnd, EnumChildProc1, 200 );
	//this->m_TabControl.SetCurSel ( enum_FutureDlg );
	//m_FutureDlg.ShowWindow ( TRUE );
	Switch_TabControl ( this->enum_FutureDlg );
	//::MessageBox ( NULL, strID, "kkk", MB_OK );
	return 0;
}

BOOL COrderTesterDlg::PreTranslateMessage(MSG* pMsg)
{
	/*if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;                // Do not process further
		}
	}*/

	return CDialog::PreTranslateMessage ( pMsg );
}