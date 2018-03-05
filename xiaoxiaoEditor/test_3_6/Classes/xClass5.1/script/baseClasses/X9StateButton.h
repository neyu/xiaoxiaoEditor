//
//  X9StateButton.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/14.
//
//

#ifndef X9StateButton_hpp
#define X9StateButton_hpp

#include "X9Node.h"

class X9StateButton : public X9Node
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    typedef enum {
        SBT_NONE = 0,
        SBT_NORMAL,
        SBT_TOUCH,
        SBT_DISABLED,
    }SBType;
    SBType _type;
public:
    X9Node* _normalNode;
    X9Node* _touchNode;
    X9Node* _disabledNode;
    X9Node* _currentNode;
    bool _isTouchContent;
    bool _isDisabledContent;
public:
    static X9StateButton* create();
    X9StateButton(const string& key);
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual void setTouchEnabled(bool enabled, const Rect& rect = Rect::ZERO)override;
    void setState(SBType type);
    virtual void dispatchTouchEvent(X9MainTouchType type, const string& typeStr, const Vec2& location, const Vec2& playerLocation, int ID)override;
};

#endif /* X9StateButton_hpp */
