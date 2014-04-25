#include "GameMenu.h"
#include "GameSceneLan.h"
#include "ChessSprite.h"

USING_NS_CC;

GameSceneLan::GameSceneLan()
{
}

GameSceneLan::~GameSceneLan()
{
}

bool GameSceneLan::init(void)
{
	GameScene::init();
    return true;
}

CCScene* GameSceneLan::scene(void)
{
    CCScene *scene = CCScene::create();

	GameSceneLan *layer = GameSceneLan::create();
	pGameScene = layer;

    scene->addChild(layer);

    return scene;
}

GameScene* GameSceneLan::getGameScene()
{
	return pGameScene;
}

bool GameSceneLan::checkChessMoveIsValid(UINT uiChessId, UINT uiPosY, UINT uiPosX)
{
	CCLog("GameSceneLan******chess move invalid");
	return GameScene::checkChessMoveIsValid(uiChessId, uiPosY, uiPosX);
}
