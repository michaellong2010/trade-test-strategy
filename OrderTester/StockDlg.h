#pragma once


// CStockDlg dialog

class CStockDlg : public CDialog
{
	DECLARE_DYNAMIC(CStockDlg)

public:
	CStockDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CStockDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_STOCK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
	CString m_strAccount;
public:
	void SetAccount(CString strText);
	CComboBox m_boxBuySell;
	CComboBox m_boxDayTrade;
	CComboBox m_boxOrderType;
	CEdit m_editPrice;
	CEdit m_editQty;
	CEdit m_editStockNo;
	afx_msg void OnBnClickedButtonSendorder();
	afx_msg void OnBnClickedButtonSendorderasync();
	afx_msg void OnBnClickedButtonCancelStockno();
	afx_msg void OnBnClickedButtonCancelSqlno();
	afx_msg void OnBnClickedButtonDecrease();
	afx_msg void OnBnClickedButton1();
};
