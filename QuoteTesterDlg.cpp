
// QuoteTesterDlg.cpp : 實作檔
//

#include "stdafx.h"
#include "QuoteTester.h"
#include "QuoteTesterDlg.h"
//#include "afxdialogex.h"
//#include "Functions.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define WM_RUN_IN_UI_THREAD (WM_USER + 2)


// CQuoteTesterDlg 對話方塊

int m_nType = 0;

int m_nMarketTot = 0;
int m_nMarketBuySale = 0;
int m_nMarketHightLow = 0;
//#define GetWindowTextW GetWindowText
HANDLE t_hnd = NULL;
DWORD t_id = 0;
/*point to main dialog*/
CQuoteTesterDlg *m_pDialog;
HANDLE g_hThreads_KLine [ 3 ], g_hEvent_KLine;
DWORD g_ThreadID_KLine [ 3 ];
int position_type = -1, position_type1 = -1;

CQuoteTesterDlg::CQuoteTesterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CQuoteTesterDlg::IDD, pParent), mKline_stream( 1, 3, true ), mKline_stream_day( 4 , 1, false ), account_A( "capital_A" ), account_B ( "capital_B" )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CQuoteTesterDlg::~CQuoteTesterDlg()
{
	SKQuoteLib_LeaveMonitor();
	//CloseHandle(Wait_Connection_Event);
	CloseHandle(Wait_ProductsReady_Event);
	/*if (mpKline_stream != NULL)
		delete mpKline_stream;*/
	TStock *pTStock;
	for (deque<TStock *>::iterator itr = m_Queue_pTStock.begin(); itr != m_Queue_pTStock.end(); itr++) {
		pTStock = (TStock *)*itr;
		delete (pTStock);
	}
	TBest5 *pTBest5;
	for (deque<TBest5 *>::iterator itr = m_Queue_pTBest5.begin(); itr != m_Queue_pTBest5.end(); itr++) {
		pTBest5 = (TBest5 *)*itr;
		delete (pTBest5);
	}
	TTick *pTTick;
	TRACE("TTick deque size: %d\n", m_Queue_pTTick.size());
	for (deque<TTick *>::iterator itr = m_Queue_pTTick.begin(); itr != m_Queue_pTTick.end(); itr++) {
		pTTick = (TTick *)*itr;
		delete (pTTick);
	}

	CloseHandle(ghMutex);
}

void CQuoteTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CQuoteTesterDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_DATA,&CQuoteTesterDlg::OnData)
	ON_BN_CLICKED(IDC_BUTTON1, &CQuoteTesterDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CQuoteTesterDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CQuoteTesterDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CQuoteTesterDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CQuoteTesterDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CQuoteTesterDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON8, &CQuoteTesterDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON10, &CQuoteTesterDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON7, &CQuoteTesterDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON11, &CQuoteTesterDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, &CQuoteTesterDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CQuoteTesterDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CQuoteTesterDlg::OnBnClickedButton14)
    ON_MESSAGE(WM_RUN_IN_UI_THREAD, &CQuoteTesterDlg::OnDoTaskInMainThread)
	ON_WM_DESTROY()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()


// CQuoteTesterDlg 訊息處理常式

BOOL CQuoteTesterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 設定此對話方塊的圖示。當應用程式的主視窗不是對話方塊時，
	// 框架會自動從事此作業
	SetIcon(m_hIcon, TRUE);			// 設定大圖示
	SetIcon(m_hIcon, FALSE);		// 設定小圖示

	// TODO: 在此加入額外的初始設定

	CComboBox   *pComboBox   =   (CComboBox *)GetDlgItem(IDC_COMBO_KLine); 

	pComboBox->AddString(_T("1分鐘線"));
	pComboBox->AddString(_T("5分鐘線"));
	pComboBox->AddString(_T("30分鐘線"));
	pComboBox->AddString(_T("日線228天"));
	pComboBox->AddString(_T("完整日線"));
	pComboBox->AddString(_T("週線"));
	pComboBox->AddString(_T("月線"));

	pComboBox->SetCurSel(0);

	//Wait_Connection_Event = CreateEvent(NULL, FALSE, FALSE, NULL);
	Wait_ProductsReady_Event = CreateEvent(NULL, TRUE, FALSE, NULL);
	//mKline_stream.Push_KLine_Data(CString("1402"), CString("5+2+3"));
	//mpKline_stream = NULL;
	/*mKline_stream.load_KLine_from_archive( "1402" );
	mKline_stream.Push_KLine_Data("1402", "06/03/2014, 08:50, 913800, 914800, 913700, 914600, 3964");
	mKline_stream.Push_KLine_Data("1402", "06/05/2014, 08:50, 913800, 914800, 913700, 914600, 3964");
	mKline_stream.Push_KLine_Data("1402", "06/12/2014, 08:50, 913800, 914800, 913700, 914600, 3964");*/
	/*mKline_stream.Push_Tick_Data( "1402", 0, 90003, -999999, 999999, 3255, 66, 1 );
	mKline_stream.Push_Tick_Data( "1402", 5, 90212, 3270, 3275, 3275, 105, 0 );
	mKline_stream.Push_Tick_Data( "1402", 8, 90507, 3215, 3220, 3215, 70, 1 );
	mKline_stream.Push_Tick_Data( "1402", 3, 90123, 3220, 3230, 3225, 69, 0 );
	mKline_stream.Push_Tick_Data( "1402", 18, 90845, 3270, 3275, 3270, 75, 1 );
	mKline_stream.Push_Tick_Data( "1402", 15, 90615, 3255, 3260, 3260, 35, 1 );
	mKline_stream.Push_Tick_Data( "1402", 7, 90435, 3230, 3235, 3230, 19, 1 );
	mKline_stream.Push_Tick_Data( "1402", 6, 90235, 3265, 3260, 3265, 73, 1 );*/
	/*mKline_stream.set_KLine_ready ( "1402" );
	mKline_stream.set_KLine_ready ( "TX00" );
	mKline_stream.get_KLine_ready ( "2903" );*/
	/*int back_fill = 1;
	account_A.Place_Open_Order ( "TX00", 0, 90012, 905700, 905800, 905800, 3, back_fill, Long_position );
	account_A.Place_Open_Order ( "TX00", 1, 90040, 904600, 904900, 904600, 7, back_fill, Long_position );
	account_A.Place_Open_Order ( "TX00", 2, 90122, 905100, 905200, 905200, 2, back_fill, Long_position );
	account_A.Place_Open_Order ( "TX00", 3, 90143, 904400, 904500, 904500, 8, back_fill, Long_position );
	account_A.Place_Open_Order ( "TX00", 4, 90239, 903900, 904000, 903900, 11, back_fill,  Short_position );
	account_A.Place_Open_Order ( "TX00", 5, 90258, 904700, 904800, 904800, 20, back_fill,  Long_position );
	account_A.Place_Open_Order ( "TX00", 6, 90320, 904200, 904300, 904200, 28, back_fill, Short_position );
	account_A.Place_Open_Order ( "TX00", 7, 90342, 903700, 903800, 903700, 21, back_fill, Short_position );
	account_A.Place_Open_Order ( "TX00", 8, 90405, 903100, 903200, 903200, 105, back_fill, Short_position );
	account_A.Place_Open_Order ( "TX00", 9, 90422, 903500, 903600, 903600, 78, back_fill, Short_position );
	account_A.Place_Open_Order ( "TX00", 10, 90447, 903800, 903900, 903900, 65, back_fill, Short_position );
	account_A.Place_Open_Order ( "TX00", 11, 90456, 904300, 904400, 904400, 125, back_fill, Long_position );
	account_A.Place_Open_Order ( "TX00", 12, 90517, 904000, 904100, 904000, 26, back_fill, Long_position );
	account_A.refresh_portfolio();*/
	m_pDialog = (CQuoteTesterDlg *)AfxGetApp ()->GetMainWnd ();
	ghMutex = CreateMutex( 
		NULL,              // default security attributes
		FALSE,             // initially not owned
		NULL);             // unnamed mutex
	return TRUE;  // 傳回 TRUE，除非您對控制項設定焦點
}

