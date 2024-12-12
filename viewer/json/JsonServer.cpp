#include <unistd.h>

#include "viewer/json/JsonServer.h"
#include "common/frame/Frame.h"
#include "test/fixture/Fixture.h"
#include "common/cipher/ChaCha20.h"

namespace viewer
{
	JsonServer::JsonServer(int port, std::unique_ptr<cipher::ICiphable> cipherHandler)
		: BaseServer(port, std::move(cipherHandler))
		, mStorageManager(fixture::cctv1.ip)
	{
	}

	void JsonServer::handleData(int socketFd)
	{
		streaming(socketFd);
	}

	void JsonServer::streaming(int socketFd)
	{	
		logger.Info("streaming start");

		while (true)
		{
			nlohmann::json outJson;
			mStorageManager.GetNextData(outJson);

			std::string jsonStr = outJson.dump() + "|";
			if (SSL_write(mTlsHandler->GetSSL(), jsonStr.c_str(), jsonStr.size()) <= 0)
			{
				logger.Error("send() failed or client disconnected");
				return; // 클라이언트 연결 종료
			}
		}
	}
}