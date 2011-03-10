// LauncherDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CLauncherDialog dialog
class CLauncherDialog : public CDialog
{
// Construction
public:
	CLauncherDialog(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_LAUNCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	void LoadInfo();
	void SaveInfo();
	virtual BOOL OnInitDialog();
	void OnPaint();
	HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void OnBnClickedCancel();
	void OnBnClickedOk();
	CEdit m_ipAddressEditBox;
	CEdit m_port;
	CEdit m_password;
	CEdit m_nick;
};