// 如果將最小化按鈕加入您的對話方塊，您需要下列的程式碼，
// 以便繪製圖示。對於使用文件/檢視模式的 MFC 應用程式，
// 框架會自動完成此作業。

void CQuoteTesterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 繪製的裝置內容

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 將圖示置中於用戶端矩形
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 描繪圖示
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 當使用者拖曳最小化視窗時，
// 系統呼叫這個功能取得游標顯示。
HCURSOR CQuoteTesterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

/*******************************************************
CALLBACK
*********************************************************/
void _stdcall OnConnect( int nKind, int nErrorCode)
{
	//if( nKind == SK_SUBJECT_CONNECTION_CONNECTED)
		//AfxMessageBox(_T("CONNECTED"));
	TRACE("Run in UI thread: %x\n", GetCurrentThreadId());
	TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	if( nKind == SK_SUBJECT_CONNECTION_DISCONNECT)
		AfxMessageBox(_T("DISCONNECT"));

	CString strMsg;
	strMsg.Format(_T("Connect  nKind：%d nErrorCode：%d"),nKind,nErrorCode);

	BSTR bstrMsg = strMsg.AllocSysString();

	SendMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,98,(int)bstrMsg);

	if (GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
		SysFreeString(bstrMsg);
}

void _stdcall OnNotifyQuote( short sMarketNo, short sStockidx)
{
	TStock *tStock;
	//TRACE("Run in thread: %x\n", GetCurrentThreadId());

	//TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	int get_stock_no;
	if ( ! m_pDialog->mMap_stockidx_stockNo.count ( sStockidx ) )
		get_stock_no = 1;
	else
		get_stock_no = 0;
	//if( m_nType == 1 )
	if ( get_stock_no == 1 )
	{
		//TStock* tStock = new TStock();
		if ( !m_pDialog->m_Queue_pTStock.empty() ) {
			tStock = (TStock *) m_pDialog->m_Queue_pTStock.front();
			m_pDialog->m_Queue_pTStock.pop_front();
		}
		else
			tStock = new TStock();

		SKQuoteLib_GetStockByIndex(sMarketNo,sStockidx,tStock);

		CString strStockNo(tStock->m_caStockNo);
		CString strStockName(tStock->m_caName);
		m_pDialog->mMap_stockidx_stockNo[ sStockidx ] = tStock->m_caStockNo;

		CString strMsg;
		strMsg.Format(_T("%s %s 買價:%d 買量:%d 賣價:%d 買量:%d 成交價:%d 成交量:%d"),
			strStockNo,
			strStockName,
			tStock->m_nBid,
			tStock->m_nBc,
			tStock->m_nAsk,
			tStock->m_nAc,
			tStock->m_nClose,
			tStock->m_nTQty);

		BSTR bstrMsg = strMsg.AllocSysString();

		//SendMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);
		PostMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);

		//delete tStock;
		m_pDialog->m_Queue_pTStock.push_back(tStock);
		//tStock = NULL;

		if (GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
			SysFreeString(bstrMsg);
	}
}

void _stdcall OnNotifyTicks( short sMarketNo, short sStockidx, int nPtr)
{
	TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	if( m_nType == 2 )
	{
/*
		TTick* tTick = new TTick();

		SKQuoteLib_GetTick(sMarketNo,sStockidx,nPtr,tTick);

		CString strMsg;

		strMsg.Format(_T("TICK %d 買價:%d 賣價:%d 成交價:%d 量:%d "),
			tTick->m_nTime,
			tTick->m_nBid,
			tTick->m_nAsk,
			tTick->m_nClose,
			tTick->m_nQty);

		BSTR bstrMsg = strMsg.AllocSysString();

		SendMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);

		//delete tTick;
		//tTick = NULL;
		
		SysFreeString(bstrMsg);
*/
	}
}

