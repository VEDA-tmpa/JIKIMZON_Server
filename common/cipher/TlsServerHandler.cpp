#include "TlsServerHandler.h"

#include <openssl/err.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

namespace cipher
{
	void TlsServerHandler::Init()
	{
		SSL_load_error_strings();
		OpenSSL_add_ssl_algorithms();
		createSSLContext();
		configureContext();
	}

	void TlsServerHandler::PerformTLSHandshake()
	{
		mSSL = SSL_new(mCTX);
		SSL_set_fd(mSSL, clientFd);

		if (SSL_accept(mSSL) <= 0)
		{
			ERR_print_errors_fp(stderr);
		}
	}

	void TlsServerHandler::Shutdown()
	{
		EVP_cleanup();
		
		if (mSSL)
		{
			SSL_shutdown(mSSL);
			SSL_free(mSSL);
		}
	}


	void TlsServerHandler::createSSLContext()
	{
		const SSL_METHOD* method = TLS_server_method();
		mCTX = SSL_CTX_new(method);
		if (!mCTX)
		{
			std::cerr << "Failed create ssl context" << std::endl;
		}
	}

	void TlsServerHandler::configureContext()
	{
		if (SSL_CTX_use_certificate_file(mCTX, std::string(PROJECT_ROOT) + "/viewer/certs/server.cert", SSL_FILETYPE_PEM) <= 0)
		{
			std::cerr << "Failed get certificate" << std::endl;
		}

		if (SSL_CTX_use_PrivateKey_file(mCTX, std::string(PROJECT_ROOT) +  "/viewer/certs/server.key", SSL_FILETYPE_PEM) <= 0)
		{
			std::cerr << "Failed get private key" << std::endl;
		}
	}
}