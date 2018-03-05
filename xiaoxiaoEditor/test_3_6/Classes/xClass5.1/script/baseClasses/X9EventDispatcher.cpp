//
//  X9EventDispatcher.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/1.
//
//

#include "X9EventDispatcher.h"
#include "X9Event.h"
#include "../X9ScriptData.h"

EventFunctionObject::EventFunctionObject()
{
    isRuning = false;
    isRemoved = false;
    func = nullptr;
}
EventFunctionObject::EventFunctionObject(X9ValueObject* func)
{
//    log("EventFunctionObject()");
    isRuning = false;
    isRemoved = false;
    this->func = func;
    func->use();
}
EventFunctionObject::~EventFunctionObject()
{
//    log("~EventFunctionObject");
    if (func) {
        func->nonuse();
    }
    func = nullptr;
}
X9ValueObject* base_addEventListener(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->isString() && values[1]->isFunction(),"addEventListener Error!!!");
    const string& type = values[0]->getString();
    X9EventDispatcher* obj = dynamic_cast<X9EventDispatcher*>(target);
    if(obj->eventFunctions.find(type) == obj->eventFunctions.end()) {
        obj->eventFunctions[type] = vector<EventFunctionObject*>();
    }
    obj->eventFunctions[type].push_back(new EventFunctionObject(values[1]));
//    values[1]->use();
    obj->checkAddEventListener(type);
    return nullptr;
}
X9ValueObject* base_removeEventListener(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() <= 2 && values[0]->isString() && values[1]->isFunction(),"removeEventListener Error!!!");
    const string& type = values[0]->getString();
    X9EventDispatcher* obj = dynamic_cast<X9EventDispatcher*>(target);
    if(obj->eventFunctions.find(type) != obj->eventFunctions.end()) {
        for (auto it = obj->eventFunctions[type].begin(); it != obj->eventFunctions[type].end(); it++) {
            if ((*(*it)->func == values[1])->getBool()) {
                if ((*it)->isRuning) {
                    (*it)->isRemoved = true;
                }else{
                    obj->eventFunctions[type].erase(it);
                    if (obj->eventFunctions[type].empty()) {
                        obj->eventFunctions.erase(type);
                    }
                }
                obj->checkRemoveEventListener(type);
                break;
            }
        }
    }
    return nullptr;
}
X9ValueObject* base_dispatchEvent(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Event*>(),"dispatchEvent Error!!!");
    X9Event* event = values[0]->getObject<X9Event*>();
    X9ASSERT(event,"dispatchEvent Error!!!");
    X9EventDispatcher* obj = dynamic_cast<X9EventDispatcher*>(target);
    obj->dispatchEvent(event);
//    if(obj->eventFunctions.find(event->eventType) != obj->eventFunctions.end()) {
//        for (auto it = obj->eventFunctions[event->eventType].begin(); it != obj->eventFunctions[event->eventType].end(); it++) {
//            (*it)->getFunction()->runFunction(values);
//        }
//    }
    return nullptr;
}
X9ValueObject* base_hasEventListener(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isString(),"hasEventListener Error!!!");
    X9EventDispatcher* obj = dynamic_cast<X9EventDispatcher*>(target);
    return X9ValueObject::createWithBool(obj->hasEventListener(values[0]->getString()));
}
void X9EventDispatcher::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(,addEventListener);
    x9_AddBaseFunc(,removeEventListener);
    x9_AddBaseFunc(,dispatchEvent);
    x9_AddBaseFunc(,hasEventListener);
}
void X9EventDispatcher::setConstValues(X9ScriptClassData* classData)
{
    
}
X9EventDispatcher* X9EventDispatcher::create()
{
    CREATE_NEW_AUTO_OBJ(X9EventDispatcher,"X9EventDispatcher");
}
X9EventDispatcher::X9EventDispatcher(const string& key):X9Object(key)
{
    x9_setCtor(EventDispatcher);
}
void X9EventDispatcher::removed()
{
    for (auto it = eventFunctions.begin(); it != eventFunctions.end(); it++) {
        for (int i = 0; i<it->second.size(); i++) {
            X9_SAFE_DELETE(it->second[i]);
        }
    }
    eventFunctions.clear();
    X9Object::removed();
}
void X9EventDispatcher::initObject(const vector<X9ValueObject*>& vs)
{
    
}
bool X9EventDispatcher::hasEventListener(const string& type)
{
    return eventFunctions.find(type) != eventFunctions.end();
}
void X9EventDispatcher::dispatchEvent(X9Event* event)
{
    event->setTarget(this);
    vector<X9ValueObject*> vs;
    vs.push_back(X9ValueObject::createWithObject(event));
    if(eventFunctions.find(event->eventType) != eventFunctions.end()) {
        for (auto it = eventFunctions[event->eventType].begin(); it != eventFunctions[event->eventType].end(); it++) {
            (*it)->isRuning = true;
            (*it)->func->getFunction()->runFunction(vs);
            (*it)->isRuning = false;
        }
        for (int i = eventFunctions[event->eventType].size()-1; i>=0; i--) {
            if (eventFunctions[event->eventType][i]->isRemoved) {
                eventFunctions[event->eventType].erase(eventFunctions[event->eventType].begin()+i);
            }
        }
        if (eventFunctions[event->eventType].empty()) {
            eventFunctions.erase(event->eventType);
        }
    }
}