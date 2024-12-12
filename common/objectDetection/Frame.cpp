#include "common/objectDetection/Frame.h"

namespace objectDetection
{
	void Frame::ToJson(nlohmann::json& OUT json) const
	{
		json["frameId"] = mFrameStruct.frameId;
		json["timestamp"] = mFrameStruct.timestamp;
		
		json["objects"] = nlohmann::json::array();
		for (const auto& object : GetObjects(mFrameStruct))
		{
			nlohmann::json outObjectJson;
			object.ToJson(outObjectJson);
			json["objects"].push_back(outObjectJson);
		}
	}

	void Frame::FromJson(const nlohmann::json& json)
	{
		mFrameStruct.frameId = json["frameId"].get<int>();
		mFrameStruct.timestamp = json["timestamp"].get<std::string>();
		
		for (const auto& objectJson : json["objects"])
		{
			Object object;
			object.FromJson(objectJson);

			mFrameStruct.objects.push_back(object.GetObjectStruct());
		}
	}	

	void Frame::FromFrameStruct(FrameStruct frameStruct)
	{
		mFrameStruct.frameId = frameStruct.frameId;
		mFrameStruct.timestamp = frameStruct.timestamp;
		mFrameStruct.objects = frameStruct.objects;
	}

	const int& Frame::GetId() const 
	{ 
		return mFrameStruct.frameId; 
	}

	const std::string& Frame::GetTimestamp() const 
	{ 
		return mFrameStruct.timestamp; 
	}

	const std::vector<ObjectStruct>& Frame::GetObjectStructs() const 
	{
		return mFrameStruct.objects; 
	}

	std::vector<Object> Frame::GetObjects() const
	{
		std::vector<Object> objects;
		for (const auto& objectStruct : mFrameStruct.objects)
		{
			Object object;
			object.FromObjectStruct(objectStruct);
			objects.push_back(object);
		}
		return objects;
	}

	std::vector<Object> Frame::GetObjects(FrameStruct frameStruct)
	{
		std::vector<Object> objects;
		for (const auto& objectStruct : frameStruct.objects)
		{
			Object object;
			object.FromObjectStruct(objectStruct);
			objects.push_back(object);
		}
		return objects;
	}
}