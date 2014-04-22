#ifndef __CHESS_H__
#define __CHESS_H__

#include "cocos2d.h"


class Chess : public cocos2d::CCSprite , public cocos2d::CCTargetedTouchDelegate
{
public:
    Chess();
    ~Chess();

    static Chess* create(const char *pszFileName);
    static Chess* createWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect);
    virtual void onEnter(void);
    virtual void onExit(void);

    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

private:
    cocos2d::CCPoint position;
    UINT uiChessId;
};

#endif // __CHESS_H__
