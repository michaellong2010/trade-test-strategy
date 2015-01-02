// OverseaFutureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderTester.h"
#include "OverseaFutureDlg.h"
//#include "afxdialogex.h"
#include "Functions.h"
#include "OrderTesterDlg.h"

// COverseaFutureDlg dialog

IMPLEMENT_DYNAMIC(COverseaFutureDlg, CDialog)

COverseaFutureDlg::COverseaFutureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COverseaFutureDlg::IDD, pParent)
{

}

COverseaFutureDlg::~COverseaFutureDlg()
{

}

void COverseaFutureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BUYSELL, m_boxBuySell);
	DDX_Control(pDX, IDC_COMBO_DAYTRADE, m_boxDaytrade);
	DDX_Control(pDX, IDC_COMBO_NEWCLOSE, m_boxNewClose);
	DDX_Control(pDX, IDC_COMBO_ORDERTYPE, m_boxOrderType);
	DDX_Control(pDX, IDC_COMBO_SPECIALTRADETYPE, m_boxSpecialTradeType);
	DDX_Control(pDX, IDC_COMBO_CORRECT_SPECIALTRADETYPE, m_boxCorrectSpecialTradeType);
}


BEGIN_MESSAGE_MAP(COverseaFutureDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SENDOVERSEAFUTURE, &COverseaFutureDlg::OnBnClickedButtonSendoverseafuture)
	ON_BN_CLICKED(IDC_BUTTON2, &COverseaFutureDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON_GETOVERSEAPRODUCT, &COverseaFutureDlg::OnBnClickedButtonGetoverseaproduct)
	ON_BN_CLICKED(IDC_BUTTON_GETOVERSEAOPENINTEREST, &COverseaFutureDlg::OnBnClickedButtonGetoverseaopeninterest)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_SQLNO, &COverseaFutureDlg::OnBnClickedButtonCancelSqlno)
	ON_BN_CLICKED(IDC_BUTTON_DECREASE, &COverseaFutureDlg::OnBnClickedButtonDecrease)
	ON_BN_CLICKED(IDC_BUTTON_OFCORRECTPRICE, &COverseaFutureDlg::OnBnClickedButtonOfcorrectprice)
	ON_BN_CLICKED(IDC_BUTTON_SENDOSSPREAD, &COverseaFutureDlg::OnBnClickedButtonSendosspread)
	ON_BN_CLICKED(IDC_BUTTON_SENDOSSPREADASYNC, &COverseaFutureDlg::OnBnClickedButtonSendosspreadasync)
END_MESSAGE_MAP()


// COverseaFutureDlg message handlers

void COverseaFutureDlg::SetAccount(CString strAccount)
{
	m_strAccount = strAccount;
}

void COverseaFutureDlg::OnBnClickedButtonSendoverseafuture()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("OFAccount is not assigned"));
		return;
	}

	CString strTrade;
	CString strStockNo;
	CString strYearMonth;
	CString strOrder;
	CString strOrderNumerator;
	CString strTrigger;
	CString strTriggerNumerator;
	CString strQty;
	int		nQty;
	
	GetDlgItemText(IDC_EDIT_TRADENAME,strTrade);
	GetDlgItemText(IDC_EDIT_STOCKNO,strStockNo);
	GetDlgItemText(IDC_EDIT_YEARMONTH,strYearMonth);
	GetDlgItemText(IDC_EDIT_ORDER,strOrder);
	GetDlgItemText(IDC_EDIT_ORDERNUMERATOR,strOrderNumerator);
	GetDlgItemText(IDC_EDIT_TRIGGER,strTrigger);
	GetDlgItemText(IDC_EDIT_TRIGGERNUMERATOR,strTriggerNumerator);
	GetDlgItemText(IDC_EDIT_QTY,strQty);

	nQty = atoi(strQty);

	int nBuySell			= m_boxBuySell.GetCurSel();
	int nNewClose			= m_boxNewClose.GetCurSel();
	int nDayTrade			= m_boxDaytrade.GetCurSel();
	int nTradeType			= m_boxOrderType.GetCurSel();
	int nSpecalTradeType	= m_boxSpecialTradeType.GetCurSel();


	int nSize = 1024;
	TCHAR caBuf[1024] = {0};

	int nCode = SendOverseaFutureOrder(m_strAccount,strTrade,strStockNo,strYearMonth,nBuySell,nNewClose,nDayTrade,nTradeType,nSpecalTradeType
			,nQty,strOrder,strOrderNumerator,strTrigger,strTriggerNumerator,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);

	pDlg->AddReport( strMsg);

}


