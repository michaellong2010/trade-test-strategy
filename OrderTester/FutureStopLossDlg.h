#pragma once


// CFutureStopLossDlg dialog

class CFutureStopLossDlg : public CDialog
{
	DECLARE_DYNAMIC(CFutureStopLossDlg)

public:
	CFutureStopLossDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFutureStopLossDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_FUTURESTOPLOSS };
	
private:
	CString m_strAccount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void SetAccount( CString strAccount );

	afx_msg void OnBnClickedButtonSendfuturestoploss();
	CComboBox m_boxBuySell;
	CComboBox m_boxOrderType;
	CComboBox m_boxDayTrade;
	afx_msg void OnBnClickedButtonCancelfuturestoploss();
	CComboBox m_boxKind;
	CComboBox m_boxReportStutus;
	afx_msg void OnBnClickedButtonStoplossreport();
	afx_msg void OnBnClickedButtonSendfuturemovingstoploss();
	CComboBox m_boxBuySell2;
	CComboBox m_boxOrderType2;
	CComboBox m_boxDayTrade2;
	afx_msg void OnBnClickedButtonCancelfuturestoploss2();
};
