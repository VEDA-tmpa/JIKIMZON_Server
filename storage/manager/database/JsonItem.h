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
			logger.Debug("Json: " + json.dump(4));

			mItemStruct.Data = serializeJsonToBytes(json);
			mItemStruct.HeaderStruct.ItemSize = mItemStruct.Data.size();
			
			logger.Debug("Item Data Size: " + std::to_string(mItemStruct.HeaderStruct.ItemSize));
			logger.Debug("Serialized Data: " + std::string(mItemStruct.Data.begin(), mItemStruct.Data.end()));
			logger.Debug("Item total Size: " + std::to_string(sizeof(ItemHeaderStruct) + mItemStruct.HeaderStruct.ItemSize));
		}

		// void Deserialize(const std::vector<uint8_t>& rawData) override 
		// {
		// 	mItemStruct.HeaderStruct.ItemSize = *reinterpret_cast<const uint32_t*>(rawData.data());
		// 	logger.Debug("Item Size: " + std::to_string(mItemStruct.HeaderStruct.ItemSize));

		// 	DeserializeData(rawData);
		// }

		/* Data 에 대해서만 역직렬화 하는 것 */
		void Deserialize(const std::vector<uint8_t>& rawData) override
		{
			mItemStruct.Data = rawData;

			mJson = deserializeBytesToJson(rawData);
			logger.Debug("Deserialized JSON: " + mJson.dump(4));
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
				logger.Error("JSON parse error: " + std::string(e.what()));
				throw std::runtime_error("Failed to parse JSON data");
			}
		}

	private:
		static logger::Logger logger;

		nlohmann::json mJson;
	};

	logger::Logger JsonItem::logger("JsonItem");

}

#endif // JSON_ITEM_H
