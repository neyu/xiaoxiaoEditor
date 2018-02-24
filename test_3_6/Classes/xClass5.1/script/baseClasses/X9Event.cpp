//
//  X9Event.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/1.
//
//

#include "X9Event.h"

//void baseSet_event_type(X9RunObject* target,X9ValueObject* value)
//{
//    CCASSERT(false,"set type Error!!!");
//}
X9ValueObject* baseGet_event_type(X9RunObject* target)
{
    X9Event* event = dynamic_cast<X9Event*>(target);
    return X9ValueObject::createWithString(event->eventType);
}
//void baseSet_event_target(X9RunObject* target,X9ValueObject* value)
//{
//    CCASSERT(false,"set type Error!!!");
//}
X9ValueObject* baseGet_event_target(X9RunObject* target)
{
    X9Event* event = dynamic_cast<X9Event*>(target);
    return event->targetValueObj->clone();
}

void X9Event::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseGet(event_, type);
    x9_AddBaseGet(event_, target);
}
void X9Event::setConstValues(X9ScriptClassData* classData)
{
    
}

X9Event* X9Event::newEvent(X9Library* library,const string& type)
{
    vector<X9ValueObject*> vs;
    vs.push_back(X9ValueObject::createWithString(type));
    return dynamic_cast<X9Event*>(library->createObject("Event",vs));
}
X9Event* X9Event::create()
{
    CREATE_NEW_AUTO_OBJ(X9Event,"X9Event");
}
X9Event::X9Event(const string& key):X9Object(key)
{
    eventType = "";
    targetValueObj = nullptr;
    x9_setCtor(Event);
}
void X9Event::removed()
{
    targetValueObj->nonuse();
    targetValueObj = nullptr;
    eventType = "";
    X9Object::removed();
}
void X9Event::use()
{
//    log("X9Event::use()");
    X9Object::use();
}
void X9Event::nonuse()
{
//    log("X9Event::nonuse()");
    X9Object::nonuse();
}
void X9Event::initObject(const vector<X9ValueObject*>& vs)
{
    runSuperCtor("Object",vs);
    X9ASSERT(vs.size() == 1 && vs[0]->isString(),"new Event Error!!!");
    eventType = vs[0]->getString();
    _isExoteric = true;
}
void X9Event::setTarget(X9Object* obj)
{
    if (targetValueObj) {
        targetValueObj->nonuse();
        targetValueObj = nullptr;
    }
    targetValueObj = X9ValueObject::createWithObject(obj);
    targetValueObj->use();
}
string X9Event::toString()
{
    std::ostringstream oss;
    oss<<"[";
    oss<<className;
    oss<<" type:";
    oss<<eventType;
    oss<<"]";
    return oss.str();
}