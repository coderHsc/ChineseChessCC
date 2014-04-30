#ifndef __GAME_SCENENET_H__
#define __GAME_SCENENET_H__

#include "cocos2d.h"
#include "GameScene.h"
#include "HttpClient.h"

class GameSceneNet : public GameScene
{
public:
	GameSceneNet();
	~GameSceneNet();

    virtual bool init(void);
    static cocos2d::Scene* scene(void);

	CREATE_FUNC(GameSceneNet);

	virtual bool checkChessMoveIsValid(UINT uiChessId, UINT uiPosX, UINT uiPosY);
	virtual void moveChess(UINT uiChessId, UINT uiPosX, UINT uiPosY);
	static GameScene* getGameScene();

    virtual void setGameWin(void);

private:
    void setNetLabel(const char* pstr, const cocos2d::Color3B& color);

    void moveChessSelf(UINT uiChessId, UINT uiPosX, UINT uiPosY);

	void getNetIdFromServer(void);
    void receiveNetIdFromServer(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
    void getOppenetIdFromServer(float dt);
    void receiveOpponentIdFromServer(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
    void sendLeaveGameToServer(void);

	void sendMoveToServer(UINT uiChessId, UINT uiPosX, UINT uiPosY);
	void receiveMoveFromServerTimerBack(float dt);
    void receiveMoveFromServer(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);

	UINT uiNetId;
    UINT uiLocalColor;
    UINT uiOpponentId;
	bool bCanMove;
    cocos2d::LabelTTF *pNetLabel;
};

#endif // __GAME_SCENENET_H__
