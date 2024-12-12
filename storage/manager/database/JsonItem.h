#ifndef JSON_ITEM_H
#define JSON_ITEM_H

#include <nlohmann/json.hpp>
#include <string>

#include "storage/manager/database/BaseItem.h"
#include "common/log/Logger.h"

namespace storage
{

	class JsonItem : public BaseItem 
	{
	public:
		JsonItem() = default;
		JsonItem(const nlohmann::json& json) 
			: mJson(json)
		{
			mItemStruct.Data = serializeJsonToBytes(json);
			mItemStruct.HeaderStruct.DataSize = mItemStruct.Data.size();
		}

		/* Data 에 대해서만 역직렬화 하는 것 */
		void Deserialize(const std::vector<uint8_t>& rawData) override
		{
			mItemStruct.Data = rawData;

			mJson = deserializeBytesToJson(rawData);
		} 

		nlohmann::json GetData() const
		{
			return mJson;
		}

	private:
		std::vector<uint8_t> serializeJsonToBytes(const nlohmann::json& json) const
		{
			std::string jsonString = json.dump();
			return std::vector<uint8_t>(jsonString.begin(), jsonString.end());
		}

		nlohmann::json deserializeBytesToJson(const std::vector<uint8_t>& rawData) const
		{
			try
			{
				std::string jsonString(rawData.begin(), rawData.end());
				return nlohmann::json::parse(jsonString);
			}
			catch (const nlohmann::json::parse_error& e)
			{
				throw std::runtime_error("Failed to parse JSON data");
			}
		}

	private:
		nlohmann::json mJson;
	};
}

#endif // JSON_ITEM_H
