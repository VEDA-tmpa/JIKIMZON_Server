#ifndef TLS_SERVER_HANDLER_H
#define TLS_SERVER_HANDLER_H

#include <openssl/ssl.h>
#include <nlohmann/json.hpp>
#include <vector>

namespace cipher
{
	class TlsServerHandler 
	{
	public:
		TlsServerHandler() = delete;
		~TlsServerHandler() = delete;

		void Init();
		void PerformTLSHandshake();
		void Shutdown();

	private:
		SSL_CTX* mCTX;
		SSL* mSSL;

		void createSSLContext();
		void configureContext();
	};
}

#endif // TLS_SERVER_HANDLER_H