//
//  X9CallBackAction.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9CallBackAction_hpp
#define X9CallBackAction_hpp

#include "X9Action.h"
class X9CallBackAction : public X9Action
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    vector<X9ValueObject*> args;
    X9FunctionValueObject* callback;
public:
    static X9CallBackAction* create();
    X9CallBackAction();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual void updateAction(float v)override;
    virtual X9Action* clone()override;
};

#endif /* X9CallBackAction_hpp */
