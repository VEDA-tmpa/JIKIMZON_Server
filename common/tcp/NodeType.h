#ifndef NODE_TYPE_H
#define NODE_TYPE_H

#include <string>

namespace tcp
{
	typedef struct host {
		std::string ip;
		int port;
	} host_t;
}

#endif // NODE_TYPE_H