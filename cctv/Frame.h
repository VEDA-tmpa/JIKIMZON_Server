#ifndef FRAME_H
#define FRAME_H

namespace cctv
{
	enum Frame 
	{
		WIDTH = 640,
		HEIGHT = 480,
		PIXEL_SIZE = 3,
		FRAME_SIZE = (WIDTH * HEIGHT * PIXEL_SIZE)
	};	
}

#endif // FRAME_H