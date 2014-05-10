#ifndef __INPUT_H__
#define __INPUT_H__

#include "cocos2d.h"

class Input : public cocos2d::Layer, public cocos2d::IMEDelegate
{
public:
    Input();
    ~Input();

    virtual bool init(void);
    static cocos2d::Scene* scene(void);

    CREATE_FUNC(Input);

    void onClickTrackNode(cocos2d::TextFieldTTF* pTextField, bool bClicked);

    virtual void keyboardWillShow(cocos2d::IMEKeyboardNotificationInfo& info);

    // Layer
    bool onTouchBegan(cocos2d::Touch  *touch, cocos2d::Event  *event);
    void onTouchEnded(cocos2d::Touch  *touch, cocos2d::Event  *event);

private:

    void menuInputOk(Ref* pSender);

    cocos2d::TextFieldTTF* _pTextFieldUserName;
    cocos2d::TextFieldTTF* _pTextFieldPasswd;
    cocos2d::TextFieldTTF* _pActiveField;

    cocos2d::Point  _beginPos;
};

#endif // __INPUT_H__
