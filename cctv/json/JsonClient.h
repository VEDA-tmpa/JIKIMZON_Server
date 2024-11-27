#ifndef JSON_CLIENT_H
#define JSON_CLIENT_H

#include <nlohmann/json.hpp>

#include "common/tcp/client/BaseClient.h"

namespace cctv
{
    class JsonClient : public tcp::BaseClient
    {
    public:
        JsonClient(const std::string& host, int port, std::unique_ptr<cipher::ICiphable> cipherHandler);
        ~JsonClient();

    protected:
        void handleData(const void* buffer, size_t size) override;

    private:
		void saveJson(nlohmann::json json);

		std::string mDataBuffer;
    };
}

#endif /* JSON_CLIENT_H */
