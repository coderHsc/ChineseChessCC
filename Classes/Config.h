#ifndef __C_C_CONFIG_H__
#define __C_C_CONFIG_H__

#include "cocos2d.h"

#define CF_F(str) Config::getFilename(str)
#define CF_S(str) Config::getString(str)
#define CF_FT(str) Config::getFontsSize(str)
#define CF_T(str) Config::getTimerSize(str)
#define CF_P(str) Config::getPos(str)
#define CF_N(str) Config::getNetPath(str)

struct CHESS_JITTER_ARRT {
	float	fWidth;
	float	fAngle;
	float	fLoop;
	float	fOffset;
    float   fScaleExtent;
    float   fScaleTo;
    float   fScaleBack;
};

class Config
{
public:
	Config();
	~Config();

	static bool init();

	static std::string getFilename(std::string strFileIndex);
    static std::string getString(std::string strStringIndex);
    static int getFontsSize(std::string strFontsSizeIndex);
    static float getTimerSize(std::string strTimerSizeIndex);
    static float getPos(std::string strPosIndex);
    static std::string getNetPath(std::string strNetIndex);
    static cocos2d::Size& getChessSize();
    static cocos2d::Size& getBoxSize();
    static cocos2d::Size& getEdgeSize();
    static CHESS_JITTER_ARRT& getChessJitter();

private:
	static void procOneSizeConfig(std::string strIndex, float fValue);

    static std::map<std::string, std::string> mFiles;
    static std::map<std::string, std::string> mStrings;
    static std::map<std::string, int> _mFonts;
    static std::map<std::string, float> _mTimer;
    static std::map<std::string, float> _mPos;
    static std::map<std::string, std::string> _mNetPath;
    static cocos2d::Size _sizeChess;
    static cocos2d::Size _sizeBox;
    static cocos2d::Size _sizeEdge;
    static CHESS_JITTER_ARRT _stChessJitter;
};
#endif // __C_C_CONFIG_H__
