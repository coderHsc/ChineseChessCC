#include "ChessSprite.h"

USING_NS_CC;

Chess::Chess(void)
{
    return;
}

Chess::~Chess(void)
{
    return;
}

Chess* Chess::create(const char *pszFileName)
{
    Chess *pChess = new Chess();
    pChess->initWithFile(pszFileName);
    pChess->autorelease();

    return pChess;
}

Chess* Chess::createWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
    Chess *pChess = new Chess();
    pChess->initWithTexture(pTexture, rect);
    pChess->autorelease();

    return pChess;
}

void Chess::onEnter(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();

    return;
}

void Chess::onExit(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();

    return;
}

bool Chess::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    CCPoint point = convertTouchToNodeSpaceAR(touch);
    CCSize s = getTexture()->getContentSize();
    bool ret = CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height).containsPoint(point);

    if (true == ret)
    {
        CCActionInterval*  actionTo = CCRotateTo::create(2, 45);
        this->runAction(actionTo);

        position = getPosition();
    }
    return ret;
}

void Chess::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    setPosition(ccp(touch->getLocation().x, touch->getLocation().y));

    return;
}

void Chess::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    CCPoint point = touch->getLocation();
    point.y = CCEGLView::sharedOpenGLView()->getFrameSize().height - point.y;

    UINT uiX = (point.x - 30.0) / 100.0 + 1;
    UINT uiY = (point.y - 20.0) / 60.0 + 1;
    CCLog("%d %d", uiX, uiY);

    if ((0 == uiX) || (0 == uiY) || (9 < uiX) || (10 < uiY))
    {
        setPosition(position);
        return;
    }

    CCPoint toPoint;
    toPoint.x = (uiX - 1) * 100 + 80;
    toPoint.y = (uiY - 1) * 60 + 50;
    CCLog("%f %f", toPoint.x, toPoint.y);
    CCLog("%f %f", point.x, point.y);
    if (15.0 < toPoint.getDistance(point))
    {
        setPosition(position);
        return;
    }
    toPoint.y = CCEGLView::sharedOpenGLView()->getFrameSize().height - toPoint.y;
    setPosition(toPoint);

    return;
}
