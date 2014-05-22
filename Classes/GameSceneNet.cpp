#include "GameMenu.h"
#include "GameSceneNet.h"
#include "ChessSprite.h"
#include "Config.h"

USING_NS_CC;
using namespace cocos2d::network;

UINT g_uiNetId = 0;

GameSceneNet::GameSceneNet()
:uiNetId(0), bCanMove(false), pNetLabel(NULL), strOppoName(""), uiLocalColor(10000)
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

    this->setTurnLabel(CF_S("info_black_turn").c_str(), Color3B::BLACK);
    this->pNetLabel = Label::createWithTTF(CF_S("netinfo_loading"), Config::getFilename("fonts_cn"), CF_FT("font_info"));
    this->pInfoGround->addChild(this->pNetLabel);
    this->pNetLabel->setPosition(this->pInfoGround->getContentSize().width * 0.5, this->pInfoGround->getContentSize().height * CF_P("pos_game_netinfo"));

    this->strServerHost = CF_N("host");
    this->uiNetId = g_uiNetId;
    if (0 != this->uiNetId)
    {
        this->schedule(schedule_selector(GameSceneNet::getOppenetIdFromServer), CF_T("timer_netgame_normal"));
        this->setNetLabel(CF_S("netinfo_wait_oppo").c_str(), Color3B::BLUE);
    }
    else
    {
        this->setNetLabel(CF_S("netinfo_no_login").c_str(), Color3B::RED);
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

void GameSceneNet::setLoginInfo(UINT uiNetId)
{
    g_uiNetId = uiNetId;
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
    Size& sizeBox = Config::getBoxSize();
    Size& sizeEdge = Config::getEdgeSize();

    Point toPoint;
    toPoint.x = (uiPosX - 1) * sizeBox.width + sizeBox.width * 0.5 + sizeEdge.width;
    toPoint.y = (uiPosY - 1) * sizeBox.height + sizeBox.height * 0.5 + sizeEdge.height;
    toPoint.y = Director::getInstance()->getOpenGLView()->getFrameSize().height - toPoint.y;

    auto pChess = this->getChildByTag(CHESS_TAG_BASE + uiChessId);
    pChess->setPosition(toPoint);
}

bool GameSceneNet::moveChess(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
	log("GameSceneNet******move chess");
	this->sendMoveToServer(uiChessId, uiPosY, uiPosX);

    if (true == GameScene::moveChess(uiChessId, uiPosY, uiPosX))
    {
        return true;
    }
    else
    {
        this->schedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack), CF_T("timer_netgame_normal"));
        return false;
    }

}

void GameSceneNet::sendMoveToServer(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
	HttpRequest* request = new HttpRequest();
    std::string strHost = this->strServerHost + CF_N("path_move_send");
    request->setUrl(strHost.c_str());
    request->setRequestType(HttpRequest::Type::POST);

	char aucBuf[256] = { 0 };
	sprintf(aucBuf, "uid=%d&chess=%d&posX=%d&posY=%d&ignore=0", this->uiNetId, uiChessId, uiPosY, uiPosX);
	request->setRequestData(aucBuf, strlen(aucBuf) + 1);

	request->setTag("POST chess move");
	HttpClient::getInstance()->send(request);
	request->release();
}

