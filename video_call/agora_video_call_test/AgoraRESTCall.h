#pragma once
#include <string>
using namespace std;

class CAgoraRESTCall
{
public:
	CAgoraRESTCall(string base_uri) { _base_uri = base_uri; };
	~CAgoraRESTCall() {};
	BOOL AgoraRESTSearch();
	BOOL AgoraRESTPing();
protected:

private:
	string sendRequest(string req);
	string _base_uri;


};
