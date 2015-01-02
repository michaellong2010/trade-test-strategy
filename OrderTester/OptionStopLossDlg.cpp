// OptionStopLossDlg.cpp : implementation file
//

#include "stdafx.h"
#include "OrderTester.h"
#include "OptionStopLossDlg.h"
//#include "afxdialogex.h"
#include "Functions.h"
#include "OrderTesterDlg.h"


// COptionStopLossDlg dialog

IMPLEMENT_DYNAMIC(COptionStopLossDlg, CDialog)

COptionStopLossDlg::COptionStopLossDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionStopLossDlg::IDD, pParent)
{

}

COptionStopLossDlg::~COptionStopLossDlg()
{
}

void COptionStopLossDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_BUYSELL, m_boxBuySell);
	DDX_Control(pDX, IDC_COMBO_ORDERTYPE, m_boxOrderType);
	DDX_Control(pDX, IDC_COMBO_NEWCLOSE, m_boxNewClose);
}


BEGIN_MESSAGE_MAP(COptionStopLossDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_SENDOPTIONSTOPLOSS, &COptionStopLossDlg::OnBnClickedButtonSendoptionstoploss)
	ON_BN_CLICKED(IDC_BUTTON_CANCELOPTIONSTOPLOSS, &COptionStopLossDlg::OnBnClickedButtonCanceloptionstoploss)
END_MESSAGE_MAP()


// COptionStopLossDlg message handlers
void COptionStopLossDlg::SetAccount(CString strAccount)
{
	m_strAccount = strAccount;
}

void COptionStopLossDlg::OnBnClickedButtonSendoptionstoploss()
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
	int nNewClose	= m_boxNewClose.GetCurSel();

	int nCode = SendOptionOrder(m_strAccount,strStockNo,nTradeType,nNewClose,nBuySell,strPrice,nQty,caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);

	pDlg->AddReport( strMsg);

}


void COptionStopLossDlg::OnBnClickedButtonCanceloptionstoploss()
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

	int nCode = CancelOptionStopLoss(m_strAccount,strBookNo,"0","0","0","0","0","0","0",caBuf,&nSize);

	COrderTesterDlg* pDlg = (COrderTesterDlg*) AfxGetMainWnd();

	CString strMsg;
	strMsg.Format(_T("Return Code : %d  Message:%s"),nCode,caBuf);

	pDlg->AddReport( strMsg);
}
