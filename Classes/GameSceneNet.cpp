#include "GameMenu.h"
#include "GameSceneNet.h"
#include "ChessSprite.h"

#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

char* apstrColor[3] = {
    "invalid",
    "Black",
    "Red"
};

GameSceneNet::GameSceneNet()
:uiNetId(0), bCanMove(false), pNetLabel(NULL), uiOpponentId(0), uiLocalColor(10000)
{
}

GameSceneNet::~GameSceneNet()
{
	unscheduleAllSelectors();
    if (0 != uiNetId)
    {
        sendLeaveGameToServer();
    }
}

bool GameSceneNet::init(void)
{
	GameScene::init();
    if (0 == this->uiNetId)
    {
        getNetIdFromServer();
    }
    pNetLabel = CCLabelTTF::create("getting net id", "Gill Sans Ultra Bold", 20);
    pInfoGround->addChild(pNetLabel);
    pNetLabel->setPosition(ccp(pInfoGround->getContentSize().width * 0.5, pInfoGround->getContentSize().height - 80));

    return true;
}

CCScene* GameSceneNet::scene(void)
{
    CCScene *scene = CCScene::create();

	GameSceneNet *layer = GameSceneNet::create();
	pGameScene = layer;

    scene->addChild(layer);

    return scene;
}

GameScene* GameSceneNet::getGameScene()
{
	return pGameScene;
}

bool GameSceneNet::checkChessMoveIsValid(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
    if ((false == this->bCanMove) || (uiLocalColor == this->getLastMoveColor()))
    {
        CCLog("Can't move chess");
        return false;
    }

	CCLog("GameSceneNet******chess move is valid");
	return GameScene::checkChessMoveIsValid(uiChessId, uiPosY, uiPosX);
}

void GameSceneNet::moveChessSelf(UINT uiChessId, UINT uiPosX, UINT uiPosY)
{
    CCPoint toPoint;
    toPoint.x = (uiPosX - 1) * 60 + 48;
    toPoint.y = (uiPosY - 1) * 60 + 50;
    toPoint.y = CCEGLView::sharedOpenGLView()->getFrameSize().height - toPoint.y;

    auto pChess = this->getChildByTag(CHESS_TAG_BASE + uiChessId);
    pChess->setPosition(toPoint);
}

void GameSceneNet::moveChess(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
	CCLog("GameSceneNet******move chess");
	sendMoveToServer(uiChessId, uiPosY, uiPosX);

    GameScene::moveChess(uiChessId, uiPosY, uiPosX);

    schedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack), 1.0f);
	return;
}

void GameSceneNet::sendMoveToServer(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://114.215.193.215:9090/chessMove");
	request->setRequestType(CCHttpRequest::kHttpPost);

	char aucBuf[256] = { 0 };
	sprintf(aucBuf, "uiNetId=%d&moveinfo=%d-%d-%d", uiNetId, uiChessId, uiPosY, uiPosX);
	request->setRequestData(aucBuf, strlen(aucBuf) + 1);

	request->setTag("POST chess move");
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameSceneNet::receiveMoveFromServerTimerBack(float dt)
{
	CCLog("timer %f", dt);
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://114.215.193.215:9090/chessMoveGet");
	request->setRequestType(CCHttpRequest::kHttpPost);
	request->setResponseCallback(this, httpresponse_selector(GameSceneNet::receiveMoveFromServer));
    char aucBuf[256] = { 0 };
    sprintf(aucBuf, "uiNetId=%d", uiNetId);
    request->setRequestData(aucBuf, strlen(aucBuf) + 1);
    request->setTag("GET chess move");
	CCHttpClient::getInstance()->send(request);
	request->release();

	unschedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack));
}

void GameSceneNet::receiveMoveFromServer(CCHttpClient* client, CCHttpResponse* response)
{
	if (!response->isSucceed())
	{
		CCLog("response failed in receiveMoveFromServer");
		CCLog("error buffer: %s", response->getErrorBuffer());
		return;
	}

	std::vector<char> *buffer = response->getResponseData();
	std::string strBuff(buffer->begin(), buffer->end());

    UINT uiLive = 0;
    UINT uiReadChess = 0;
    UINT uiReadPosX = 0;
    UINT uiReadPosY = 0;
    sscanf(strBuff.c_str(), "%d-%d-%d-%d", &uiLive, &uiReadChess, &uiReadPosX, &uiReadPosY);

    if (0 != uiReadChess)
    {
        GameSceneNet::moveChessSelf(uiReadChess, uiReadPosX, uiReadPosY);
        GameScene::moveChess(uiReadChess, uiReadPosX, uiReadPosY);
    }
    else
    {
        schedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack), 1.0f);
    }

    if (0 == uiLive)
    {
        bCanMove = false;
        this->setNetLabel("Opponent\nleave game", ccRED);
        unschedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack));
    }
}

