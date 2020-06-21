
// ServerDlg.cpp : implementation file
//
#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

string file_path;
int iPort = 10000;
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
	: CDialog(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, Ctrl_ipAddress);
	DDX_Control(pDX, IDC_EDIT_PORT, edt_port);
	DDX_Control(pDX, IDC_LIST_ONLCLIENT, listCtrl_Onl_Client);
	DDX_Control(pDX, IDC_LIST_CLIENT, listCtrl_All_Client);
	DDX_Control(pDX, IDC_LIST_FILES, listCtrl_File_List);
	DDX_Control(pDX, IDC_EDIT_LOG, edt_log);
	DDX_Control(pDX, IDC_BUTTON_BROWSE, but_upload);
	DDX_Control(pDX, IDC_BUTTON_REMOVE, but_remove);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_SOCKET, SockMsg)

	ON_BN_CLICKED(IDC_BUTTON_START, &CServerDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CServerDlg::OnBnClickedButtonBrowse)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CServerDlg::OnBnClickedButtonRemove)
END_MESSAGE_MAP()


// CServerDlg message handlers

BOOL CServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

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
	//char* ipAdd = getIpAddress();
	//CString tmp (ipAdd);
	//MessageBox(tmp);

	//init port number
	char tmpPort[10];
	_itoa_s(PORT, tmpPort, 10);
	CString cstrPort(tmpPort);
	edt_port.SetWindowText(cstrPort);

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
		CDialog::OnSysCommand(nID, lParam);
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
		CDialog::OnPaint();
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
	fileInfo tmpFile;
	tmpFile.name = name;
	tmpFile.owner = owner;
	tmpFile.path = path;
	fileInfoArr.push_back(tmpFile);

	updateFileListFile();
	updateFileList();
}

void CServerDlg::updateFileListFile()
{
	ofstream f;
	f.open("fileInfo.txt");
	for (int i = 0; i < fileInfoArr.size(); i++)
	{
		string name = CStringA(fileInfoArr[i].name);
		string owner = CStringA(fileInfoArr[i].owner);
		string path = CStringA(fileInfoArr[i].path);

		string line = name + ':' + owner + ':' + path;

		f << line << endl;
	}
	f.close();
}

void CServerDlg::removeFileInfo(int index)
{
	vector<fileInfo>::iterator it;
	it = fileInfoArr.begin();
	for (int i = 0; i < fileInfoArr.size(); i++)
	{
		if (i == index)
		{
			fileInfoArr.erase(it);
			break;
		}
		++it;
	}

	updateFileListFile();
	updateFileList();
}

void CServerDlg::updateFileList()
{
	int arrCount = fileInfoArr.size();
	//Empty list
	listCtrl_File_List.DeleteAllItems();

	for (int i = 0; i < arrCount; i++)
	{
		fileInfo tmpFileInfo = fileInfoArr[i];
		listCtrl_File_List.InsertItem(i, tmpFileInfo.name);
		listCtrl_File_List.SetItemText(i, 1, tmpFileInfo.owner);
	}
}

void CServerDlg::Split(CString src, CString des[2])
{
	int p1, p2;

	p1 = src.Find(_T("\r\n"), 0);
	des[0] = src.Mid(0, p1);

	p2 = src.Find(_T("\r\n"), p1 + 1);
	des[1] = src.Mid(p1 + 2, p2 - (p1 + 2));

}

void CServerDlg::mSend(SOCKET sk, CString Command)
{
	int Len = Command.GetLength();
	Len += Len;
	PBYTE sendBuff = new BYTE[1000];
	memset(sendBuff, 0, 1000);
	memcpy(sendBuff, (PBYTE)(LPCTSTR)Command, Len);
	send(sk, (char*)&Len, sizeof(Len), 0);
	send(sk, (char*)sendBuff, Len, 0);
	delete[] sendBuff;
}

int CServerDlg::mRecv(SOCKET sk, CString& Command)
{
	PBYTE buffer = new BYTE[1000];
	memset(buffer, 0, 1000);
	recv(sk, (char*)&buffLength, sizeof(int), 0);
	recv(sk, (char*)buffer, buffLength, 0);
	TCHAR* ttc = (TCHAR*)buffer;
	Command = ttc;
	delete[] buffer;
	if (Command.GetLength() == 0)
		return -1;
	return 0;

}

