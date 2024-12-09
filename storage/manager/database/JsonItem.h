#ifndef JSON_ITEM_H
#define JSON_ITEM_H

#include <nlohmann/json.hpp>
#include <string>

#include "storage/manager/database/BaseItem.h"

namespace storage
{
	class JsonItem : public BaseItem 
	{
	public:
		JsonItem(const nlohmann::json& json) 
			: mJson(json)
		{
			mItemStruct.Data = serializeJsonToBytes(json);
			mItemStruct.HeaderStruct.ItemSize = mItemStruct.Data.size();
		}

		std::vector<uint8_t> Serialize() const override 
		{
			return mItemStruct.Data;
		}

		void Deserialize(const std::vector<uint8_t>& rawData) override 
		{
			mItemStruct.HeaderStruct.ItemSize = *reinterpret_cast<const uint32_t*>(rawData.data());
			mItemStruct.Data = rawData;

			mJson = deserializeBytesToJson(rawData);
		}

		size_t Size() const override 
		{
			return mItemStruct.Data.size();
		}

	private:
		std::vector<uint8_t> serializeJsonToBytes(const nlohmann::json& json) const
		{
			std::string jsonString = json.dump();
			return std::vector<uint8_t>(jsonString.begin(), jsonString.end());
		}

		nlohmann::json deserializeBytesToJson(const std::vector<uint8_t>& rawData) const
		{
			uint32_t itemSize = *reinterpret_cast<const uint32_t*>(rawData.data());

			std::vector<uint8_t> jsonData(rawData.begin() + sizeof(ItemHeaderStruct), rawData.end());

			std::string jsonString(jsonData.begin(), jsonData.end());
			return nlohmann::json::parse(jsonString);
		}

	private:
		nlohmann::json mJson;

		ItemStruct mItemStruct;
	};
}

#endif // JSON_ITEM_H
