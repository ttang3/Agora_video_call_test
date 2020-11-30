
// AgoraVideoCallTest.h : PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include "stdafx.h"  to produce PCH file"
#endif

#include "resource.h"


// CAgoraVideoCallTestApp: 

class CAgoraVideoCallTestApp : public CWinApp
{
public:
	CAgoraVideoCallTestApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

extern CAgoraVideoCallTestApp theApp;