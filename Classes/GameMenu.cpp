#include "GameMenu.h"
#include "GameScene.h"
#include "GameSceneNet.h"

#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;

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
	CCMenuItemLabel* pMenuItem = CCMenuItemLabel::create(label, this, menu_selector(GameMenu::menuNormalGameCallback));

	CCLabelTTF* labelNet = CCLabelTTF::create("netTest", "Arial", 48);
	CCMenuItemLabel* pMenuItemNetTest = CCMenuItemLabel::create(labelNet, this, menu_selector(GameMenu::onMenuGetTestClicked));

	CCLabelTTF* labelNetPost = CCLabelTTF::create("netPostTest", "Arial", 48);
	CCMenuItemLabel* pMenuItemNetPostTest = CCMenuItemLabel::create(labelNetPost, this, menu_selector(GameMenu::onMenuPostTestClicked));

	CCLabelTTF* labelLan = CCLabelTTF::create("NetGame", "Arial", 48);
	CCMenuItemLabel* pMenuItemLanTest = CCMenuItemLabel::create(labelLan, this, menu_selector(GameMenu::menuNetGameCallback));

    pItemMenu->addChild(pMenuItem);
	pItemMenu->addChild(pMenuItemNetTest);
	pItemMenu->addChild(pMenuItemNetPostTest);
	pItemMenu->addChild(pMenuItemLanTest);
	pMenuItem->setPosition(ccp(0, 0));
	pMenuItemNetTest->setPosition(ccp(0, 80));
	pMenuItemLanTest->setPosition(ccp(0, 160));
	pMenuItemNetPostTest->setPosition(ccp(0, 240));

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

void GameMenu::menuNormalGameCallback(CCObject* pSender)
{
    // get the userdata, it's the index of the menu item clicked
    CCMenuItem* pMenuItem = (CCMenuItem *)(pSender);

    // create the test scene and run it
    CCDirector::sharedDirector()->replaceScene(GameScene::scene());

    return;
}

void GameMenu::menuNetGameCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->replaceScene(GameSceneNet::scene());

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

void GameMenu::onMenuGetTestClicked(CCObject* pSender)
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://114.215.193.215:9090/pretestPrintRequest");
	request->setRequestType(CCHttpRequest::kHttpGet);
	//request->setResponseCallback(this, httpresponse_selector(HttpClientTest::onHttpRequestCompleted));
	request->setTag("GET test1");
	CCHttpClient::getInstance()->send(request);
	request->release();
}

void GameMenu::onMenuPostTestClicked(CCObject* pSender)
{
	CCHttpRequest* request = new CCHttpRequest();
	request->setUrl("http://114.215.193.215:9090/pretestPrintRequest");
	request->setRequestType(CCHttpRequest::kHttpPost);
	//request->setResponseCallback(this, httpresponse_selector(HttpClientTest::onHttpRequestCompleted));

	// write the post data
	char postData[22] = "binary=hello\0\0cocos2d";
	request->setRequestData(postData, 22);

	request->setTag("POST test1");
	CCHttpClient::getInstance()->send(request);
	request->release();
}