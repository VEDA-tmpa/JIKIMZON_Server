#include "common/objectDetection/Object.h"

namespace objectDetection
{
	void Object::ToJson(nlohmann::json& OUT json) const
	{
		json["className"] = mObjectStruct.className;
		json["height"] = mObjectStruct.height;
		json["width"] = mObjectStruct.width;
		json["x"] = mObjectStruct.x;
		json["y"] = mObjectStruct.y;
	}

	void Object::FromJson(const nlohmann::json& json)
	{
		mObjectStruct.className = json["className"].get<std::string>();
		mObjectStruct.height = json["height"].get<int>();
		mObjectStruct.width = json["width"].get<int>();
		mObjectStruct.x = json["x"].get<int>();
		mObjectStruct.y = json["y"].get<int>();
	}

	void Object::FromObjectStruct(const ObjectStruct& objectStruct)
	{
		mObjectStruct.className = objectStruct.className;
		mObjectStruct.height = objectStruct.height;
		mObjectStruct.width = objectStruct.width;
		mObjectStruct.x = objectStruct.x;
		mObjectStruct.y = objectStruct.y;
	}

	ObjectStruct Object::GetObjectStruct() const
	{
		return mObjectStruct;
	}
}