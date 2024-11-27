#include <vector>
#include <string>

#include "common/objectDetection/IJsonable.h"
#include "common/objectDetection/Object.h"

namespace objectDetection
{
	/*
	{
		"frameId": 32,
		"object": [
			{
				"className": "cardboard",
				"height": 618,
				"width": 1258,
				"x": -29,
				"y": 0
			}
		],
		"timestamp": "20241126_114616.057"
	}
	*/
	struct FrameStruct
	{
		int frameId;
		std::string timestamp;
		std::vector<ObjectStruct> objects;
	};

	class Frame : public IJsonable
	{
	public:
		void ToJson(nlohmann::json& OUT json) const override;
		void FromJson(const nlohmann::json& json) override;

		void FromFrameStruct(FrameStruct frameStruct);

		const int& GetId() const;
		const std::string& GetTimestamp() const;
		const std::vector<ObjectStruct>& GetObjectStructs() const;
		
		std::vector<Object> GetObjects() const;
		static std::vector<Object> GetObjects(FrameStruct frameStruct);

	private:
		FrameStruct mFrameStruct;
	};
}