void Parse(CString source, CString& User, CString& Pass) {
	CString demp = (CString)"";
	int pos = -1;
	for (int i = 0; i < source.GetLength(); i++) {
		if (source[i] == ' ') {
			User = demp;
			pos = i + 1;
			demp = (CString)"";
			break;
		}
		else {
			demp += source[i];
		}
	}
	for (int i = pos; i < source.GetLength(); i++) {
		demp += source[i];
	}
	Pass = demp;
}

bool CServerDlg::checkAcc(CString inputName, CString inputPass) 
{
	bool res = false;
	string str_inputName = CStringA(inputName);
	string str_inputPass = CStringA(inputPass);
	if (str_inputName == "" || str_inputPass == "")
		return false;
	ifstream fi;
	fi.open("account.txt");
	if (fi)
	{
		while (!fi.eof())
		{
			string line;
			getline(fi, line);
			//username and password seperate by ':'
			int found = line.find(':');
			string fileUsername, filePass;
			if (found != string::npos)
			{
				fileUsername = line.substr(0, found);
				filePass = line.substr(found + 1, line.size() - found - 1);
			}

			if (str_inputName == fileUsername && str_inputPass == filePass)
			{
				res = true;
				break;
			}
		}
		fi.close();
	}
	return res;
}

bool CServerDlg::checkUsernameExist(CString inputName)
{
	bool res = false;
	string str_inputName = CStringA(inputName);
	ifstream fi;
	fi.open("account.txt");
	if (fi)
	{
		while (!fi.eof())
		{
			string line;
			getline(fi, line);
			//username and password seperate by ':'
			int found = line.find(':');
			string fileUsername, filePass;
			if (found != string::npos)
			{
				fileUsername = line.substr(0, found);
			}

			if (str_inputName == fileUsername)
			{
				res = true;
				break;
			}
		}
		fi.close();
	}
	return res;
}

void CServerDlg::loadAccountToList()
{
	//Empty list
	listCtrl_All_Client.DeleteAllItems();

	ifstream fi;
	fi.open("account.txt");
	int i = 0;
	if (fi.is_open())
	{
		while (!fi.eof())
		{
			string line;
			getline(fi, line);
			//username and password seperate by ':'
			int found = line.find(':');
			string fileUsername, filePass;
			if (found != string::npos)
			{
				fileUsername = line.substr(0, found);
			}
			CString cstrUsername = _T("");
			cstrUsername = fileUsername.c_str();
			listCtrl_All_Client.InsertItem(i, cstrUsername);
			i++;
		}
		fi.close();
	}
}

void CServerDlg::loadFileInfo()
{
	fileInfoArr.clear();
	ifstream fi;
	fi.open("fileInfo.txt");
	int i = 0;
	if (fi.is_open())
	{
		while (!fi.eof())
		{
			string line;
			getline(fi, line);

			//Empty line then break
			if (line == "")
				break;
			//username and password seperate by ':'
			int found1 = line.find(':');
			string fileName, fileOwner, filePath;
			if (found1 != string::npos)
			{
				fileName = line.substr(0, found1);
				int found2 = line.find(':', found1 + 1);
				if (found2 != string::npos)
				{
					fileOwner = line.substr(found1 + 1, found2 - found1 - 1);
					filePath = line.substr(found2 + 1, line.size() - found2 - 1);
				}
			}
			CString cstr_name;
			CString cstr_owner;
			CString cstr_path;

			cstr_name = fileName.c_str();
			cstr_owner = fileOwner.c_str();
			cstr_path = filePath.c_str();
			
			fileInfo tmpFile;
			tmpFile.name = cstr_name;
			tmpFile.owner = cstr_owner;
			tmpFile.path = cstr_path;

			fileInfoArr.push_back(tmpFile);
		}
		fi.close();
	}

	updateFileList();
}

string CServerDlg::getFilePath(string fileName)
{
	for (int i = 0; i < fileInfoArr.size(); ++i)
	{
		if (fileInfoArr[i].name == fileName.c_str())
		{
			return CStringA(fileInfoArr[i].path);
		}
	}
	return "";
}

void CServerDlg::updateAccount(CString inputName, CString inputPass)
{
	fstream fo;
	fo.open("account.txt", fstream::app);
	fo << CStringA(inputName) << ':' << CStringA(inputPass) << endl;
	fo.close();
}

