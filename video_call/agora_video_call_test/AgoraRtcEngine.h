#pragma once

#include <IAgoraRtcEngine.h>
#include "AgoraRtcEngineEventHandler.h"

// #define ENABLE_CODEC	1

using namespace agora::rtc;


class CAgoraRtcEngine
{
public:
	~CAgoraRtcEngine(void);

	void SetMsgHandlerWnd(HWND hWnd);
	HWND GetMsgHandlerWnd();

	BOOL JoinChannel(LPCTSTR lpChannelName, UINT nUID = 0,LPCTSTR lpToken = NULL);
	BOOL LeaveChannel();
	CString GetChanelName();

	BOOL EnableVideo(BOOL bEnable = TRUE);
	BOOL IsVideoEnabled();

	BOOL MuteLocalAudio(BOOL bMuted = TRUE);
	BOOL IsLocalAudioMuted();

	BOOL MuteLocalVideo(BOOL bMuted = TRUE);
	BOOL IsLocalVideoMuted();

	static IRtcEngine *GetEngine();
	
	static CString GetSDKVersion();    

protected:
	CAgoraRtcEngine(void);

private:
	DWORD	m_dwEngineFlag;
	static  CAgoraRtcEngine	*m_lpAgoraRtcEngine;
	static	IRtcEngine		*m_lpAgoraEngine;
	
	CString		m_strChannelName;
	BOOL		m_bVideoEnable;

	BOOL		m_bLocalAudioMuted;
	BOOL		m_bLocalVideoMuted;

public:
	static CAgoraRtcEngine *GetAgoraRtcEngine(LPCTSTR lpVendorKey = NULL);
	static void CloseAgoraRtcEngine();

	static CAgoraRtcEngineEventHandler m_EngineEventHandler;
};
