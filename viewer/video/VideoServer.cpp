#include <unistd.h>

#include "viewer/video/VideoServer.h"
#include "common/frame/Frame.h"
#include "test/fixture/Fixture.h"
#include "common/cipher/ChaCha20.h"

namespace viewer
{
	VideoServer::VideoServer(int port)
		: BaseServer(port)
		, mStorageManager(fixture::cctv1.ip)
	{
	}

	void VideoServer::handleData(int socketFd)
	{	
		streaming(socketFd);
	}

	void VideoServer::streaming(int socketFd)
	{
		logger.Info("streaming start");

		while (true)
		{
			frame::H264::GOP outGop;
			mStorageManager.GetNextData(outGop);

			std::vector<frame::Frame> gop = outGop.GetFrames();

			for (int i = 0; i < gop.size(); ++i)
			{
				std::vector<uint8_t> buffer;
				logger.Info("VideoServer::streaming() send frame id: " +std::to_string(gop[i].GetHeader().GetFrameId()));
				gop[i].Serialize(buffer);


				if (SSL_write(mTlsHandler->GetSSL(), reinterpret_cast<char*>(buffer.data()), buffer.size()) <= 0)
				{
					logger.Error("send() failed or client disconnected");
					return; // 클라이언트 연결 종료
				}
			}
		}
	}
}