LRESULT CServerDlg::SockMsg(WPARAM wParam, LPARAM lParam)
{
	CString User, Pass;
	if (WSAGETSELECTERROR(lParam))
	{
		// Display the error and close the socket
		closesocket(wParam);
	}
	switch (WSAGETSELECTEVENT(lParam))
	{
		//Accept connection
	case FD_ACCEPT:
	{
		//pSock[number_Socket].sockClient = accept(wParam, NULL, NULL);
		SockName tmpClient;
		pSock.push_back(tmpClient);
		(pSock[number_Socket]).sockClient = accept(wParam, NULL, NULL);
		break;
	}
	//Data incoming
	case FD_READ:
	{
		vector<SockName>::iterator it;
		int pos = -1;
		it = pSock.begin();
		// Stt client sent data
		for (int i = 0; i < number_Socket; i++)
		{
			if ((pSock[i]).sockClient == wParam)
			{
				if (i < number_Socket) {
					pos = i;
					break;
				}
			}
			++it;
		}

		//Receiving data
		CString revData;
		if (mRecv(wParam, revData) < 0)
			break;
		Split(revData, strResult);
		int flag = _ttoi(strResult[0]);

		switch (flag)
		{
		case 1://Login
		{
			UpdateData(TRUE);
			t = 0;
			// Parse strResult[1] to username and pass;
			Parse(strResult[1], User, Pass);
			//char* temp = ConvertToChar(User);
			if (number_Socket > 0)
			{
				//Check if the logining user is already online
				for (int i = 0; i < number_Socket; i++)
				{
					if ((pSock[i]).username == User)
					{
						t = 1;
						break;
					}
				}
			}

			//Check if the username and password is correct
			bool is_correct = checkAcc(User, Pass);
			if (is_correct && t == 0)
			{
				(pSock[number_Socket]).username = User;
				Command = _T("1\r\n1\r\n");

				logMess += User + _T(" login.\r\n");

				//Appear client name on the Online Clients List
				updateOnlineClients();
				UpdateData(FALSE);

				//Increase number of online client
				number_Socket++;
			}
			else
			{
				Command = _T("1\r\n0\r\n");
				//if ((*it).username == "")
					//pSock.erase(it);
			}

			mSend(wParam, Command);
			if (is_correct)
				refreshFileList();
			UpdateData(FALSE);
			break;
		}

		case 2: //Register
		{

			// Parse strResult[1] to username and pass;
			Parse(strResult[1], User, Pass);

			if (checkAcc(User, Pass)) {
				Command = _T("2\r\n0\r\n");
				//logMess += "Register failed \r\n";
			}
			else {
				Command = _T("2\r\n1\r\n");
				logMess += "User: " + CStringA(User) + " added into database.\r\n";

				//Save account info to file
				updateAccount(User, Pass);

				//update list Ctrl
				loadAccountToList();
			}

			mSend(wParam, Command);
			UpdateData(FALSE);

			break;
		}


		case 3: //Log out
		{
			/*int post = -1;
			for (int i = 0; i < number_Socket; i++)
			{
				if (pSock[i].sockClient == wParam)
				{
					if (i < number_Socket)
						post = i;
				}
			}*/

			//Log print logout info
			logMess += CStringA(User) + " logout.\r\n";
			closesocket(wParam);

			pSock.erase(it);
			updateOnlineClients();
			number_Socket--;

			//Appear client name on the Online Clients List
			listCtrl_Onl_Client.DeleteItem(0);
			UpdateData(FALSE);
			break;
		}
		//Client request download
		case 4:
		{
			iPort++;
			string s_port = to_string(iPort);
			CString cs_port(s_port.c_str());

			logMess += CStringA((pSock[pos]).username) + " download file " + CStringA(strResult[1]) + ".\r\n";
			UpdateData(FALSE);

			Command = _T("4\r\n") + cs_port + _T("\r\n");
			mSend(wParam, Command);
			string fileName = CStringA(strResult[1]);
			file_path = getFilePath(fileName);

			//Wrong filename
			if (file_path == "")
			{
				Command = _T("4\r\n-1\r\n");
				break;
			}

			//ThreadParam* param = new ThreadParam;
			//param->mDlg = m_hWnd;
			//AfxBeginThread(sendFile, (LPVOID)param);
			HMODULE hModule = ::GetModuleHandle(NULL);

			DWORD sendFileThreadId;
			HANDLE hsendFile;

			hsendFile = CreateThread(NULL, 0, sendFile, NULL, 0, &sendFileThreadId);
			break;
		}
		//case 5: //Refresh file from client
		//{

		//	for (int i = 0; i < listCtrl_File_List.GetItemCount(); i++) {
		//		CString demp = listCtrl_File_List.GetItemText(i, 0);
		//		Command = _T("3\r\n") + demp + _T("\r\n");
		//		mSend(wParam, Command);
		//	}
		//	//CA2T str((to_string(number_Socket)+"- Update\r\n").c_str()) ;
		//	//m_msgString += str ;
		//	UpdateData(FALSE);
		//	break;
		//}

		//Client upload
		case 6:
		{
			logMess += CStringA((pSock[pos]).username) + " uploaded file " + CStringA(strResult[1]) + ".\r\n";
			//UpdateData(FALSE);

			//Receive file
			CString fileName = strResult[0];
			CString fileOwner = strResult[1];

			if (receiveFile((*it).sockClient, fileName))
				Command = _T("6\r\n1\r\n");
			else
				Command = _T("6\r\n0\r\n");

			mSend(wParam, Command);

			break;
		}

		}
		updateLog();
		break;
	}

	case FD_CLOSE: //client log out
	{
		UpdateData(TRUE);
		int pos = -1;
		vector<SockName>::iterator it;
		it = pSock.begin();
		// Stt client sent data
		for (int i = 0; i < number_Socket; i++)
		{
			if ((pSock[i]).sockClient == wParam)
			{
				if (i < number_Socket) {
					pos = i;
					break;
				}
			}
			++it;
		}

		if (pos != -1)
		{
			logMess += CStringA((pSock[pos]).username) + " logout.\r\n";
			closesocket(wParam);

			//delete[](it);
			pSock.erase(it);

			number_Socket--;
			updateOnlineClients();
			UpdateData(FALSE);
			closesocket(wParam);
		}
		updateLog();
		UpdateData(FALSE);
		break;
	}

	}
	return 0;
}

