#pragma once


// COptionStopLossDlg dialog

class COptionStopLossDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionStopLossDlg)

public:
	COptionStopLossDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionStopLossDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OPTIONSTOPLOSS };
private:
	CString m_strAccount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void SetAccount(CString strAccount);

	afx_msg void OnBnClickedButtonSendoptionstoploss();
	CComboBox m_boxBuySell;
	CComboBox m_boxOrderType;
	CComboBox m_boxNewClose;
	afx_msg void OnBnClickedButtonCanceloptionstoploss();
};
