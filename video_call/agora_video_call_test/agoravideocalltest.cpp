//

#include "stdafx.h"
#include "agoravideocalltest.h"
#include "agoravideocalltestDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAgoraVideoCallTestApp

BEGIN_MESSAGE_MAP(CAgoraVideoCallTestApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


CAgoraVideoCallTestApp::CAgoraVideoCallTestApp()
{
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;
}

CAgoraVideoCallTestApp theApp;


BOOL CAgoraVideoCallTestApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	SetRegistryKey(_T("Generated automatically"));



	CAgoraVideoCallTestDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{

	}
	else if (nResponse == IDCANCEL)
	{

	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "Exit\n");
	}

	return FALSE;
}

