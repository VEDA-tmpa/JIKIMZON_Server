#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>

#include "save.hpp"

int main() {
    Save* save = new Save(512);

    nlohmann::json jsonData1 =
    {
        {"id", 1},
        {"time", "2024-12-08T12:00:00Z"},
        {"object",
            {
                {
                    {"name", "Object1"},
                    {"width", 100},
                    {"height", 200}
                },
                {
                    {"name", "Object2"},
                    {"width", 150},
                    {"height", 250}
                },
            }
        }
    };

    nlohmann::json jsonData2 =
    {
        {"id", 2},
        {"time", "2024-12-08T12:00:00Z"},
        {"object",
            {
                {
                    {"name", "Object1"},
                    {"width", 100},
                    {"height", 200}
                },
                {
                    {"name", "Object2"},
                    {"width", 150},
                    {"height", 250}
                },
            }
        }
    };

    nlohmann::json jsonData3 =
    {
        {"id", 3},
        {"time", "2024-12-08T12:00:00Z"},
        {"object",
            {
                {
                    {"name", "Object1"},
                    {"width", 100},
                    {"height", 200}
                },
                {
                    {"name", "Object2"},
                    {"width", 150},
                    {"height", 250}
                },
            }
        }
    };

    std::string jsonString1 = jsonData1.dump();
    save->SaveJson(jsonString1);

    std::string jsonString2 = jsonData2.dump();
    save->SaveJson(jsonString2);

    std::string jsonString3 = jsonData3.dump();
    save->SaveJson(jsonString3);

    // std::string result = save->ReadJson(512);
    // std::cout << "Json\n" << result << std::endl;

    return 0;
}