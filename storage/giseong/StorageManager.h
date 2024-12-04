#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <string>
#include <filesystem>
#include <fstream>

#include "VideoHeader.h"
#include "common/frame/Frame.h"

namespace storage
{
	class StorageManager
	{
	public:
		StorageManager(const std::string& ip);
		~StorageManager();

		void SaveFrame(const frame::Frame frame);
		void GetNextFrame(OUT frame::frame frame);

	private:
		void updateHeader();
		void loadHeader();

		std::string mId;
		std::string mStoragePath;		
		std::string mVideoPath;		

		video::HeaderStruct mHeader;
	};
}
	
#endif // STORAGE_MANAGER_H
