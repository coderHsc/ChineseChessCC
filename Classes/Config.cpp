#include "Config.h"

USING_NS_CC;

std::map<std::string, std::string> Config::mFiles;
std::map<std::string, std::string> Config::mStrings;
std::map<std::string, int> Config::_mFonts;
std::map<std::string, float> Config::_mTimer;
std::map<std::string, float> Config::_mPos;
Size Config::_sizeChess;
Size Config::_sizeBox;
Size Config::_sizeEdge;
CHESS_JITTER_ARRT Config::_stChessJitter;

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

    txt_vec = FileUtils::getInstance()->getValueVectorFromFile("size.xml");
    for (auto& value : txt_vec)
    {
        auto text_map = value.asValueMap();
        for (auto& text_map_pair : text_map)
        {
            log("read config : key %s, value %f.", text_map_pair.first.c_str(), text_map_pair.second.asFloat());
            Config::procOneSizeConfig(text_map_pair.first, text_map_pair.second.asFloat());
        }
    }

    return true;
}

void Config::procOneSizeConfig(std::string strIndex, float fValue)
{
    if (0 == strIndex.compare("chess_width"))
    {
        Config::_sizeChess.width = fValue;
    }
    else if (0 == strIndex.compare("chess_height"))
    {
        Config::_sizeChess.height = fValue;
    }
    else if (0 == strIndex.compare("box_width"))
    {
        Config::_sizeBox.width = fValue;
    }
    else if (0 == strIndex.compare("box_height"))
    {
        Config::_sizeBox.height = fValue;
    }
    else if (0 == strIndex.compare("left_edge"))
    {
        Config::_sizeEdge.width = fValue;
    }
    else if (0 == strIndex.compare("top_edge"))
    {
        Config::_sizeEdge.height = fValue;
    }
    else if (0 == strIndex.compare("chess_jitter_width"))
    {
        Config::_stChessJitter.fWidth = fValue;
    }
    else if (0 == strIndex.compare("chess_jitter_angle"))
    {
        Config::_stChessJitter.fAngle = fValue;
    }
    else if (0 == strIndex.compare("chess_jitter_loop"))
    {
        Config::_stChessJitter.fLoop = fValue;
    }
    else if (0 == strIndex.compare("chess_jitter_offset"))
    {
        Config::_stChessJitter.fOffset = fValue;
    }
    else if (std::string::npos != strIndex.find("font"))
    {
    	Config::_mFonts[strIndex] = int(fValue);
    }
    else if (std::string::npos != strIndex.find("timer"))
    {
    	Config::_mTimer[strIndex] = fValue;
    }
    else if (std::string::npos != strIndex.find("pos"))
    {
    	Config::_mPos[strIndex] = fValue;
    }
    else
    {
        assert(0);
    }

    return;
}

std::string Config::getFilename(std::string strFileIndex)
{
	return Config::mFiles[strFileIndex];
}

std::string Config::getString(std::string strStringIndex)
{
	return Config::mStrings[strStringIndex];
}

int Config::getFontsSize(std::string strFontsSizeIndex)
{
	return Config::_mFonts[strFontsSizeIndex];
}

float Config::getTimerSize(std::string strTimerSizeIndex)
{
	return Config::_mTimer[strTimerSizeIndex];
}

float Config::getPos(std::string strPosIndex)
{
	return Config::_mPos[strPosIndex];
}

Size& Config::getChessSize()
{
	return Config::_sizeChess;
}

Size& Config::getBoxSize()
{
	return Config::_sizeBox;
}

Size& Config::getEdgeSize()
{
	return Config::_sizeEdge;
}

CHESS_JITTER_ARRT& Config::getChessJitter()
{
	return Config::_stChessJitter;
}
