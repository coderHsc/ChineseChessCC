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

    ChessGame *pChessGame;

};

#endif // __GAME_SCENE_H__
