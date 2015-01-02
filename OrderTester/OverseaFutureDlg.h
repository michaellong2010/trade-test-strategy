#pragma once


// COverseaFutureDlg dialog

class COverseaFutureDlg : public CDialog
{
	DECLARE_DYNAMIC(COverseaFutureDlg)

public:
	COverseaFutureDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~COverseaFutureDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_OVERSEAFUTURE };

private:
	CString m_strAccount;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	void SetAccount(CString strAccount);

	afx_msg void OnBnClickedButtonSendoverseafuture();
	CComboBox m_boxBuySell;
	CComboBox m_boxDaytrade;
	CComboBox m_boxNewClose;
	CComboBox m_boxOrderType;
	CComboBox m_boxSpecialTradeType;
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonGetoverseaproduct();
	afx_msg void OnBnClickedButtonGetoverseaopeninterest();
	afx_msg void OnBnClickedButtonCancelSqlno();
	afx_msg void OnBnClickedButtonDecrease();
	afx_msg void OnBnClickedButtonOfcorrectprice();
	CComboBox m_boxCorrectSpecialTradeType;
	afx_msg void OnBnClickedButtonSendosspread();
	afx_msg void OnBnClickedButtonSendosspreadasync();
};
