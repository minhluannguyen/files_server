
// ServerDlg.h : header file
//

#pragma once
#define IPADDRESS "171.0.0.1"

// CServerDlg dialog
class CServerDlg : public CDialogEx
{
// Construction
public:
	CServerDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	// Show the machine's IP address
	CIPAddressCtrl Ctrl_ipAddress;
	// Provide port for server
	CEdit edt_port;
	// Show online client
	CListCtrl listCtrl_Onl_Client;
	// Show all available client
	CListCtrl listCtrl_All_Client;
	// File list to download or remove
	CListCtrl listCtrl_File_List;

	//
	CSocket sockServer;

public:
	afx_msg void OnBnClickedButtonStart();
protected:
	// Log history
	CEdit edt_log;
};
