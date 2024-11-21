#ifndef CIPHER_H
#define CIPHER_H

#include <cstdint>
#include <vector>

namespace cipher
{
	class Cipher
	{
	public:
		Cipher() = delete;
		Cipher(std::vector<uint8_t> key);
		~Cipher() = default;

		void Chacha20_encrypt_decrypt(std::vector<uint8_t> nonce, 
									  const std::vector<uint8_t>& input, 
									  std::vector<uint8_t>& output);

	private:
		std::vector<uint8_t> mKey;
	};
}

#endif // CIPHER_H