openssl req -new -newkey rsa:2048 -nodes -keyout server.key -out server.csr
openssl x509 -req -sha256 -days 365 -in server.csr -signkey server.key -out server_cert.pem
