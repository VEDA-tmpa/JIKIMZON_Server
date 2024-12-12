#include <openssl/evp.h>
#include <fstream>
#include <iostream>

#include "ChaCha20.h"

namespace cipher
{
	logger::Logger ChaCha20::logger("ChaCha20");

	ChaCha20::ChaCha20(std::vector<uint8_t> key)
		: mKey(key)
	{
		if (key.size() != 32)
		{
			throw std::invalid_argument("Key must be 32 bytes for ChaCha20");
		}
	}

	std::vector<uint8_t> ChaCha20::LoadKeyFromFile(const std::string& filePath) 
	{
		logger.Debug(filePath);

		std::ifstream file(filePath, std::ios::binary);
		if (file.is_open() == false) 
		{
			throw std::runtime_error("Failed to open key file: " + filePath);
		}

		std::vector<uint8_t> key((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();

		if (key.size() != 32) {
			throw std::runtime_error("Key file must contain exactly 32 bytes for ChaCha20");
		}

		return key;
	}

	void ChaCha20::EncryptDecrypt(std::vector<uint8_t> nonce, 
								const std::vector<uint8_t>& input, 
								std::vector<uint8_t>& output)
	{
		EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
		if (!ctx) 
		{
			throw std::runtime_error("Failed to create EVP_CIPHER_CTX");
		}

		// Initialize the context for ChaCha20 encryption
		if (EVP_EncryptInit_ex(ctx, EVP_chacha20(), nullptr, mKey.data(), nonce.data()) != 1) 
		{
			EVP_CIPHER_CTX_free(ctx);
			throw std::runtime_error("Failed to initialize ChaCha20 context");
		}

		int len = 0;
		output.resize(input.size());

		// Encrypt/Decrypt data
		if (EVP_EncryptUpdate(ctx, output.data(), &len, input.data(), input.size()) != 1) 
		{
			EVP_CIPHER_CTX_free(ctx);
			throw std::runtime_error("Failed to process ChaCha20 data");
		}

		int final_len = 0;
		if (EVP_EncryptFinal_ex(ctx, output.data() + len, &final_len) != 1) 
		{
			EVP_CIPHER_CTX_free(ctx);
			throw std::runtime_error("Failed to finalize ChaCha20 encryption");
		}

		// Free the context
		EVP_CIPHER_CTX_free(ctx);
	}
}