void _stdcall OnNotifyTicksGet( short sMarketNo, short sStockidx, int nPtr, int nTime,int nBid, int nAsk, int nClose, int nQty)
{
	//TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	//TRACE("Run in thread: %x\n", GetCurrentThreadId());
	//if( m_nType == 2 )
	{
		CString strMsg;

		/*strMsg.Format(_T("TICK 編號:%d 時間:%d 買價:%d 賣價:%d 成交價:%d 量:%d "),
			nPtr,
			nTime,
			nBid,
			nAsk,
			nClose,
			nQty);*/
		m_pDialog->mKline_stream.Push_Tick_Data( m_pDialog->mMap_stockidx_stockNo[ sStockidx ], nPtr,
			nTime,
			nBid,
			nAsk,
			nClose,
			nQty, 0 );
		m_pDialog->mKline_stream_day.Push_Tick_Data( m_pDialog->mMap_stockidx_stockNo[ sStockidx ], nPtr,
			nTime,
			nBid,
			nAsk,
			nClose,
			nQty, 0 );
		string symbol;
		//int position_type = -1, position_type1 = -1;
		list <double> *pList_15min_MA10, *pList_15min_MA22, *pList_day_MA10, *pList_day_MA22;
		double MA10_15min, MA22_15min, MA10_day, MA22_day;
		symbol = m_pDialog->mMap_stockidx_stockNo[ sStockidx ] + "_15min";
		pList_15min_MA10 = m_pDialog->mKline_stream.mMap_MA10[ symbol ];
		pList_15min_MA22 = m_pDialog->mKline_stream.mMap_MA22[ symbol ];
		symbol = m_pDialog->mMap_stockidx_stockNo[ sStockidx ] + "_full_day";
		pList_day_MA10 = m_pDialog->mKline_stream_day.mMap_MA10[ symbol ];
		pList_day_MA22 = m_pDialog->mKline_stream_day.mMap_MA22[ symbol ];
		MA10_15min = *( pList_15min_MA10->rbegin() );
		MA22_15min = *( pList_15min_MA22->rbegin() );
		MA10_day = *( pList_day_MA10->rbegin() );
		MA22_day = *( pList_day_MA22->rbegin() );
		double close_price = 0;
		close_price = nClose / 100;
		if ( close_price > MA10_15min && close_price > MA22_15min ) { //account_A hold long position
			position_type = Long_position;
		}
		else
			if ( close_price < MA10_15min && close_price < MA22_15min ) { //account_A hold short position
				position_type = Short_position;
			}
			else
				if ( close_price < MA10_15min && close_price > MA22_15min ) { //account_A exit long position
					//position_type = Close_all_position;
					//position_type = Close_long_position;
				}
				else
					if ( close_price > MA10_15min && close_price < MA22_15min ) { //account_A exit short position
						//position_type = Close_all_position;
						//position_type = Close_short_position;
					}
		/*if ( nTime > 134400)
			position_type = Close_all_position;*/
		m_pDialog->account_A.Place_Open_Order ( m_pDialog->mMap_stockidx_stockNo[ sStockidx ], nPtr,
			nTime,
			nBid,
			nAsk,
			nClose,
			nQty, 0, position_type );
		if ( ! ( nPtr % 200 ) )
			m_pDialog->account_A.refresh_portfolio();
		if ( close_price > MA10_day && close_price > MA22_day ) { //account_B hold long position
			position_type1 = Long_position;
		}
		else
			if ( close_price < MA10_day && close_price < MA22_day ) { //account_B hold short position
				position_type1 = Short_position;
			}
			else
				if ( close_price < MA10_day && close_price > MA22_day ) { //account_B exit long position
				}
				else
					if ( close_price > MA10_day && close_price < MA22_day ) { //account_B exit short position
					}
		/*if ( nTime > 134400)
			position_type1 = Close_all_position;*/
		m_pDialog->account_B.Place_Open_Order ( m_pDialog->mMap_stockidx_stockNo[ sStockidx ], nPtr,
			nTime,
			nBid,
			nAsk,
			nClose,
			nQty, 0, position_type1 );
		if ( ! ( nPtr % 200 ) )
			m_pDialog->account_B.refresh_portfolio();
		/*BSTR bstrMsg = strMsg.AllocSysString();

		if (GetCurrentThreadId() == AfxGetApp()->m_nThreadID) {
			SendMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);
			SysFreeString(bstrMsg);
		}
		else
			PostMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);*/

	}
}

void _stdcall OnNotifyHistoryTicksGet( short sMarketNo, short sStockidx, int nPtr, int nTime,int nBid, int nAsk, int nClose, int nQty)
{
	TTick* tTick;
	DWORD dwWaitResult;
	//TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	//TRACE("Run in thread: %x\n", GetCurrentThreadId());
	//if( m_nType == 2 )
	{
		//if ( !m_pDialog->m_Queue_pTTick.empty() ) {
			/*dwWaitResult = WaitForSingleObject( 
				m_pDialog->ghMutex,    // handle to mutex
				INFINITE);  // no time-out interval*/
			//tTick = m_pDialog->m_Queue_pTTick.front();
			//m_pDialog->m_Queue_pTTick.pop_front();
			//ReleaseMutex(m_pDialog->ghMutex);
		//}
		//else {
			//ReleaseMutex(m_pDialog->ghMutex);
			//tTick = new TTick();
		//}

		CString strMsg;

		/*strMsg.Format(_T("TICK 編號:%d 時間:%d 買價:%d 賣價:%d 成交價:%d 量:%d "),
			nPtr,
			nTime,
			nBid,
			nAsk,
			nClose,
			nQty);*/
		/*TRACE("TICK 編號:%d 時間:%d 買價:%d 賣價:%d 成交價:%d 量:%d ",
			nPtr,
			nTime,
			nBid,
			nAsk,
			nClose,
			nQty);*/
		/*TRACE("%d\n", nPtr);
		short	sPageNo = 1;
		if ( nPtr == 1000 ) {*/
			//SKQuoteLib_RequestTicks(&sPageNo, "");
			//m_pDialog->OnBnClickedButton14();
		//}
		m_pDialog->mKline_stream.Push_Tick_Data( m_pDialog->mMap_stockidx_stockNo[ sStockidx ], nPtr,
			nTime,
			nBid,
			nAsk,
			nClose,
			nQty, 1 );
		m_pDialog->mKline_stream_day.Push_Tick_Data ( m_pDialog->mMap_stockidx_stockNo[ sStockidx ], nPtr,
			nTime,
			nBid,
			nAsk,
			nClose,
			nQty, 1 );
		string symbol;
		//static int position_type = -1, position_type1 = -1;
		list <double> *pList_15min_MA10, *pList_15min_MA22, *pList_day_MA10, *pList_day_MA22;
		double MA10_15min, MA22_15min, MA10_day, MA22_day;
		symbol = m_pDialog->mMap_stockidx_stockNo[ sStockidx ] + "_15min";
		pList_15min_MA10 = m_pDialog->mKline_stream.mMap_MA10[ symbol ];
		pList_15min_MA22 = m_pDialog->mKline_stream.mMap_MA22[ symbol ];
		symbol = m_pDialog->mMap_stockidx_stockNo[ sStockidx ] + "_full_day";
		pList_day_MA10 = m_pDialog->mKline_stream_day.mMap_MA10[ symbol ];
		pList_day_MA22 = m_pDialog->mKline_stream_day.mMap_MA22[ symbol ];
		MA10_15min = *( pList_15min_MA10->rbegin() );
		MA22_15min = *( pList_15min_MA22->rbegin() );
		MA10_day = *( pList_day_MA10->rbegin() );
		MA22_day = *( pList_day_MA22->rbegin() );
		double close_price = 0;
		close_price = nClose / 100;
		if ( close_price > MA10_15min && close_price > MA22_15min ) { //account_A hold long position
			position_type = Long_position;
		}
		else
			if ( close_price < MA10_15min && close_price < MA22_15min ) { //account_A hold short position
				position_type = Short_position;
			}
			else
				if ( close_price < MA10_15min && close_price > MA22_15min ) { //account_A exit long position
					//position_type = Close_all_position;
					//position_type = Close_long_position;
					//position_type = Close_short_position;
				}
				else
					if ( close_price > MA10_15min && close_price < MA22_15min ) { //account_A exit short position
						//position_type = Close_all_position;
						//position_type = Close_short_position;
						//position_type = Close_long_position;
					}
		/*if ( nTime > 134400)
			position_type = Close_all_position;*/
		m_pDialog->account_A.Place_Open_Order ( m_pDialog->mMap_stockidx_stockNo[ sStockidx ], nPtr,
			nTime,
			nBid,
			nAsk,
			nClose,
			nQty, 0, position_type );
		if ( ! ( nPtr % 200 ) )
			m_pDialog->account_A.refresh_portfolio();
		
		if ( close_price > MA10_day && close_price > MA22_day ) { //account_B hold long position
			position_type1 = Long_position;
		}
		else
			if ( close_price < MA10_day && close_price < MA22_day ) { //account_B hold short position
				position_type1 = Short_position;
			}
			else
				if ( close_price < MA10_day && close_price > MA22_day ) { //account_B exit long position
				}
				else
					if ( close_price > MA10_day && close_price < MA22_day ) { //account_B exit short position
					}

		/*if ( nTime > 134400)
			position_type1 = Close_all_position;*/
		/*int final_position =-1;
		if ( position_type == Long_position && position_type1 == Long_position )
			final_position = Long_position;
		else
			if ( position_type == Short_position && position_type1 == Short_position )
				final_position = Long_position;*/

		m_pDialog->account_B.Place_Open_Order ( m_pDialog->mMap_stockidx_stockNo[ sStockidx ], nPtr,
			nTime,
			nBid,
			nAsk,
			nClose,
			nQty, 0, position_type1 );
		if ( ! ( nPtr % 200 ) )
			m_pDialog->account_B.refresh_portfolio();
		/*tTick->m_nAsk = nAsk;
		tTick->m_nBid = nBid;
		tTick->m_nClose = nClose;
		tTick->m_nPtr = nPtr;
		tTick->m_nQty = nQty;
		tTick->m_nTime = nTime;*/

		//BSTR bstrMsg = strMsg.AllocSysString();

		//m_pDialog->m_Queue_pTTick.push_back(tTick);
		if (GetCurrentThreadId() == AfxGetApp()->m_nThreadID) {
			//SendMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);
			//SysFreeString(bstrMsg);
			//PostThreadMessage( t_id, WM_HISTORY_TICK, sStockidx,(int)tTick );
		}
		/*else
			PostMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);*/
	}
}

