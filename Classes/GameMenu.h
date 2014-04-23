#ifndef __GAME_MENU_H__
#define __GAME_MENU_H__

#include "cocos2d.h"


class GameMenu : public cocos2d::CCLayer
{
public:
    GameMenu();
    ~GameMenu();

    void menuCallback(CCObject * pSender);
    void menuCloseCallback(CCObject* pSender);

    virtual bool init(void);
    static cocos2d::CCScene* scene(void);

    CREATE_FUNC(GameMenu);

};

#endif // __GAME_MENU_H__