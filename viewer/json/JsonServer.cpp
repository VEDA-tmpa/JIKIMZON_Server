#include <unistd.h>

#include "viewer/json/JsonServer.h"
#include "common/frame/Frame.h"
#include "test/fixture/Fixture.h"
#include "common/cipher/ChaCha20.h"

namespace viewer
{
	JsonServer::JsonServer(int port, std::unique_ptr<cipher::ICiphable> cipherHandler)
		: BaseServer(port, std::move(cipherHandler))
	{
	}

	void JsonServer::handleData(int socketFd)
	{
		streaming(socketFd); // TODO: storage 메서드를 이용하도록 리팩토링
	}

	void JsonServer::streaming(int socketFd)
	{	
	}
}