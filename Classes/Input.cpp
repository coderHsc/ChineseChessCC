#include "Input.h"
#include "GameMenu.h"
#include "GameSceneNet.h"

USING_NS_CC;

static Rect getRect(Node * node)
{
    Rect rc;
    rc.origin = node->getPosition();
    rc.size = node->getContentSize();
    rc.origin.x -= rc.size.width / 2;
    rc.origin.y -= rc.size.height / 2;
    return rc;
}

Input::Input()
: _pTextFieldUserName(NULL), _pTextFieldPasswd(NULL), _pActiveField(NULL)
{
}

Input::~Input()
{
}

bool Input::init(void)
{
    Size size = Director::getInstance()->getWinSize();

    Menu* pItemMenu = Menu::create();

    TTFConfig ttfConfig("fonts/arial.ttf", 48);

    auto label = Label::createWithTTF(ttfConfig, "OK");
    MenuItemLabel* pMenuItem = MenuItemLabel::create(label, CC_CALLBACK_1(Input::menuInputOk, this));

    pItemMenu->addChild(pMenuItem);
    pMenuItem->setPosition(0, -200);

    this->addChild(pItemMenu);

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Input::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(Input::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    _pTextFieldUserName = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>", "fonts/arial.ttf", 48);
    this->addChild(_pTextFieldUserName);
    _pTextFieldUserName->setPosition(size.width * 0.5, size.height * 0.5 + 60);

    _pTextFieldPasswd = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>", "fonts/arial.ttf", 48);
    this->addChild(_pTextFieldPasswd);
    _pTextFieldPasswd->setPosition(size.width * 0.5, size.height * 0.5 - 30);
    _pTextFieldPasswd->setSecureTextEntry(true);

    return true;
}

Scene* Input::scene(void)
{
    Scene *scene = Scene::create();

    Input *layer = Input::create();

    scene->addChild(layer);

    return scene;
}

void Input::menuInputOk(Ref* pSender)
{
    std::string strUser = _pTextFieldUserName->getString();
    std::string strPwd = _pTextFieldPasswd->getString();

    log("input: user %s, passwd %s", strUser.c_str(), strPwd.c_str());

    GameSceneNet::setLoginInfo(strUser, strPwd);

    Director::getInstance()->replaceScene(GameMenu::scene());
}

bool Input::onTouchBegan(Touch  *touch, Event  *event)
{
    log("++++++++++++++++++++++++++++++++++++++++++++");
    _beginPos = touch->getLocation();
    return true;
}

void Input::onTouchEnded(Touch  *touch, Event  *event)
{
    if (!_pTextFieldUserName)
    {
        return;
    }

    auto endPos = touch->getLocation();

    float delta = 5.0f;
    if (::abs(endPos.x - _beginPos.x) > delta
        || ::abs(endPos.y - _beginPos.y) > delta)
    {
        // not click
        _beginPos.x = _beginPos.y = -1;
        return;
    }

    // decide the trackNode is clicked.
    Rect rectUser = getRect(_pTextFieldUserName);
    Rect rectPwd = getRect(_pTextFieldPasswd);
    auto point = convertTouchToNodeSpaceAR(touch);
    log("KeyboardNotificationLayer:clickedAt(%f,%f)", point.x, point.y);

    log("KeyboardNotificationLayer:UserField at(origin:%f,%f, size:%f,%f)",
        rectUser.origin.x, rectUser.origin.y, rectUser.size.width, rectUser.size.height);
    log("KeyboardNotificationLayer:PwdField at(origin:%f,%f, size:%f,%f)",
        rectPwd.origin.x, rectPwd.origin.y, rectPwd.size.width, rectPwd.size.height);

    this->onClickTrackNode(_pTextFieldUserName, rectUser.containsPoint(point));
    this->onClickTrackNode(_pTextFieldPasswd, rectPwd.containsPoint(point));
    log("----------------------------------");
}

void Input::onClickTrackNode(TextFieldTTF* pTextField, bool bClicked)
{
    if (bClicked)
    {
        // TextFieldTTFTest be clicked
        log("TextFieldTTFDefaultTest:TextFieldTTF attachWithIME");
        pTextField->attachWithIME();
        this->_pActiveField = pTextField;
    }
    else
    {
        // TextFieldTTFTest not be clicked
        log("TextFieldTTFDefaultTest:TextFieldTTF detachWithIME");
        pTextField->detachWithIME();
    }
}

void Input::keyboardWillShow(IMEKeyboardNotificationInfo& info)
{
    log("TextInputTest:keyboardWillShowAt(origin:%f,%f, size:%f,%f)",
        info.end.origin.x, info.end.origin.y, info.end.size.width, info.end.size.height);

    if (!this->_pActiveField)
    {
        return;
    }

    Rect rectTracked = getRect(this->_pActiveField);
    
    log("TextInputTest:trackingNodeAt(origin:%f,%f, size:%f,%f)",
        rectTracked.origin.x, rectTracked.origin.y, rectTracked.size.width, rectTracked.size.height);

    // if the keyboard area doesn't intersect with the tracking node area, nothing need to do.
    if (!rectTracked.intersectsRect(info.end))
    {
        return;
    }

    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    float adjustVert = info.end.getMaxY() - rectTracked.getMinY();
    log("TextInputTest:needAdjustVerticalPosition(%f)", adjustVert);

    // move all the children node of KeyboardNotificationLayer
    auto& children = getChildren();
    Node * node = 0;
    ssize_t count = children.size();
    Point pos;
    for (int i = 0; i < count; ++i)
    {
        node = children.at(i);
        pos = node->getPosition();
        pos.y += adjustVert;
        node->setPosition(pos);
    }
}