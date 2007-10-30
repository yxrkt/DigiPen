// RCMDlg.h : header file
//

#pragma once

#include <string>

// CRCMDlg dialog
class CRCMDlg : public CDialog
{
// Construction
public:
	CRCMDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_RCM_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
  const static UINT MAX_MESSAGE_LEN = 260;

	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

  void OnButtonSend();

  CEdit *m_pOut, *m_pIn, *m_pName;
  std::string strBuf;
};
