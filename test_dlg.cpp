// test_dlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "test_dlg.h"


// Ctest_dlg dialog

IMPLEMENT_DYNAMIC(Ctest_dlg, CDialog)

Ctest_dlg::Ctest_dlg(CWnd* pParent /*=NULL*/)
	: CDialog(Ctest_dlg::IDD, pParent)
{

}

Ctest_dlg::~Ctest_dlg()
{
}

void Ctest_dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

LRESULT Ctest_dlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) {
	if ( message == WM_CLOSE || message == WM_DESTROY || ( message == WM_SYSCOMMAND && wParam == SC_CLOSE ) )
		TRACE ( "%x %x\n", message, wParam );
	return CDialog::WindowProc ( message, wParam, lParam );
}

BEGIN_MESSAGE_MAP(Ctest_dlg, CDialog)
END_MESSAGE_MAP()


// Ctest_dlg message handlers
