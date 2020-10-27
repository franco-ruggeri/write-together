# Configuration of server private key and certificate

Use or create the directory src/certificate, as client and server applications are expected to find resources there.
Set it as the working directory from a terminal for the following commands.

+ openssl req -out server_ca.pem -new -x509 -nodes<br>
    Leave all the interactive fields empty (typing `.`), except 'Common Name', which must be `127.0.0.1` or `localhost`,
    used by the client to connect to the server (and by the server to listen on from incoming sockets)
+ mv privkey.pem server_privkey.pem
+ create server_index.txt file and write `00` at the beginning of it
+ openssl genrsa -out server_local.key 2048
+ openssl req -key server_local.key -new -out server_local.req<br>
    As in the previous case, all interactive fields empty (`.`), except the Common Name
+ openssl x509 -req -in server_local.req -CA server_ca.pem -CAkey server_privkey.pem -CAserial server_index.txt -out server_local.pem
