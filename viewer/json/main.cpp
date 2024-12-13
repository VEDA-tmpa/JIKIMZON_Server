#include <stdlib.h>
#include <memory>

#include "viewer/json/JsonServer.h"
#include "test/fixture/Fixture.h"
#include "common/cipher/ChaCha20CipherAdapter.h"
#include "common/cipher/ICiphable.h"

int main(void)
{
	viewer::JsonServer jsonServer(viewer::JsonServer::PORT);
	jsonServer.Start();

	return EXIT_SUCCESS;
}