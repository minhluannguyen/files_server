#pragma once
#include<iostream>
#include <vector>
using namespace std;


struct fileInfo {
	CString name;
	CString owner;
	CString path;
};

// ClientMenuDlg dialog

class ClientMenuDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ClientMenuDlg)

public:
	ClientMenuDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ClientMenuDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	// File list
	CListCtrl listCtrl_File_List;
	// History log
	CEdit edt_log;

	//Fileinfo
	vector<fileInfo> fileInfoArr;
	void addFileInfo(CString, CString, CString);
	void removeFileInfo(int index);
	void updateFileList();

	//username
	CString username;
public:
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedButtonRemove();

	//get username
	void getUsername(CString);
};
