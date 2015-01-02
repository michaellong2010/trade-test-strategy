// FutureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderTester.h"
#include "FutureDlg.h"
//#include "afxdialogex.h"
#include "Functions.h"
#include "OrderTesterDlg.h"


// CFutureDlg dialog

IMPLEMENT_DYNAMIC(CFutureDlg, CDialog)

CFutureDlg::CFutureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFutureDlg::IDD, pParent)
{
	m_strAccount = _T("");
}

CFutureDlg::~CFutureDlg()
{
}

void CFutureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BUYSELL, m_boxBuySell);
	DDX_Control(pDX, IDC_COMBO_ORDERTYPE, m_boxOrderType);
	DDX_Control(pDX, IDC_COMBO_DAYTRADE, m_boxDayTrade);
	DDX_Control(pDX, IDC_COMBO_CORRECT_ORDERTYPE, m_boxCorrcetOrderType);
}


BEGIN_MESSAGE_MAP(CFutureDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SENDFUTUREORDER, &CFutureDlg::OnBnClickedButtonSendfutureorder)
	ON_BN_CLICKED(IDC_BUTTON_SENDFUTUREORDERASYNC, &CFutureDlg::OnBnClickedButtonSendfutureorderasync)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_STOCKNO, &CFutureDlg::OnBnClickedButtonCancelStockno)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL_SQLNO, &CFutureDlg::OnBnClickedButtonCancelSqlno)
	ON_BN_CLICKED(IDC_BUTTON_DECREASE, &CFutureDlg::OnBnClickedButtonDecrease)
	ON_BN_CLICKED(IDC_BUTTON_SEND_CORRECT, &CFutureDlg::OnBnClickedButtonSendCorrect)
	ON_BN_CLICKED(IDC_BUTTON_CORRECTBYBOOKNO, &CFutureDlg::OnBnClickedButtonCorrectbybookno)
	ON_BN_CLICKED(IDC_BUTTON_OPENINTEREST, &CFutureDlg::OnBnClickedButtonOpeninterest)
	ON_BN_CLICKED(IDC_BUTTON_FUTURERIGHT, &CFutureDlg::OnBnClickedButtonFutureright)
	ON_BN_CLICKED(IDC_BUTTON_EXECUTIONREPORT, &CFutureDlg::OnBnClickedButtonExecutionReport)
END_MESSAGE_MAP()


// CFutureDlg message handlers

void CFutureDlg::SetAccount(CString strAccount)
{
	m_strAccount = strAccount;
}

void CFutureDlg::OnBnClickedButtonSendfutureorder()
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
	CString strQty;
	int		nQty;

	GetDlgItemText(IDC_EDIT_STOCKNO,strStockNo);
	GetDlgItemText(IDC_EDIT_PRICE,strPrice);
	GetDlgItemText(IDC_EDIT_QTY,strQty);
	
	nQty = atoi( CStringA(strQty) );

	int nBuySell	= m_boxBuySell.GetCurSel();
	int nTradeType	= m_boxOrderType.GetCurSel();
	int nDayTrade	= m_boxDayTrade.GetCurSel();

	int nCode = SendFutureOrder(m_strAccount,strStockNo,nTradeType,nDayTrade,nBuySell,strPrice,nQty,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	
	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);

	pDlg->AddReport( strMsg);
}

