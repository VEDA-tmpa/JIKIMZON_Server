#ifndef H_264_ITEM_H
#define H_264_ITEM_H


#include "storage/manager/database/BaseItem.h"
#include "common/frame/Frame.h"

namespace storage
{
	class H264Item : public BaseItem 
	{
	public:
		H264Item() = default;

		H264Item(const std::vector<frame::Frame>& gop)
			: mGop(gop)
		{
			{
				// Validation
				const frame::Header& headerOfFirstFrame = gop[0].GetHeader();
				if (headerOfFirstFrame.GetGopStartFlag() != frame::GOP_START_FLAG::TRUE) 
				{
					throw std::invalid_argument("Invalid GOP start flag.");
				}
				if (headerOfFirstFrame.GetGopSize() != gop.size()) 
				{
					throw std::invalid_argument("Mismatch between GOP size and frame count.");
				}
			}

			{
				// Serialize the GOP
				for (const auto& frame : gop) 
				{
					std::vector<uint8_t> outFrameBuffer;
					frame.Serialize(outFrameBuffer)

					mItemStruct.Data.insert(mItem.data.end(), outFrameBuffer.begin(), outFrameBuffer.end());
				}

				// setup ItemHeader
				mItemStruct.HeaderStruct.ItemSize = mItemStruct.Data.size();
			}
		}

		std::vector<uint8_t> H264Item::Serialize() const override
		{
			std::vector<uint8_t> serializedData;

			// Serialize Item Header
			uint32_t serializedItemSize = htonl(mItemStruct.HeaderStruct.ItemSize);
			serializedData.insert(serializedData.end(),
								  reinterpret_cast<const uint8_t*>(&serializedItemSize),
								  reinterpret_cast<const uint8_t*>(&serializedItemSize) + sizeof(serializedItemSize));

			// Serialize Frame Data
			serializedData.insert(serializedData.end(), mItemStruct.Data.begin(), mItemStruct.Data.end());

			return serializedData;
		}

		void H264Item::Deserialize(const std::vector<uint8_t>& rawData) override 
		{
			size_t offset = 0;

			mItemStruct.HeaderStruct.ItemSize = ntohl(*reinterpret_cast<const uint32_t*>(&rawData[offset]));
			offset += sizeof(ItemHeaderStruct);

			// Deserialize Frame Data
			mItemStruct.Data = std::vector<uint8_t>(rawData.begin() + offset, rawData.end());

			// Reconstruct GOP
			mGop.clear();
			offset = 0;
			while (offset < mItemStruct.Data.size())
			{
				frame::Frame frame;

				// Deserialize individual frame
				offset += frame.Deserialize(std::vector<uint8_t>(mItemStruct.Data.begin() + offset, mItemStruct.Data.end()));
				mGop.push_back(frame);
			}
		}

		std::vector<frame::Frame> GetData()
		{
			return mGop;
		}

	private:
		std::vector<frame::Frame> mGop;
	};
}

#endif // H_264_ITEM_H