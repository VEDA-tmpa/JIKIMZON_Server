#ifndef CHACHA20_CIPHER_ADAPTER_H
#define CHACHA20_CIPHER_ADAPTER_H

#include "common/cipher/ICiphable.h"
#include "common/cipher/ChaCha20.h"

namespace cipher 
{
	class ChaCha20CipherAdapter : public ICiphable 
	{
	public:
		ChaCha20CipherAdapter(const std::string& keyFilePath);

		void Encrypt(const std::vector<uint8_t>& input, 
					std::vector<uint8_t>& output, 
					const std::vector<uint8_t>& nonce) override;

		void Decrypt(const std::vector<uint8_t>& input, 
					std::vector<uint8_t>& output, 
					const std::vector<uint8_t>& nonce) override;

	private:
		ChaCha20 mChaCha20;
	};
}

#endif // CHACHA20_CIPHER_ADAPTER_H
