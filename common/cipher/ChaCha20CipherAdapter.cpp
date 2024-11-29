#include "common/cipher/ChaCha20CipherAdapter.h"
#include "common/cipher/ChaCha20.h"

namespace cipher
{
	ChaCha20CipherAdapter::ChaCha20CipherAdapter(const std::string& keyFilePath)
		: mChaCha20(cipher::ChaCha20::LoadKeyFromFile(keyFilePath))
	{
	}

	void ChaCha20CipherAdapter::Encrypt(const std::vector<uint8_t>& input, 
										std::vector<uint8_t>& output, 
										const std::vector<uint8_t>& nonce)
	{
		mChaCha20.EncryptDecrypt(nonce, input, output);
	}

	void ChaCha20CipherAdapter::Decrypt(const std::vector<uint8_t>& input, 
										std::vector<uint8_t>& output, 
										const std::vector<uint8_t>& nonce)
	{
		mChaCha20.EncryptDecrypt(nonce, input, output);
	}
}