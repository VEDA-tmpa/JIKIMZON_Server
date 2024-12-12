#ifndef ITEM_FACTORY_H
#define ITEM_FACTORY_H

#include <memory>
#include <nlohmann/json.hpp>
#include "common/frame/GOP.h"

#include "storage/manager/database/BaseItem.h"
#include "storage/manager/database/H264Item.h"
#include "storage/manager/database/JsonItem.h"

namespace storage 
{
	template <typename T>
	class ItemFactory 
	{
	public:
		virtual ~ItemFactory() = default;

		virtual std::unique_ptr<BaseItem> CreateItem(const T& data) const = 0;

		virtual T Deserialize(const std::vector<uint8_t>& item) const = 0;

		static std::unique_ptr<ItemFactory<T>> CreateFactory();
	};

	template <>
	class ItemFactory<nlohmann::json>
	{
	public:
		std::unique_ptr<BaseItem> CreateItem(const nlohmann::json& data)
		{
			return std::make_unique<JsonItem>(data);
		}

		nlohmann::json Deserialize(const std::vector<uint8_t>& binaryData) const
		{
			JsonItem outJsonItem;
			outJsonItem.Deserialize(binaryData);

			return outJsonItem.GetData();
		}

		static std::unique_ptr<ItemFactory<nlohmann::json>> CreateFactory() 
		{
			return std::make_unique<ItemFactory<nlohmann::json>>();
		}
	};

	template <>
	class ItemFactory<frame::H264::GOP>
	{
	public:
		std::unique_ptr<BaseItem> CreateItem(const frame::H264::GOP& data)
		{
			return std::make_unique<H264Item>(data);
		}

		frame::H264::GOP Deserialize(const std::vector<uint8_t>& binaryData) const
		{
			H264Item outH264Item;
			outH264Item.Deserialize(binaryData); 

			return outH264Item.GetData();
		}

		static std::unique_ptr<ItemFactory<frame::H264::GOP>> CreateFactory() 
		{
			return std::make_unique<ItemFactory<frame::H264::GOP>>();
		}
	};
}

#endif // ITEM_FACTORY_H