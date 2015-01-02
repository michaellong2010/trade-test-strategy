// ForeignStockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderTester.h"
#include "ForeignStockDlg.h"
//#include "afxdialogex.h"
#include "Functions.h"
#include "OrderTesterDlg.h"


// CForeignStockDlg dialog

IMPLEMENT_DYNAMIC(CForeignStockDlg, CDialog)

CForeignStockDlg::CForeignStockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CForeignStockDlg::IDD, pParent)
{

}

CForeignStockDlg::~CForeignStockDlg()
{
}

void CForeignStockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ACCOUNTTYPE, m_boxAccountType);
	DDX_Control(pDX, IDC_COMBO_BUYSELL, m_boxBuySell);
	DDX_Control(pDX, IDC_COMBO_CURRENCY1, m_boxCurrency1);
	DDX_Control(pDX, IDC_COMBO_CURRENCY2, m_boxCurrency2);
	DDX_Control(pDX, IDC_COMBO_CURRENCY3, m_boxCurrency3);
	DDX_Control(pDX, IDC_COMBO_EXCHANGE, m_boxExchange);
	DDX_Control(pDX, IDC_COMBO_CANCEL_EXCHANGE, m_boxCancelExg);
}


BEGIN_MESSAGE_MAP(CForeignStockDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SENDFOREIGNSTOCK, &CForeignStockDlg::OnBnClickedButtonSendforeignstock)
	ON_BN_CLICKED(IDC_BUTTON_SENDFOREINGOSTOCKASYNC, &CForeignStockDlg::OnBnClickedButtonSendforeingostockasync)
	ON_BN_CLICKED(IDC_BUTTON_CANCELSEQNO, &CForeignStockDlg::OnBnClickedButtonCancelseqno)
	ON_BN_CLICKED(IDC_BUTTON_CANCELBOOKNO, &CForeignStockDlg::OnBnClickedButtonCancelbookno)
END_MESSAGE_MAP()


// CForeignStockDlg message handlers

void CForeignStockDlg::SetAccount(CString strAccount)
{
	m_strAccount = strAccount;
}

void CForeignStockDlg::OnBnClickedButtonSendforeignstock()
{
	// TODO: Add your control notification handler code here

	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("OSAccount is not assigned"));
		return;
	}
	
	int nSize = 1024;
	TCHAR caBuf[1024] = {0};
	
	CString strStockNo;
	CString strPrice;
	CString strQty;
	int		nQty;

	GetDlgItemText(IDC_EDIT_STOCKNO,strStockNo);
	GetDlgItemText(IDC_EDIT_PRICE,strPrice);
	GetDlgItemText(IDC_EDIT_QTY,strQty);
	
	nQty = atoi( CStringA(strQty) );

	int nBuySell		= m_boxBuySell.GetCurSel();
	int nAccountType	= m_boxAccountType.GetCurSel()+1;

	CString strCurrency1;
	m_boxCurrency1.GetWindowTextA(strCurrency1);
	
	CString strCurrency2;
	m_boxCurrency2.GetWindowTextA(strCurrency2);
	
	CString strCurrency3;
	m_boxCurrency3.GetWindowTextA(strCurrency3);
	
	CString strExchangeNo;
	m_boxExchange.GetWindowTextA(strExchangeNo);

	int nCode = SendForeignStockOrder(m_strAccount,strStockNo,strExchangeNo,nBuySell,strPrice,nQty,strCurrency1,strCurrency2,strCurrency3,nAccountType,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);

	pDlg->AddReport( strMsg);

}


void CForeignStockDlg::OnBnClickedButtonSendforeingostockasync()
{
	// TODO: Add your control notification handler code here

	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("OSAccount is not assigned"));
		return;
	}
	
	int nSize = 1024;
	TCHAR caBuf[1024] = {0};
	
	CString strStockNo;
	CString strPrice;
	CString strQty;
	int		nQty;

	GetDlgItemText(IDC_EDIT_STOCKNO,strStockNo);
	GetDlgItemText(IDC_EDIT_PRICE,strPrice);
	GetDlgItemText(IDC_EDIT_QTY,strQty);
	
	nQty = atoi( CStringA(strQty) );

	int nBuySell		= m_boxBuySell.GetCurSel();
	int nAccountType	= m_boxAccountType.GetCurSel()+1;

	CString strCurrency1;
	m_boxCurrency1.GetWindowTextA(strCurrency1);
	
	CString strCurrency2;
	m_boxCurrency2.GetWindowTextA(strCurrency2);
	
	CString strCurrency3;
	m_boxCurrency3.GetWindowTextA(strCurrency3);
	
	CString strExchangeNo;
	m_boxExchange.GetWindowTextA(strExchangeNo);

	int nCode = SendForeignStockOrderAsync(m_strAccount,strStockNo,strExchangeNo,nBuySell,strPrice,nQty,strCurrency1,strCurrency2,strCurrency3,nAccountType,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return Code : %d"),nCode);

	pDlg->AddReport( strMsg);
}


void CForeignStockDlg::OnBnClickedButtonCancelseqno()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("OSAccount is not assigned"));
		return;
	}
	
	int nSize = 1024;
	TCHAR caBuf[1024] = {0};
	
	CString strExchangeNo;
	m_boxExchange.GetWindowTextA(strExchangeNo);

	CString strSeqNo;
	GetDlgItemText(IDC_EDIT_CANCEL_SEQNO,strSeqNo);

	int nCode = CancelForeignStockOrderBySeqNo(m_strAccount,strSeqNo,strExchangeNo,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return Code : %d"),nCode);

	pDlg->AddReport( strMsg);

}


void CForeignStockDlg::OnBnClickedButtonCancelbookno()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("OSAccount is not assigned"));
		return;
	}
	
	int nSize = 1024;
	TCHAR caBuf[1024] = {0};
	
	CString strExchangeNo;
	m_boxExchange.GetWindowTextA(strExchangeNo);

	CString strBookNo;
	GetDlgItemText(IDC_EDIT_CANCEL_BOOKNO,strBookNo);

	int nCode = CancelForeignStockOrderByBookNo(m_strAccount,strBookNo,strExchangeNo,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return Code : %d"),nCode);

	pDlg->AddReport( strMsg);
}
