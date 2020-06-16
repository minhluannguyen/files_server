// ClientMenuDlg.cpp : implementation file
//

#include "pch.h"
#include "Client.h"
#include "ClientMenuDlg.h"
#include "afxdialogex.h"


// ClientMenuDlg dialog

IMPLEMENT_DYNAMIC(ClientMenuDlg, CDialogEx)

ClientMenuDlg::ClientMenuDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG1, pParent)
{

}

ClientMenuDlg::~ClientMenuDlg()
{
}

void ClientMenuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FILES, listCtrl_File_List);
	DDX_Control(pDX, IDC_EDIT_LOG, edt_log);
}


BEGIN_MESSAGE_MAP(ClientMenuDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &ClientMenuDlg::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &ClientMenuDlg::OnBnClickedButtonRemove)
END_MESSAGE_MAP()

BOOL ClientMenuDlg::OnInitDialog()
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

	//File list init
	listCtrl_File_List.SetExtendedStyle(LVS_EX_GRIDLINES | TVS_FULLROWSELECT);
	listCtrl_File_List.InsertColumn(0, _T("File Name"), LVCFMT_LEFT, 200);
	listCtrl_File_List.InsertColumn(1, _T("Uploader"), LVCFMT_LEFT, 120);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// ClientMenuDlg message handlers

void ClientMenuDlg::addFileInfo(CString name, CString owner, CString path)
{
	fileInfo file;
	file.name = name;
	file.owner = owner;
	file.path = path;

	fileInfoArr.push_back(file);
}

void ClientMenuDlg::removeFileInfo(int index)
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

void ClientMenuDlg::updateFileList()
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


void ClientMenuDlg::OnBnClickedButtonBrowse()
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString str_fileName, str_fileOwner, str_filePath;
	CFileDialog t1(true);
	if (t1.DoModal() == IDOK)
	{
		str_fileName = t1.GetFileName();
		str_filePath = t1.GetPathName();
		str_fileOwner = (*this).username;
		addFileInfo(str_fileName, str_fileOwner, str_filePath);
		updateFileList();
		UpdateData(FALSE);

		//add socket upload file
	}
}


void ClientMenuDlg::OnBnClickedButtonRemove()
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

		//add socket remove file
	}
}

void ClientMenuDlg::getUsername(CString str)
{
	username = str;
}
