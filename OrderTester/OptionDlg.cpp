// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderTester.h"
#include "OptionDlg.h"
//#include "afxdialogex.h"
#include "Functions.h"
#include "OrderTesterDlg.h"


// COptionDlg dialog

IMPLEMENT_DYNAMIC(COptionDlg, CDialog)

COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
{

}

COptionDlg::~COptionDlg()
{
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BUYSELL, m_boxBuySell);
	DDX_Control(pDX, IDC_COMBO_ORDERTYPE, m_boxOrderType);
	DDX_Control(pDX, IDC_COMBO_NEWCLOSE, m_boxNewClose);
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SENDOPTIONORDER, &COptionDlg::OnBnClickedButtonSendoptionorder)
	ON_BN_CLICKED(IDC_BUTTON_SENDOPTIONORDERASYNC, &COptionDlg::OnBnClickedButtonSendoptionorderasync)
END_MESSAGE_MAP()


// COptionDlg message handlers
void COptionDlg::SetAccount(CString strAccount)
{
	m_strAccount = strAccount;
}


void COptionDlg::OnBnClickedButtonSendoptionorder()
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
	int nNewClose	= m_boxNewClose.GetCurSel();

	int nCode = SendOptionOrder(m_strAccount,strStockNo,nTradeType,nNewClose,nBuySell,strPrice,nQty,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);

	pDlg->AddReport( strMsg);
}


void COptionDlg::OnBnClickedButtonSendoptionorderasync()
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
	int nNewClose	= m_boxNewClose.GetCurSel();

	int nCode = SendOptionOrderAsync(m_strAccount,strStockNo,nTradeType,nNewClose,nBuySell,strPrice,nQty);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;

	if( nCode > 0 )
		strMsg.Format(_T("Return Error Code : %d "),nCode);
	else
		strMsg.Format(_T("Thread ID : %d "),nCode);

	pDlg->AddReport( strMsg);

}
