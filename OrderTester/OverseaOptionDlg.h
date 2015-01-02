#pragma once


// COverseaOptionDlg dialog

class COverseaOptionDlg : public CDialog
{
	DECLARE_DYNAMIC(COverseaOptionDlg)

public:
	COverseaOptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COverseaOptionDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OVERSEAOPTION };

private:
	CString m_strAccount;

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void SetAccount(CString strAccount);
	afx_msg void OnBnClickedButtonSendoverseaoption();
	CComboBox m_boxBuySell;
	CComboBox m_boxNewClose;
	CComboBox m_boxDaytrade;
	CComboBox m_boxOrderType;
	CComboBox m_boxSpecialTradeType;
	afx_msg void OnBnClickedButtonSendoverseaoptionasync();
	afx_msg void OnBnClickedButtonGetoverseaoptionproduct();
};
