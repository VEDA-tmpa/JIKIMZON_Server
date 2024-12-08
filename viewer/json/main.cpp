#include <stdlib.h>
#include <memory>

#include "viewer/json/JsonServer.h"
#include "test/fixture/Fixture.h"
#include "common/cipher/ChaCha20CipherAdapter.h"
#include "common/cipher/ICiphable.h"

int main(void)
{
	const std::string keyFilePath = std::string(PROJECT_ROOT) + "/viewer/keyfile.bin";
	std::unique_ptr<cipher::ICiphable> chacha20Handler = std::make_unique<cipher::ChaCha20CipherAdapter>(keyFilePath);

	viewer::JsonServer jsonServer(viewer::JsonServer::PORT, std::move(chacha20Handler));
	jsonServer.Start();

	return EXIT_SUCCESS;
}