#ifndef BASE_ITEM_H
#define BASE_ITEM_H

#include "storage/manager/database/StorageFile.h"

namespace storage
{
	class BaseItem
	{
	public:
		virtual ~BaseItem() = default;

		std::vector<uint8_t> Serialize() const
		{
			std::vector<uint8_t> serializedData;
			
			// ItemHeaderStruct의 DataSize를 먼저 삽입
			uint32_t DataSize = mItemStruct.HeaderStruct.DataSize;
			serializedData.insert(serializedData.end(), reinterpret_cast<uint8_t*>(&DataSize), reinterpret_cast<uint8_t*>(&DataSize) + sizeof(DataSize));
			
			// 실제 데이터를 삽입
			serializedData.insert(serializedData.end(), mItemStruct.Data.begin(), mItemStruct.Data.end());
			
			return serializedData;
		}

		virtual void Deserialize(const std::vector<uint8_t>& rawData) = 0;

		size_t Size() const
		{
			return sizeof(ItemHeaderStruct) + mItemStruct.HeaderStruct.DataSize;
		}
	
	protected:
		ItemStruct mItemStruct;
	};
}

#endif // BASE_ITEM_H