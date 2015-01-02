#pragma once


// Ctest_dlg dialog

class Ctest_dlg : public CDialog
{
	DECLARE_DYNAMIC(Ctest_dlg)

public:
	Ctest_dlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~Ctest_dlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};
