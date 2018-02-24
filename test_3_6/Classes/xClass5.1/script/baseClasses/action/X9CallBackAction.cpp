//
//  X9CallBackAction.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9CallBackAction.h"

void X9CallBackAction::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9CallBackAction::setConstValues(X9ScriptClassData* classData)
{
    
}


X9_CPP_CREATE(CallBackAction,Action)

void X9CallBackAction::removed()
{
    for (int i = 0; i<args.size(); i++) {
        args[i]->nonuse();
    }
    args.clear();
    callback->nonuse();
    callback = nullptr;
    X9Action::removed();
}
void X9CallBackAction::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() >= 1 && vs[0]->isFunction(),"new CallBackAction Error!!!");
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(X9ValueObject::createWithNumber(0));
    runSuperCtor("Action",timeVs);
    callback = vs[0]->getFunction();
    callback->use();
    for (int i = 1; i<vs.size(); i++) {
        args.push_back(vs[i]);
        vs[i]->use();
    }
}
void X9CallBackAction::updateAction(float v)
{
    if (v >= 1) {
        callback->runFunction(args);
    }
}
X9Action* X9CallBackAction::clone()
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithFunction(callback->target, callback->functionName, callback->isSuper));
    for (int i = 0; i<args.size(); i++) {
        values.push_back(args[i]->clone());
    }
    return dynamic_cast<X9Action*>(getLibrary()->createObject("CallBackAction", values));
}