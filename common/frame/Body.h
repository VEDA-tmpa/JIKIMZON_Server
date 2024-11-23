#ifndef BODY_H
#define BODY_H

#include <cstdint>
#include <vector>

#include "common/ISerializable.h"

namespace frame
{
	class Body : public common::ISerializable
	{
	public:
		void Serialize(std::vector<uint8_t>& OUT buffer) const override;
		void Deserialize(std::vector<uint8_t>& buffer) override;	

		std::vector<uint8_t> GetImage() const;
		void SetBody(std::vector<uint8_t> image);

	private:
		std::vector<uint8_t> mImage;
	};
}

#endif // BODY_H