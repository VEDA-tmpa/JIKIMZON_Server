#ifndef JSON_CLIENT_H
#define JSON_CLIENT_H

#include <nlohmann/json.hpp>

#include "common/tcp/client/BaseClient.h"

#include "storage/manager/StorageManager.h"

namespace cctv
{
	class JsonClient : public tcp::BaseClient
	{
	public:
		enum { JSON_DELIMITER = '|' };
		
		JsonClient(const std::string& host, int port);
		~JsonClient() = default;

	protected:
		void handleData() override;

	private:
		static logger::Logger logger;

		nlohmann::json receiveJson(const void* buffer, size_t size);

		storage::StorageManager<nlohmann::json> mStorageManager;

		std::string mDataBuffer;
	};
}

#endif /* JSON_CLIENT_H */
