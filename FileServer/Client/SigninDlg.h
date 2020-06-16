#pragma once


// SigninDlg dialog

class SigninDlg : public CDialogEx
{
	DECLARE_DYNAMIC(SigninDlg)

public:
	SigninDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~SigninDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SIGNIN_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	// New username
	CEdit edt_username;
	// New password
	CEdit edt_pass;
	// New comfirm password
	CEdit edt_comf_password;
	// Accept sign in button (check)
	CButton but_accept;
	// Exit sign in menu
	CButton but_cancel;
};
