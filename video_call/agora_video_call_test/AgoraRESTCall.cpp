#include <stdio.h>

#include <iostream>
#include <string>
#include "stdafx.h"
#include <afxwin.h>
#include "AgoraRESTCall.h"


#include <urlmon.h>
#include <sstream>

#include <atlbase.h>
#include <msxml6.h>
#include <comutil.h> // _bstr_t


using namespace std;


string CAgoraRESTCall::sendRequest(string req)
{

	HRESULT hr;
	CComPtr<IXMLHTTPRequest> request;


	_bstr_t tmp(req.c_str());
	CoInitialize(NULL);
	hr = request.CoCreateInstance(CLSID_XMLHTTP60);
	SUCCEEDED(hr) ? 0 : throw hr;
	DWORD err = GetLastError();
	hr = request->open(
		_bstr_t("GET"),
		tmp,
		_variant_t(VARIANT_FALSE),
		_variant_t(),
		_variant_t());

	// You can pass request body, now it is empty.
	hr = request->send(_variant_t());

	// get status - 200 if succuss
	long status;
	hr = request->get_status(&status);

	
	VARIANT responseVariant;
	hr = request->get_responseStream(&responseVariant);
	IStream* stream = (IStream*)responseVariant.punkVal;

	char buffer[MAX_PATH];
	unsigned long bytesRead;
	stringstream ss;
	stream->Read(buffer, MAX_PATH, &bytesRead);
	while (bytesRead > 0U)
	{
		ss.write(buffer, (long long)bytesRead);
		stream->Read(buffer, MAX_PATH, &bytesRead);
	}
	stream->Release();
	string resultString = ss.str();

	return resultString;
}

BOOL CAgoraRESTCall::AgoraRESTSearch()
{
	string result = sendRequest(_base_uri + "/");
	LPCSTR tmp = result.c_str();
	MessageBoxA(NULL, tmp, "search_result", MB_OK);
			return true;
}

BOOL CAgoraRESTCall::AgoraRESTPing()
{
	string result = sendRequest(_base_uri + "/test/v1/ping");
	LPCSTR tmp = result.c_str();
	MessageBoxA(NULL, tmp, "ping_result", MB_OK);
	return true;
}

