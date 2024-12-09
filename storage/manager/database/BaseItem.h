#ifndef BASE_ITEM_H
#define BASE_ITEM_H

#include "storage/manager/database/StorageFile.h"

class BaseItem
{
public:
	virtual ~BaseItem() = default;

	virtual std::vector<uint8_t> Serialize() const = 0;
	virtual void Deserialize(const std::vector<uint8_t>& rawData) = 0;

	virtual size_t Size() const = 0;
};

#endif // BASE_ITEM_H