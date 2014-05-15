#include "GameMenu.h"
#include "GameScene.h"
#include "GameSceneNet.h"
#include "Login.h"
#include "Config.h"
#include "HttpClient.h"

USING_NS_CC;
using namespace cocos2d::network;

GameMenu::GameMenu()
{
}


GameMenu::~GameMenu()
{
}

bool GameMenu::init(void)
{
    Size sizeWin = Director::getInstance()->getWinSize();

    Menu* pItemMenu = Menu::create();

    //TTFConfig ttfConfig(CF_F("fonts_en").c_str(), 48);
    TTFConfig ttfConfig2(CF_F("fonts_cn").c_str(), CF_FT("font_menu"));

    auto label = Label::createWithTTF(ttfConfig2, CF_S("label_normal_start"));
    MenuItemLabel* pMenuItem = MenuItemLabel::create(label, CC_CALLBACK_1(GameMenu::menuNormalGameCallback, this));

    auto labelInput = Label::createWithTTF(ttfConfig2, CF_S("label_login"));
    MenuItemLabel* pMenuInput = MenuItemLabel::create(labelInput, CC_CALLBACK_1(GameMenu::menuUserLoginCallback, this));

    auto labelLan = Label::createWithTTF(ttfConfig2, CF_S("label_net_start"));
    MenuItemLabel* pMenuItemLanTest = MenuItemLabel::create(labelLan, CC_CALLBACK_1(GameMenu::menuNetGameCallback, this));

    pItemMenu->addChild(pMenuItem);
	pItemMenu->addChild(pMenuInput);
	pItemMenu->addChild(pMenuItemLanTest);
	pMenuItem->setPosition(0, sizeWin.height * CF_P("pos_main_start"));
    pMenuInput->setPosition(0, sizeWin.height * CF_P("pos_main_login"));
	pMenuItemLanTest->setPosition(0, sizeWin.height * CF_P("pos_main_netstart"));

    pItemMenu->setPositionY(0);
    this->addChild(pItemMenu);

    MenuItemImage *pCloseItem = MenuItemImage::create(CF_F("image_closeNormal"), CF_F("image_closeSelect"),
        CC_CALLBACK_1(GameMenu::menuCloseCallback, this));
    Menu* pCloseMenu = Menu::create(pCloseItem, NULL);
    pCloseMenu->setPositionY(sizeWin.height * CF_P("pos_main_close"));
    this->addChild(pCloseMenu);

	return true;
}

Scene* GameMenu::scene(void)
{
    Scene *scene = Scene::create();

    GameMenu *layer = GameMenu::create();

    scene->addChild(layer);

    return scene;
}

void GameMenu::menuNormalGameCallback(Ref* pSender)
{
    // create the test scene and run it
    CCDirector::getInstance()->replaceScene(GameScene::scene());

    return;
}

void GameMenu::menuNetGameCallback(Ref* pSender)
{
	CCDirector::getInstance()->replaceScene(GameSceneNet::scene());

	return;
}

void GameMenu::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.", "Alert");
#else
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void GameMenu::menuUserLoginCallback(Ref* pSender)
{
    CCDirector::getInstance()->replaceScene(Login::scene());

    return;
}
