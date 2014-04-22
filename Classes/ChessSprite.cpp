#include "ChessSprite.h"
#include "GameScene.h"

USING_NS_CC;

ChessSprite::ChessSprite(void)
{
    return;
}

ChessSprite::~ChessSprite(void)
{
    return;
}

ChessSprite* ChessSprite::create(const char *pszFileName)
{
    ChessSprite *pChess = new ChessSprite();
    pChess->initWithFile(pszFileName);
    pChess->autorelease();

    return pChess;
}

ChessSprite* ChessSprite::createWithTexture(CCTexture2D *pTexture, const CCRect& rect)
{
    ChessSprite *pChess = new ChessSprite();
    pChess->initWithTexture(pTexture, rect);
    pChess->autorelease();

    return pChess;
}

void ChessSprite::onEnter(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();

    return;
}

void ChessSprite::onExit(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();

    return;
}

bool ChessSprite::ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event)
{
    CCPoint point = convertTouchToNodeSpaceAR(touch);
    CCSize s = getTexture()->getContentSize();
    bool ret = CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height).containsPoint(point);

    if (true == ret)
    {
        CCActionInterval* actionTo = CCRotateTo::create(2, 45);
        CCActionInterval* actionBack = CCRotateTo::create(2, -45);
        CCAction* actionRep = CCRepeatForever::create((CCActionInterval*)CCSequence::create(actionTo, actionBack, NULL));
        this->runAction(actionRep);
        position = getPosition();
        CCLog("%d, %f", this->getZOrder(), this->getVertexZ());
        this->setZOrder(2000);
    }
    return ret;
}

void ChessSprite::ccTouchMoved(CCTouch* touch, CCEvent* event)
{
    setPosition(ccp(touch->getLocation().x, touch->getLocation().y));

    return;
}

void ChessSprite::ccTouchEnded(CCTouch* touch, CCEvent* event)
{
    this->stopAllActions();
    this->setZOrder(1);
    //将touch坐标初始化为屏幕坐标系
    CCPoint point = touch->getLocation();
    point.y = CCEGLView::sharedOpenGLView()->getFrameSize().height - point.y;

    //计算落点的坐标
    UINT uiX = (point.x - 30.0) / 100.0 + 1;
    UINT uiY = (point.y - 20.0) / 60.0 + 1;
    CCLog("%d %d", uiX, uiY);

    //判断落点合法性
    if ((0 == uiX) || (0 == uiY) || (9 < uiX) || (10 < uiY))
    {
        setPosition(position);
        return;
    }

    //检查落点的匹配区间
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

    //判断落子是否符合游戏规则
    if (false == GameScene::getGameScene()->checkChessMoveIsValid(uiChessId, uiX, uiY))
    {
        CCLog("chess moving is unfit for game, chess id :%d, move (%d, %d)", uiChessId, uiX, uiY);
        setPosition(position);
        return;
    }

    //可以落子
    toPoint.y = CCEGLView::sharedOpenGLView()->getFrameSize().height - toPoint.y;
    setPosition(toPoint);

    //由棋局进行局面处理
    GameScene::getGameScene()->moveChess(uiChessId, uiX, uiY);

    return;
}

void ChessSprite::setChessId(UINT uiId)
{
    uiChessId = uiId;
}