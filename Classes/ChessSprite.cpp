#include "ChessSprite.h"
#include "GameScene.h"
#include "Config.h"

USING_NS_CC;

#define Z_TOP_CHESS 2000
#define Z_BOTTOM_CHESS 1

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

ChessSprite* ChessSprite::createWithTexture(Texture2D *pTexture, const Rect& rect)
{
    ChessSprite *pChess = new ChessSprite();
    pChess->initWithTexture(pTexture, rect);
    pChess->autorelease();

    return pChess;
}

void ChessSprite::onEnter(void)
{
    Sprite::onEnter();

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(ChessSprite::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(ChessSprite::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(ChessSprite::onTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return;
}

void ChessSprite::onExit(void)
{
    Sprite::onExit();

    return;
}

bool ChessSprite::onTouchBegan(Touch* touch, Event* event)
{
    Point point = this->convertTouchToNodeSpaceAR(touch);
    auto s = this->getContentSize();
    bool ret = Rect(-s.width / 2, -s.height / 2, s.width, s.height).containsPoint(point);

    if (true == ret)
    {
        float fLoop = Config::getChessJitter().fLoop;
        float fWidth = Config::getChessJitter().fWidth;
        float fAngle = Config::getChessJitter().fAngle;
        auto actionTo = RotateTo::create(fLoop, fAngle);
        auto actionBack = RotateTo::create(fLoop, -fAngle);
        auto actionRep = RepeatForever::create(Sequence::create(actionTo, actionBack, NULL));
        this->runAction(actionRep);

        auto actionMoveTo1 = MoveBy::create(fLoop, Point(fWidth, fWidth));
        auto actionMoveTo2 = MoveBy::create(fLoop, Point(fWidth, -fWidth));
        auto actionMoveTo3 = MoveBy::create(fLoop, Point(-fWidth, -fWidth));
        auto actionMoveTo4 = MoveBy::create(fLoop, Point(-fWidth, fWidth));
        auto actionRep2 = RepeatForever::create(Sequence::create(actionMoveTo1, actionMoveTo2, actionMoveTo3, actionMoveTo4, NULL));
        this->runAction(actionRep2);

        this->position = getPosition();
        log("chess z order %d, vertex.z %f", this->getLocalZOrder(), this->getPositionZ());
        this->setLocalZOrder(Z_TOP_CHESS);
    }
    return ret;
}

void ChessSprite::onTouchMoved(Touch* touch, Event* event)
{
    this->setPosition(touch->getLocation().x, touch->getLocation().y);

    return;
}

void ChessSprite::onTouchEnded(Touch* touch, Event* event)
{
    this->stopAllActions();
    this->setLocalZOrder(Z_BOTTOM_CHESS);

    //将touch坐标初始化为屏幕坐标系
    Point point = touch->getLocation();
    point.y = Director::getInstance()->getOpenGLView()->getFrameSize().height - point.y;

    //取尺寸配置
    Size& sizeBox = Config::getBoxSize();
    Size& sizeEdge = Config::getEdgeSize();

    //计算落点的坐标
    UINT uiX = (point.x - sizeEdge.width) / sizeBox.width + 1;
    UINT uiY = (point.y - sizeEdge.height) / sizeBox.height + 1;
    log("compute chess target %d %d", uiX, uiY);

    //判断落点合法性
    if ((0 == uiX) || (0 == uiY) || (9 < uiX) || (10 < uiY))
    {
        this->setPosition(this->position);
        return;
    }

    //检查落点的匹配区间
    Point toPoint;
    toPoint.x = (uiX - 1) * sizeBox.width + sizeBox.width * 0.5 + sizeEdge.width;
    toPoint.y = (uiY - 1) * sizeBox.height + sizeBox.height * 0.5 + sizeEdge.height;
    log("target point %f %f", toPoint.x, toPoint.y);
    log("touch point %f %f", point.x, point.y);
    if (15.0 < toPoint.getDistance(point))
    {
        this->setPosition(this->position);
        return;
    }

    //判断落子是否符合游戏规则
    if (false == GameScene::getGameScene()->checkChessMoveIsValid(uiChessId, uiX, uiY))
    {
        log("chess moving is unfit for game, chess id :%d, move (%d, %d)", uiChessId, uiX, uiY);
        this->setPosition(this->position);
        return;
    }

    //可以落子
    toPoint.y = Director::getInstance()->getOpenGLView()->getFrameSize().height - toPoint.y;
    float fOffset = Config::getChessJitter().fOffset;
    this->setPosition(toPoint.x + fOffset * CCRANDOM_MINUS1_1(), toPoint.y + fOffset * CCRANDOM_MINUS1_1());

    //由棋局进行局面处理
    GameScene::getGameScene()->moveChess(uiChessId, uiX, uiY);

    return;
}

void ChessSprite::setChessId(UINT uiId)
{
    this->uiChessId = uiId;
}