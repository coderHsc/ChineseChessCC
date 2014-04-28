#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Chess.h"


class GameScene : public cocos2d::CCLayer
{
public:
    GameScene();
    ~GameScene();

    virtual bool init(void);
    static cocos2d::CCScene* scene(void);

    CREATE_FUNC(GameScene);

    virtual bool checkChessMoveIsValid(UINT uiChessId, UINT uiPosX, UINT uiPosY);
    virtual void moveChess(UINT uiChessId, UINT uiPosX, UINT uiPosY);
    static GameScene* getGameScene();
	static GameScene* pGameScene;

    virtual void setGameWin(void);

    UINT uiLastMoveColor;

private:
	void CreateChesses(void);

	void resetChessGame();

    void menuCloseGame(CCObject* pSender);
	
	ChessGame* pChessGame;
};

#define CHESS_TAG_BASE  100

#endif // __GAME_SCENE_H__
