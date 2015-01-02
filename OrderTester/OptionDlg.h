#pragma once


// COptionDlg dialog

class COptionDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionDlg)

public:
	COptionDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OPTION };

private:
	CString m_strAccount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	void SetAccount(CString strAccount);

	afx_msg void OnBnClickedButtonSendoptionorder();
	CComboBox m_boxBuySell;
	CComboBox m_boxOrderType;
	CComboBox m_boxNewClose;
	afx_msg void OnBnClickedButtonSendoptionorderasync();
};