void COverseaFutureDlg::OnBnClickedButton2()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("OFAccount is not assigned"));
		return;
	}

	CString strTrade;
	CString strStockNo;
	CString strYearMonth;
	CString strOrder;
	CString strOrderNumerator;
	CString strTrigger;
	CString strTriggerNumerator;
	CString strQty;
	int		nQty;
	
	GetDlgItemText(IDC_EDIT_TRADENAME,strTrade);
	GetDlgItemText(IDC_EDIT_STOCKNO,strStockNo);
	GetDlgItemText(IDC_EDIT_YEARMONTH,strYearMonth);
	GetDlgItemText(IDC_EDIT_ORDER,strOrder);
	GetDlgItemText(IDC_EDIT_ORDERNUMERATOR,strOrderNumerator);
	GetDlgItemText(IDC_EDIT_TRIGGER,strTrigger);
	GetDlgItemText(IDC_EDIT_TRIGGERNUMERATOR,strTriggerNumerator);
	GetDlgItemText(IDC_EDIT_QTY,strQty);

	nQty = atoi(strQty);

	int nBuySell			= m_boxBuySell.GetCurSel();
	int nNewClose			= m_boxNewClose.GetCurSel();
	int nDayTrade			= m_boxDaytrade.GetCurSel();
	int nTradeType			= m_boxOrderType.GetCurSel();
	int nSpecalTradeType	= m_boxSpecialTradeType.GetCurSel();

	int nCode = SendOverseaFutureOrderAsync(m_strAccount,strTrade,strStockNo,strYearMonth,nBuySell,nNewClose,nDayTrade,nTradeType,nSpecalTradeType
			,nQty,strOrder,strOrderNumerator,strTrigger,strTriggerNumerator);

		//SendOptionOrder(m_strAccount,strStockNo,nTradeType,nNewClose,nBuySell,strPrice,nQty,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	
	if( nCode > 0 )
		strMsg.Format(_T("Return Error Code : %d "),nCode);
	else
		strMsg.Format(_T("Thread ID : %d "),nCode);

	pDlg->AddReport( strMsg);

}

void COverseaFutureDlg::OnBnClickedButtonGetoverseaproduct()
{
	// TODO: Add your control notification handler code here

	int nCode = GetOverseaFutures();
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return  Code : %d "),nCode);

	pDlg->AddReport( strMsg);

}


void COverseaFutureDlg::OnBnClickedButtonGetoverseaopeninterest()
{
	// TODO: Add your control notification handler code here

	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("OFAccount is not assigned"));
		return;
	}

	int nCode = GetOverseaFutureOpenInterest(m_strAccount);
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return  Code : %d "),nCode);

	pDlg->AddReport( strMsg);
}


void COverseaFutureDlg::OnBnClickedButtonCancelSqlno()
{
	// TODO: Add your control notification handler code here
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("OFAccount is not assigned"));
		return;
	}
	
	CString strSeqkNo;
	GetDlgItemText(IDC_EDIT_CANCEL_SQLNO,strSeqkNo);
	
	int nSize = 1024;
	TCHAR caBuf[1024] = {0};

	int nCode = OverseaCancelOrderBySeqNo(m_strAccount,strSeqkNo,caBuf,&nSize);
	//int nCode = OverseaCancelOrderByBookNo(m_strAccount,strSeqkNo,caBuf,&nSize);

	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	pDlg->AddReport( strMsg );
}


void COverseaFutureDlg::OnBnClickedButtonDecrease()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("OFAccount is not assigned"));
		return;
	}
	
	int nSize = 1024;
	TCHAR caBuf[1024] = {0};

	CString strSeqkNo;
	GetDlgItemText(IDC_EDIT_DECREASE_SQENO,strSeqkNo);

	
	CString strQty;
	GetDlgItemText(IDC_EDIT_DECREASE_QTY,strQty);

	int nQty = atoi(strQty);

	int nCode = OverseaDecreaseOrderBySeqNo(m_strAccount,strSeqkNo,nQty,caBuf,&nSize);
	
	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	pDlg->AddReport( strMsg );
}


void COverseaFutureDlg::OnBnClickedButtonOfcorrectprice()
{
	// TODO: Add your control notification handler code here
	
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("OFAccount is not assigned"));
		return;
	}

	CString strBookNo;
	CString strOrder;
	CString strOrderNumerator;
	CString strTrigger;
	CString strTriggerNumerator;
	
	GetDlgItemText(IDC_EDIT_CORRECT_BOOKNO,strBookNo);
	GetDlgItemText(IDC_EDIT_CORRECT_ORDER,strOrder);
	GetDlgItemText(IDC_EDIT_CORRECT_ORDERNUMERATOR,strOrderNumerator);
	GetDlgItemText(IDC_EDIT_CORRECT_TRIGGER,strTrigger);
	GetDlgItemText(IDC_EDIT_CORRECT_TRIGGERNUMERATOR,strTriggerNumerator);
	
	int nSpecalTradeType = m_boxCorrectSpecialTradeType.GetCurSel();

	int nSize = 1024;
	TCHAR caBuf[1024] = {0};

	int nCode = OverseaCorrectPriceByBookNo(m_strAccount,strBookNo,nSpecalTradeType,strOrder,strOrderNumerator,strTrigger,strTriggerNumerator,caBuf,&nSize);
	
	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	pDlg->AddReport( strMsg );

}


