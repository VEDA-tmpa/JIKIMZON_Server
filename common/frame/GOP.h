#ifndef GOP_H
#define GOP_H

#include <vector>

#include "common/frame/Frame.h"

namespace frame
{
	namespace H264
	{
		class GOP
		{
		public:
			GOP() = default;
			GOP(const std::vector<frame::Frame>& gop);

			std::vector<Frame> GetFrames();

		private:
			std::vector<Frame> mFrames;
		};
	}
}

#endif // GOP_H