void _stdcall OnNotifyBest5( short sMarketNo, short sStockidx)
{
	TBest5* tBest5;
	//TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	return;
	TRACE("Run in thread: %x\n", GetCurrentThreadId());
	if( m_nType == 2 )
	{
		//TBest5* tBest5 = new TBest5();

		if ( !m_pDialog->m_Queue_pTBest5.empty() ) {
			tBest5 = m_pDialog->m_Queue_pTBest5.front();
			m_pDialog->m_Queue_pTBest5.pop_front();
		}
		else
			tBest5 = new TBest5();

		SKQuoteLib_GetBest5(sMarketNo,sStockidx,tBest5);
		
		CString strMsg;
		strMsg.Format(_T("Best5 %d %d %d %d"),
			tBest5->m_nBid1,
			tBest5->m_nBidQty1,
			tBest5->m_nAsk1,
			tBest5->m_nAskQty1);

		BSTR bstrMsg = strMsg.AllocSysString();

		if (GetCurrentThreadId() == AfxGetApp()->m_nThreadID) {
			SendMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);
			SysFreeString(bstrMsg);
		}
		else
			PostMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);

		m_pDialog->m_Queue_pTBest5.push_back(tBest5);
		//delete tBest5;
		//tBest5 = NULL;
	}
}

void _stdcall OnNotifyKLineData( char * caStockNo, char * caData )
{
	DWORD cur_tid;
	/*running in the same thread with thread issue SKQuoteLib_GetKLine()*/
	//TRACE("Run in thread: %x\n", GetCurrentThreadId());
	//TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	
	//CQuoteTesterDlg *pDialog;
	/*if (GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
		m_pDialog = (CQuoteTesterDlg *) AfxGetMainWnd();
	else
		m_pDialog = (CQuoteTesterDlg *)AfxGetApp ()->GetMainWnd ();*/

	//if( m_nType == 3 )
	{
		CString strStockNo(caStockNo);
		CString strData(caData);

		CString strMsg;
		strMsg.Format(_T("%s %s"),strStockNo,strData);
		//pDialog->mKline_stream.Push_KLine_Data(strStockNo, strData);
		//cur_tid = GetCurrentThreadId();
		//m_pDialog->mKline_stream_day.Push_KLine_Data( caStockNo, caData );
		if ( GetCurrentThreadId() == g_ThreadID_KLine[ 0 ])  //thread 5min KLine
		{
			m_pDialog->mKline_stream.Push_KLine_Data( caStockNo, caData );
		}
		else 
			if ( GetCurrentThreadId() == g_ThreadID_KLine[ 1 ] )  //thread day KLine
			{
				m_pDialog->mKline_stream_day.Push_KLine_Data( caStockNo, caData );
			}
			else 
				if ( GetCurrentThreadId() == g_ThreadID_KLine[ 2 ]) {  //post message only

		//TRACE("%s %s \n", caStockNo, caData );
		
		BSTR bstrMsg = strMsg.AllocSysString();

		if (GetCurrentThreadId() == AfxGetApp()->m_nThreadID) {
			SysFreeString(bstrMsg);
			SendMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);
		}
		else
			PostMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);
		//PostThreadMessage(AfxGetApp()->m_nThreadID, WM_DATA,m_nType,(int)bstrMsg);
		//PostThreadMessage(t_id, WM_DATA,m_nType,(int)bstrMsg);
		}
	}
}

void _stdcall OnNotifyMarketTot( char cMarketNo , short sPrt, long lTime, long lTotv, long lTots, long lTotc )
{
	if( m_nType == 4 )
	{
		m_nMarketTot = sPrt;
		
		int nMarketNo =cMarketNo;

		CString strMsg;
		strMsg.Format(_T("%d 時間:%d 成交(億):%d 筆數:%d 張數:%d"),
				nMarketNo,lTime,lTotv,lTots,lTotc);

		BSTR bstrMsg = strMsg.AllocSysString();

		SendMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);
		
		SysFreeString(bstrMsg);
	}
}

void _stdcall OnNotifyMarketBuySell( char cMarketNo , short sPrt, long lTime,long lBc, long lSc, long lBs, long lSs )
{
	if( m_nType == 4 )
	{
		m_nMarketBuySale = sPrt;
		
		int nMarketNo =cMarketNo;

		CString strMsg;
		strMsg.Format(_T("%d 時間:%d 買進筆數:%d 賣出筆數:%d 買進張數:%d 賣出張數:%d"),
			nMarketNo,lTime,lBc, lSc, lBs, lSs);

		BSTR bstrMsg = strMsg.AllocSysString();

		SendMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);
		
		SysFreeString(bstrMsg);
	}
}

