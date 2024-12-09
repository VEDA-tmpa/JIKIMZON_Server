#ifndef H_264_ITEM_H
#define H_264_ITEM_H


#include "storage/manager/database/BaseItem.h"
#include "common/frame/Frame.h"

namespace storage
{
	class H264Item : public BaseItem 
	{
	public:
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

		std::vector<uint8_t> Serialize() const override
		{
			return mItemStruct.Data;
		}

		void Deserialize(const std::vector<uint8_t>& rawData) override 
		{
			{
				mItemStruct.HeaderStruct.ItemSize = rawData.size();
				mItemStruct.Data = rawData;
			}

			std::vector<frame::Frame> gop;
			
			size_t offset = 0;
			while (offset < rawData.size())
			{
				frame::Header header;
				{
					std::vector<uint8_t> headerBuffer(rawData.begin() + offset, rawData.begin() + offset + sizeof(frame::HeaderStruct));
					header.Deserialize(headerBuffer);
					offset += sizeof(frame::HeaderStruct);
				}

    			frame::Body body;
				{
					std::vector<uint8_t> bodyBuffer(rawData.begin() + offset, rawData.begin() + offset + header.GetBodySize());
					body.Deserialize(bodyBuffer);
					offset += header.GetBodySize();
				}

				frame::Frame frame(header, body);

				gop.push_back(frame);
			}

			mGop = gop;
		}

		size_t Size() const override
		{
			return mItemStruct.Data.size();
		}

	private:
		std::vector<frame::Frame> mGop;

		ItemStruct mItemStruct;
	};
}

#endif // H_264_ITEM_H