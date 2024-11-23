#include "Body.h"

namespace frame
{
	void Body::Serialize(std::vector<uint8_t>& OUT buffer) const
    {
        buffer.clear();
        buffer.resize(mImage.size());
        std::memcpy(buffer.data(), mImage.data(), mImage.size());
    }

    void Body::Deserialize(std::vector<uint8_t>& buffer)
    {
        mImage.resize(buffer.size());
        std::memcpy(mImage.data(), buffer.data(), buffer.size());
    }

    std::vector<uint8_t> Body::GetImage() const
    {
        return mImage;
    }

	void Body::SetBody(std::vector<uint8_t> image)
	{
		mImage = image;
	}
}