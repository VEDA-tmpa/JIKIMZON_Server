#ifndef I_CIPHABLE_H
#define I_CIPHABLE_H

#include <vector>
#include <cstdint>

namespace cipher 
{
	class ICiphable 
	{
	public:
		virtual ~ICiphable() = default;

		virtual void Encrypt(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, const std::vector<uint8_t>& nonce) = 0;
		virtual void Decrypt(const std::vector<uint8_t>& input, std::vector<uint8_t>& output, const std::vector<uint8_t>& nonce) = 0;
	};
}

#endif // I_CIPHABLE_H