//
//  X9Action.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9Action.h"
#include "X9DisplayObject.h"

//X9ValueObject* base_global_gotoScene(X9RunObject* target,const vector<X9ValueObject*>& values)
//{
//    X9ASSERT(values.size() == 1 && values[0]->isString(),"gotoScene Error!!!");
//    target->getLibrary()->player->gotoScene(values[0]->getString());
//    return X9ValueObject::create();
//}
//X9ValueObject* baseGet_global_userDefault(X9RunObject* target)
//{
//    X9BaseGlobal* obj = dynamic_cast<X9BaseGlobal*>(target);
//    return X9ValueObject::createWithObject(obj->userDefault);
//}

void X9Action::setBaseFunctions(X9Library* library, const string& className)
{
//    x9_AddBaseFunc(global_,isTouchWorking);
//    x9_AddBaseFunc(global_,nextScene);
//    x9_AddBaseFunc(global_,prevScene);
//    x9_AddBaseFunc(global_,gotoScene);
//    x9_AddBaseGet(global_,currentScene);
//    x9_AddBaseGet(global_,userDefault);
}
void X9Action::setConstValues(X9ScriptClassData* classData)
{
    
}

X9Action* X9Action::newAction(X9Library* library,float time)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithNumber(time));
    return dynamic_cast<X9Action*>(library->createObject("Action", values));
}

X9_CPP_CREATE_KEY(Action,Object)



void X9Action::removed()
{
    isRuning = false;
    X9Object::removed();
}
void X9Action::initObject(const vector<X9ValueObject*>& vs)
{
    runSuperCtor("Object",vs);
    isRuning = false;
    X9ASSERT(vs.size() == 1 && vs[0]->isNumber(),"new Action Error!!!");
    time = vs[0]->getNumber();
}
void X9Action::setTarget(X9DisplayObject* target)
{
    this->target = target;
    isRuning = true;
}
X9Action* X9Action::clone()
{
    return newAction(getLibrary(),time);
}