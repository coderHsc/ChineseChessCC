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

    void CreateChesses(void);

    void resetChessGame();
    bool checkChessMoveIsValid(UINT uiChessId, UINT uiPosX, UINT uiPosY);
    void moveChess(UINT uiChessId, UINT uiPosX, UINT uiPosY);
    static GameScene* getGameScene();

private:
    ChessGame* pChessGame;
    UINT uiLastMoveColor;
};

#endif // __GAME_SCENE_H__
