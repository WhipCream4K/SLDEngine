#include "JSONParser.h"

#include "nlohmann/json.hpp"

#include <fstream>

using nlohmann::json;

class SLD::JSONParser::ImplJSONParser
{
public:
	void Read(const std::string& filePath);
private:
	json m_pJSONObject;
};

void SLD::JSONParser::ImplJSONParser::Read(const std::string& filePath)
{
	std::ifstream loadFile{ filePath };
	json obj{};

	loadFile >> obj;
}

SLD::JSONParser::JSONParser()
	: m_pImplJSONParser(std::make_unique<ImplJSONParser>())
{
}

SLD::JSONParser::~JSONParser() = default;
