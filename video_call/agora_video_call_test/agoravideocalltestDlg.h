
#pragma once
#include "afxmenubutton.h"
#include "AgoraRtcEngine.h"
#include "AgoraRESTCall.h"
#include "afxwin.h"


class CAgoraVideoCallTestDlg : public CDialog
{
public:
	CAgoraVideoCallTestDlg(CWnd* pParent = NULL);

	virtual ~CAgoraVideoCallTestDlg()
	{
		if (m_lpAgoraRESTCall != NULL)
			delete m_lpAgoraRESTCall;

		if (m_VideoDeviceTestMgr != NULL)
			delete m_VideoDeviceTestMgr;
	}

	enum { IDD = IDD_AGORATUTORIAL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	HICON m_hIcon;

	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();

	afx_msg void OnBnClickedBtnjoin();
	afx_msg void OnBnClickedBtnleave();
	afx_msg void OnBnClickedBtndisvid();
	afx_msg void OnBnClickedBtnmutelocaud();
	afx_msg void OnBnClickedTest();
	afx_msg void OnBnClickedShare();
	afx_msg void OnBnClickedSearch();
	afx_msg void OnBnClickedPing();

	// ENGINE EVENT MESSAGE
	afx_msg LRESULT OnFirstRemoteVideoDecoded(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

private:
	CString				m_strChannelName;

	CAgoraRtcEngine		*m_lpAgoraRtcEngine;

	CAgoraRESTCall		*m_lpAgoraRESTCall;

	AVideoDeviceManager	*m_VideoDeviceTestMgr;

	BOOL			m_ScreenShareStarted;
	BOOL			m_VideoDeviceTestStarted;

	HBITMAP			m_OrigLocalBitmap;
	HBITMAP			m_OrigRemoteBitmap;

	CStatic		m_wndRemote;
	CStatic		m_wndLocal;

	TCHAR		m_APP_ID[MAX_PATH];
	TCHAR		m_MEDIA_TOKEN[MAX_PATH];
	TCHAR		m_MYRESTBASEURI[MAX_PATH];
	BOOL		m_useMediaToken;

public:
	
	CButton m_wndScreenShare;
	CButton m_VideoDeviceTest;
	
};
