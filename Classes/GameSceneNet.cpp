#include "GameMenu.h"
#include "GameSceneNet.h"
#include "ChessSprite.h"

USING_NS_CC;
using namespace cocos2d::network;

char* apstrColor[3] = {
    (char*)"invalid",
    (char*)"Black",
    (char*)"Red"
};

std::string strUserName;
std::string strPasswd;

GameSceneNet::GameSceneNet()
:uiNetId(0), bCanMove(false), pNetLabel(NULL), uiOpponentId(0), uiLocalColor(10000)
{
}

GameSceneNet::~GameSceneNet()
{
	this->unscheduleAllSelectors();
    if (0 != this->uiNetId)
    {
        this->sendLeaveGameToServer();
    }
}

bool GameSceneNet::init(void)
{
	GameScene::init();

    this->pNetLabel = Label::createWithTTF("getting net id", "fonts/GILSANUB.ttf", 20);
    this->pInfoGround->addChild(this->pNetLabel);
    this->pNetLabel->setPosition(this->pInfoGround->getContentSize().width * 0.5, this->pInfoGround->getContentSize().height - 80);

    this->strServerHost = std::string("http://192.168.1.176:9090/");
    if (0 == this->uiNetId)
    {
        this->getNetIdFromServer();
    }

    return true;
}

Scene* GameSceneNet::scene(void)
{
    Scene *scene = Scene::create();

	GameSceneNet *layer = GameSceneNet::create();
	pGameScene = layer;

    scene->addChild(layer);

    return scene;
}

GameScene* GameSceneNet::getGameScene()
{
	return pGameScene;
}

void GameSceneNet::setLoginInfo(std::string strUser, std::string strPwd)
{
    strUserName = strUser;
    strPasswd = strPwd;
}

bool GameSceneNet::checkChessMoveIsValid(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
    if ((false == this->bCanMove) || (this->uiLocalColor == this->getLastMoveColor()))
    {
        log("Can't move chess");
        return false;
    }

	log("GameSceneNet******chess move is valid");
	return GameScene::checkChessMoveIsValid(uiChessId, uiPosY, uiPosX);
}

void GameSceneNet::moveChessSelf(UINT uiChessId, UINT uiPosX, UINT uiPosY)
{
    Point toPoint;
    toPoint.x = (uiPosX - 1) * 60 + 48;
    toPoint.y = (uiPosY - 1) * 60 + 50;
    toPoint.y = Director::getInstance()->getOpenGLView()->getFrameSize().height - toPoint.y;

    auto pChess = this->getChildByTag(CHESS_TAG_BASE + uiChessId);
    pChess->setPosition(toPoint);
}

void GameSceneNet::moveChess(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
	log("GameSceneNet******move chess");
	this->sendMoveToServer(uiChessId, uiPosY, uiPosX);

    GameScene::moveChess(uiChessId, uiPosY, uiPosX);

    this->schedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack), 1.0f);
	return;
}

void GameSceneNet::sendMoveToServer(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
	HttpRequest* request = new HttpRequest();
    std::string strHost = this->strServerHost + std::string("chessMove");
    request->setUrl(strHost.c_str());
    request->setRequestType(HttpRequest::Type::POST);

	char aucBuf[256] = { 0 };
	sprintf(aucBuf, "uiNetId=%d&moveinfo=%d-%d-%d", this->uiNetId, uiChessId, uiPosY, uiPosX);
	request->setRequestData(aucBuf, strlen(aucBuf) + 1);

	request->setTag("POST chess move");
	HttpClient::getInstance()->send(request);
	request->release();
}

void GameSceneNet::receiveMoveFromServerTimerBack(float dt)
{
	log("timer %f", dt);
	HttpRequest* request = new HttpRequest();
    std::string strHost = this->strServerHost + std::string("chessMoveGet");
    request->setUrl(strHost.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(GameSceneNet::receiveMoveFromServer));
    char aucBuf[256] = { 0 };
    sprintf(aucBuf, "uiNetId=%d", this->uiNetId);
    request->setRequestData(aucBuf, strlen(aucBuf) + 1);
    request->setTag("GET chess move");
	HttpClient::getInstance()->send(request);
	request->release();

	this->unschedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack));
}

void GameSceneNet::receiveMoveFromServer(HttpClient* client, HttpResponse* response)
{
	if (!response->isSucceed())
	{
		log("response failed in receiveMoveFromServer");
		log("error buffer: %s", response->getErrorBuffer());
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
        this->schedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack), 1.0f);
    }

    if (0 == uiLive)
    {
        this->bCanMove = false;
        this->setNetLabel("Opponent\nleave game", Color3B::RED);
        this->unschedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack));
    }
}

