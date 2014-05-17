#include "Login.h"
#include "GameMenu.h"
#include "GameSceneNet.h"
#include "Config.h"

USING_NS_CC;
using namespace cocos2d::network;

static Rect getRect(Node * node)
{
    Rect rc;
    rc.origin = node->getPosition();
    rc.size = node->getContentSize();
    rc.origin.x -= rc.size.width / 2;
    rc.origin.y -= rc.size.height / 2;
    return rc;
}

Login::Login()
: _pTextFieldUserName(NULL), _pTextFieldPasswd(NULL), _pActiveField(NULL)
{
}

Login::~Login()
{
}

bool Login::init(void)
{
    Size size = Director::getInstance()->getWinSize();

    Menu* pItemMenu = Menu::create();

    TTFConfig ttfConfig(CF_F("fonts_cn").c_str(), CF_FT("font_menu"));

    auto pLabelLogin = Label::createWithTTF(ttfConfig, CF_S("label_login_login"));
    MenuItemLabel* pMenuItemLogin = MenuItemLabel::create(pLabelLogin, CC_CALLBACK_1(Login::menuLoginOk, this));

    pItemMenu->addChild(pMenuItemLogin);
    pMenuItemLogin->setPosition(size.width * CF_P("pos_login_login_w"), size.height * CF_P("pos_login_login_h"));

    auto pLabelReg = Label::createWithTTF(ttfConfig, CF_S("label_login_register"));
    MenuItemLabel* pMenuItemReg = MenuItemLabel::create(pLabelReg, CC_CALLBACK_1(Login::menuRegOk, this));

    pItemMenu->addChild(pMenuItemReg);
    pMenuItemReg->setPosition(size.width * CF_P("pos_login_reg_w"), size.height * CF_P("pos_login_reg_h"));

    pItemMenu->setPosition(0, 0);
    this->addChild(pItemMenu);

    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(Login::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(Login::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    _pTextFieldUserName = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>", CF_F("fonts_en"), CF_FT("font_input"));
    this->addChild(_pTextFieldUserName);
    _pTextFieldUserName->setPosition(size.width * 0.5, size.height * CF_P("pos_login_user"));

    _pTextFieldPasswd = TextFieldTTF::textFieldWithPlaceHolder("<click here for input>", CF_F("fonts_en"), CF_FT("font_input"));
    this->addChild(_pTextFieldPasswd);
    _pTextFieldPasswd->setPosition(size.width * 0.5, size.height * CF_P("pos_login_pwd"));
    _pTextFieldPasswd->setSecureTextEntry(true);

    _pInfoLabel = Label::createWithTTF(CF_S("label_login_please"), CF_F("fonts_cn"), CF_FT("font_menu"));
    this->addChild(_pInfoLabel);
    _pInfoLabel->setPosition(size.width * 0.5, size.height * CF_P("pos_login_info"));

    Menu* pItemMenuClose = Menu::create();
    auto labelClose = Label::createWithTTF(CF_S("label_return"), CF_F("fonts_cn"), CF_FT("font_info"));
    MenuItemLabel* pMenuClose = MenuItemLabel::create(labelClose, CC_CALLBACK_1(Login::menuClose, this));
    pItemMenuClose->addChild(pMenuClose);
    pItemMenuClose->setPosition(size.width * CF_P("pos_login_return_w"), size.height * CF_P("pos_login_return_h"));
    this->addChild(pItemMenuClose);

    this->strServerHost = CF_N("host");

    return true;
}

Scene* Login::scene(void)
{
    Scene *scene = Scene::create();

    Login *layer = Login::create();

    scene->addChild(layer);

    return scene;
}

void Login::menuLoginOk(Ref* pSender)
{
    std::string strUser = _pTextFieldUserName->getString();
    std::string strPwd = _pTextFieldPasswd->getString();

    HttpRequest* request = new HttpRequest();

    std::string strHost = this->strServerHost + CF_N("path_login");
    request->setUrl(strHost.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(Login::receiveLoginResult));

    char aucBuf[256] = { 0 };
    sprintf(aucBuf, "user=%s&passwd=%s&ignore=x", strUser.c_str(), strPwd.c_str());
    request->setRequestData(aucBuf, strlen(aucBuf) + 1);

    request->setTag("regUser");
    HttpClient::getInstance()->send(request);

    log("request register a new user");
}

void Login::menuRegOk(Ref* pSender)
{
    std::string strUser = _pTextFieldUserName->getString();
    std::string strPwd = _pTextFieldPasswd->getString();

    HttpRequest* request = new HttpRequest();

    std::string strHost = this->strServerHost + CF_N("path_register");
    request->setUrl(strHost.c_str());
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback(this, httpresponse_selector(Login::receiveRegResult));

    char aucBuf[256] = { 0 };
    sprintf(aucBuf, "user=%s&passwd=%s&ignore=x", strUser.c_str(), strPwd.c_str());
    request->setRequestData(aucBuf, strlen(aucBuf) + 1);

    request->setTag("regUser");
    HttpClient::getInstance()->send(request);

    log("request register a new user");
}

void Login::menuClose(Ref* pSender)
{
    Director::getInstance()->replaceScene(GameMenu::scene());
}

void Login::receiveRegResult(HttpClient* client, HttpResponse* response)
{
    //检查响应是否成功
    if (!response->isSucceed())
    {
        log("response failed in receiveRegResult");
        log("error buffer: %s", response->getErrorBuffer());
        response->getHttpRequest()->release();
        return;
    }

    //读取响应
    std::vector<char> *buffer = response->getResponseData();
    std::string strBuff(buffer->begin(), buffer->end());
    log("receive register result %s", strBuff.c_str());

    UINT uiReadId = 0;
    sscanf(strBuff.c_str(), "uid=%d", &uiReadId);

    if (0 == uiReadId)
    {
        std::string strRetReason(strBuff.substr(strBuff.find("reason") + 6));
        log("register error: %s", strRetReason.c_str());

        //替换网络信息标签
        auto pStr = String::createWithFormat("%s\n%s", CF_S("userinfo_register_failed").c_str(), strRetReason.c_str());
        this->setInfoLabel(pStr->getCString(), Color3B::RED);
        response->getHttpRequest()->release();

        return;
    }

    //替换信息标签
    auto pStr = String::createWithFormat("%s", CF_S("userinfo_register_succeed").c_str());
    this->setInfoLabel(pStr->getCString(), Color3B::BLUE);
    response->getHttpRequest()->release();

    return;
}

void Login::receiveLoginResult(HttpClient* client, HttpResponse* response)
{
    //检查响应是否成功
    if (!response->isSucceed())
    {
        log("response failed in receiveLoginResult");
        log("error buffer: %s", response->getErrorBuffer());
        response->getHttpRequest()->release();
        return;
    }

    //读取响应
    std::vector<char> *buffer = response->getResponseData();
    std::string strBuff(buffer->begin(), buffer->end());
    log("receive login result %s", strBuff.c_str());

    UINT uiReadId = 0;
    sscanf(strBuff.c_str(), "uid=%d", &uiReadId);

    if (0 == uiReadId)
    {
        std::string strRetReason(strBuff.substr(strBuff.find("reason") + 6));
        log("login error: %s", strRetReason.c_str());

        //替换网络信息标签
        auto pStr = String::createWithFormat("%s\n%s", CF_S("userinfo_login_failed").c_str(), strRetReason.c_str());
        this->setInfoLabel(pStr->getCString(), Color3B::RED);
        response->getHttpRequest()->release();

        return;
    }

    //记录分配的NETID
    GameSceneNet::setLoginInfo(uiReadId);

    //替换信息标签
    auto pStr = String::createWithFormat("%s", CF_S("userinfo_login_succeed").c_str());
    this->setInfoLabel(pStr->getCString(), Color3B::BLUE);
    response->getHttpRequest()->release();

    return;
}

void Login::setInfoLabel(const char* pstr, const cocos2d::Color3B& color)
{
    _pInfoLabel->setString(pstr);
    _pInfoLabel->setColor(color);
}

bool Login::onTouchBegan(Touch  *touch, Event  *event)
{
    log("++++++++++++++++++++++++++++++++++++++++++++");
    _beginPos = touch->getLocation();
    return true;
}

void Login::onTouchEnded(Touch  *touch, Event  *event)
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

void Login::onClickTrackNode(TextFieldTTF* pTextField, bool bClicked)
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

void Login::keyboardWillShow(IMEKeyboardNotificationInfo& info)
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