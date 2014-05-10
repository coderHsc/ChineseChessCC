#ifndef __GAME_MENU_H__
#define __GAME_MENU_H__

#include "cocos2d.h"


class GameMenu : public cocos2d::Layer
{
public:
    GameMenu();
    ~GameMenu();

    void menuNormalGameCallback(Ref* pSender);
    void menuInputId(Ref* pSender);
    void onMenuPostTestClicked(Ref* pSender);
    void menuNetGameCallback(Ref* pSender);

    void menuCloseCallback(Ref* pSender);

    virtual bool init(void);
    static cocos2d::Scene* scene(void);

    CREATE_FUNC(GameMenu);

};

#endif // __GAME_MENU_H__
