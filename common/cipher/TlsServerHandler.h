#ifndef TLS_SERVER_HANDLER_H
#define TLS_SERVER_HANDLER_H

#include <openssl/ssl.h>
#include <vector>

namespace cipher
{
	class TlsServerHandler 
	{
	public:
		TlsServerHandler() = default;
		~TlsServerHandler() = default;

		void Init();
		void PerformTLSHandshake(int clientFd);
		void Shutdown();

	private:
		SSL_CTX* mCTX;
		SSL* mSSL;

		void createSSLContext();
		void configureContext();
	};
}

#endif // TLS_SERVER_HANDLER_H