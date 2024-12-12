#include "common/frame/GOP.h"

namespace frame
{
	namespace H264
	{
		GOP::GOP(const std::vector<Frame>& gop)
		{
			const Header& headerOfFirstFrame = gop[0].GetHeader();
			if (headerOfFirstFrame.GetGopStartFlag() != GOP_START_FLAG::TRUE) 
			{
				throw std::invalid_argument("Invalid GOP start flag.");
			}
			if (headerOfFirstFrame.GetGopSize() != gop.size()) 
			{
				throw std::invalid_argument("Mismatch between GOP size and frame count.");
			}
		}

		std::vector<Frame> GOP::GetFrames()
		{
			return mFrames;
		}
	}
}