void GameSceneNet::getNetIdFromServer(void)
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://114.215.193.215:9090/newIdGet");
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setResponseCallback(this, httpresponse_selector(GameSceneNet::receiveNetIdFromServer));
	request->setTag("GET net id");
	CCHttpClient::getInstance()->send(request);
    
    CCLog("request a net id from server");
}

void GameSceneNet::receiveNetIdFromServer(CCHttpClient* client, CCHttpResponse* response)
{
    //检查响应是否成功
	if (!response->isSucceed())
	{
		CCLog("response failed in receiveNetIdFromServer");
		CCLog("error buffer: %s", response->getErrorBuffer());
		return;
	}

    //读取响应
	std::vector<char> *buffer = response->getResponseData();
    std::string strBuff(buffer->begin(), buffer->end());

    UINT uiReadId = 0;
    sscanf(strBuff.c_str(), "%d", &uiReadId);

	if (0 == uiReadId)
	{
		CCLog("get net id %d error", uiReadId);
		exit(0);
	}

    //记录分配的NETID
	this->uiNetId = uiReadId;
	CCLog("get net id %d", uiReadId);
    
    //替换网络信息标签
    CCString* pStr = CCString::createWithFormat("get net id %d", uiReadId);
    this->setNetLabel("get net id", ccYELLOW);
    response->getHttpRequest()->release();

    schedule(schedule_selector(GameSceneNet::getOppenetIdFromServer), 1.0f);
}

void GameSceneNet::getOppenetIdFromServer(float dt)
{
    CCLog("request a net id from server");

    //请求分配对手
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://114.215.193.215:9090/findOpponent");
    request->setRequestType(CCHttpRequest::kHttpPost);
    request->setResponseCallback(this, httpresponse_selector(GameSceneNet::receiveOpponentIdFromServer));
    char aucBuf[256] = { 0 };
    sprintf(aucBuf, "uiNetId=%d", uiNetId);
    request->setRequestData(aucBuf, strlen(aucBuf) + 1);
    request->setTag("Find opponent");
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void GameSceneNet::receiveOpponentIdFromServer(CCHttpClient* client, CCHttpResponse* response)
{
    //检查响应是否成功
    if (!response->isSucceed())
    {
        CCLog("response failed in receiveNetIdFromServer");
        CCLog("error buffer: %s", response->getErrorBuffer());
        return;
    }

    //读取响应
    std::vector<char> *buffer = response->getResponseData();
    std::string strBuff(buffer->begin(), buffer->end());

    UINT uiReadId = 0;
    UINT uiReadColor = 0;
    sscanf(strBuff.c_str(), "opponent=%dcolor=%d", &uiReadId, &uiReadColor);

    if (0 == uiReadId)
    {
        CCLog("get opponent %d error", uiReadId);
        exit(0);
    }

    if (1 == uiReadId)
    {
        CCLog("have no opponent now");
    }
    else
    {
        this->uiOpponentId = uiReadId;
        this->uiLocalColor = uiReadColor;

        if (CHESSCOCLOR_RED == uiReadColor)
        {
            schedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack), 1.0f);
        }
        this->setLastMoveColor(CHESSCOCLOR_RED);

        unschedule(schedule_selector(GameSceneNet::getOppenetIdFromServer));

        //允许移动操作
        bCanMove = true;

        //替换网络信息标签
        CCString* pStr = CCString::createWithFormat("Local color %s", apstrColor[uiReadColor]);
        this->setNetLabel(pStr->getCString(), ccYELLOW);
    }
}

void GameSceneNet::sendLeaveGameToServer(void)
{
    CCLog("send leave game to server");

    //通知本地下线
    CCHttpRequest* request = new CCHttpRequest();
    request->setUrl("http://114.215.193.215:9090/leaveGame");
    request->setRequestType(CCHttpRequest::kHttpPost);
    char aucBuf[256] = { 0 };
    sprintf(aucBuf, "uiNetId=%d", uiNetId);
    request->setRequestData(aucBuf, strlen(aucBuf) + 1);
    request->setTag("Leave game");
    CCHttpClient::getInstance()->send(request);
    request->release();
}

void GameSceneNet::setGameWin(void)
{
    this->bCanMove = false;
    GameScene::setGameWin();
    unschedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack));
}

void GameSceneNet::setNetLabel(const char* pstr, const cocos2d::ccColor3B& color)
{
    pNetLabel->setString(pstr);
    pNetLabel->setColor(color);
}
