#ifndef FRAME_H
#define FRAME_H

#include <cstdint>
#include <vector>

#include "common/frame/ISerializable.h"
#include "Header.h"
#include "Body.h"

namespace frame
{
	enum class ImageFormat : uint8_t
	{
		RAW = 0,
		H264 = 1
	};

	class Frame : public common::ISerializable
	{
	public:
		Frame() = default;
		Frame(Header header, Body body);
		~Frame() = default;

		void Serialize(std::vector<uint8_t>& OUT buffer) const override;
		void Deserialize(std::vector<uint8_t>& buffer) override;

		size_t GetSize() const;
		const Header& GetHeader() const; 
		const Body& GetBody() const;

	private:
		Header mHeader;
		Body mBody;
	};
}

#endif // FRAME_H