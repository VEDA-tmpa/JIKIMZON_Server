#include <stdlib.h>
#include <memory>

#include "viewer/video/VideoServer.h"
#include "test/fixture/Fixture.h"
#include "common/cipher/ICiphable.h"

int main(void)
{
	viewer::VideoServer videoServer(viewer::VideoServer::PORT);
	videoServer.Start();

	return EXIT_SUCCESS;
}