// OverseaOptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderTester.h"
#include "OverseaOptionDlg.h"
//#include "afxdialogex.h"
#include "Functions.h"
#include "OrderTesterDlg.h"


// COverseaOptionDlg dialog

IMPLEMENT_DYNAMIC(COverseaOptionDlg, CDialog)

COverseaOptionDlg::COverseaOptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COverseaOptionDlg::IDD, pParent)
{

}

COverseaOptionDlg::~COverseaOptionDlg()
{
}

void COverseaOptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BUYSELL, m_boxBuySell);
	DDX_Control(pDX, IDC_COMBO_NEWCLOSE, m_boxNewClose);
	DDX_Control(pDX, IDC_COMBO_DAYTRADE, m_boxDaytrade);
	DDX_Control(pDX, IDC_COMBO_ORDERTYPE, m_boxOrderType);
	DDX_Control(pDX, IDC_COMBO_SPECIALTRADETYPE, m_boxSpecialTradeType);
}


BEGIN_MESSAGE_MAP(COverseaOptionDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SENDOVERSEAOPTION, &COverseaOptionDlg::OnBnClickedButtonSendoverseaoption)
	ON_BN_CLICKED(IDC_BUTTON_SENDOVERSEAOPTIONASYNC, &COverseaOptionDlg::OnBnClickedButtonSendoverseaoptionasync)
	ON_BN_CLICKED(IDC_BUTTON_GETOVERSEAOPTIONPRODUCT, &COverseaOptionDlg::OnBnClickedButtonGetoverseaoptionproduct)
END_MESSAGE_MAP()


// COverseaOptionDlg message handlers
void COverseaOptionDlg::SetAccount(CString strAccount)
{
	m_strAccount = strAccount;
}

void COverseaOptionDlg::OnBnClickedButtonSendoverseaoption()
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
	CString strCallPut;
	CString strStrikePrice;
	int		nQty;
	
	GetDlgItemText(IDC_EDIT_TRADENAME,strTrade);
	GetDlgItemText(IDC_EDIT_STOCKNO,strStockNo);
	GetDlgItemText(IDC_EDIT_YEARMONTH,strYearMonth);
	GetDlgItemText(IDC_EDIT_ORDER,strOrder);
	GetDlgItemText(IDC_EDIT_ORDERNUMERATOR,strOrderNumerator);
	GetDlgItemText(IDC_EDIT_TRIGGER,strTrigger);
	GetDlgItemText(IDC_EDIT_TRIGGERNUMERATOR,strTriggerNumerator);
	GetDlgItemText(IDC_EDIT_QTY,strQty);
	GetDlgItemText(IDC_EDIT_CALLPUT,strCallPut);
	GetDlgItemText(IDC_EDIT_STRIKEPRICE,strStrikePrice);

	nQty = atoi(strQty);

	int nBuySell			= m_boxBuySell.GetCurSel();
	int nNewClose			= m_boxNewClose.GetCurSel();
	int nDayTrade			= m_boxDaytrade.GetCurSel();
	int nTradeType			= m_boxOrderType.GetCurSel();
	int nSpecalTradeType	= m_boxSpecialTradeType.GetCurSel();


	int nSize = 1024;
	TCHAR caBuf[1024] = {0};

	int nCode = SendOverseaOptionOrder(m_strAccount,strTrade,strStockNo,strYearMonth,strStrikePrice,strCallPut,nBuySell,nNewClose,nDayTrade,nTradeType,nSpecalTradeType
			,nQty,strOrder,strOrderNumerator,strTrigger,strTriggerNumerator,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);

	pDlg->AddReport( strMsg);
}


void COverseaOptionDlg::OnBnClickedButtonSendoverseaoptionasync()
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
	CString strCallPut;
	CString strStrikePrice;
	int		nQty;
	
	GetDlgItemText(IDC_EDIT_TRADENAME,strTrade);
	GetDlgItemText(IDC_EDIT_STOCKNO,strStockNo);
	GetDlgItemText(IDC_EDIT_YEARMONTH,strYearMonth);
	GetDlgItemText(IDC_EDIT_ORDER,strOrder);
	GetDlgItemText(IDC_EDIT_ORDERNUMERATOR,strOrderNumerator);
	GetDlgItemText(IDC_EDIT_TRIGGER,strTrigger);
	GetDlgItemText(IDC_EDIT_TRIGGERNUMERATOR,strTriggerNumerator);
	GetDlgItemText(IDC_EDIT_QTY,strQty);
	GetDlgItemText(IDC_EDIT_CALLPUT,strCallPut);
	GetDlgItemText(IDC_EDIT_STRIKEPRICE,strStrikePrice);

	nQty = atoi(strQty);

	int nBuySell			= m_boxBuySell.GetCurSel();
	int nNewClose			= m_boxNewClose.GetCurSel();
	int nDayTrade			= m_boxDaytrade.GetCurSel();
	int nTradeType			= m_boxOrderType.GetCurSel();
	int nSpecalTradeType	= m_boxSpecialTradeType.GetCurSel();



	int nCode = SendOverseaOptionOrderAsync(m_strAccount,strTrade,strStockNo,strYearMonth,strStrikePrice,strCallPut,nBuySell,nNewClose,nDayTrade,nTradeType,nSpecalTradeType
			,nQty,strOrder,strOrderNumerator,strTrigger,strTriggerNumerator);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	if( nCode > 0 )
		strMsg.Format(_T("Return Error Code : %d "),nCode);
	else
		strMsg.Format(_T("Thread ID : %d "),nCode);

	pDlg->AddReport( strMsg);
}


void COverseaOptionDlg::OnBnClickedButtonGetoverseaoptionproduct()
{
	// TODO: Add your control notification handler code here
	int nCode = GetOverseaOptions();
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return  Code : %d "),nCode);

	pDlg->AddReport( strMsg);
}
