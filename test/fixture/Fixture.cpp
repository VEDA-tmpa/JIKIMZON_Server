#include "Fixture.h"

namespace fixture
{
    tcp::host_t cctv1 = { 
        .ip = "192.168.50.14",
		.videoPort = 12345,
		.jsonPort = 56789
    };
}
