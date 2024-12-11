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
				if (static_cast<frame::GOP_START_FLAG>(headerOfFirstFrame.GetGopStartFlag()) != frame::GOP_START_FLAG::TRUE) 
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
					frame.Serialize(outFrameBuffer);

					mItemStruct.Data.insert(mItemStruct.Data.end(), outFrameBuffer.begin(), outFrameBuffer.end());
				}

				// setup ItemHeader
				mItemStruct.HeaderStruct.ItemSize = mItemStruct.Data.size();
			}
		}

		/* Data 에 대해서만 역직렬화 하는 것 */
		void Deserialize(const std::vector<uint8_t>& rawData) override 
		{
			mItemStruct.Data.insert(mItemStruct.Data.end(), rawData.begin(), rawData.end());

			// Reconstruct GOP
			mGop.clear();
			size_t offset = 0;
			while (offset < mItemStruct.Data.size())
			{
				frame::Frame frame;

				std::vector<uint8_t> buffer(mItemStruct.Data.begin() + offset, mItemStruct.Data.end());
				frame.Deserialize(buffer);
				mGop.push_back(frame);

				offset += frame.GetSize();
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