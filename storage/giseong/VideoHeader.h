#ifndef VIDEOHEADER_H
#define VIDEOHEADER_H

#include <cstdint>

namespace video
{
	/*
	----------- Metadata -----------
	|          startIndex   (4)    |
	|           endIndex    (4)    |
	|         currentIndex  (4)    |
	------------- Data -------------
	|                              |
	|                              |
	|                              |
	|                              |
	|                              |
	|                              |
	|                              |
	--------------------------------
	*/

	struct storage
	{
		uint32_t maximumSize;
	};

	struct HeaderStruct
	{
		uint32_t startIndex;
		uint32_t endIndex;     

		uint32_t currentIndex; 

		uint32_t paddingIndex;
	};
}

#endif // VIDEOHEADER_H