void _stdcall OnNotifyMarketHighLow( char cMarketNo , short sPrt, long lTime, short sUp, short sDown, short sHigh, short sLow, short sNoChange )
{
	if( m_nType == 4 )
	{
		m_nMarketHightLow = sPrt;
		int nMarketNo =cMarketNo;

		CString strMsg;
		strMsg.Format(_T("%d 時間:%d 上漲家:%d 下跌家:%d 漲停家:%d 跌停家:%d  平盤家:%d"),
			 nMarketNo,lTime,sUp,  sDown,  sHigh,  sLow,  sNoChange );

		BSTR bstrMsg = strMsg.AllocSysString();

		SendMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);
		
		SysFreeString(bstrMsg);
	}
}

void _stdcall OnNotifyStrikePrices( BSTR BProduct, BSTR BName, BSTR BCall, BSTR BPut, int nStrikePrice, int nYearMonth )
{
	if( m_nType == 5 )
	{
		CString strProduct(BProduct);
		CString strName(BName);
		CString strCall(BCall);
		CString strPut(BPut);

		CString strMsg;

		strMsg.Format(_T("%s %s  Call: %s  Put: %s  履約價:%d 年月:%d"),
			   strProduct,strName,strCall,strPut,nStrikePrice,nYearMonth);

		BSTR bstrMsg = strMsg.AllocSysString();

		SendMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,m_nType,(int)bstrMsg);
		
		SysFreeString(bstrMsg);

	}
}

void _stdcall OnNotifyServerTime1( short sHour, short sMinute, short sSecond, int nTotal) {
	m_pDialog->mKline_stream.sync_server_time ( nTotal );
	m_pDialog->mKline_stream_day.sync_server_time ( nTotal );
}

void _stdcall OnNotifyServerTime( short sHour, short sMinute, short sSecond, int nTotal)
{
	CString strMsg;
	strMsg.Format(_T("時間 %d:%d:%d"),sHour,sMinute,sSecond);

	BSTR bstrMsg = strMsg.AllocSysString();

	SendMessage(FindWindow(NULL,_T("QuoteTester")),WM_DATA,99,(int)bstrMsg);
		
	SysFreeString(bstrMsg);
	//SKQuoteLib_AttchServerTimeCallBack( (UINT_PTR)NULL );
	SKQuoteLib_AttchServerTimeCallBack( (UINT_PTR)OnNotifyServerTime1 );
	m_pDialog->mKline_stream.KLine_server_time( nTotal );
	m_pDialog->mKline_stream_day.KLine_server_time( nTotal );
}

void _stdcall OnNotifyProductsReady()
{
	CQuoteTesterDlg *pDialog = (CQuoteTesterDlg *) AfxGetMainWnd();
	//AfxMessageBox(_T("商品載入完成!"));
	if (GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
		SetEvent(pDialog->Wait_ProductsReady_Event);
	TRACE("Run in UI thread: %x\n", GetCurrentThreadId());
	TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
void CQuoteTesterDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	int  nCode = 0;

	CEdit* pEdit; 
    pEdit = (CEdit*) GetDlgItem(IDC_EDIT_ID); 
	CString strText;
	pEdit->GetWindowTextW(strText);

    pEdit = (CEdit*) GetDlgItem(IDC_EDIT_Pass); 
	CString strPassword;
	pEdit->GetWindowTextW(strPassword);

	CStringA strTempA(strText);
	const CHAR* caText = (LPCSTR)strTempA;
	
	CStringA strTempB(strPassword);
	const CHAR* caPass = (LPCSTR)strTempB;

	//nCode = SKQuoteLib_Initialize(caText,caPass);
	nCode = SKQuoteLib_Initialize("S122811334", "swat9110");

	if( nCode == 0 )
	{
		//註冊CALLBACK
		nCode = SKQuoteLib_AttachConnectionCallBack( (UINT_PTR) OnConnect);
		nCode = SKQuoteLib_AttachQuoteCallBack( (UINT_PTR) OnNotifyQuote);
		nCode = SKQuoteLib_AttachBest5CallBack( (UINT_PTR) OnNotifyBest5);
		//nCode = SKQuoteLib_AttachTicksCallBack( (UINT_PTR) OnNotifyTicks);
		nCode = SKQuoteLib_AttachTicksGetCallBack( (UINT_PTR) OnNotifyTicksGet);
		nCode = SKQuoteLib_AttachKLineDataCallBack( (UINT_PTR)OnNotifyKLineData );
		nCode = SKQuoteLib_AttachMarketTotCallBack( (UINT_PTR)OnNotifyMarketTot );
		nCode = SKQuoteLib_AttachMarketBuySellCallBack( (UINT_PTR)OnNotifyMarketBuySell );
		nCode = SKQuoteLib_AttachMarketHighLowCallBack( (UINT_PTR)OnNotifyMarketHighLow );
		nCode = SKQuoteLib_AttachStrikePricesCallBack( (UINT_PTR)OnNotifyStrikePrices );
		nCode = SKQuoteLib_AttchServerTimeCallBack( (UINT_PTR)OnNotifyServerTime );
		nCode = SKQuoteLib_AttachHistoryTicksGetCallBack( (UINT_PTR) OnNotifyHistoryTicksGet);
		nCode = SKQuoteLib_AttachProductsReadyCallBack( (UINT_PTR) OnNotifyProductsReady);

		AfxMessageBox(_T("初始成功"));
	}
}


void CQuoteTesterDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	m_nType = 1;

	/*Make run in UI thread*/
	//ResetEvent(Wait_Connection_Event);
	//ResetEvent(Wait_ProductsReady_Event);
	int nCOde = SKQuoteLib_EnterMonitor();

	CString strMsg;
	strMsg.Format(_T("SKQuoteLib_EnterMonitor  Code %d"),nCOde);

	BSTR bstrMsg = strMsg.AllocSysString();

	SendMessage(WM_DATA,98,(int)bstrMsg);

	if (GetCurrentThreadId() == AfxGetApp()->m_nThreadID)
		SysFreeString(bstrMsg);
}

void CQuoteTesterDlg::OnBnClickedButton3()
{
	// TODO: Add your control notification handler code here
	SKQuoteLib_LeaveMonitor();
}


