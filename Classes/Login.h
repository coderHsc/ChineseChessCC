#ifndef __C_C_LOGIN_H__
#define __C_C_LOGIN_H__

#include "cocos2d.h"
#include "HttpClient.h"

class Login : public cocos2d::Layer, public cocos2d::IMEDelegate
{
public:
    Login();
    ~Login();

    virtual bool init(void);
    static cocos2d::Scene* scene(void);

    CREATE_FUNC(Login);

    void onClickTrackNode(cocos2d::TextFieldTTF* pTextField, bool bClicked);

    virtual void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info);

    // Layer
    bool onTouchBegan(cocos2d::Touch  *touch, cocos2d::Event  *event);
    void onTouchEnded(cocos2d::Touch  *touch, cocos2d::Event  *event);

private:
    void setInfoLabel(const char* pstr, const cocos2d::Color3B& color);

    void menuLoginOk(Ref* pSender);
    void menuRegOk(Ref* pSender);
    void menuClose(Ref * pSender);

    void receiveRegResult(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);
    void receiveLoginResult(cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response);

    cocos2d::TextFieldTTF* _pTextFieldUserName;
    cocos2d::TextFieldTTF* _pTextFieldPasswd;
    cocos2d::TextFieldTTF* _pActiveField;

    cocos2d::Point  _beginPos;

    cocos2d::Label* _pInfoLabel;
    std::string strServerHost;
};

#endif // __C_C_LOGIN_H__