typedef int (WINAPI* Proc_Add_SendFutureOrderAsync) ( const TCHAR* lpszAccount, const TCHAR* lpszStockNo, unsigned short usTradeType, unsigned short usDayTrade, unsigned short usBuySell, const TCHAR* lpszPrice, int nQty);
void CFutureDlg::OnBnClickedButtonSendfutureorderasync()
{
	// TODO: Add your control notification handler code here

	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TFAccount is not assigned"));
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

	int nCode = SendFutureOrderAsync(m_strAccount,strStockNo,nTradeType,nDayTrade,nBuySell,strPrice,nQty);
	/*int nCode = 0;
	HINSTANCE m_CapitalOrderDll = AfxLoadLibrary("SKOrderLib.dll");
	if (!m_CapitalOrderDll)
	{
		AfxMessageBox("Error: Cannot find component \"SKOrderLib.dll\"");
		//return FALSE;
	}
	else {
		Proc_Add_SendFutureOrderAsync pFunc_SendFutureOrderAsync = ( Proc_Add_SendFutureOrderAsync ) GetProcAddress ( m_CapitalOrderDll, "SendFutureOrderAsync" );
		if ( pFunc_SendFutureOrderAsync ) {
			nCode = SendFutureOrderAsync ( m_strAccount, strStockNo, nTradeType, nDayTrade, nBuySell, strPrice, nQty );
		}
	}
	AfxFreeLibrary( m_CapitalOrderDll );*/

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	
	CString strMsg;

	if( nCode > 0 )
		strMsg.Format(_T("Return Error Code : %d "),nCode);
	else
		strMsg.Format(_T("Thread ID : %d "),nCode);

	pDlg->AddReport( strMsg);
}


void CFutureDlg::OnBnClickedButtonCancelStockno()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TFAccount is not assigned"));
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


void CFutureDlg::OnBnClickedButtonCancelSqlno()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TFAccount is not assigned"));
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


void CFutureDlg::OnBnClickedButtonDecrease()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TFAccount is not assigned"));
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


void CFutureDlg::OnBnClickedButtonSendCorrect()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TFAccount is not assigned"));
		return;
	}
	
	int nSize = 1024;
	TCHAR caBuf[1024] = {0};

	CString strSeqkNo;
	GetDlgItemText(IDC_EDIT_CORRECT_SEQNO,strSeqkNo);
	
	CString strPrice;
	GetDlgItemText(IDC_EDIT_CORRECT_PRICE,strPrice);
	
	int nTradeType	= m_boxCorrcetOrderType.GetCurSel();

	int nCode = CorrectPriceBySeqNo(m_strAccount,strSeqkNo,strPrice,nTradeType,caBuf,&nSize);

	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	pDlg->AddReport( strMsg );

}


void CFutureDlg::OnBnClickedButtonCorrectbybookno()
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
	GetDlgItemText(IDC_EDIT_CORRECT_BOOKNO,strBookNo);
	
	CString strPrice;
	GetDlgItemText(IDC_EDIT_CORRECT_PRICE,strPrice);
	
	int nTradeType	= m_boxCorrcetOrderType.GetCurSel();

	int nCode = CorrectPriceByBookNo(m_strAccount,_T("TF"),strBookNo,strPrice,nTradeType,caBuf,&nSize);

	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	pDlg->AddReport( strMsg );

}


void CFutureDlg::OnBnClickedButtonOpeninterest()
{
	// TODO: Add your control notification handler code here
	
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TFAccount is not assigned"));
		return;
	}

	int nCode = GetOpenInterest( m_strAccount);
	
	CString strMsg;
	strMsg.Format(_T("Return Code : %d"),nCode);
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	pDlg->AddReport( strMsg );
}


void CFutureDlg::OnBnClickedButtonFutureright()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TFAccount is not assigned"));
		return;
	}

	int nCode = GetFutureRight( m_strAccount);
	
	CString strMsg;
	strMsg.Format(_T("Return Code : %d"),nCode);
	
	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	pDlg->AddReport( strMsg );
}

void CFutureDlg::OnBnClickedButtonExecutionReport()
{
	// TODO: Add your control notification handler code here
	if( m_strAccount.IsEmpty() )
	{
		MessageBox(_T("TFAccount is not assigned"));
		return;
	}

	//GetExecutionReport ( TCHAR* lpszAccount,TCHAR* lpszStockNo,int nMarket, int nBuySell,int nDataNum, int nType );
	int nCode = GetExecutionReport ( m_strAccount.GetBuffer(), CString("0").GetBuffer(), 0, 0, 6, 1 );
	CString strMsg;
	strMsg.Format(_T("Return Code : %d"),nCode);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();
	pDlg->AddReport( strMsg );
}