LRESULT CQuoteTesterDlg::OnData(WPARAM wParam,LPARAM lParam)
{
	if( wParam == 99 )
	{
		BSTR bstrData = (BSTR)lParam;

		CString strReport( bstrData);

		GetDlgItem(IDC_STATIC_TIME)->SetWindowTextW(strReport); 

		SysFreeString(bstrData);
	
		return 0;
	}
	else if( wParam == 98 )
	{
		BSTR bstrData = (BSTR)lParam;

		CString strReport( bstrData);
	
		CListBox   *pListBox   =   (CListBox *)GetDlgItem(IDC_LIST); 

		pListBox->InsertString(0,strReport);

		SysFreeString(bstrData);
	}
	else if( m_nType == wParam )
	{
		BSTR bstrData = (BSTR)lParam;

		CString strReport( bstrData);
	
		CListBox   *pListBox   =   (CListBox *)GetDlgItem(IDC_LIST); 

		pListBox->InsertString(0,strReport);

		SysFreeString(bstrData);
	
		return 0;
	}
}



void CQuoteTesterDlg::OnBnClickedButton4()
{
	// TODO: Add your control notification handler code here
	int		nCode = 0;
	short	sPageNo = 1;

	m_nType = 1;

	CListBox   *pListBox   =   (CListBox *)GetDlgItem(IDC_LIST); 
	pListBox->ResetContent();

	CEdit* pEdit; 
    pEdit = (CEdit*) GetDlgItem(IDC_EDIT_Stock); 
	CString strText;
	pEdit->GetWindowTextW(strText);

	CStringA strTempA(strText);
	char*   caText   =   strTempA.GetBuffer(strTempA.GetLength()); 

	nCode = SKQuoteLib_RequestStocks(&sPageNo,caText);
	TRACE("Run in UI thread: %x\n", GetCurrentThreadId());
#ifdef _UNICODE
	//knight
#endif
#ifdef _MBCS 
	knight1
#endif
}


void CQuoteTesterDlg::OnBnClickedButton5()
{
	// TODO: Add your control notification handler code here

	int		nCode = 0;
	short	sPageNo = 1;

	m_nType = 2;

	CListBox   *pListBox   =   (CListBox *)GetDlgItem(IDC_LIST); 
	pListBox->ResetContent();

	CEdit* pEdit; 
    pEdit = (CEdit*) GetDlgItem(IDC_EDIT_Ticks); 
	CString strText;
	pEdit->GetWindowTextW(strText);
	
	CStringA strTempA(strText);
	char*   caText   =   strTempA.GetBuffer(strTempA.GetLength()); 
	
	//SKQuoteLib_RequestTicks(&sPageNo,caText);
	//SKQuoteLib_RequestTicks(&sPageNo, "1402");
	SKQuoteLib_RequestTicks(&sPageNo, "TX00");
}


void CQuoteTesterDlg::OnBnClickedButton6()
{
	// TODO: Add your control notification handler code here

	CComboBox   *pComboBox   =   (CComboBox *)GetDlgItem(IDC_COMBO_KLine); 

	int nType = pComboBox->GetCurSel();

	m_nType = 3;

	CListBox   *pListBox   =   (CListBox *)GetDlgItem(IDC_LIST); 
	pListBox->ResetContent();

	CEdit* pEdit; 
    pEdit = (CEdit*) GetDlgItem(IDC_EDIT_KLine); 
	CString strText;
	pEdit->GetWindowTextW(strText);
	
	CStringA strTempA(strText);
	char*   caText   =   strTempA.GetBuffer(strTempA.GetLength()); 

	//strcpy(caText, "TX00");
	//nType = 1;
	//SKQuoteLib_GetKLine(caText,nType);
	/*if ( mKline_stream.get_KLine_ready( "TX00" ) == false ) {
		mKline_stream.load_KLine_from_archive( "TX00" );
		SKQuoteLib_GetKLine("TX00", 1);
		SKQuoteLib_AttchServerTimeCallBack( (UINT_PTR)OnNotifyServerTime );
		//OnBnClickedButton7(); //request server time;
		mKline_stream.set_KLine_ready( "TX00" );
		mKline_stream.candlestick_collapse( "TX00" );
	}*/

	if ( mKline_stream_day.get_KLine_ready( "TX00" ) == false ) {
	mKline_stream_day.load_KLine_from_archive( "TX00" );
	SKQuoteLib_GetKLine( "TX00", 4 );
	/*SKQuoteLib_AttchServerTimeCallBack( (UINT_PTR)OnNotifyServerTime );
	//OnBnClickedButton7(); //request server time;*/
	mKline_stream_day.set_KLine_ready( "TX00" );
	//mKline_stream_day.candlestick_collapse( "TX00" );
	}
	//SKQuoteLib_GetKLine("TX00", 2);
}

void CQuoteTesterDlg::OnBnClickedButton8()
{
	// TODO: Add your control notification handler code here
	CListBox   *pListBox   =   (CListBox *)GetDlgItem(IDC_LIST); 
	pListBox->ResetContent();

	CComboBox   *pComboBox   =   (CComboBox *)GetDlgItem(IDC_COMBO_KLine); 
	int nType = pComboBox->GetCurSel();

	m_nType = 4;
}


void CQuoteTesterDlg::OnBnClickedButton10()
{
	// TODO: Add your control notification handler code here

	CListBox   *pListBox   =   (CListBox *)GetDlgItem(IDC_LIST); 
	pListBox->ResetContent();
	
	CComboBox   *pComboBox   =   (CComboBox *)GetDlgItem(IDC_COMBO_KLine); 
	int nType = pComboBox->GetCurSel();

	m_nType = 5;

	SKQuoteLib_GetStrikePrices();
}

void CQuoteTesterDlg::OnBnClickedButton7()
{
	// TODO: Add your control notification handler code here

	SKQuoteLib_RequestServerTime();
}

void CQuoteTesterDlg::OnBnClickedButton11()
{
	// TODO: Add your control notification handler code here

	char cVersion[1024]  = {0};
	int nSize = sizeof(cVersion);

	SKQuoteLib_GetVersion( cVersion, &nSize);

	AfxMessageBox((CString)cVersion);
}

void CQuoteTesterDlg::OnBnClickedButton12()
{
	// TODO: Add your control notification handler code here

	if( m_nType == 1 )
	{
		TStock* tStock = new TStock();
		BSTR bstrMsg;

		for(int i = 0; i<5; i++)
		{
			int nStockidx = 0;
			while(true)
			{
				int nCode = SKQuoteLib_GetStockByIndex(i,nStockidx,tStock);

				if(nCode != 0)
					break;

				CString strStockNo(tStock->m_caStockNo);
				CString strStockName(tStock->m_caName);

				CString strMsg;
				strMsg.Format(_T("Stockidx:%d  StockNo:%s  StockName:%s"),
					tStock->m_sStockidx,
					strStockNo,
					strStockName);

				bstrMsg = strMsg.AllocSysString();

				SendMessage(WM_DATA,m_nType,(int)bstrMsg);

				nStockidx++;
			}
		}

		SysFreeString(bstrMsg);

		delete tStock;
		tStock = NULL;
	}
}

