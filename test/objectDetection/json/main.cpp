#include <iostream>
#include <nlohmann/json.hpp>

#include "common/objectDetection/Frame.h"

int main() 
{
	objectDetection::FrameStruct frameStruct = {
		.frameId = 32,
		.timestamp = "20241126_114616.057",
		.objects = {
			{
				.className = "cardboard",
				.height = 618,
				.width = 1258,
				.x = -29,
				.y = 0
			}
		}
	};
	objectDetection::Frame frame;
	frame.FromFrameStruct(frameStruct);


	nlohmann::json outFrameJson;
	frame.ToJson(outFrameJson);


	// JSON 출력
	std::cout << outFrameJson.dump(4) << std::endl;

	return 0;
}
