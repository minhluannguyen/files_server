
// ServerDlg.h : header file
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "framework.h"
#include<iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

//#define IPADDRESS "171.0.0.1"
#define PORT 25000
#define SEND_BUFFER_SIZE 10000
#define WM_SOCKET WM_USER+1
#define RECV_BUFFER_SIZE 10000

struct fileInfo {
	CString name;
	CString owner;
	CString path;
};

// CServerDlg dialog
class CServerDlg : public CDialog
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
	// Log history
	CEdit edt_log;
	CString logMess;
	// Add file to the server
	CButton but_upload;
	// Remove file from list
	CButton but_remove;

	
	//DWORD WINAPI CServerDlg::sendFile(LPVOID);

	//Fileinfo
	vector<fileInfo> fileInfoArr;
	void addFileInfo(CString, CString, CString);
	void removeFileInfo(int index);
	void updateFileList();
	void updateFileListFile();
	void loadFileInfo();
	string getFilePath(string);

	//Process info of account
	bool checkAcc(CString inputName, CString inputPass);
	bool checkUsernameExist(CString inputName);
	void updateAccount(CString inputName, CString inputPass);
	void loadAccountToList();

	void updateLog();

	void updateOnlineClients();

public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonBrowse();	
	afx_msg void OnBnClickedButtonRemove();

	LRESULT SockMsg(WPARAM wParam, LPARAM lParam);
	
	//Socket stuff
	struct SockName
	{
		SOCKET sockClient;
		CString username;
	};

	SOCKET sockServer, sockClient, flag, sclient;
	struct sockaddr_in serverAdd;
	int msgType;
	int buffLength, t, lenguser, flagsend, kq, count_sock;
	int number_Socket;
	int first_send;
	vector<SockName> pSock;
	CString strResult[2];
	CString Command;

	void refreshFileList();

	//Send and receive data/file
	void Split(CString src, CString des[2]);
	void mSend(SOCKET sk, CString Command);
	int mRecv(SOCKET sk, CString& Command);
	//int iPort;
	//string file_name;
};

DWORD WINAPI sendFile(LPVOID);
bool receiveFile(SOCKET, CString);