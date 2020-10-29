# Configuration of server private key and certificate

The repository contains default certificate and private key for the server, used to establish encrypted connections with SSL. However, since this repository is open source, that private key is not really private, so you should generate by yourself a new certificate.
You can follow several guides to do this (for example using [GPG](https://www.gnupg.org/documentation/manuals/gnupg/Howto-Create-a-Server-Cert.html) or [OpenSSL](http://www.infidigm.net/articles/qsslsocket_for_ssl_beginners/)).

In order to integrate the new generated files with the code, you should:
- Rename the local certificate file as *local_certificate.pem* and put it in *src/server/certificate*
- Rename the private key file as *private.key* and put it in *src/server/certificate*
- Rename the CA certificate as *ca_certificate.pem* and put it in *src/client/certificate* 