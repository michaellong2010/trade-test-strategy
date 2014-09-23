
// QuoteTesterDlg.h : ���Y��
//
#include "KLine_Stream.h"
#include "Functions.h"
#include "Account.h"
#include <deque>
#include <cmath>
#include <math.h>
#pragma once

using namespace std; 
#define WM_DATA WM_USER+1
#define WM_TICK WM_USER+3
#define WM_HISTORY_TICK WM_USER+5

// CQuoteTesterDlg ��ܤ��
class CQuoteTesterDlg : public CDialogEx
{
// �غc
public:
	CQuoteTesterDlg(CWnd* pParent = NULL);	// �зǫغc�禡
	~CQuoteTesterDlg();

// ��ܤ�����
	enum { IDD = IDD_QUOTETESTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩


// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
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

	boolean ReadyForTrading;

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

	/*fstream only be accessed by belong thread�ACKLineStream feature in multiple symbols single time frame*/
	CKLineStream mKline_stream, mKline_stream_day;

	/*implement object pointer queue to reduce dynamic alloc and improve efficiency*/
	deque <TStock *> m_Queue_pTStock;
	deque <TBest5 *> m_Queue_pTBest5;
	deque <TTick *> m_Queue_pTTick;

	/*Request Quote or Tick, Best5 use stock index correspond to stock No.�AKLine use stock No. as index�Athe stock index may be different every day*/
	map <short, string> mMap_stockidx_stockNo;

	HANDLE ghMutex;

	/*build account*/
	CAccount account_A, account_B;
	int trading_date;

	/*user-defined MA paramter�Ainclude MA period(candlestick number)
	MA margin=MA*(1+N%)*/
	int mMA1_period, mMA2_period, mMA3_period;
	double mMA1_margin, mMA2_margin, mMA3_margin;
};
