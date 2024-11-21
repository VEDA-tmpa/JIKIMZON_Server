#ifndef CIPHER_H
#define CIPHER_H

#include <cstdint>
#include <vector>

namespace cipher
{
	class ChaCha20
	{
	public:
		ChaCha20() = delete;
		ChaCha20(std::vector<uint8_t> key);
		~ChaCha20() = default;

		void EncryptDecrypt(std::vector<uint8_t> nonce, 
							const std::vector<uint8_t>& input, 
							std::vector<uint8_t>& output);

	private:
		std::vector<uint8_t> mKey;
	};
}

#endif // CIPHER_H