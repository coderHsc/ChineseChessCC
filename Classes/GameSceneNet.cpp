#include "GameMenu.h"
#include "GameSceneNet.h"
#include "ChessSprite.h"

#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

GameSceneNet::GameSceneNet()
:uiNetId(0), bCanMove(false)
{
}

GameSceneNet::~GameSceneNet()
{
	unscheduleAllSelectors();
}

bool GameSceneNet::init(void)
{
	GameScene::init();
	getNetIdFromServer();
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
    if (false == this->bCanMove)
    {
        return false;
    }

	CCLog("GameSceneNet******chess move is valid");
	return GameScene::checkChessMoveIsValid(uiChessId, uiPosY, uiPosX);
}

void GameSceneNet::moveChess(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
	CCLog("GameSceneNet******move chess");
	sendMoveToServer(uiChessId, uiPosY, uiPosX);
	GameScene::moveChess(uiChessId, uiPosY, uiPosX);

	schedule(schedule_selector(GameSceneNet::receiveMoveFromServerTimerBack), 3.0f);
	return;
}

void GameSceneNet::sendMoveToServer(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://192.168.1.176:9090/chessMove");
	request->setRequestType(CCHttpRequest::kHttpPost);

	char aucBuf[256] = { 0 };
	sprintf(aucBuf, "chess=%d&posy=%d&posx=%d", uiChessId, uiPosY, uiPosX);
	request->setRequestData(aucBuf, strlen(aucBuf) + 1);

	request->setTag("POST chess move");
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameSceneNet::receiveMoveFromServerTimerBack(float dt)
{
	CCLog("timer %f", dt);
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://192.168.1.176:9090/chessMoveGet");
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setResponseCallback(this, httpresponse_selector(GameSceneNet::receiveMoveFromServer));
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
	CCLog(strBuff.c_str());
}

void GameSceneNet::getNetIdFromServer()
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://192.168.1.176:9090/newIdGet");
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setResponseCallback(this, httpresponse_selector(GameSceneNet::receiveNetIdFromServer));
	request->setTag("GET net id");
	CCHttpClient::getInstance()->send(request);
	request->release();
}
void GameSceneNet::receiveNetIdFromServer(CCHttpClient* client, CCHttpResponse* response)
{
	if (!response->isSucceed())
	{
		CCLog("response failed in receiveNetIdFromServer");
		CCLog("error buffer: %s", response->getErrorBuffer());
		return;
	}

	std::vector<char> *buffer = response->getResponseData();
	std::string strBuff(buffer->begin(), buffer->end());
	
	UINT uiReadId = 0;
	sscanf(strBuff.c_str(), "%d", &uiReadId);

	if (0 == uiReadId)
	{
		CCLog("get net id %d error", uiReadId);
		exit(0);
	}

	this->uiNetId = uiReadId;
	CCLog("get net id %d", uiReadId);
}

