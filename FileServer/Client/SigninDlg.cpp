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
END_MESSAGE_MAP()


// SigninDlg message handlers
