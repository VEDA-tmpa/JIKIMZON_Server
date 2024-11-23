#ifndef BODY_H
#define BODY_H

#define OUT

#include <cstdint>
#include <vector>

namespace frame
{
	class Body
	{
	public:
		void Serialize(std::vector<uint8_t>& OUT buffer) const;
		void Deserialize(std::vector<uint8_t>& buffer);	

		std::vector<uint8_t> GetImage() const;
		void SetBody(std::vector<uint8_t> image);

	private:
		std::vector<uint8_t> mImage;
	};
}

#endif // BODY_H