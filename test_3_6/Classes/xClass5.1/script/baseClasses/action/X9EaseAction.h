//
//  X9EaseAction.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9EaseAction_hpp
#define X9EaseAction_hpp

#include "X9Action.h"
class X9EaseAction : public X9Action
{
    static map<string,std::function<float(float)>> _easeFuncs;
    void initObject(const vector<X9ValueObject*>& vs);
public:
    string type;
    bool isType;
    X9FunctionValueObject* easeFunc;
    X9Action* act;
public:
    static X9EaseAction* newEaseAction(X9Library* library,const string& type, X9Action* act);
    static X9EaseAction* newEaseAction(X9Library* library,const X9FunctionValueObject* func, X9Action* act);
    static X9EaseAction* create();
    X9EaseAction();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual void setTarget(X9DisplayObject* target)override;
    virtual void makeTime()override;
    virtual void updateAction(float v)override;
    virtual X9Action* clone()override;
};

#endif /* X9EaseAction_hpp */
