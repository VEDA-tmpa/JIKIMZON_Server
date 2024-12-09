#ifndef STORAGE_MANAGER_H
#define STORAGE_MANAGER_H

#include <string>

#include "common/frame/Frame.h"
#include "common/log/Logger.h"
#include "storage/manager/database/StorageFile.h"

#define OUT

namespace storage
{
	class StorageManager
	{
	public:
		StorageManager(const std::string ip);
		~StorageManager() = default;

		void SaveItem(const storage::BaseItem& item);
		void GetNextItem(OUT storage::BaseItem& item) const;

	private:
		static logger::Logger logger;

		std::string mId;
		std::string mStorageDirPath;
		std::string mStorageFilePath;

		mutable storage::StorageFile mStorageFile;
	};
}

#endif // STORAGE_MANAGER_H