void GameSceneNet::receiveMoveFromServerTimerBack(float dt)
{
	log("timer %f", dt);
	HttpRequest* request = new HttpRequest();
    std::string strHost = this->strServerHost + CF_N("path_move_get");
    request->setUrl(strHost.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(GameSceneNet::receiveMoveFromServer));
    char aucBuf[256] = { 0 };
    sprintf(aucBuf, "uid=%d&ignore=0", this->uiNetId);
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
    log("receive opponent move %s", strBuff.c_str());

    char aucBuf[256] = { 0 };
    sscanf(strBuff.c_str(), "ret=%s ", aucBuf);
    std::string strRet(aucBuf);

    if (0 == strRet.compare("nonew"))
    {
        this->schedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack), CF_T("timer_netgame_short"));
    }
    else if (0 == strRet.compare("oppoleave"))
    {
        this->bCanMove = false;
        this->setNetLabel(CF_S("netinfo_oppo_leave").c_str(), Color3B::RED);
        this->unschedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack));
    }
    else if (0 == strRet.compare("success"))
    {
        UINT uiReadChess = 0;
        UINT uiReadPosX = 0;
        UINT uiReadPosY = 0;
        sscanf(strBuff.c_str(), "ret=success chess=%d posx=%d posy=%d", &uiReadChess, &uiReadPosX, &uiReadPosY);
        GameSceneNet::moveChessSelf(uiReadChess, uiReadPosX, uiReadPosY);
        GameScene::moveChess(uiReadChess, uiReadPosX, uiReadPosY);
    }
}

void GameSceneNet::getOppenetIdFromServer(float dt)
{
    log("request a net id from server");

    //请求分配对手
    HttpRequest* request = new HttpRequest();
    std::string strHost = this->strServerHost + CF_N("path_match_oppo");
    request->setUrl(strHost.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(GameSceneNet::receiveOpponentIdFromServer));
    char aucBuf[256] = { 0 };
    sprintf(aucBuf, "uid=%d&ignore=0", this->uiNetId);
    request->setRequestData(aucBuf, strlen(aucBuf) + 1);
    request->setTag("Find opponent");
    HttpClient::getInstance()->send(request);
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
    log("receive find game %s", strBuff.c_str());

    UINT uiGame = 0;
    UINT uiBlack = 0;
    char aucRed[32] = { 0 };
    char aucBlack[32] = { 0 };
    sscanf(strBuff.c_str(), "game=%d uib=%d black=%s red=%s", &uiGame, &uiBlack, &aucBlack, &aucRed);

    if (0 == uiGame)
    {
        log("have no opponent now");
    }
    else
    {
        this->unschedule(schedule_selector(GameSceneNet::getOppenetIdFromServer));

        if(this->uiNetId == uiBlack)
        {
            this->strOppoName = std::string(aucRed);
            this->uiLocalColor = CHESSCOCLOR_BLACK;
        }
        else
        {
            this->strOppoName = std::string(aucBlack);
            this->uiLocalColor = CHESSCOCLOR_RED;
        }

        if (CHESSCOCLOR_RED == this->uiLocalColor)
        {
            this->schedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack), CF_T("timer_netgame_short"));
        }
        this->setLastMoveColor(CHESSCOCLOR_RED);

        //允许移动操作
        this->bCanMove = true;

        //替换网络信息标签
        if (CHESSCOCLOR_RED == this->uiLocalColor)
        {
            this->setNetLabel(CF_S("netinfo_local_red").c_str(), Color3B::YELLOW);
        }
        else
        {
            this->setNetLabel(CF_S("netinfo_local_black").c_str(), Color3B::YELLOW);
        }

        log("opponent name %s", this->strOppoName.c_str());
        auto pStr = String::createWithFormat("%s: %s", CF_S("netinfo_oppo_name").c_str(), this->strOppoName.c_str());
        auto pLabelOp = Label::createWithTTF(pStr->getCString(), Config::getFilename("fonts_cn"), CF_FT("font_info"));
        this->pInfoGround->addChild(pLabelOp);
        pLabelOp->setPosition(this->pInfoGround->getContentSize().width * 0.5, this->pInfoGround->getContentSize().height * CF_P("pos_game_opponame"));
        pLabelOp->setColor(Color3B::BLUE);
    }
    response->getHttpRequest()->release();
}

void GameSceneNet::sendLeaveGameToServer(void)
{
    log("send leave game to server");

    //通知本地下线
    HttpRequest* request = new HttpRequest();
    std::string strHost = this->strServerHost + CF_N("path_leave_game");
    request->setUrl(strHost.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    char aucBuf[256] = { 0 };
    sprintf(aucBuf, "uid=%d&ignore=0", this->uiNetId);
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
