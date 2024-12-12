#ifndef JSON_ITEM_FACTORY_H
#define JSON_ITEM_FACTORY_H

#include "ItemFactory.h"
#include "JsonItem.h"

namespace storage
{
	class JsonItemFactory : public ItemFactory<nlohmann::json>
	{
	public:
		BaseItem CreateItem(const nlohmann::json& data) const override
		{
			return JsonItem item(data);
		}
	};
}

#endif // JSON_ITEM_FACTORY_H
