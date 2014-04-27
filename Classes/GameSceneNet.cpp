#include "GameMenu.h"
#include "GameSceneNet.h"
#include "ChessSprite.h"

#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

GameSceneNet::GameSceneNet()
:uiNetId(0), bCanMove(false), pTipsLabel(NULL)
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
        CCLog("Can't move chess");
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
	request->setUrl("http://localhost:9090/chessMove");
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
	request->setUrl("http://localhost:9090/chessMoveGet");
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
	CCLog("%s", strBuff.c_str());
}

void GameSceneNet::getNetIdFromServer()
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://localhost:9090/newIdGet");
	request->setRequestType(CCHttpRequest::kHttpGet);
	request->setResponseCallback(this, httpresponse_selector(GameSceneNet::receiveNetIdFromServer));
	request->setTag("GET net id");
	CCHttpClient::getInstance()->send(request);
	request->release();
    
    CCLog("request a net id from server");
    
    pTipsLabel = CCLabelTTF::create("getting net id", "Arial", 30);
    this->addChild(pTipsLabel);
    pTipsLabel->setPosition(ccp(480, 320));
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
    pTipsLabel->setString(pStr->getCString());
    pTipsLabel->setColor(ccYELLOW);
    
    //允许移动操作
    bCanMove = true;
    
    CCLog("request a net id from server");
    
    //请求分配对手
    CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://localhost:9090/findOpponent");
	request->setRequestType(CCHttpRequest::kHttpPost);
	request->setResponseCallback(this, httpresponse_selector(GameSceneNet::receiveMoveFromServer));
	char aucBuf[256] = { 0 };
	sprintf(aucBuf, "uiChessId=%d", uiNetId);
	request->setRequestData(aucBuf, strlen(aucBuf) + 1);
    request->setTag("Find opponent");
	CCHttpClient::getInstance()->send(request);
	request->release();
}

