
// ServerDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CServerDlg dialog



CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, Ctrl_ipAddress);
	DDX_Control(pDX, IDC_EDIT_PORT, edt_port);
	DDX_Control(pDX, IDC_LIST_ONLCLIENT, listCtrl_Onl_Client);
	DDX_Control(pDX, IDC_LIST_CLIENT, listCtrl_All_Client);
	DDX_Control(pDX, IDC_LIST_FILES, listCtrl_File_List);
	DDX_Control(pDX, IDC_EDIT_LOG, edt_log);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, but_upload);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, but_remove);
	DDX_Control(pDX, IDC_BUTTON_DOWN, but_down);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CServerDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CServerDlg::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CServerDlg::OnBnClickedButtonRemove)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	if (AfxSocketInit() == FALSE)
	{
		MessageBox(_T("Init socket failed!"));
		return FALSE;
	}

	//GetIPAddress test
	char* ipAdd = getIpAddress();
	CString tmp (ipAdd);
	MessageBox(tmp);

	edt_port.SetWindowText(_T("0"));

	//File list init
	listCtrl_File_List.SetExtendedStyle(LVS_EX_GRIDLINES | TVS_FULLROWSELECT);
	listCtrl_File_List.InsertColumn(0, _T("File Name"), LVCFMT_LEFT, 200);
	listCtrl_File_List.InsertColumn(1, _T("Uploader"), LVCFMT_LEFT, 120);

	listCtrl_All_Client.SetExtendedStyle(LVS_EX_GRIDLINES | TVS_FULLROWSELECT);
	listCtrl_Onl_Client.SetExtendedStyle(LVS_EX_GRIDLINES | TVS_FULLROWSELECT);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CServerDlg::addFileInfo(CString name, CString owner, CString path)
{
	fileInfo file;
	file.name = name;
	file.owner = owner;
	file.path = path;

	fileInfoArr.push_back(file);
}

void CServerDlg::removeFileInfo(int index)
{
	vector<fileInfo>::iterator it;
	it = fileInfoArr.begin();
	int i = 0;
	for (it; it != fileInfoArr.end(); ++it)
	{
		if (i == index)
		{
			fileInfoArr.erase(it);
			break;
		}
		else
		{
			i++;
		}
	}
}

void CServerDlg::updateFileList()
{

	int listCount = listCtrl_File_List.GetItemCount();
	int arrCount = fileInfoArr.size();
	//Empty list
	for (int i = 0; i < listCount; i++)
	{
		listCtrl_File_List.DeleteItem(0);
	}

	for (int i = 0; i < arrCount; i++)
	{
		fileInfo tmpFileInfo = fileInfoArr[i];
		listCtrl_File_List.InsertItem(i, tmpFileInfo.name);
		listCtrl_File_List.SetItemText(i, 1, tmpFileInfo.owner);
	}
}

void CServerDlg::OnBnClickedButtonStart()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString str_port;
	CString str_ip;

	edt_port.GetWindowText(str_port);
	if (str_port.IsEmpty())
	{
		MessageBox(_T("Please enter port number!"));
	}
	//
	sockServer.Create(_ttoi(str_port), SOCK_STREAM, _T(IPADDRESS));

	sockServer.Listen(5);


	//Enable all the listCtrl and Button
	listCtrl_Onl_Client.EnableWindow();
	listCtrl_All_Client.EnableWindow();
	listCtrl_File_List.EnableWindow();
	edt_log.EnableWindow();
	but_upload.EnableWindow();
	but_remove.EnableWindow();
	but_down.EnableWindow();

}


void CServerDlg::OnBnClickedButtonBrowse()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString str_fileName, str_fileOwner, str_filePath;
	CFileDialog t1(true);
	if (t1.DoModal() == IDOK)
	{
		str_fileName = t1.GetFileName();
		str_filePath = t1.GetPathName();
		str_fileOwner = _T("Admin");
		addFileInfo(str_fileName, str_fileOwner, str_filePath);
		updateFileList();
		UpdateData(FALSE);
	}
}


void CServerDlg::OnBnClickedButtonRemove()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	POSITION pos = listCtrl_File_List.GetFirstSelectedItemPosition();
	if (pos == NULL)
		MessageBox(_T("No file was selected!!!"), _T("Warning!"), MB_ICONWARNING);
	else 
	{
		int index = listCtrl_File_List.GetNextSelectedItem(pos);
		removeFileInfo(index);
		updateFileList();
		UpdateData(FALSE);
	}
}
