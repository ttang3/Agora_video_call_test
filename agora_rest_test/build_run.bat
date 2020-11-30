FOR /F %%i IN ('cd') DO set GOPATH=%%i
echo %GOPATH%

go build -v -x -o .\bin\agora_rest_test.exe .\src\agora_rest_test

.\bin\agora_rest_test.exe 8889 .\bin\server_cert.pem .\bin\server.key.pem