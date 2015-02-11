
// OrderTesterDlg.h : header file
//

#pragma once

#include <map>
#include <vector>
#include "StockDlg.h"
#include "FutureDlg.h"
#include "OptionDlg.h"
#include "ForeignStockDlg.h"
#include "FutureStopLossDlg.h"
#include "OptionStopLossDlg.h"
#include "OverseaFutureDlg.h"
#include "OverseaOptionDlg.h"

using namespace std;

#define CDialogEx CDialog
#define WM_MESSAGE WM_USER+100
/*20141227 added by michael*/
#define WM_LOGIN ( WM_USER + 101 )
/*20141229 added by michael*/
#define WM_REFRESH_FUTURE_RIGHT ( WM_USER + 102 )
#define WM_REFRESH_OPEN_INTEREST ( WM_USER + 103 )
// COrderTesterDlg dialog
class COrderTesterDlg : public CDialogEx
{
	DECLARE_DYNAMIC(COrderTesterDlg)
private:
	CStockDlg			m_StockDlg;
	CFutureDlg			m_FutureDlg;
	COptionDlg			m_OptionDlg;
	CForeignStockDlg	m_ForeignStockDlg;
	CFutureStopLossDlg	m_FutureStopLossDlg;
	COptionStopLossDlg	m_OptionStopLossDlg;
	COverseaFutureDlg	m_OverseaFutureDlg;
	COverseaOptionDlg	m_OverseaOptionDlg;

	bool		m_bRegister;
// Construction
public:
	COrderTesterDlg(CWnd* pParent = NULL);	// standard constructor
	~COrderTesterDlg ( );

// Dialog Data
	enum { IDD = IDD_ORDERTESTER_DIALOG };
	enum { enum_StockDlg = 0, enum_FutureDlg, enum_OptionDlg, enum_ForeignStockDlg, enum_FutureStopLossDlg, enum_OptionStopLossDlg, enum_OverseaFutureDlg, enum_OverseaOptionDlg } m_enum_TabSel;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnLogin(WPARAM wParam,LPARAM lParam);
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	
	afx_msg LRESULT OnMessage(WPARAM wParam,LPARAM lParam);

	afx_msg void OnBnClickedButtonInit();
	CComboBox m_boxTSAccount;
	CComboBox m_boxTFAccount;
	CComboBox m_boxOSAccount;
	CComboBox m_boxOFAccount;
	CEdit m_editID;
	CEdit m_editPassWord;
	
	vector<CString> DivideByComma( CString strBuf);

	void AddAccount(const CString& strAccount);
	CListBox m_listReports;
	void AddReport(const CString& strReports);
	CTabCtrl m_TabControl;
	afx_msg void OnSelchangeComboTsaccount();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnTcnSelchangeTabControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchangingTabControl(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboTfaccount();
	afx_msg void OnCbnSelchangeComboOsaccount();
	afx_msg void OnCbnSelchangeComboOfaccount();
	afx_msg void OnNcDestroy();
/*20141227 added by michael*/
	static int m_Account_count;
	int m_Account_index, m_get_TF_accounts;
	CString m_strAccount;
	HANDLE g_hEvent_Account_Ready/*, g_hEvent_FutureRight_Ready, g_hEvent_OpenInterest_Ready*/;
/*20150101 added by michael*/
	DWORD m_WndOwnerThreadID;
	void Switch_TabControl ( int NewPage );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
/* 20150113 added by michael */
	double margin, free_margin, equity;
	map< string, double > mMap_origin_margin, mMap_keep_margin, mMap_perpip_value;
	int m_nlong_position, m_nshort_position;
	map< string, int > mMap_open_interest;
	BOOL isTimer_start;
};

class CAboutDlg : public CDialogEx
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
};

struct TLogin {
	char LoginID [ 30 ], Password [ 30 ];
};

#define LOGIN_COPYDATA 0
#define ORDER_COPYDATA 1