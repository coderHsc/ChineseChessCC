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
        auto* actionTo = RotateTo::create(0.02f, 10);
        auto* actionBack = RotateTo::create(0.02f, -10);
        auto* actionRep = RepeatForever::create(Sequence::create(actionTo, actionBack, NULL));
        this->runAction(actionRep);
        this->position = getPosition();
        log("chess z order %d, vertex.z %f", this->getZOrder(), this->getVertexZ());
        this->setZOrder(2000);
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
    this->setZOrder(1);
    //将touch坐标初始化为屏幕坐标系
    Point point = touch->getLocation();
    point.y = Director::getInstance()->getOpenGLView()->getFrameSize().height - point.y;

    //计算落点的坐标
    UINT uiX = (point.x - 18.0) / 60.0 + 1;
    UINT uiY = (point.y - 20.0) / 60.0 + 1;
    log("compute chess target %d %d", uiX, uiY);

    //判断落点合法性
    if ((0 == uiX) || (0 == uiY) || (9 < uiX) || (10 < uiY))
    {
        this->setPosition(this->position);
        return;
    }

    //检查落点的匹配区间
    Point toPoint;
    toPoint.x = (uiX - 1) * 60 + 48;
    toPoint.y = (uiY - 1) * 60 + 50;
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
    this->setPosition(toPoint);

    //由棋局进行局面处理
    GameScene::getGameScene()->moveChess(uiChessId, uiX, uiY);

    return;
}

void ChessSprite::setChessId(UINT uiId)
{
    this->uiChessId = uiId;
}