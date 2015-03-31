
// QuoteTesterDlg.h : 標頭檔
//
#pragma once
#include "KLine_Stream.h"
#include "Functions.h"
//#include "Account.h"
#include <deque>
#include <cmath>
#include <math.h>
#include "Order.h"

using namespace std; 
#define WM_DATA WM_USER+1
#define WM_TICK WM_USER+3
#define WM_HISTORY_TICK WM_USER+5
#define CDialogEx CDialog

// CQuoteTesterDlg_New_UI 對話方塊
class CQuoteTesterDlg_New_UI : public CDialogEx
{
	DECLARE_DYNAMIC(CQuoteTesterDlg_New_UI)
// 建構
public:
	CQuoteTesterDlg_New_UI(CWnd* pParent = NULL);	// 標準建構函式
	~CQuoteTesterDlg_New_UI();

// 對話方塊資料
	enum { IDD = IDD_QUOTETESTER_DIALOG1 };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援


// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	int enable_MA_margin;
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	
	afx_msg LRESULT OnData(WPARAM wParam,LPARAM lParam);

	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();

	afx_msg LRESULT OnDoTaskInMainThread(WPARAM wParam,LPARAM lParam);
	HANDLE Wait_Connection_Event, Wait_ProductsReady_Event;
	afx_msg void OnDestroy();
	afx_msg void OnNcDestroy();
	afx_msg void OnChange_MA1_Period();
	afx_msg void OnKillfocus_MA1_Period();
	afx_msg void OnKillfocus_MA2_Period();
	afx_msg void OnKillfocus_MA3_Period();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnKillfocus_MA1_margin();
	afx_msg void OnKillfocus_MA2_margin();
	afx_msg void OnKillfocus_MA3_margin();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnKillfocus_Stoploss ();

	boolean ReadyForTrading;

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

	/*fstream only be accessed by belong thread，CKLineStream feature in multiple symbols single time frame*/
	CKLineStream mKline_stream, mKline_stream1;

	/*implement object pointer queue to reduce dynamic alloc and improve efficiency*/
	deque <TStock *> m_Queue_pTStock;
	deque <TBest5 *> m_Queue_pTBest5;
	deque <TTick *> m_Queue_pTTick;

	/*Request Quote or Tick, Best5 use stock index correspond to stock No.，KLine use stock No. as index，the stock index may be different every day*/
	map <short, string> mMap_stockidx_stockNo;

	HANDLE ghMutex;

	/*build account*/
	CAccount account_A, account_B;
	int trading_date;

	/*user-defined MA paramter，include MA period(candlestick number)
	MA margin=MA*(1+N%)*/
	int mMA1_period, mMA2_period, mMA3_period;
	double mMA1_margin, mMA2_margin, mMA3_margin;
	double mMA1_margin_factor, mMA2_margin_factor, mMA3_margin_factor;

	/*combobox strategy methods combination*/
	CComboBox m_ComboBox1_strategy;
	void AdjustDropDownWidth(int nID);
	int m_strategy, m_stoploss, m_en_stoploss, m_en_trailing_stop, m_en_trade_MA_ambigous;
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck6();

	CCapitalOrder m_Order_operator1, m_Order_operator2;
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CStatusBarCtrl m_bar;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton16();
	BOOL m_en_gap;
	BOOL m_bid_ask_weight_ratio;
	TStrategy_info m_Strategy1, m_Strategy2;
	CListBox m_listReports;
	BOOL m_en_open_interest;
	afx_msg void OnCbnSelchangeCombo2();
	void load_account_strategy ( TStrategy_info account_strategy );
	void backup_account_strategy ( TStrategy_info & account_strategy );
	int m_cur_setting_strategy;
	BOOL m_simulation_only;
	afx_msg void OnBnClickedCheck8();
	int mEscapeTradingDays;
	BOOL need_reconnect, isTimer_start;
	int m_cur_connection_phase, m_timer_countdown;
	void LoginOrderAccount ( const char *ID, const char *Pass );
	string m_login_id, m_login_pass;
	int mTrailingTriggerPoints, mTrailingPercent;
};