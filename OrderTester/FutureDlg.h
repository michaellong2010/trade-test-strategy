#pragma once


// CFutureDlg dialog

class CFutureDlg : public CDialog
{
	DECLARE_DYNAMIC(CFutureDlg)

public:
	CFutureDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFutureDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_FUTURE };

private:
	CString m_strAccount;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:

	void SetAccount( CString strAccount );

	afx_msg void OnBnClickedButtonSendfutureorder();
	CComboBox m_boxBuySell;
	CComboBox m_boxOrderType;
	CComboBox m_boxDayTrade;
	afx_msg void OnBnClickedButtonSendfutureorderasync();
	afx_msg void OnBnClickedButtonCancelStockno();
	afx_msg void OnBnClickedButtonCancelSqlno();
	afx_msg void OnBnClickedButtonDecrease();
	CComboBox m_boxCorrcetOrderType;
	afx_msg void OnBnClickedButtonSendCorrect();
	afx_msg void OnBnClickedButtonCorrectbybookno();
	afx_msg void OnBnClickedButtonOpeninterest();
	afx_msg void OnBnClickedButtonFutureright();
	afx_msg void OnBnClickedButtonExecutionReport();
};
