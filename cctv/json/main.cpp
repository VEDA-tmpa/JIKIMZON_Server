#include <stdlib.h>
#include <vector>
#include <memory>

#include "cctv/json/JsonClient.h"
#include "test/fixture/Fixture.h"
#include "common/cipher/ICiphable.h"

int main(void)
{
	cctv::JsonClient client(fixture::cctv1.ip, fixture::cctv1.jsonPort);
	client.Start();

	return EXIT_SUCCESS;
}