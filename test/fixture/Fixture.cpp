#include "Fixture.h"

namespace fixture
{
	tcp::host_t cctv1 = { 
		.ip = "192.168.50.14",
		.videoPort = 12345,
		.jsonPort = 56789
	};
	tcp::host_t cctv2 = { 
		.ip = "192.168.50.15", // 임의의 값
		.videoPort = 12345,
		.jsonPort = 56789
	};
	tcp::host_t cctv3 = { 
		.ip = "192.168.50.16", // 임의의 값
		.videoPort = 12345,
		.jsonPort = 56789
	};
}
