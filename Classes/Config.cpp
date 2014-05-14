#include "Config.h"

USING_NS_CC;

std::map<std::string, std::string> Config::mFiles;
std::map<std::string, std::string> Config::mStrings;

Config::Config()
{
}

Config::~Config()
{
}

bool Config::init()
{
    ValueVector txt_vec = FileUtils::getInstance()->getValueVectorFromFile("filenames.xml");
    for (auto& value : txt_vec)
    {
        auto text_map = value.asValueMap();
        for (auto& text_map_pair : text_map)
        {
            Config::mFiles[text_map_pair.first] = text_map_pair.second.asString();
            log("read config : key %s, string %s.", text_map_pair.first.c_str(), text_map_pair.second.asString().c_str());
        }
    }

    txt_vec = FileUtils::getInstance()->getValueVectorFromFile("strings.xml");
    for (auto& value : txt_vec)
    {
        auto text_map = value.asValueMap();
        for (auto& text_map_pair : text_map)
        {
            Config::mStrings[text_map_pair.first] = text_map_pair.second.asString();
            log("read config : key %s, string %s.", text_map_pair.first.c_str(), text_map_pair.second.asString().c_str());
        }
    }

    return true;
}

std::string Config::getFilename(std::string strFileIndex)
{
	return Config::mFiles[strFileIndex];
}

std::string Config::getString(std::string strStringIndex)
{
	return Config::mStrings[strStringIndex];
}
