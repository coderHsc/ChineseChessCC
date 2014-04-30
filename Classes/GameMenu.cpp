#include "GameMenu.h"
#include "GameScene.h"
#include "GameSceneNet.h"
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
    Menu* pItemMenu = Menu::create();
    
    LabelTTF* label = LabelTTF::create("start", "Arial", 48);
    MenuItemLabel* pMenuItem = MenuItemLabel::create(label, CC_CALLBACK_1(GameMenu::menuNormalGameCallback, this));

	LabelTTF* labelNet = LabelTTF::create("netTest", "Arial", 48);
    MenuItemLabel* pMenuItemNetTest = MenuItemLabel::create(labelNet, CC_CALLBACK_1(GameMenu::onMenuGetTestClicked, this));

	LabelTTF* labelNetPost = LabelTTF::create("netPostTest", "Arial", 48);
    MenuItemLabel* pMenuItemNetPostTest = MenuItemLabel::create(labelNetPost, CC_CALLBACK_1(GameMenu::onMenuPostTestClicked, this));

	LabelTTF* labelLan = LabelTTF::create("NetGame", "Arial", 48);
    MenuItemLabel* pMenuItemLanTest = MenuItemLabel::create(labelLan, CC_CALLBACK_1(GameMenu::menuNetGameCallback, this));

    pItemMenu->addChild(pMenuItem);
	//pItemMenu->addChild(pMenuItemNetTest);
	//pItemMenu->addChild(pMenuItemNetPostTest);
	pItemMenu->addChild(pMenuItemLanTest);
	pMenuItem->setPosition(ccp(0, 0));
	pMenuItemNetTest->setPosition(ccp(0, 80));
	pMenuItemLanTest->setPosition(ccp(0, 160));
	pMenuItemNetPostTest->setPosition(ccp(0, 240));

    this->addChild(pItemMenu);

    MenuItemImage *pCloseItem = MenuItemImage::create("CloseNormal.png", "CloseSelected.png",
        CC_CALLBACK_1(GameMenu::menuCloseCallback, this));
    Menu* pCloseMenu = Menu::create(pCloseItem, NULL);
    pCloseMenu->setPositionY(200);
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
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());

    return;
}

void GameMenu::menuNetGameCallback(Ref* pSender)
{
	CCDirector::sharedDirector()->replaceScene(GameSceneNet::scene());

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

void GameMenu::onMenuGetTestClicked(Ref* pSender)
{
    HttpRequest* request = new HttpRequest();
	request->setUrl("http://114.215.193.215:9090/pretestPrintRequest");
    request->setRequestType(HttpRequest::Type::GET);
	request->setTag("GET test1");
	HttpClient::getInstance()->send(request);
	request->release();
}

void GameMenu::onMenuPostTestClicked(Ref* pSender)
{
    HttpRequest* request = new HttpRequest();
	request->setUrl("http://192.168.1.176:9090/findOpponent");
    request->setRequestType(HttpRequest::Type::POST);

	// write the post data
	char postData[22] = "uiChessId=22";
	request->setRequestData(postData, 22);

	request->setTag("POST test1");
    HttpClient::getInstance()->send(request);
	request->release();
}