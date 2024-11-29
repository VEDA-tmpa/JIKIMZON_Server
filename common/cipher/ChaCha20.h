#ifndef CIPHER_H
#define CIPHER_H

#include <cstdint>
#include <vector>

#include "common/log/Logger.h"

namespace cipher
{
	class ChaCha20
	{
	public:
		ChaCha20() = delete;
		ChaCha20(std::vector<uint8_t> key);
		~ChaCha20() = default;

        static std::vector<uint8_t> LoadKeyFromFile(const std::string& filePath);

		void EncryptDecrypt(std::vector<uint8_t> nonce, 
							const std::vector<uint8_t>& input, 
							std::vector<uint8_t>& output);

	private:
		static logger::Logger logger;

		std::vector<uint8_t> mKey;
	};
}

#endif // CIPHER_H