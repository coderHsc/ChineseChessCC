#ifndef __GAME_SCENENET_H__
#define __GAME_SCENENET_H__

#include "cocos2d.h"
#include "GameScene.h"

#include "cocos-ext.h"

class GameSceneNet : public GameScene
{
public:
	GameSceneNet();
	~GameSceneNet();

    virtual bool init(void);
    static cocos2d::CCScene* scene(void);

	CREATE_FUNC(GameSceneNet);

	virtual bool checkChessMoveIsValid(UINT uiChessId, UINT uiPosX, UINT uiPosY);
	virtual void moveChess(UINT uiChessId, UINT uiPosX, UINT uiPosY);
	static GameScene* getGameScene();

private:
	void sendMoveToServer(UINT uiChessId, UINT uiPosX, UINT uiPosY);
	void receiveMoveFromServerTimerBack(float dt);
	void receiveMoveFromServer(cocos2d::extension::CCHttpClient* client, cocos2d::extension::CCHttpResponse* response);
};

#endif // __GAME_SCENENET_H__
