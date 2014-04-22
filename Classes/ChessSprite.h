#ifndef __CHESSSPRITE_H__
#define __CHESSSPRITE_H__

#include "cocos2d.h"


class ChessSprite : public cocos2d::CCSprite , public cocos2d::CCTargetedTouchDelegate
{
public:
    ChessSprite();
    ~ChessSprite();

    static ChessSprite* create(const char *pszFileName);
    static ChessSprite* createWithTexture(cocos2d::CCTexture2D *pTexture, const cocos2d::CCRect& rect);
    virtual void onEnter(void);
    virtual void onExit(void);

    virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
    virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

    void setChessId(UINT uiChessId);
    UINT uiChessId;

private:
    cocos2d::CCPoint position;
};

#endif // __CHESSSPRITE_H__
