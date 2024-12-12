#ifndef JSON_CLIENT_H
#define JSON_CLIENT_H

#include <nlohmann/json.hpp>

#include "common/tcp/client/BaseClient.h"
#include "storage/manager/StorageManager.h"
#include "storage/manager/database/JsonItem.h"

namespace cctv
{
    class JsonClient : public tcp::BaseClient
    {
    public:
		enum { JSON_DELIMITER = '|' };
		
        JsonClient(const std::string& host, int port, std::unique_ptr<cipher::ICiphable> cipherHandler);
        ~JsonClient() = default;

    protected:
        void handleData() override;

    private:
		static logger::Logger logger;

		nlohmann::json receiveJson(const void* buffer, size_t size);

		// storage::StorageManager<storage::JsonItem> mStorageManager;
		storage::StorageManager<nlohmann::json> mStorageManager;

		std::string mDataBuffer;
    };
}

#endif /* JSON_CLIENT_H */
