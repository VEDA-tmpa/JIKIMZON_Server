#ifndef NODE_TYPE_H
#define NODE_TYPE_H

#include <string>

namespace tcp
{
	typedef struct host {
		std::string ip;
		int videoPort;
		int jsonPort;
	} host_t;
}

#endif // NODE_TYPE_H