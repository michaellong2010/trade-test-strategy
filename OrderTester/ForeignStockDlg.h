#pragma once


// CForeignStockDlg dialog

class CForeignStockDlg : public CDialog
{
	DECLARE_DYNAMIC(CForeignStockDlg)

public:
	CForeignStockDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CForeignStockDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_FOREIGNSTOCK };
private:
	CString m_strAccount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	void SetAccount(CString strAccount);

	afx_msg void OnBnClickedButtonSendforeignstock();
	CComboBox m_boxAccountType;
	CComboBox m_boxBuySell;
	CComboBox m_boxCurrency1;
	CComboBox m_boxCurrency2;
	CComboBox m_boxCurrency3;
	CComboBox m_boxExchange;
	afx_msg void OnBnClickedButtonSendforeingostockasync();
	CComboBox m_boxCancelExg;
	afx_msg void OnBnClickedButtonCancelseqno();
	afx_msg void OnBnClickedButtonCancelbookno();
};
