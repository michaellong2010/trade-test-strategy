
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


#define WM_MESSAGE WM_USER+100
#define CDialogEx CDialog
// COrderTesterDlg dialog
class COrderTesterDlg : public CDialogEx
{
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

// Dialog Data
	enum { IDD = IDD_ORDERTESTER_DIALOG };

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

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	HACCEL  m_hAccelTable;
// Implementation
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
public:
	afx_msg void OnNcDestroy();
	virtual void PostNcDestroy();   // default to delete this.
};
extern CAboutDlg about_dlg;