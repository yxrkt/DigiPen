// RCMDlg.cpp : implementation file
//

#include "stdafx.h"
#include "RCM.h"
#include "RCMDlg.h"
#include "Networking.h"

Networking NETWORKING;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CRCMDlg dialog


CRCMDlg::CRCMDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRCMDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRCMDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CRCMDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
  ON_COMMAND( IDC_BUTTONSEND, OnButtonSend )
END_MESSAGE_MAP()


// CRCMDlg message handlers

void CRCMDlg::OnButtonSend()
{
  char szBuf[ MAX_MESSAGE_LEN ];
  memset( szBuf, '\0', MAX_MESSAGE_LEN  );

  m_pName->GetWindowTextA( szBuf, MAX_MESSAGE_LEN );
  strBuf += szBuf;
  strBuf += ": ";

  memset( szBuf, '\0', MAX_MESSAGE_LEN );
  m_pIn->GetWindowTextA( szBuf, MAX_MESSAGE_LEN );
  strBuf += szBuf;
  strBuf += 13;
  strBuf += '\n';

  m_pOut->SetWindowTextA( strBuf.c_str() );
  m_pIn->SetWindowTextA( "" );
}

BOOL CRCMDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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
  m_pOut  = ( CEdit * ) GetDlgItem( IDC_EDITOUT );
  m_pIn   = ( CEdit * ) GetDlgItem( IDC_EDITIN );
  m_pName = ( CEdit * ) GetDlgItem( IDC_EDITNAME );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRCMDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CRCMDlg::OnPaint()
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
HCURSOR CRCMDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

