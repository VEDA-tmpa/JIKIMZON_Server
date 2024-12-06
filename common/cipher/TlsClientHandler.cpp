#include "TlsClientHandler.h"

namespace cipher
{
	TlsClientHandler::~TlsClientHandler()
	{
		SSL_shutdown(mSSL);
		SSL_free(mSSL);

		SSL_CTX_free(mCTX);
		EVP_cleanup();
	}

	void TlsClientHandler::Init()
	{
		SSL_library_init();
		SSL_load_error_strings();
		OpenSSL_add_ssl_algorithms();
		createSSLContext();
	}

	void TlsClientHandler::createSSLContext()
	{
		const SSL_METHOD* method = TLS_client_method(); // 클라이언트용
		mCTX = SSL_CTX_new(method);
		if (!mCTX)
		{
			perror("Unable to create SSL context");
			ERR_print_errors_fp(stderr);
			exit(EXIT_FAILURE);
		}
	}

	void TlsClientHandler::PerformTLSHandshake(int clientFd)
	{
		mSSL = SSL_new(mCTX);
		SSL_set_fd(mSSL, clientFd);

		if (SSL_connect(mSSL) <= 0)
		{
			ERR_print_errors_fp(stderr);
			exit(EXIT_FAILURE);
		}
		std::cout << "Connected with " << SSL_get_cipher(mSSL) << " encryption" << std::endl;
	}

	SSL* TlsClientHandler::GetSSL()
	{
		return mSSL;
	}
}