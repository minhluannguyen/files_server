// SigninDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "SigninDlg.h"
#include "afxdialogex.h"


// SigninDlg dialog

IMPLEMENT_DYNAMIC(SigninDlg, CDialogEx)

SigninDlg::SigninDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SIGNIN_DIALOG, pParent)
{

}

SigninDlg::~SigninDlg()
{
}

void SigninDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, edt_username);
	DDX_Control(pDX, IDC_EDIT_PASS, edt_pass);
	DDX_Control(pDX, IDC_EDIT_COMF_PASS, edt_comf_password);
	DDX_Control(pDX, IDC_BUTTON_AC, but_accept);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, but_cancel);
}


BEGIN_MESSAGE_MAP(SigninDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &SigninDlg::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_AC, &SigninDlg::OnBnClickedButtonAc)
END_MESSAGE_MAP()


// SigninDlg message handlers


void SigninDlg::OnBnClickedButtonCancel()
{
	// TODO: Add your control notification handler code here
	SigninDlg::OnCancel();
}


void SigninDlg::OnBnClickedButtonAc()
{
	// TODO: Add your control notification handler code here
	
	//Get text from edit box
	CString str_username, str_password, str_comf_password;
	edt_username.GetWindowText(str_username);
	edt_pass.GetWindowText(str_password);
	edt_comf_password.GetWindowText(str_comf_password);
	//Check password and comfirm password
	if (str_password != str_comf_password)
	{
		MessageBox(_T("Comfirm password doesn't match password!"), _T("Warining"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}

	//Check username from server
	// ...
}
