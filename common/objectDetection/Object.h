#ifndef OBJECT_DETECTION_OBJECT_H
#define OBJECT_DETECTION_OBJECT_H

#include <string>

#include "common/objectDetection/IJsonable.h"

namespace objectDetection
{
	struct ObjectStruct
	{
		std::string className;
		int height;
		int width;
		int x;
		int y;
	};

	class Object : public objectDetection::IJsonable
	{
	public:
		void ToJson(nlohmann::json& OUT json) const override;
		void FromJson(const nlohmann::json& json) override;

		void FromObjectStruct(const ObjectStruct& objectStruct);
		ObjectStruct GetObjectStruct() const;

	private:
		ObjectStruct mObjectStruct;
	};
}

#endif // OBJECT_DETECTION_OBJECT_H