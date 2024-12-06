#ifndef TLS_CLIENT_HANDLER_H
#define TLS_CLIENT_HANDLER_H

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

namespace cipher
{
	class TlsClientHandler 
	{
	public:
		TlsClientHandler() = default;
		~TlsClientHandler();

		void Init();
		void PerformTLSHandshake(int clientFd);
		SSL* GetSSL();

	private:
		SSL_CTX* mCTX;
		SSL* mSSL;

		void createSSLContext();
	};
}
    
#endif // TLS_CLIENT_HANDLER_H