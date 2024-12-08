#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <string>

#include "common/frame/Frame.h"
#include "common/log/Logger.h"
#include "storage/StorageFile.h"

#define OUT

namespace storage
{
	class StorageManager
	{
	public:
		StorageManager(const std::string ip);
		~StorageManager() = default;

		void SaveFrame(const frame::Frame& frame);
		void GetNextFrame(OUT frame::Frame& frame) const;

	private:
		static void seekToOffsetFromHeader(std::ifstream& storageFile, uint32_t offset = 0);
		static void loadFrameFromFile(std::ifstream &storageFile, OUT frame::Frame& frame);

		static logger::Logger logger;

		std::string mId;
		std::string mStorageDirPath;
		std::string mStorageFilePath;

		mutable storage::Header mFileHeader;
	};
}

#endif // STORAGE_MANAGER_H