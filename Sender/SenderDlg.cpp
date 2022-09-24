
// SenderDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "Sender.h"
#include "SenderDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSenderDlg dialog



CSenderDlg::CSenderDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SENDER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SOCKETTYPE, m_SocketTypes);
	DDX_Control(pDX, IDC_COMBO_SOCKETPROTOCOL, m_ProtocolTypes);
	DDX_Control(pDX, IDC_EDIT_DATABESEND, m_DataSendEdit);
	DDX_Control(pDX, IDC_EDIT_DATARECIVED, m_DataReciveEdit);
	DDX_Control(pDX, IDC_STATIC_RESPONSE_STATUS, m_ResponseStatusStatic);
	DDX_Control(pDX, IDC_IPADDRESS_SERVERIP, m_ServerIpAddress);
	DDX_Control(pDX, IDC_EDIT_SERVERPORT, m_ServerPort);
}

BEGIN_MESSAGE_MAP(CSenderDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SEND, &CSenderDlg::OnBnClickedButtonSend)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CSenderDlg::OnBnClickedButtonQuit)
END_MESSAGE_MAP()


// CSenderDlg message handlers

BOOL CSenderDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	AddSocketTypes();
	AddProtocolTypes();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSenderDlg::OnPaint()
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
HCURSOR CSenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSenderDlg::OnBnClickedButtonSend()
{
	// TODO: Add your control notification handler code here
	CreateThread(NULL, 0, SendThread, this, 0, 0);
}


void CSenderDlg::OnBnClickedButtonQuit()
{
	// TODO: Add your control notification handler code here
	m_ClientSocket.Detach();
	m_ClientSocket.Close();
	ExitProcess(0);
}

void CSenderDlg::AddSocketTypes()
{
	m_SocketTypes.InsertString(0, "SOCK_STREAM(流式套接字)");
	m_SocketTypes.InsertString(1, "SOCK_DGRAM(数据包套接字)");
	m_SocketTypes.SetCurSel(0);
	m_SockType = SOCKET_TYPES::STREAM;
}

void CSenderDlg::AddProtocolTypes()
{
	m_ProtocolTypes.InsertString(0, "TCP协议");
	m_ProtocolTypes.InsertString(1, "UDP协议");
	m_ProtocolTypes.SetCurSel(0);
	m_ProtocolType = PROTOCOL_TYPES::TCP;
}

DWORD __stdcall CSenderDlg::SendThread(LPVOID lpParam)
{
	CSenderDlg* pSenderDlg = (CSenderDlg*)lpParam;
	char szIP[MAX_PATH] = {0x0};
	char szPort[MAX_PATH] = {0x0};

	AfxSocketInit();

	pSenderDlg->GetServerIp(szIP, MAX_PATH);
	pSenderDlg->GetServerPort(szPort, MAX_PATH);
	UINT uPort = atoi(szPort);

	switch (pSenderDlg->m_SockType)
	{
	case SOCKET_TYPES::STREAM:break;
	case SOCKET_TYPES::DGRAM:break;
	}

	switch (pSenderDlg->m_ProtocolType)
	{
	case PROTOCOL_TYPES::TCP:break;
	case PROTOCOL_TYPES::UDP:break;
	}

	if (!pSenderDlg->m_ClientSocket.Create())
	{
		return 0;
	}

	if (!pSenderDlg->m_ClientSocket.Connect(szIP, uPort))
	{
		pSenderDlg->m_ClientSocket.Close();
		return 0;
	}

	CSocketFile socketFile(&pSenderDlg->m_ClientSocket);
	CArchive archiveStore(&socketFile, CArchive::store), archiveLoad(&socketFile, CArchive::load);

	char szDataSend[MAX_PATH] = {0x0};
	pSenderDlg->GetDlgItemText(IDC_EDIT_DATABESEND, szDataSend, MAX_PATH);

	archiveStore.Write(szDataSend, strlen(szDataSend));
	archiveStore.Flush();
	archiveStore.Close();

	char szDataRecive[MAX_PATH] = { 0x0 };
	archiveLoad.Read(szDataRecive, MAX_PATH);
	archiveLoad.Close();
	pSenderDlg->SetDlgItemText(IDC_EDIT_DATARECIVED, szDataRecive);

	socketFile.Close();
	pSenderDlg->m_ClientSocket.Close();
	return 0;
}

char* CSenderDlg::GetServerIp(char* szIP, size_t len)
{
	BYTE ip[4] = { 0x0 };
	m_ServerIpAddress.GetAddress(ip[0], ip[1], ip[2], ip[3]);
	sprintf_s(szIP, len, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
	return szIP;
}

char* CSenderDlg::GetServerPort(char* szPort, size_t len)
{
	GetDlgItem(IDC_EDIT_SERVERPORT)->GetWindowText(szPort, len);
	return szPort;
}
