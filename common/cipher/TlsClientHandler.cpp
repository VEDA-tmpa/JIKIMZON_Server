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
		std::cout << "TlsClientHandler::Init() start" << std::endl;

		SSL_library_init();
		SSL_load_error_strings();
		OpenSSL_add_ssl_algorithms();
		createSSLContext();

		std::cout << "TlsClientHandler::Init() end" << std::endl;
	}

	void TlsClientHandler::createSSLContext()
	{
		std::cout << "TlsClientHandler::createSSLContext() start" << std::endl;

		const SSL_METHOD* method = TLS_client_method(); // 클라이언트용
		mCTX = SSL_CTX_new(method);
		if (!mCTX)
		{
			perror("Unable to create SSL context");
			ERR_print_errors_fp(stderr);
			exit(EXIT_FAILURE);
		}

		std::cout << "TlsClientHandler::createSSLContext() end" << std::endl;
	}

	void TlsClientHandler::PerformTLSHandshake(int serverFd)
	{
		std::cout << "TlsClientHandler::PerformTLSHandshake() start" << std::endl;

		mSSL = SSL_new(mCTX);
		std::cout << "TlsClientHandler::PerformTLSHandshake() SSL_new(mCTX)" << std::endl;

		SSL_set_fd(mSSL, serverFd);
		std::cout << "TlsClientHandler::PerformTLSHandshake() SSL_set_fd(mSSL, clientFd)" << std::endl;

		if (SSL_connect(mSSL) <= 0)
		{
			ERR_print_errors_fp(stderr);
			exit(EXIT_FAILURE);
		}
		std::cout << "Connected with " << SSL_get_cipher(mSSL) << " encryption" << std::endl;

		std::cout << "TlsClientHandler::PerformTLSHandshake() end" << std::endl;
	}

	SSL* TlsClientHandler::GetSSL()
	{
		std::cout << "TlsClientHandler::GetSSL() start" << std::endl;

		return mSSL;
	}
}