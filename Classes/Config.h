#ifndef __C_C_CONFIG_H__
#define __C_C_CONFIG_H__

#include "cocos2d.h"

#define CF_F(str) Config::getFilename(str)
#define CF_S(str) Config::getString(str)

class Config
{
public:
	Config();
	~Config();

	static bool init();

	static std::string getFilename(std::string uiFileIndex);
    static std::string getString(std::string uiStringIndex);

private:
    static std::map<std::string, std::string> mFiles;
    static std::map<std::string, std::string> mStrings;
};
#endif // __C_C_CONFIG_H__
