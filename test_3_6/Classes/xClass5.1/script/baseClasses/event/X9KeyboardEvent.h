//
//  X9KeyboardEvent.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/24.
//
//

#ifndef X9KeyboardEvent_hpp
#define X9KeyboardEvent_hpp

#include "../X9Event.h"
class X9KeyboardEvent : public X9Event
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    static string KEY_DOWN;
    static string KEY_UP;
    string keyName;
public:
    static X9KeyboardEvent* newKeyboardEvent(X9Library* library,const string& type, const string& keyName);
    static X9KeyboardEvent* create();
    X9KeyboardEvent();
    
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
};

#endif /* X9KeyboardEvent_hpp */