void CServerDlg::refreshFileList()
{
	for (int i = 0; i < pSock.size(); ++i)
	{
		for (int j = 0; j < listCtrl_File_List.GetItemCount(); j++) {
			CString fileName = listCtrl_File_List.GetItemText(j, 0);
			CString fileOwner = listCtrl_File_List.GetItemText(j, 1);
			Command = _T("3\r\n") + fileName + _T(" ") + fileOwner + _T("\r\n");
			mSend(pSock[i].sockClient, Command);
		}
	}
}

void CServerDlg::updateOnlineClients()
{
	listCtrl_Onl_Client.DeleteAllItems();

	for (int i = 0; i < pSock.size(); ++i)
	{
		listCtrl_Onl_Client.InsertItem(0, pSock[i].username);
	}
}

void CServerDlg::updateLog()
{
	edt_log.SetWindowText(logMess);
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

	//Start listen socket
	sockServer = socket(AF_INET, SOCK_STREAM, 0);
	serverAdd.sin_family = AF_INET;
	serverAdd.sin_port = htons(PORT);
	serverAdd.sin_addr.s_addr = htonl(INADDR_ANY);//inet_addr("127.0.0.1");
	bind(sockServer, (SOCKADDR*)&serverAdd, sizeof(serverAdd));
	listen(sockServer, 32);

	int err = WSAAsyncSelect(sockServer, m_hWnd, WM_SOCKET, FD_READ | FD_ACCEPT | FD_CLOSE);
	if (err)
		MessageBox((LPCTSTR)"Cant call WSAAsyncSelect");

	number_Socket = 0;

	//pSock = new SockName[200];


	//Enable all the listCtrl and Button
	listCtrl_Onl_Client.EnableWindow();
	listCtrl_All_Client.EnableWindow();
	listCtrl_File_List.EnableWindow();
	edt_log.EnableWindow();
	but_upload.EnableWindow();
	but_remove.EnableWindow();

	//load file info
	loadFileInfo();
	//read account info
	loadAccountToList();

	//log started server
	logMess = "Server started!\r\n";
	updateLog();

	UpdateData(FALSE);
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

		logMess += "admin uploaded " + CStringA(str_fileName) + ".\r\n";

		updateLog();
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
		CString str_fileName = listCtrl_File_List.GetItemText(index, 0);
		removeFileInfo(index);
		
		logMess += "admin removed " + CStringA(str_fileName) + ".\r\n";

		updateLog();
		UpdateData(FALSE);
	}
}