void COverseaFutureDlg::OnBnClickedButtonSendosspread()
{
	// TODO: Add your control notification handler code here

	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("OFAccount is not assigned"));
		return;
	}

	CString strTrade;
	CString strStockNo;
	CString strYearMonth;
	CString strYearMonth2;
	CString strOrder;
	CString strOrderNumerator;
	CString strTrigger;
	CString strTriggerNumerator;
	CString strQty;
	int		nQty;
	
	GetDlgItemText(IDC_EDIT_TRADENAME,strTrade);
	GetDlgItemText(IDC_EDIT_STOCKNO,strStockNo);
	GetDlgItemText(IDC_EDIT_YEARMONTH,strYearMonth);
	GetDlgItemText(IDC_EDIT_YEARMONTH2,strYearMonth2);
	GetDlgItemText(IDC_EDIT_ORDER,strOrder);
	GetDlgItemText(IDC_EDIT_ORDERNUMERATOR,strOrderNumerator);
	GetDlgItemText(IDC_EDIT_TRIGGER,strTrigger);
	GetDlgItemText(IDC_EDIT_TRIGGERNUMERATOR,strTriggerNumerator);
	GetDlgItemText(IDC_EDIT_QTY,strQty);

	nQty = atoi(strQty);

	int nBuySell			= m_boxBuySell.GetCurSel();
	int nNewClose			= m_boxNewClose.GetCurSel();
	int nDayTrade			= m_boxDaytrade.GetCurSel();
	int nTradeType			= m_boxOrderType.GetCurSel();
	int nSpecalTradeType	= m_boxSpecialTradeType.GetCurSel();


	int nSize = 1024;
	TCHAR caBuf[1024] = {0};

	int nCode = SendOverseaFutureSpreadOrder(m_strAccount,strTrade,strStockNo,strYearMonth,strYearMonth2,nBuySell,nNewClose,nDayTrade,nTradeType,nSpecalTradeType
			,nQty,strOrder,strOrderNumerator,strTrigger,strTriggerNumerator,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);

	pDlg->AddReport( strMsg);
}


void COverseaFutureDlg::OnBnClickedButtonSendosspreadasync()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("OFAccount is not assigned"));
		return;
	}

	CString strTrade;
	CString strStockNo;
	CString strYearMonth;
	CString strYearMonth2;
	CString strOrder;
	CString strOrderNumerator;
	CString strTrigger;
	CString strTriggerNumerator;
	CString strQty;
	int		nQty;
	
	GetDlgItemText(IDC_EDIT_TRADENAME,strTrade);
	GetDlgItemText(IDC_EDIT_STOCKNO,strStockNo);
	GetDlgItemText(IDC_EDIT_YEARMONTH,strYearMonth);
	GetDlgItemText(IDC_EDIT_YEARMONTH2,strYearMonth2);
	GetDlgItemText(IDC_EDIT_ORDER,strOrder);
	GetDlgItemText(IDC_EDIT_ORDERNUMERATOR,strOrderNumerator);
	GetDlgItemText(IDC_EDIT_TRIGGER,strTrigger);
	GetDlgItemText(IDC_EDIT_TRIGGERNUMERATOR,strTriggerNumerator);
	GetDlgItemText(IDC_EDIT_QTY,strQty);

	nQty = atoi(strQty);

	int nBuySell			= m_boxBuySell.GetCurSel();
	int nNewClose			= m_boxNewClose.GetCurSel();
	int nDayTrade			= m_boxDaytrade.GetCurSel();
	int nTradeType			= m_boxOrderType.GetCurSel();
	int nSpecalTradeType	= m_boxSpecialTradeType.GetCurSel();

	int nCode = SendOverseaFutureSpreadOrderAsync(m_strAccount,strTrade,strStockNo,strYearMonth,strYearMonth2,nBuySell,nNewClose,nDayTrade,nTradeType,nSpecalTradeType
			,nQty,strOrder,strOrderNumerator,strTrigger,strTriggerNumerator);

		//SendOptionOrder(m_strAccount,strStockNo,nTradeType,nNewClose,nBuySell,strPrice,nQty,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	
	if( nCode > 0 )
		strMsg.Format(_T("Return Error Code : %d "),nCode);
	else
		strMsg.Format(_T("Thread ID : %d "),nCode);

	pDlg->AddReport( strMsg);
}
