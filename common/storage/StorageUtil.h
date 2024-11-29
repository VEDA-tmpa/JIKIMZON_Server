#ifndef STORAGE_UTIL_H
#define STORAGE_UTIL_H

#include <cstdio>

namespace storage
{
	static void SaveToFile(FILE* file, const char* data, size_t size)
	{
		fwrite(data, sizeof(char), size, file);
	}
}

#endif // STORAGE_UTIL_H