DWORD WINAPI sendFile(LPVOID)
{
	// TODO: code your application's behavior here.

	// Khoi tao thu vien Socket
	if (AfxSocketInit() == FALSE)
	{
		//cout << "Khong the khoi tao Socket Libraray";
		return FALSE;
	}

	CSocket ServerSocket; //cha
						  // Tao socket cho server, dang ky port la 1234, giao thuc TCP
	if (ServerSocket.Create(iPort, SOCK_STREAM, NULL) == 0) //SOCK_STREAM or SOCK_DGRAM.
	{
		//cout << "Khoi tao that bai !!!" << endl;
		//cout << ServerSocket.GetLastError();
		return FALSE;
	}
	else
	{
		//cout << "Server khoi tao thanh cong !!!" << endl;

		if (ServerSocket.Listen(32) == FALSE)
		{
			//cout << "Khong the lang nghe tren port nay !!!" << endl;
			ServerSocket.Close();
			return FALSE;
		}

	}


	CSocket Connector;
	// Khoi tao mot socket de duy tri viec ket noi va trao doi du lieu
	if (ServerSocket.Accept(Connector))
	{
		//cout << "Da co Client ket Noi !!!" << endl << endl;
		///-----------------------------------------------------------------------------------
		/// Tham so truyen vao: char* file_name, CSocket Connector
		///-----------------------------------------------------------------------------------
		int file_size, bytes_to_send, bytes_sent;
		byte* buffer = NULL;

		// Mo file

		FILE* fi;
		errno_t err = fopen_s(&fi, file_path.c_str(), "rb");
		if (err != 0)
			return 0;

		// Lay kich thuoc file
		fseek(fi, 0, SEEK_END);
		file_size = ftell(fi);
		fseek(fi, 0, SEEK_SET);

		// Gui kich thuoc file

		bytes_to_send = sizeof(file_size);
		do
		{
			int size = file_size + sizeof(file_size) - bytes_to_send;
			bytes_sent = send(Connector, (char*)&size, bytes_to_send, 0);

			if (bytes_sent == SOCKET_ERROR)
				return 0;

			bytes_to_send -= bytes_sent;
		} while (bytes_to_send > 0);


		// Gui file's data 
		buffer = new byte[SEND_BUFFER_SIZE];
		bytes_to_send = file_size;// So bytes data can gui

		do {
			int buffer_size;
			buffer_size = (SEND_BUFFER_SIZE > bytes_to_send) ? bytes_to_send : SEND_BUFFER_SIZE;
			fread(buffer, buffer_size, 1, fi);

			// Neu gui bi loi thi gui lai goi tin do
			do {
				bytes_sent = send(Connector, (char*)buffer, buffer_size, 0);
			} while (bytes_sent == -1);

			// Cap nhat lai so bytes can gui
			bytes_to_send -= bytes_sent;
		} while (bytes_to_send > 0);

		if (buffer) delete[] buffer;
		fclose(fi);
		///------------------------------------------------------------------------------------
	}
	Connector.Close();
	ServerSocket.Close();
	return 1;
}

bool receiveFile(SOCKET ClientSocket, CString fileName)
{
	//cout << "Ket noi toi Server thanh cong !!!" << endl << endl;
	///-------------------------------------------------------------------------------
	/// Tham so truyen vao: char* file_name, CSocket ClientSocket
	///-----------------------------------------------------------------------------------
	// Khai bao
	int file_size = 0, bytes_recevived, bytes_to_receive;
	byte* buffer = NULL;

	// Mo file
	string str_filename = CStringA(fileName);
	FILE* fo;
	errno_t err = fopen_s(&fo, str_filename.c_str(), "wb");
	if (err != 0)
		return 0;
	// Nhan kich thuoc file
	int size = 0;
	bytes_to_receive = sizeof(file_size);
	do
	{
		size = (file_size)+sizeof(file_size) - bytes_to_receive;
		bytes_recevived = recv(ClientSocket, (char*)&size, bytes_to_receive, 0);

		if (bytes_recevived == SOCKET_ERROR || bytes_recevived == 0)
			return 0;

		bytes_to_receive -= bytes_recevived;
	} while (bytes_to_receive > 0);

	// Nhan file's data
	buffer = new byte[RECV_BUFFER_SIZE];
	bytes_to_receive = size;

	do {
		int buffer_size;
		buffer_size = (RECV_BUFFER_SIZE > bytes_to_receive) ? bytes_to_receive : RECV_BUFFER_SIZE;

		do {
			bytes_recevived = recv(ClientSocket, (char*)buffer, buffer_size, 0);
		} while (bytes_recevived == -1);

		fwrite((char*)buffer, bytes_recevived, 1, fo);

		memset(buffer, 0, RECV_BUFFER_SIZE);
		bytes_to_receive -= bytes_recevived;
	} while (bytes_to_receive > 0);

	if (buffer) delete[] buffer;
	fclose(fo);
	///-------------------------------------------------------------------------------

// Dong ket noi
	//closesocket(ClientSocket);
	return 1;
}