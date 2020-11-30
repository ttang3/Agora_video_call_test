
#Starting REST Service

On Windows:

	#Install Golang for Windows:
	curl -k https://golang.org/dl/go1.15.5.windows-amd64.msi -o go1.15.5.windows-amd64.msi
	run go1.15.5.windows-amd64.msi to install

	cd agora_rest_test
	 run "build_run.bat" to start the REST Service

#On Linux,
	
	cd agora_rest_test
 	#Refer to README
	run "make" to build, then "make run_secure" to start the REST Service

#For Video call test

	It is tested in VS 2019 of Windows 10. 
	(1) the Agora SDK is downloaded and inclused onto video/sdk.
		Follwoing is the steps to install Agora SDK.
		Curl -q https://download.agora.io/sdk/release/Agora_Native_SDK_for_Windows_v3_1_2_FULL.zip -o sdk.zip
		unzip
		copy libs sub folder to video_call/sdk.
	(2) cd video_call, in init.ini file, Keep APP_ID there, and USE_MEDIA_TOKEN to 0, if want to use media_token, 
		set to none zero and apply media_token in Agora console.

#How to test?

	(1) cp video_call/x64/(debug/release)/agora_video_call_test.exe, agora_rtc_sdk.dll and init.ini to another Wndows 10 machine. 
		and run agora_video_call_test.exe there, click "join".
	(2) cd agora_rest_test, run "build_run.bat"
	(3) Run video_call/x64/(debug/release)/agora_video_call_test.exe locally, click "Join",  "leave", "start share", mute audio , video, start video device test etc.
		See effect on the other Widnow 10 Machine.









