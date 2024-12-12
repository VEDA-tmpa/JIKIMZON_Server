#ifndef H264_ITEM_FACTORY_H
#define H264_ITEM_FACTORY_H

#include "ItemFactory.h"
#include "H264Item.h"

namespace storage
{
	class H264ItemFactory : public ItemFactory<frame::H264::GOP>
	{
	public:
		BaseItem CreateItem(const frame::H264::GOP& data) const override
		{
			return H264Item item(data);
		}
	};
}

#endif // H264_ITEM_FACTORY_H
