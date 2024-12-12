#ifndef ITEM_FACTORY_H
#define ITEM_FACTORY_H

template <typename T>
class ItemFactory
{
public:
	template <typename T>
	std::unique_ptr<ItemFactory<T>> CreateDefaultFactory();

	template <>
	std::unique_ptr<ItemFactory<nlohmann::json>> CreateDefaultFactory()
	{
		return std::make_unique<JsonItemFactory>();
	}

	template <>
	std::unique_ptr<ItemFactory<frame::H264::GOP>> CreateDefaultFactory()
	{
		return std::make_unique<H264ItemFactory>();
	}
	
    virtual ~ItemFactory() = default;

    virtual BaseItem CreateItem(const T& data) const = 0;
};

#endif // ITEM_FACTORY_H