/*20140914 added by michael*/
DWORD WINAPI ThreadProc_15m_KLine ( LPVOID lpParam ) {
	CQuoteTesterDlg *pDialog;
	DWORD dwWaitResult;

	pDialog = (CQuoteTesterDlg *) lpParam;
#if 1
	if ( pDialog->mKline_stream.get_KLine_ready( "TX00" ) == false ) {
		pDialog->mKline_stream.load_KLine_from_archive( "TX00" );
		dwWaitResult = ::WaitForSingleObject ( g_hEvent_KLine, INFINITE );
		SKQuoteLib_GetKLine("TX00", 1);
		::SetEvent ( g_hEvent_KLine );
	/*SKQuoteLib_AttchServerTimeCallBack( (UINT_PTR)OnNotifyServerTime );
	//OnBnClickedButton7(); //request server time;*/
		pDialog->mKline_stream.set_KLine_ready( "TX00" );
		pDialog->mKline_stream.candlestick_collapse( "TX00" );
	}
#endif
	return 0;
}

DWORD WINAPI ThreadProc_show_KLine ( LPVOID lpParam ) {
	//SKQuoteLib_GetKLine("TX00", 1);
	//SKQuoteLib_GetKLine("TX00", 4);
	return 0;
}

DWORD WINAPI ThreadProc_Day_KLine ( LPVOID lpParam ) {
	CQuoteTesterDlg *pDialog;
	DWORD dwWaitResult;

	pDialog = (CQuoteTesterDlg *) lpParam;
#if 1
	//SKQuoteLib_GetKLine(caText,nType); nType is combo-box selection which is zero-base
	if ( pDialog->mKline_stream_day.get_KLine_ready( "TX00" ) == false ) {
		pDialog->mKline_stream_day.load_KLine_from_archive( "TX00" );
		dwWaitResult = ::WaitForSingleObject ( g_hEvent_KLine, INFINITE );
		SKQuoteLib_GetKLine( "TX00", 4 );
		::SetEvent ( g_hEvent_KLine );
	/*SKQuoteLib_AttchServerTimeCallBack( (UINT_PTR)OnNotifyServerTime );
	//OnBnClickedButton7(); //request server time;*/
		pDialog->mKline_stream_day.set_KLine_ready( "TX00" );
	//mKline_stream_day.candlestick_collapse( "TX00" );
	}
#endif
	return 0;
}

/*20140825 added by michael*/
DWORD WINAPI do_quote(LPVOID dlg) {
	int  nCode = 0;
	DWORD dwEvent;
	CWnd *pWnd;
	MSG msg;

	CQuoteTesterDlg *pDialog;

	pDialog = (CQuoteTesterDlg *) dlg;
	/*CEdit* pEdit; 
	pEdit = (CEdit*) pDialog->GetDlgItem(IDC_EDIT_ID); 
	CString strText;
	pEdit->GetWindowTextW(strText);

    pEdit = (CEdit*) pDialog->GetDlgItem(IDC_EDIT_Pass); 
	CString strPassword;
	pEdit->GetWindowTextW(strPassword);

	CStringA strTempA(strText);
	const CHAR* caText = strTempA.GetString();
	
	CStringA strTempB(strPassword);
	const CHAR* caPass = (LPCSTR)strTempB;

	//nCode = SKQuoteLib_Initialize(caText,caPass);
	nCode = SKQuoteLib_Initialize("S122811334", "swat9110");*/

	pDialog->ReadyForTrading = false;
	//if( nCode == 0 )
	{
		//註冊CALLBACK
		/*nCode = SKQuoteLib_AttachConnectionCallBack( (UINT_PTR) OnConnect);
		nCode = SKQuoteLib_AttachQuoteCallBack( (UINT_PTR) OnNotifyQuote);
		nCode = SKQuoteLib_AttachBest5CallBack( (UINT_PTR) OnNotifyBest5);
		//nCode = SKQuoteLib_AttachTicksCallBack( (UINT_PTR) OnNotifyTicks);
		nCode = SKQuoteLib_AttachTicksGetCallBack( (UINT_PTR) OnNotifyTicksGet);
		nCode = SKQuoteLib_AttachKLineDataCallBack( (UINT_PTR)OnNotifyKLineData );
		nCode = SKQuoteLib_AttachMarketTotCallBack( (UINT_PTR)OnNotifyMarketTot );
		nCode = SKQuoteLib_AttachMarketBuySellCallBack( (UINT_PTR)OnNotifyMarketBuySell );
		nCode = SKQuoteLib_AttachMarketHighLowCallBack( (UINT_PTR)OnNotifyMarketHighLow );
		nCode = SKQuoteLib_AttachStrikePricesCallBack( (UINT_PTR)OnNotifyStrikePrices );
		nCode = SKQuoteLib_AttchServerTimeCallBack( (UINT_PTR)OnNotifyServerTime );
		nCode = SKQuoteLib_AttachHistoryTicksGetCallBack( (UINT_PTR) OnNotifyHistoryTicksGet);
		nCode = SKQuoteLib_AttachProductsReadyCallBack( (UINT_PTR) OnNotifyProductsReady);

		AfxMessageBox(_T("初始成功"));*/

		/*switch to main thread and enter monitor then wait connecttion callback*/
		if (GetCurrentThreadId() != AfxGetApp()->m_nThreadID) {
			::SendMessage(pDialog->m_hWnd, WM_RUN_IN_UI_THREAD, 101, (int)0);
			dwEvent = WaitForSingleObject(pDialog->Wait_ProductsReady_Event, INFINITE);
			if ( dwEvent == WAIT_OBJECT_0 ) {
				pDialog->ReadyForTrading = true;
				/*pWnd = pDialog->GetDlgItem(IDC_BUTTON13);
				pWnd->EnableWindow( FALSE );
				pWnd = pDialog->GetDlgItem(IDC_BUTTON14);
				pWnd->EnableWindow( TRUE );*/
			}			
		}
		else {
			pDialog->OnBnClickedButton2();
		}
	}
	//pDialog->OnBnClickedButton2();
		//dwEvent = WaitForSingleObject(pDialog->Wait_ProductsReady_Event, INFINITE);

	DWORD dwThreadID, dwWaitResult;
	if (pDialog->ReadyForTrading == true) {
		/*K line data*/
		TRACE("Run in thread: %x\n", GetCurrentThreadId());
		TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
		//pDialog->mpKline_stream = new CKLineStream();
		//pDialog->OnBnClickedButton6();
		g_hEvent_KLine = ::CreateEvent(NULL, FALSE, TRUE, NULL);
		g_hThreads_KLine [ 0 ] = ::CreateThread(NULL, 0, ThreadProc_15m_KLine, (LPVOID) dlg, 0, g_ThreadID_KLine);
		g_hThreads_KLine [ 1 ] = ::CreateThread(NULL, 0, ThreadProc_Day_KLine, (LPVOID) dlg, 0, g_ThreadID_KLine+1);
		g_hThreads_KLine [ 2 ] = ::CreateThread(NULL, 0, ThreadProc_show_KLine, (LPVOID) dlg, 0, g_ThreadID_KLine+2);
		dwWaitResult = WaitForMultipleObjects(
        2,   // number of handles in array
        g_hThreads_KLine,     // array of thread handles
        TRUE,          // wait until all are signaled
        INFINITE);
		pDialog->OnBnClickedButton5();
	}

#if 1
	TTick *tTick;
	//DWORD dwWaitResult;
	short sStockidx;
	while(::GetMessage( &msg, NULL, 0, 0 )) {
		if ( msg.message == WM_HISTORY_TICK ) {
			nCode = 0;
			tTick = ( TTick * ) msg.lParam;
			sStockidx = (int) msg.wParam;
			m_pDialog->mKline_stream.Push_Tick_Data( m_pDialog->mMap_stockidx_stockNo[ sStockidx ], tTick->m_nPtr,
			tTick->m_nTime,
			tTick->m_nBid,
			tTick->m_nAsk,
			tTick->m_nClose,
			tTick->m_nQty, 1 );
			dwWaitResult = WaitForSingleObject( 
				m_pDialog->ghMutex,    // handle to mutex
				INFINITE);  // no time-out interval
			m_pDialog->m_Queue_pTTick.push_back(tTick);
			ReleaseMutex(m_pDialog->ghMutex);
		}
	}
#endif
	return 0;
}

