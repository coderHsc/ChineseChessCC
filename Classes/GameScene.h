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

    UINT getChessColor(UINT uiChessId);
    UINT getLastMoveColor(void);

protected:
    void setLastMoveColor(UINT uiChessColor);

    cocos2d::CCSprite *pInfoGround;

private:
	void CreateChesses(void);
	void resetChessGame();

    void menuCloseGame(CCObject* pSender);

    void moveChessToTrash(UINT uiChessId);
    void setTurnLabel(const char* pstr, const cocos2d::ccColor3B& color);

	ChessGame* pChessGame;
    cocos2d::CCLabelTTF* pTurnLabel;
    UINT uiLastMoveColor;
    UINT uiTrashNum;
};

#define CHESS_TAG_BASE  100

#endif // __GAME_SCENE_H__
