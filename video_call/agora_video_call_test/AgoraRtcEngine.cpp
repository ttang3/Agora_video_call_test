#include "StdAfx.h"

#include "AgoraRtcEngine.h"

CAgoraRtcEngine *CAgoraRtcEngine::m_lpAgoraRtcEngine = NULL;
IRtcEngine *CAgoraRtcEngine::m_lpAgoraEngine = NULL;
CAgoraRtcEngineEventHandler CAgoraRtcEngine::m_EngineEventHandler;

CAgoraRtcEngine::CAgoraRtcEngine(void)
	: m_dwEngineFlag(0)
	, m_bVideoEnable(FALSE)
	, m_bLocalAudioMuted(FALSE)
{
	m_strChannelName.Empty();
	m_bLocalVideoMuted = FALSE;
}

CAgoraRtcEngine::~CAgoraRtcEngine(void)
{
}

/**
   gets current SDK version number string 
*/
CString CAgoraRtcEngine::GetSDKVersion()
{
	int nBuildNumber = 0;
	const char *lpszEngineVer = getAgoraRtcEngineVersion(&nBuildNumber);

	CString strEngineVer;

#ifdef UNICODE
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, lpszEngineVer, -1, strEngineVer.GetBuffer(256), 256);
	strEngineVer.ReleaseBuffer();
#endif

	return strEngineVer;
}

/** 
    gets the Agora Engine instance 
*/
IRtcEngine *CAgoraRtcEngine::GetEngine()
{
	if(m_lpAgoraEngine == NULL)
		m_lpAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();

	return m_lpAgoraEngine;
}

/** 
    create or return existing AgoraObject 
*/
CAgoraRtcEngine *CAgoraRtcEngine::GetAgoraRtcEngine(LPCTSTR lpAppId)
{
	if (m_lpAgoraRtcEngine == NULL)
		m_lpAgoraRtcEngine = new CAgoraRtcEngine();

	if (m_lpAgoraEngine == NULL)
		m_lpAgoraEngine = (IRtcEngine *)createAgoraRtcEngine();

	if (lpAppId == NULL)
		return m_lpAgoraRtcEngine;

	RtcEngineContext ctx;

	ctx.eventHandler = &m_EngineEventHandler;
#ifdef UNICODE
	char szAppId[128];

	::WideCharToMultiByte(CP_ACP, 0, lpAppId, -1, szAppId, 128, NULL, NULL);
	ctx.appId = szAppId;
#else
	ctx.appId = lpAppId;
#endif

	m_lpAgoraEngine->initialize(ctx);

	return m_lpAgoraRtcEngine;
}

/** 
    clean up the AgoraObject 
*/
void CAgoraRtcEngine::CloseAgoraRtcEngine()
{
	if(m_lpAgoraEngine != NULL)
		m_lpAgoraEngine->release();

	if(m_lpAgoraRtcEngine != NULL)
		delete m_lpAgoraRtcEngine;

	m_lpAgoraEngine = NULL;
	m_lpAgoraRtcEngine = NULL;
}

/**
  Setting Message Handler 

Parameters:
    @param hWnd     A handle to the handler
 */
void CAgoraRtcEngine::SetMsgHandlerWnd(HWND hWnd)
{
	m_EngineEventHandler.SetMsgReceiver(hWnd);
}

/** 
    retrieve the handle of the message handler
*/
HWND CAgoraRtcEngine::GetMsgHandlerWnd()
{
	return m_EngineEventHandler.GetMsgReceiver();
}

/**
   Join a channel for streaming or communication.

Parameters:
@param lpChannelName    name of the channel to join
@param uUID     User ID
@param lpToken  token key
*/
BOOL CAgoraRtcEngine::JoinChannel(LPCTSTR lpChannelName, UINT nUID,LPCTSTR lpToken)
{
	int nRet = 0;

#ifdef UNICODE
	CHAR szChannelName[128];
	::WideCharToMultiByte(CP_UTF8, 0, lpChannelName, -1, szChannelName, 128, NULL, NULL);

	char szToken[128];
	::WideCharToMultiByte(CP_UTF8, 0, lpToken, -1, szToken, 128, NULL, NULL);

	if(0 == _tcslen(lpToken))
		nRet = m_lpAgoraEngine->joinChannel(NULL, szChannelName, NULL, nUID); 
	else
		nRet = m_lpAgoraEngine->joinChannel(szToken, szChannelName, NULL, nUID);
#else
	if(0 == _tcslen(lpToken))
		nRet = m_lpAgoraEngine->joinChannel(NULL, lpChannelName, NULL, nUID);
	else
		nRet = m_lpAgoraEngine->joinChannel(lpToken, lpChannelName, NULL, nUID);
#endif

	if (nRet == 0)
		m_strChannelName = lpChannelName;

	return nRet == 0 ? TRUE : FALSE;
}

/**
    Leave the channel
*/
BOOL CAgoraRtcEngine::LeaveChannel()
{
	m_lpAgoraEngine->stopPreview();
	int nRet = m_lpAgoraEngine->leaveChannel();

	return nRet == 0 ? TRUE : FALSE;
}

/**
    get current channel name
*/
CString CAgoraRtcEngine::GetChanelName()
{
	return m_strChannelName;
}

/**
    turn video on/off
 Parameters:
    @param bEnable true if turn on else turn off
*/
BOOL CAgoraRtcEngine::EnableVideo(BOOL bEnable)
{
	int nRet = 0;

	if (bEnable)
		nRet = m_lpAgoraEngine->enableVideo();
	else
		nRet = m_lpAgoraEngine->disableVideo();

	if (nRet == 0)
		m_bVideoEnable = bEnable;

	return nRet == 0 ? TRUE : FALSE;
}

/**
    check if video enabled
*/
BOOL CAgoraRtcEngine::IsVideoEnabled()
{
	return m_bVideoEnable;
}
/**
    mute local audio on/off
 Parameters:
    @param bMuted true if muted else not muted
*/
BOOL CAgoraRtcEngine::MuteLocalAudio(BOOL bMuted)
{
	ASSERT(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	int ret = rep.muteLocalAudioStream((bool)bMuted);
	if (ret == 0)
		m_bLocalAudioMuted = bMuted;

	return ret == 0 ? TRUE : FALSE;
}

/** 
    check if local audio is muted
*/
BOOL CAgoraRtcEngine::IsLocalAudioMuted()
{
	return m_bLocalAudioMuted;
}

/**
    mute local video on/off
 Parameters:
    @param bMuted true if muted else not muted
*/
BOOL CAgoraRtcEngine::MuteLocalVideo(BOOL bMuted)
{
	ASSERT(m_lpAgoraEngine != NULL);

	RtcEngineParameters rep(*m_lpAgoraEngine);

	int ret = rep.muteLocalVideoStream((bool)bMuted);
	if (ret == 0)
		m_bLocalVideoMuted = bMuted;

	return ret == 0 ? TRUE : FALSE;
}
/** 
    check if local video is muted
*/
BOOL CAgoraRtcEngine::IsLocalVideoMuted()
{
	return m_bLocalVideoMuted;
}
