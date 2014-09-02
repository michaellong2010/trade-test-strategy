
// QuoteTesterDlg.h : ���Y��
//
#include "KLine_Stream.h"
#include "Functions.h"
#include <deque>
#pragma once

using namespace std; 
#define WM_DATA WM_USER+1

// CQuoteTesterDlg ��ܤ��
class CQuoteTesterDlg : public CDialogEx
{
// �غc
public:
	CQuoteTesterDlg(CWnd* pParent = NULL);	// �зǫغc�禡
	~CQuoteTesterDlg();

// ��ܤ�����
	enum { IDD = IDD_QUOTETESTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩


// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	
	afx_msg LRESULT OnData(WPARAM wParam,LPARAM lParam);

	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();

	afx_msg LRESULT OnDoTaskInMainThread(WPARAM wParam,LPARAM lParam);
	HANDLE Wait_Connection_Event, Wait_ProductsReady_Event;
	afx_msg void OnDestroy();
	afx_msg void OnNcDestroy();

	boolean ReadyForTrading;

	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra,
		AFX_CMDHANDLERINFO* pHandlerInfo);

	/*fstream only be accessed by belong thread*/
	CKLineStream mKline_stream;

	/*implement object pointer queue to reduce dynamic alloc and improve efficiency*/
	deque <TStock *> m_Queue_pTStock;
	deque <TBest5*> m_Queue_pTBest5;
};