void GameSceneNet::getNetIdFromServer(void)
{
    if ((0 == strUserName.length()) || (0 == strPasswd.length()))
    {
        this->setNetLabel("error login info", Color3B::RED);
        return;
    }

	HttpRequest* request = new HttpRequest();

    std::string strHost = this->strServerHost + std::string("newIdGet");
    request->setUrl(strHost.c_str());
	request->setRequestType(HttpRequest::Type::POST);
	request->setResponseCallback(this, httpresponse_selector(GameSceneNet::receiveNetIdFromServer));

    char aucBuf[256] = { 0 };
    sprintf(aucBuf, "user=%s&passwd=%s", strUserName.c_str(), strPasswd.c_str());
    request->setRequestData(aucBuf, strlen(aucBuf) + 1);

    request->setTag("GET net id");
	HttpClient::getInstance()->send(request);

    log("request a net id from server");
}

void GameSceneNet::receiveNetIdFromServer(HttpClient* client, HttpResponse* response)
{
    //检查响应是否成功
	if (!response->isSucceed())
	{
		log("response failed in receiveNetIdFromServer");
		log("error buffer: %s", response->getErrorBuffer());
		return;
	}

    //读取响应
	std::vector<char> *buffer = response->getResponseData();
    std::string strBuff(buffer->begin(), buffer->end());

    UINT uiReadId = 0;
    sscanf(strBuff.c_str(), "%d", &uiReadId);

	if (0 == uiReadId)
	{
		log("get net id %d error", uiReadId);
		exit(0);
	}

    //记录分配的NETID
	this->uiNetId = uiReadId;
	log("get net id %d", uiReadId);

    //替换网络信息标签
    auto pStr = String::createWithFormat("get net id %d", uiReadId);
    this->setNetLabel(pStr->getCString(), Color3B::YELLOW);
    response->getHttpRequest()->release();

    this->schedule(schedule_selector(GameSceneNet::getOppenetIdFromServer), 1.0f);
}

void GameSceneNet::getOppenetIdFromServer(float dt)
{
    log("request a net id from server");

    //请求分配对手
    HttpRequest* request = new HttpRequest();
    std::string strHost = this->strServerHost + std::string("findOpponent");
    request->setUrl(strHost.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(GameSceneNet::receiveOpponentIdFromServer));
    char aucBuf[256] = { 0 };
    sprintf(aucBuf, "uiNetId=%d", this->uiNetId);
    request->setRequestData(aucBuf, strlen(aucBuf) + 1);
    request->setTag("Find opponent");
    HttpClient::getInstance()->send(request);
    request->release();
}

void GameSceneNet::receiveOpponentIdFromServer(HttpClient* client, HttpResponse* response)
{
    //检查响应是否成功
    if (!response->isSucceed())
    {
        log("response failed in receiveNetIdFromServer");
        log("error buffer: %s", response->getErrorBuffer());
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
        log("get opponent %d error", uiReadId);
        this->menuCloseGame(this);
    }

    if (1 == uiReadId)
    {
        log("have no opponent now");
    }
    else
    {
        this->unschedule(schedule_selector(GameSceneNet::getOppenetIdFromServer));

        this->uiOpponentId = uiReadId;
        this->uiLocalColor = uiReadColor;

        if (CHESSCOCLOR_RED == uiReadColor)
        {
            this->schedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack), 1.0f);
        }
        this->setLastMoveColor(CHESSCOCLOR_RED);

        //允许移动操作
        this->bCanMove = true;

        //替换网络信息标签
        auto pStr = String::createWithFormat("Local color %s", apstrColor[uiReadColor]);
        this->setNetLabel(pStr->getCString(), Color3B::YELLOW);
    }
}

void GameSceneNet::sendLeaveGameToServer(void)
{
    log("send leave game to server");

    //通知本地下线
    HttpRequest* request = new HttpRequest();
    std::string strHost = this->strServerHost + std::string("leaveGame");
    request->setUrl(strHost.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    char aucBuf[256] = { 0 };
    sprintf(aucBuf, "uiNetId=%d", this->uiNetId);
    request->setRequestData(aucBuf, strlen(aucBuf) + 1);
    request->setTag("Leave game");
    HttpClient::getInstance()->send(request);
    request->release();
}

void GameSceneNet::setGameWin(void)
{
    this->bCanMove = false;
    GameScene::setGameWin();
    this->unschedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack));
}

void GameSceneNet::setNetLabel(const char* pstr, const cocos2d::Color3B& color)
{
    this->pNetLabel->setString(pstr);
    this->pNetLabel->setColor(color);
}
