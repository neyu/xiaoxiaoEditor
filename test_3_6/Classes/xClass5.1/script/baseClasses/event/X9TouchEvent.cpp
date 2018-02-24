//
//  X9TouchEvent.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/24.
//
//

#include "X9TouchEvent.h"


X9ValueObject* baseGet_touchEvent_x(X9RunObject* target)
{
    X9TouchEvent* event = dynamic_cast<X9TouchEvent*>(target);
    return X9ValueObject::createWithNumber(event->location.x);
}
X9ValueObject* baseGet_touchEvent_y(X9RunObject* target)
{
    X9TouchEvent* event = dynamic_cast<X9TouchEvent*>(target);
    return X9ValueObject::createWithNumber(event->location.y);
}
X9ValueObject* baseGet_touchEvent_stageX(X9RunObject* target)
{
    X9TouchEvent* event = dynamic_cast<X9TouchEvent*>(target);
    return X9ValueObject::createWithNumber(event->stageLocatiion.x);
}
X9ValueObject* baseGet_touchEvent_stageY(X9RunObject* target)
{
    X9TouchEvent* event = dynamic_cast<X9TouchEvent*>(target);
    return X9ValueObject::createWithNumber(event->stageLocatiion.x);
}



string X9TouchEvent::TOUCH_BEGIN = "touchBegin";
string X9TouchEvent::TOUCH_MOVED = "touchMoved";
string X9TouchEvent::TOUCH_ENDED = "touchENded";
string X9TouchEvent::TOUCH_OUTSIDE_MOVED = "touchOutsideMoved";
string X9TouchEvent::TOUCH_OUTSIDE_ENDED = "touchOutsideEnded";
string X9TouchEvent::TOUCH_OUT = "touchOut";
string X9TouchEvent::TOUCH_OVER = "touchOver";
string X9TouchEvent::TOUCH_CANCELLED = "touchCancelled";
string X9TouchEvent::DRAG_BEGIN = "dragBegin";
string X9TouchEvent::DRAG_MOVED = "dragMoved";
string X9TouchEvent::DRAG_ENDED = "dragEnded";
void X9TouchEvent::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseGet(touchEvent_, x);
    x9_AddBaseGet(touchEvent_, y);
    x9_AddBaseGet(touchEvent_, stageX);
    x9_AddBaseGet(touchEvent_, stageY);
}
void X9TouchEvent::setConstValues(X9ScriptClassData* classData)
{
    x9_AddConstValue(String, TOUCH_BEGIN, TOUCH_BEGIN);
    x9_AddConstValue(String, TOUCH_MOVED, TOUCH_MOVED);
    x9_AddConstValue(String, TOUCH_ENDED, TOUCH_ENDED);
    x9_AddConstValue(String, TOUCH_OUTSIDE_MOVED, TOUCH_OUTSIDE_MOVED);
    x9_AddConstValue(String, TOUCH_OUTSIDE_ENDED, TOUCH_OUTSIDE_ENDED);
    x9_AddConstValue(String, TOUCH_OUT, TOUCH_OUT);
    x9_AddConstValue(String, TOUCH_OVER, TOUCH_OVER);
    x9_AddConstValue(String, TOUCH_CANCELLED, TOUCH_CANCELLED);
    x9_AddConstValue(String, DRAG_BEGIN, DRAG_BEGIN);
    x9_AddConstValue(String, DRAG_MOVED, DRAG_MOVED);
    x9_AddConstValue(String, DRAG_ENDED, DRAG_ENDED);
}

X9TouchEvent* X9TouchEvent::newTouchEvent(X9Library* library,const string& type, int ID, const Vec2& location, const Vec2& stageLocation)
{
    vector<X9ValueObject*> vs;
    vs.push_back(X9ValueObject::createWithString(type));
    X9TouchEvent* event = dynamic_cast<X9TouchEvent*>(library->createObject("TouchEvent",vs));
    event->location = location;
    event->stageLocatiion = stageLocation;
    event->ID = ID;
    return event;
}
X9TouchEvent* X9TouchEvent::create()
{
    CREATE_NEW_AUTO_OBJ(X9TouchEvent,);
}
X9TouchEvent::X9TouchEvent():X9Event("X9TouchEvent")
{
    x9_setCtor(TouchEvent);
}
void X9TouchEvent::removed()
{
    X9Event::removed();
}
void X9TouchEvent::initObject(const vector<X9ValueObject*>& vs)
{
    runSuperCtor("Event",vs);
}