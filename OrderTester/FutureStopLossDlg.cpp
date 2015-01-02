// FutureStopLossDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderTester.h"
#include "FutureStopLossDlg.h"
//#include "afxdialogex.h"
#include "Functions.h"
#include "OrderTesterDlg.h"


// CFutureStopLossDlg dialog

IMPLEMENT_DYNAMIC(CFutureStopLossDlg, CDialog)

CFutureStopLossDlg::CFutureStopLossDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFutureStopLossDlg::IDD, pParent)
{

}

CFutureStopLossDlg::~CFutureStopLossDlg()
{
}

void CFutureStopLossDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BUYSELL, m_boxBuySell);
	DDX_Control(pDX, IDC_COMBO_ORDERTYPE, m_boxOrderType);
	DDX_Control(pDX, IDC_COMBO_DAYTRADE, m_boxDayTrade);
	DDX_Control(pDX, IDC_COMBO_KIND, m_boxKind);
	DDX_Control(pDX, IDC_COMBO_REPORTSTATUS, m_boxReportStutus);
	DDX_Control(pDX, IDC_COMBO_BUYSELL2, m_boxBuySell2);
	DDX_Control(pDX, IDC_COMBO_ORDERTYPE2, m_boxOrderType2);
	DDX_Control(pDX, IDC_COMBO_DAYTRADE2, m_boxDayTrade2);
}


BEGIN_MESSAGE_MAP(CFutureStopLossDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SENDFUTURESTOPLOSS, &CFutureStopLossDlg::OnBnClickedButtonSendfuturestoploss)
	ON_BN_CLICKED(IDC_BUTTON_CANCELFUTURESTOPLOSS, &CFutureStopLossDlg::OnBnClickedButtonCancelfuturestoploss)
	ON_BN_CLICKED(IDC_BUTTON_STOPLOSSREPORT, &CFutureStopLossDlg::OnBnClickedButtonStoplossreport)
	ON_BN_CLICKED(IDC_BUTTON_SENDFUTUREMOVINGSTOPLOSS, &CFutureStopLossDlg::OnBnClickedButtonSendfuturemovingstoploss)
	ON_BN_CLICKED(IDC_BUTTON_CANCELFUTURESTOPLOSS2, &CFutureStopLossDlg::OnBnClickedButtonCancelfuturestoploss2)
END_MESSAGE_MAP()


// CFutureStopLossDlg message handlers

void CFutureStopLossDlg::SetAccount(CString strAccount)
{
	m_strAccount = strAccount;
}

void CFutureStopLossDlg::OnBnClickedButtonSendfuturestoploss()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TFAccount is not assigned"));
		return;
	}

	int nSize = 1024;
	TCHAR caBuf[1024] = {0};

	CString strStockNo;
	CString strPrice;
	CString strTriggerPrice;
	CString strQty;
	int		nQty;

	GetDlgItemText(IDC_EDIT_STOCKNO,strStockNo);
	GetDlgItemText(IDC_EDIT_PRICE,strPrice);
	GetDlgItemText(IDC_EDIT_TRIGGERPRICE,strTriggerPrice);
	GetDlgItemText(IDC_EDIT_QTY,strQty);
	
	nQty = atoi( CStringA(strQty) );

	int nBuySell	= m_boxBuySell.GetCurSel();
	int nTradeType	= m_boxOrderType.GetCurSel();
	int nDayTrade	= m_boxDayTrade.GetCurSel();

	int nCode = SendFutureStopLoss(m_strAccount,strStockNo,nTradeType,nDayTrade,nBuySell,strPrice,nQty,strTriggerPrice,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	
	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);

	pDlg->AddReport( strMsg);
}


void CFutureStopLossDlg::OnBnClickedButtonCancelfuturestoploss()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TFAccount is not assigned"));
		return;
	}

	int nSize = 1024;
	TCHAR caBuf[1024] = {0};
	
	CString strBookNo;
	GetDlgItemText(IDC_EDIT_CANCEL_BOOKNO,strBookNo);

	int nCode = CancelFutureStopLoss(m_strAccount,strBookNo,"0","0","0","0","0","0","0",caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	
	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);

	pDlg->AddReport( strMsg);

}


void CFutureStopLossDlg::OnBnClickedButtonStoplossreport()
{
	// TODO: Add your control notification handler code here
	
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TFAccount is not assigned"));
		return;
	}
	
	int nStatus	= m_boxReportStutus.GetCurSel();

	CString strKind;
	m_boxKind.GetWindowTextA(strKind);
	
	int nCode = GetStopLossReport(m_strAccount,nStatus,strKind);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	
	CString strMsg;
	strMsg.Format(_T("Return Code : %d"),nCode);

	pDlg->AddReport( strMsg);
}


void CFutureStopLossDlg::OnBnClickedButtonSendfuturemovingstoploss()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TFAccount is not assigned"));
		return;
	}

	int nSize = 1024;
	TCHAR caBuf[1024] = {0};

	CString strStockNo;
	CString strMovingPoint;
	CString strTriggerPrice;
	CString strQty;
	int		nQty;

	GetDlgItemText(IDC_EDIT_STOCKNO2,strStockNo);
	GetDlgItemText(IDC_EDIT_MOVINGPOINT,strMovingPoint);
	GetDlgItemText(IDC_EDIT_TRIGGERPRICE2,strTriggerPrice);
	GetDlgItemText(IDC_EDIT_QTY2,strQty);
	
	nQty = atoi( CStringA(strQty) );

	int nBuySell	= m_boxBuySell2.GetCurSel();
	int nTradeType	= m_boxOrderType2.GetCurSel();
	int nDayTrade	= m_boxDayTrade2.GetCurSel();

	
	int nCode = SendMovingStopLoss(m_strAccount,strStockNo,nTradeType,nDayTrade,nBuySell,strTriggerPrice,nQty,strMovingPoint,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);

	pDlg->AddReport( strMsg);
}

void CFutureStopLossDlg::OnBnClickedButtonCancelfuturestoploss2()
{
	// TODO: Add your control notification handler code here

	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TFAccount is not assigned"));
		return;
	}

	int nSize = 1024;
	TCHAR caBuf[1024] = {0};
	
	CString strBookNo;
	GetDlgItemText(IDC_EDIT_CANCEL_BOOKNO2,strBookNo);

	int nCode = CancelMovingStopLoss(m_strAccount,strBookNo,"0","0","0","0","0","0","0",caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	
	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);

	pDlg->AddReport( strMsg);
}
