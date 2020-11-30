
#include "stdafx.h"
#include "agoravideocalltest.h"
#include "agoravideocalltestDlg.h"
#include "afxdialogex.h"

#include "AgoraEventDef.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);

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

CAgoraVideoCallTestDlg::CAgoraVideoCallTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAgoraVideoCallTestDlg::IDD, pParent)
	, m_strChannelName(_T("AgoraTest"))
	, m_lpAgoraRESTCall(NULL)
	, m_ScreenShareStarted(FALSE)
	, m_VideoDeviceTestMgr(NULL)
	, m_VideoDeviceTestStarted(FALSE)
	, m_useMediaToken(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAgoraVideoCallTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDCHANNEL, m_strChannelName);
	DDX_Control(pDX, IDC_STATIC_REMOTE, m_wndRemote);
	DDX_Control(pDX, IDC_STATIC_LOCAL, m_wndLocal);
	DDX_Control(pDX, IDC_BTNSHARE, m_wndScreenShare);
	DDX_Control(pDX, IDC_BTNTEST, m_VideoDeviceTest);
}

BEGIN_MESSAGE_MAP(CAgoraVideoCallTestDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTNJOIN, &CAgoraVideoCallTestDlg::OnBnClickedBtnjoin)
	ON_BN_CLICKED(IDC_BTNLEAVE, &CAgoraVideoCallTestDlg::OnBnClickedBtnleave)
	ON_BN_CLICKED(IDC_BTNDISVID, &CAgoraVideoCallTestDlg::OnBnClickedBtndisvid)
	ON_BN_CLICKED(IDC_BTNMUTELOCAUD, &CAgoraVideoCallTestDlg::OnBnClickedBtnmutelocaud)
	ON_BN_CLICKED(IDC_BTNTEST, &CAgoraVideoCallTestDlg::OnBnClickedTest)
	ON_BN_CLICKED(IDC_BTNSHARE, &CAgoraVideoCallTestDlg::OnBnClickedShare)
	ON_BN_CLICKED(IDC_BTNSEARCH, &CAgoraVideoCallTestDlg::OnBnClickedSearch)
	ON_BN_CLICKED(IDC_BTNPING, &CAgoraVideoCallTestDlg::OnBnClickedPing)

	ON_MESSAGE(WM_MSGID(EID_FIRST_REMOTE_VIDEO_DECODED), &CAgoraVideoCallTestDlg::OnFirstRemoteVideoDecoded)
	
END_MESSAGE_MAP()

BOOL CAgoraVideoCallTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	SetIcon(m_hIcon, TRUE);
	SetIcon(m_hIcon, FALSE);

	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	CString csPath(path);
	int nIndex = csPath.ReverseFind(_T('\\'));

	if (nIndex > 0) {

		csPath = csPath.Left(nIndex) + _T("\\init.ini");

	}

	DWORD num = GetPrivateProfileString (_T("AgoraTest"), _T("APP_ID"), _T("a49a5bfef6064ba39c1b9495ce5f3910"), m_APP_ID, MAX_PATH, csPath);
	DWORD num1 = GetPrivateProfileString(_T("AgoraTest"), _T("MEDIA_TOKEN"), _T("006b3eac52789f14758922e38e0f4fe25beIACdansJmgVJsahlRW8rfl6VI4EJ1cRcflxWf1VEO0zoc11qgTMAAAAAEACVypXNsq7FXwEAAQCyrsVf"), m_MEDIA_TOKEN, MAX_PATH, csPath);
	m_useMediaToken = GetPrivateProfileInt(_T("AgoraTest"), _T("USE_MEDIA_TOKEN"), FALSE, csPath);

	m_OrigLocalBitmap = m_wndLocal.GetBitmap();
	m_OrigRemoteBitmap = m_wndRemote.GetBitmap();

	const string MYRESTBASEURI = "https://localhost:8889";

	m_lpAgoraRESTCall = new CAgoraRESTCall(MYRESTBASEURI);

	m_lpAgoraRtcEngine = CAgoraRtcEngine::GetAgoraRtcEngine(m_APP_ID);
	
	m_lpAgoraRtcEngine->SetMsgHandlerWnd(GetSafeHwnd());

	m_lpAgoraRtcEngine->GetEngine()->enableVideo();

	m_VideoDeviceTestMgr = new AVideoDeviceManager(m_lpAgoraRtcEngine->GetEngine());

	VideoCanvas vc;

	vc.uid = 0;
	vc.view = m_wndLocal.GetSafeHwnd();
	vc.renderMode = RENDER_MODE_FIT;

	m_lpAgoraRtcEngine->GetEngine()->setupLocalVideo(vc);

	return TRUE;
}

void CAgoraVideoCallTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAgoraVideoCallTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CAgoraVideoCallTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CAgoraVideoCallTestDlg::OnBnClickedBtnjoin()
{
	UpdateData(TRUE);

	if (m_strChannelName.IsEmpty())
		MessageBox(_T("Channel name must not be empty"), _T("Notice"), MB_OK | MB_ICONERROR);
	else {
		VideoCanvas vc;

		vc.uid = 0;
		vc.view = m_wndLocal.GetSafeHwnd();
		vc.renderMode = RENDER_MODE_FIT;

		m_lpAgoraRtcEngine->GetEngine()->setupLocalVideo(vc);

		if (m_useMediaToken != FALSE)
		{
			m_lpAgoraRtcEngine->JoinChannel(m_strChannelName, 0, m_MEDIA_TOKEN);
		}
		else {
			m_lpAgoraRtcEngine->JoinChannel(m_strChannelName, 0, _T(""));
		}
	}
}


void CAgoraVideoCallTestDlg::OnBnClickedBtnleave()
{
	m_lpAgoraRtcEngine->LeaveChannel();
	m_wndLocal.ShowWindow(SW_HIDE);
	m_wndLocal.SetBitmap(m_OrigLocalBitmap);
	m_wndLocal.ShowWindow(SW_SHOW);

	m_wndRemote.ShowWindow(SW_HIDE);
	m_wndRemote.SetBitmap(m_OrigRemoteBitmap);
	m_wndRemote.ShowWindow(SW_SHOW);
}

void CAgoraVideoCallTestDlg::OnBnClickedBtnmutelocaud()
{
	BOOL bStatValue = !m_lpAgoraRtcEngine->IsLocalAudioMuted();
	m_lpAgoraRtcEngine->MuteLocalAudio(bStatValue);
	if (bStatValue == TRUE)
	{
		GetDlgItem(IDC_BTNMUTELOCAUD)->SetWindowText(_T("Unmute Local Audio"));
	} else {
		GetDlgItem(IDC_BTNMUTELOCAUD)->SetWindowText(_T("Mute Local Audio"));
	}
}

void CAgoraVideoCallTestDlg::OnBnClickedBtndisvid()
{
	BOOL bStatValue = !m_lpAgoraRtcEngine->IsLocalVideoMuted();
	m_lpAgoraRtcEngine->MuteLocalVideo(bStatValue);
	if (bStatValue == FALSE)
	{
		GetDlgItem(IDC_BTNDISVID)->SetWindowText(_T("Dsiable Local Video"));
	}
	else {
		GetDlgItem(IDC_BTNDISVID)->SetWindowText(_T("Enable Local Video"));
	}
}

LRESULT CAgoraVideoCallTestDlg::OnFirstRemoteVideoDecoded(WPARAM wParam, LPARAM lParam)
{
	LPAGE_FIRST_REMOTE_VIDEO_DECODED lpData = (LPAGE_FIRST_REMOTE_VIDEO_DECODED)wParam;

	VideoCanvas vc;

	vc.renderMode = RENDER_MODE_FIT;
	vc.uid = lpData->uid;
	vc.view = m_wndRemote.GetSafeHwnd();

	m_lpAgoraRtcEngine->GetEngine()->setupRemoteVideo(vc);

	delete lpData;

	return 0;
}

void CAgoraVideoCallTestDlg::OnBnClickedTest()
{
	// TODO: Add your control notification handler code here
	
	int ret = 0;

	if (m_VideoDeviceTestStarted == FALSE)
	{
		ret = m_VideoDeviceTestMgr->get()->startDeviceTest(m_wndLocal.GetSafeHwnd());
		if (ret == 0)
		{
			m_VideoDeviceTest.SetWindowText(L"Stop Video Device Test");
			m_VideoDeviceTestStarted = TRUE;
			
		}
	}
	else {
		m_VideoDeviceTestMgr->get()->stopDeviceTest();
		if (ret == 0)
		{
			m_wndLocal.ShowWindow(SW_HIDE);
			m_wndLocal.SetBitmap(m_OrigLocalBitmap);
			m_wndLocal.ShowWindow(SW_SHOW);
			m_VideoDeviceTest.SetWindowText(L"Start Video Device Test");
			m_VideoDeviceTestStarted = FALSE;

		}
	}
}


void CAgoraVideoCallTestDlg::OnBnClickedShare()
{
	int ret;
	
	if (m_ScreenShareStarted == FALSE)
	{
		HWND hWnd;
		hWnd = ::GetDesktopWindow();
	
		ScreenCaptureParameters capParam;
		agora::rtc::Rectangle rcCapWnd = { 0, 0, 0, 0};

		ret = m_lpAgoraRtcEngine->GetEngine()->startScreenCaptureByWindowId(hWnd, rcCapWnd, capParam);
		if (ret == 0)
		{
			m_wndScreenShare.SetWindowText(L"Stop Screen Share");
			m_ScreenShareStarted = TRUE;
		}
	}
	else
	{
		ret = m_lpAgoraRtcEngine->GetEngine()->stopScreenCapture();
		if (ret == 0)
		{
			m_wndScreenShare.SetWindowText(L"Start Screen Share");
			m_ScreenShareStarted = FALSE;
		}
	}
}

void CAgoraVideoCallTestDlg::OnBnClickedSearch()
{

	m_lpAgoraRESTCall->AgoraRESTSearch();
}

void CAgoraVideoCallTestDlg::OnBnClickedPing()
{
	
	m_lpAgoraRESTCall->AgoraRESTPing();
}


