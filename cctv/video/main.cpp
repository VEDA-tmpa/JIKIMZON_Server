#include <stdlib.h>
#include <vector>
#include <memory>

#include "cctv/video/VideoClient.h"
#include "test/fixture/Fixture.h"
#include "common/cipher/ICiphable.h"

int main(void)
{
	cctv::VideoClient client(fixture::cctv1.ip, fixture::cctv1.videoPort);
	client.Start();

	return EXIT_SUCCESS;
}