#ifndef __CHESSSPRITE_H__
#define __CHESSSPRITE_H__

#include "cocos2d.h"


class ChessSprite : public cocos2d::Sprite
{
public:
    ChessSprite();
    ~ChessSprite();

    static ChessSprite* create(const char *pszFileName);
    static ChessSprite* createWithTexture(cocos2d::Texture2D *pTexture, const cocos2d::Rect& rect);
    virtual void onEnter(void);
    virtual void onExit(void);

    virtual bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    virtual void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    void setChessId(UINT uiChessId);

private:
    UINT uiChessId;
    cocos2d::Point position;
};

#endif // __CHESSSPRITE_H__