void CQuoteTesterDlg::OnBnClickedButton13()
{
	// TODO: Add your control notification handler code here
	GetDlgItem(IDC_BUTTON13)->EnableWindow( FALSE );
	GetDlgItem(IDC_BUTTON14)->EnableWindow( TRUE );
	int  nCode = 0;
	t_hnd = ::CreateThread(0, 0, do_quote, this, NULL, &t_id);

	CEdit* pEdit; 
	pEdit = (CEdit*) GetDlgItem(IDC_EDIT_ID); 
	CString strText;
	pEdit->GetWindowTextW(strText);

    pEdit = (CEdit*) GetDlgItem(IDC_EDIT_Pass); 
	CString strPassword;
	pEdit->GetWindowTextW(strPassword);

	CStringA strTempA(strText);
	const CHAR* caText = strTempA.GetString();
	
	CStringA strTempB(strPassword);
	const CHAR* caPass = (LPCSTR)strTempB;

	//nCode = SKQuoteLib_Initialize(caText,caPass);
	nCode = SKQuoteLib_Initialize("S122811334", "swat9110");
	if( nCode == 0 )
	{
		//註冊CALLBACK
		nCode = SKQuoteLib_AttachConnectionCallBack( (UINT_PTR) OnConnect);
		nCode = SKQuoteLib_AttachQuoteCallBack( (UINT_PTR) OnNotifyQuote);
		nCode = SKQuoteLib_AttachBest5CallBack( (UINT_PTR) OnNotifyBest5);
		//nCode = SKQuoteLib_AttachTicksCallBack( (UINT_PTR) OnNotifyTicks);
		nCode = SKQuoteLib_AttachTicksGetCallBack( (UINT_PTR) OnNotifyTicksGet);
		nCode = SKQuoteLib_AttachKLineDataCallBack( (UINT_PTR)OnNotifyKLineData );
		nCode = SKQuoteLib_AttachMarketTotCallBack( (UINT_PTR)OnNotifyMarketTot );
		nCode = SKQuoteLib_AttachMarketBuySellCallBack( (UINT_PTR)OnNotifyMarketBuySell );
		nCode = SKQuoteLib_AttachMarketHighLowCallBack( (UINT_PTR)OnNotifyMarketHighLow );
		nCode = SKQuoteLib_AttachStrikePricesCallBack( (UINT_PTR)OnNotifyStrikePrices );
		nCode = SKQuoteLib_AttchServerTimeCallBack( (UINT_PTR)OnNotifyServerTime1 );
		nCode = SKQuoteLib_AttachHistoryTicksGetCallBack( (UINT_PTR) OnNotifyHistoryTicksGet);
		nCode = SKQuoteLib_AttachProductsReadyCallBack( (UINT_PTR) OnNotifyProductsReady);

		//AfxMessageBox(_T("初始成功"));
	}

	//OnBnClickedButton1();
	//Sleep(10000);
	//OnBnClickedButton2();
	//Sleep(10000);
	//OnBnClickedButton6();
	TRACE("Run in UI thread: %x\n", GetCurrentThreadId());
	TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
}


void CQuoteTesterDlg::OnBnClickedButton14()
{
	CWnd *pWnd;

	this->OnBnClickedButton3();
	pWnd = GetDlgItem(IDC_BUTTON13);
	pWnd->EnableWindow( TRUE );
	pWnd = GetDlgItem(IDC_BUTTON14);
	pWnd->EnableWindow( FALSE );
	
	TerminateThread( t_hnd, (DWORD) 0 );
	//CloseHandle(t_hnd);
	//t_hnd = NULL;
	OnBnClickedButton3();
}

LRESULT CQuoteTesterDlg::OnDoTaskInMainThread(WPARAM wParam, LPARAM lParam) {
	if( wParam == 101 ) {
		OnBnClickedButton2();
	}

	return 0;
}

LRESULT CQuoteTesterDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	int k = 0;
	/*switch (message) {
		case WM_RUN_IN_UI_THREAD:
			break;
		default:
			return CDialogEx::WindowProc(message, wParam, lParam);
	}*/
	switch ( message ) {
		case WM_DATA:
			k = 5;
			break;
	}
	return CDialogEx::WindowProc(message, wParam, lParam);
}

void CQuoteTesterDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: Add your message handler code here
}

void CQuoteTesterDlg::OnNcDestroy()
{
	CDialogEx::OnNcDestroy();

	// TODO: Add your message handler code here
}


BOOL CQuoteTesterDlg::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	/*if ( nID == IDC_BUTTON13 ) {
		TRACE("%s %s %d\n", __FILE__, __FUNCTION__, __LINE__);
	}*/

	switch (nID) {
	  //case IDC_BUTTON13:
	  //case IDC_BUTTON14:
		  //return CDialogEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
		  //break;
	  /*case IDC_BUTTON1:
	  case IDC_BUTTON2:
	  case IDC_BUTTON3:
	  case IDC_BUTTON4:
	  case IDC_BUTTON5:
	  case IDC_BUTTON6:
	  case IDC_BUTTON7:
	  case IDC_BUTTON8:
	  case IDC_BUTTON10:
	  case IDC_BUTTON11:
	  case IDC_BUTTON12:
		  return FALSE;
		  break;*/
	  default:
		  return CWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
	}
}