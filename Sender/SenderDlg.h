
// SenderDlg.h : header file
//

#pragma once

enum SOCKET_TYPES
{
	STREAM = 1,
	DGRAM = 2
};

enum PROTOCOL_TYPES
{
	TCP = 1,
	UDP = 2
};
// CSenderDlg dialog
class CSenderDlg : public CDialog
{
// Construction
public:
	CSenderDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SENDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CComboBox m_SocketTypes;
	CComboBox m_ProtocolTypes;
	CEdit m_DataSendEdit;
	CEdit m_DataReciveEdit;
	CStatic m_ResponseStatusStatic;
	CIPAddressCtrl m_ServerIpAddress;
	CEdit m_ServerPort;
public:
	afx_msg void OnBnClickedButtonSend();
	afx_msg void OnBnClickedButtonQuit();

private:
	void AddSocketTypes();
	void AddProtocolTypes();

private:
	int m_SockType;
	int m_ProtocolType;
	CSocket m_ClientSocket;

private:
	static DWORD WINAPI SendThread(LPVOID lpParam);
	
};
