#ifndef I_JSONABLE_H
#define I_JSONABLE_H

#define OUT

#include <nlohmann/json.hpp>

namespace objectDetection
{
	class IJsonable
	{
		virtual void ToJson(nlohmann::json& OUT json) const = 0;
		virtual void FromJson(const nlohmann::json& json) = 0;
	};
};

#endif // I_JSONABLE_H