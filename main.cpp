#include <stdlib.h>

#include "cctv/Client.h"

int main(void)
{
	cctv::host_t cctv1 { 
		.ip = "192.168.50.14",
		.port = 12345
	};
	cctv::Client client(cctv1.ip, cctv1.port);
	client.Start();

	return EXIT_SUCCESS;
}