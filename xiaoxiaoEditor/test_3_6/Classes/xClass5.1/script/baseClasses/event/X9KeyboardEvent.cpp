//
//  X9KeyboardEvent.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/24.
//
//

#include "X9KeyboardEvent.h"

X9ValueObject* baseGet_keyboardEvent_keyName(X9RunObject* target)
{
    X9KeyboardEvent* event = dynamic_cast<X9KeyboardEvent*>(target);
    return X9ValueObject::createWithString(event->keyName);
}

string X9KeyboardEvent::KEY_DOWN = "keyDown";
string X9KeyboardEvent::KEY_UP = "keyUp";

void X9KeyboardEvent::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseGet(keyboardEvent_, keyName);
}
void X9KeyboardEvent::setConstValues(X9ScriptClassData* classData)
{
    x9_AddConstValue(String, KEY_DOWN, KEY_DOWN);
    x9_AddConstValue(String, KEY_UP, KEY_UP);
}

X9KeyboardEvent* X9KeyboardEvent::newKeyboardEvent(X9Library* library,const string& type, const string& keyName)
{
    vector<X9ValueObject*> vs;
    vs.push_back(X9ValueObject::createWithString(type));
    X9KeyboardEvent* event = dynamic_cast<X9KeyboardEvent*>(library->createObject("KeyboardEvent",vs));
    event->keyName = keyName;
    return event;
}
X9KeyboardEvent* X9KeyboardEvent::create()
{
    CREATE_NEW_AUTO_OBJ(X9KeyboardEvent,);
}
X9KeyboardEvent::X9KeyboardEvent():X9Event("X9KeyboardEvent")
{
    x9_setCtor(KeyboardEvent);
}
void X9KeyboardEvent::removed()
{
    X9Event::removed();
}
void X9KeyboardEvent::initObject(const vector<X9ValueObject*>& vs)
{
    runSuperCtor("Event",vs);
}