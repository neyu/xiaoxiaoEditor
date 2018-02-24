//
//  X9UpdateAction.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9UpdateAction.h"

void X9UpdateAction::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9UpdateAction::setConstValues(X9ScriptClassData* classData)
{
    
}


X9_CPP_CREATE(UpdateAction,Action)

void X9UpdateAction::removed()
{
    for (int i = 0; i<args.size(); i++) {
        args[i]->nonuse();
    }
    args.clear();
    callback->nonuse();
    callback = nullptr;
    X9Action::removed();
}
void X9UpdateAction::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() >= 2 && vs[0]->isNumber() && vs[1]->isFunction(),"new UpdateAction: Error!!!");
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(X9ValueObject::createWithNumber(vs[0]->getNumber()));
    runSuperCtor("Action",timeVs);
    callback = vs[1]->getFunction();
    callback->use();
    args.push_back(X9ValueObject::createWithNumber(0));
    args[0]->use();
    for (int i = 2; i<vs.size(); i++) {
        args.push_back(vs[i]);
        vs[i]->use();
    }
}
void X9UpdateAction::updateAction(float v)
{
    args[0]->setNumber(v);
    callback->runFunction(args);
}
X9Action* X9UpdateAction::clone()
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithFunction(callback->target, callback->functionName, callback->isSuper));
    for (int i = 0; i<args.size(); i++) {
        values.push_back(args[i]->clone());
    }
    return dynamic_cast<X9Action*>(getLibrary()->createObject("UpdateAction", values));
}