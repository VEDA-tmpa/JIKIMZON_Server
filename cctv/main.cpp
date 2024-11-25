#include <stdlib.h>
#include <vector>
#include <memory>

#include "cctv/Client.h"
#include "test/fixture/Fixture.cpp"
#include "common/cipher/ChaCha20CipherAdapter.h"
#include "common/cipher/ICiphable.h"

int main(void)
{
	const std::string keyFilePath = std::string(PROJECT_ROOT) + "/cctv/" + fixture::cctv1.ip + "/keyfile.bin";
	std::unique_ptr<cipher::ICiphable> chacha20Handler = std::make_unique<cipher::ChaCha20CipherAdapter>(keyFilePath);

	cctv::Client client(fixture::cctv1.ip, fixture::cctv1.port, std::move(chacha20Handler));
	client.Start();

	return EXIT_SUCCESS;
}