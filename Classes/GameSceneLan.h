#ifndef __GAME_SCENELAN_H__
#define __GAME_SCENELAN_H__

#include "cocos2d.h"
#include "GameScene.h"


class GameSceneLan : public GameScene
{
public:
	GameSceneLan();
	~GameSceneLan();

    virtual bool init(void);
    static cocos2d::CCScene* scene(void);

	CREATE_FUNC(GameSceneLan);

	virtual bool checkChessMoveIsValid(UINT uiChessId, UINT uiPosX, UINT uiPosY);
	static GameScene* getGameScene();
};

#endif // __GAME_SCENELAN_H__
