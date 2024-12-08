#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <string>

#include "common/frame/Frame.h"
#include "storage/StorageFile.h"

#define OUT

namespace storage
{
	class StorageManager
	{
	public:
		StorageManager(const std::string ip);
		~StorageManager();

		void SaveFrame(const frame::Frame frame);
		void GetNextFrame(OUT frame::frame frame) const;

	private:
		std::string mId;
		std::string mStorageDirPath;
		std::string mStorageFilePath;

		storage::Header mFileHeader;
	};
}

#endif // STORAGE_MANAGER_H