// StockDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderTester.h"
#include "StockDlg.h"
//#include "afxdialogex.h"
#include "Functions.h"
#include "OrderTesterDlg.h"


// CStockDlg dialog

IMPLEMENT_DYNAMIC(CStockDlg, CDialog)

CStockDlg::CStockDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStockDlg::IDD, pParent)
	, m_strAccount(_T(""))
{

}

CStockDlg::~CStockDlg()
{
}

void CStockDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BUYSELL, m_boxBuySell);
	DDX_Control(pDX, IDC_COMBO_DAYTRADE, m_boxDayTrade);
	DDX_Control(pDX, IDC_COMBO_ORDERTYPE, m_boxOrderType);
	DDX_Control(pDX, IDC_EDIT_PRICE, m_editPrice);
	DDX_Control(pDX, IDC_EDIT_QTY, m_editQty);
	DDX_Control(pDX, IDC_EDIT_STOCKNO, m_editStockNo);
}


BEGIN_MESSAGE_MAP(CStockDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SENDORDER, &CStockDlg::OnBnClickedButtonSendorder)
	ON_BN_CLICKED(IDC_BUTTON_SENDORDERASYNC, &CStockDlg::OnBnClickedButtonSendorderasync)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_STOCKNO, &CStockDlg::OnBnClickedButtonCancelStockno)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_SQLNO, &CStockDlg::OnBnClickedButtonCancelSqlno)
	ON_BN_CLICKED(IDC_BUTTON_DECREASE, &CStockDlg::OnBnClickedButtonDecrease)
	ON_BN_CLICKED(IDC_BUTTON1, &CStockDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CStockDlg message handlers


void CStockDlg::SetAccount(CString strText)
{
	m_strAccount = strText;
}


void CStockDlg::OnBnClickedButtonSendorder()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TSAccount is not assigned"));
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

	int nBuySell	= m_boxBuySell.GetCurSel();
	int nTradeType	= m_boxOrderType.GetCurSel();
	int nDayTrade	= m_boxDayTrade.GetCurSel();
	
	int nCode = SendStockOrder(m_strAccount,strStockNo,nTradeType,nDayTrade,nBuySell,strPrice,nQty,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	pDlg->AddReport( caBuf);
}


void CStockDlg::OnBnClickedButtonSendorderasync()
{
	// TODO: Add your control notification handler code here

	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TSAccount is not assigned"));
		return;
	}

	CString strStockNo;
	CString strPrice;
	CString strQty;
	int		nQty;

	GetDlgItemText(IDC_EDIT_STOCKNO,strStockNo);
	GetDlgItemText(IDC_EDIT_PRICE,strPrice);
	GetDlgItemText(IDC_EDIT_QTY,strQty);

	nQty = atoi( CStringA(strQty) );

	int nBuySell	= m_boxBuySell.GetCurSel();
	int nTradeType	= m_boxOrderType.GetCurSel();
	int nDayTrade	= m_boxDayTrade.GetCurSel();
	
	int nCode = SendStockOrderAsync(m_strAccount,strStockNo,nTradeType,nDayTrade,nBuySell,strPrice,nQty);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;

	if( nCode > 0 )
		strMsg.Format(_T("Return Error Code : %d "),nCode);
	else
		strMsg.Format(_T("Thread ID : %d "),nCode);

	pDlg->AddReport( strMsg );
}


void CStockDlg::OnBnClickedButtonCancelStockno()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TSAccount is not assigned"));
		return;
	}
	
	CString strStockNo;
	GetDlgItemText(IDC_EDIT_CANCEL_STOCKNO,strStockNo);
	
	int nSize = 1024;
	TCHAR caBuf[1024] = {0};

	int nCode = CancelOrderByStockNo(m_strAccount,strStockNo,caBuf,&nSize);

	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	pDlg->AddReport( strMsg );
}


void CStockDlg::OnBnClickedButtonCancelSqlno()
{
	// TODO: Add your control notification handler code here

	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TSAccount is not assigned"));
		return;
	}
	
	CString strSeqkNo;
	GetDlgItemText(IDC_EDIT_CANCEL_SQLNO,strSeqkNo);
	
	int nSize = 1024;
	TCHAR caBuf[1024] = {0};

	int nCode = CancelOrderBySeqNo(m_strAccount,strSeqkNo,caBuf,&nSize);

	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	pDlg->AddReport( strMsg );
}


void CStockDlg::OnBnClickedButtonDecrease()
{
	// TODO: Add your control notification handler code here

	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TSAccount is not assigned"));
		return;
	}
	
	int nSize = 1024;
	TCHAR caBuf[1024] = {0};

	CString strSeqkNo;
	GetDlgItemText(IDC_EDIT_DECREASE_SQENO,strSeqkNo);

	
	CString strQty;
	GetDlgItemText(IDC_EDIT_DECREASE_QTY,strQty);

	int nQty = atoi(strQty);

	int nCode = DecreaseOrderBySeqNo(m_strAccount,strSeqkNo,nQty,caBuf,&nSize);
	
	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	pDlg->AddReport( strMsg );
}


void CStockDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TSAccount is not assigned"));
		return;
	}
	//(LPTSTR)(LPCTSTR)

	int nCode = GetRealBalanceReport((LPTSTR)(LPCTSTR)m_strAccount);

	CString strMsg;
	strMsg.Format(_T("Return Code : %d "),nCode);
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	pDlg->AddReport( strMsg );
}
