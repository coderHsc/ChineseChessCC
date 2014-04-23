#include "GameMenu.h"
#include "GameScene.h"

USING_NS_CC;

GameMenu::GameMenu()
{
}


GameMenu::~GameMenu()
{
}

bool GameMenu::init(void)
{
    CCSize size = CCDirector::sharedDirector()->getWinSize();

    CCMenu* pItemMenu = CCMenu::create();
    
    CCLabelTTF* label = CCLabelTTF::create("start", "Arial", 48);
    CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(GameMenu::menuCallback));

    pItemMenu->addChild(pMenuItem);
    pMenuItem->setPosition(ccp(0, 0));

    this->addChild(pItemMenu);

    CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png", this,
        menu_selector(GameMenu::menuCloseCallback));
    CCMenu* pCloseMenu = CCMenu::create(pCloseItem, NULL);
    pCloseMenu->setPositionY(200);
    this->addChild(pCloseMenu);

    return true;
}

CCScene* GameMenu::scene(void)
{
    CCScene *scene = CCScene::create();

    GameMenu *layer = GameMenu::create();

    scene->addChild(layer);

    return scene;
}

void GameMenu::menuCallback(CCObject * pSender)
{
    // get the userdata, it's the index of the menu item clicked
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);
    int nIdx = pMenuItem->getZOrder() - 10000;

    // create the test scene and run it
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());

    return;
}

void GameMenu::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}
