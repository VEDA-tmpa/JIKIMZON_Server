#include <stdlib.h>
#include <memory>

#include "viewer/Server.h"
#include "test/fixture/Fixture.h"
#include "common/cipher/ChaCha20CipherAdapter.h"
#include "common/cipher/ICiphable.h"

int main(void)
{
	const std::string keyFilePath = std::string(PROJECT_ROOT) + "/cctv/" + fixture::cctv1.ip + "/keyfile.bin";
	std::unique_ptr<cipher::ICiphable> chacha20Handler = std::make_unique<cipher::ChaCha20CipherAdapter>(keyFilePath);

	viewer::Server server(viewer::Server::PORT, std::move(chacha20Handler));
	server.Start();

	return EXIT_SUCCESS;
}