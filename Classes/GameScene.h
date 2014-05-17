#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Chess.h"


class GameScene : public cocos2d::Layer
{
public:
    GameScene();
    ~GameScene();

    virtual bool init(void);
    static cocos2d::Scene* scene(void);

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
    void setTurnLabel(const char* pstr, const cocos2d::Color3B& color);

    cocos2d::Sprite *pInfoGround;

    void menuCloseGame(Ref * pSender);

private:
	void CreateChesses(void);
	void resetChessGame();

    void chessCheckmate(void);
    void kingBeCheckmate(UINT uiChessId);
    void kingUnBeCheckmate(UINT uiChessId);
    void checkmateKing(UINT uiChessId);
    void unCheckmateKing(UINT uiChessId);
    void moveChessToTrash(UINT uiChessId);

	ChessGame* pChessGame;
    cocos2d::Label* pTurnLabel;
    UINT uiLastMoveColor;
    UINT uiTrashNum;
    bool bGameStop;
};

#define CHESS_TAG_BASE  100

#endif // __GAME_SCENE_H__
