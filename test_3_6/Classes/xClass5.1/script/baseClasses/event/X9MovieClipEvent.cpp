//
//  X9MovieClipEvent.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/24.
//
//

#include "X9MovieClipEvent.h"
#include "../X9Array.h"

X9ValueObject* baseGet_movieClipEvent_frameIndex(X9RunObject* target)
{
    X9MovieClipEvent* event = dynamic_cast<X9MovieClipEvent*>(target);
    return X9ValueObject::createWithNumber(event->frameIndex);
}
X9ValueObject* baseGet_movieClipEvent_frameLabel(X9RunObject* target)
{
    X9MovieClipEvent* event = dynamic_cast<X9MovieClipEvent*>(target);
    return X9ValueObject::createWithString(event->frameLabel);
}
X9ValueObject* baseGet_movieClipEvent_loop(X9RunObject* target)
{
    X9MovieClipEvent* event = dynamic_cast<X9MovieClipEvent*>(target);
    return X9ValueObject::createWithNumber(event->loop);
}
X9ValueObject* baseGet_movieClipEvent_loopIndex(X9RunObject* target)
{
    X9MovieClipEvent* event = dynamic_cast<X9MovieClipEvent*>(target);
    return X9ValueObject::createWithNumber(event->loopIndex);
}



string X9MovieClipEvent::ENTER_FRAME = "enterFrame";
string X9MovieClipEvent::ENTER_LABEL = "enterLabel";
string X9MovieClipEvent::PLAY_COMPLETE = "playComplete";

void X9MovieClipEvent::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseGet(movieClipEvent_, frameIndex);
    x9_AddBaseGet(movieClipEvent_, frameLabel);
    x9_AddBaseGet(movieClipEvent_, loop);
    x9_AddBaseGet(movieClipEvent_, loopIndex);
}
void X9MovieClipEvent::setConstValues(X9ScriptClassData* classData)
{
    x9_AddConstValue(String, ENTER_FRAME, ENTER_FRAME);
    x9_AddConstValue(String, ENTER_LABEL, ENTER_LABEL);
    x9_AddConstValue(String, PLAY_COMPLETE, PLAY_COMPLETE);
}

X9MovieClipEvent* X9MovieClipEvent::newMovieClipEvent(X9Library* library,const string& type, int frameIndex, int loop, int loopIndex, const string& label)
{
    vector<X9ValueObject*> vs;
    vs.push_back(X9ValueObject::createWithString(type));
    X9MovieClipEvent* event = dynamic_cast<X9MovieClipEvent*>(library->createObject("MovieClipEvent",vs));
    event->frameIndex = frameIndex;
    event->frameLabel = label;
    event->loop = loop;
    event->loopIndex = loopIndex;
    return event;
}
X9MovieClipEvent* X9MovieClipEvent::newMovieClipEvent(X9Library* library,const string& type, int frameIndex, int loop, int loopIndex)
{
    vector<X9ValueObject*> vs;
    vs.push_back(X9ValueObject::createWithString(type));
    X9MovieClipEvent* event = dynamic_cast<X9MovieClipEvent*>(library->createObject("MovieClipEvent",vs));
    event->frameIndex = frameIndex;
    event->frameLabel = "";
    event->loop = loop;
    event->loopIndex = loopIndex;
    return event;
}
X9MovieClipEvent* X9MovieClipEvent::create()
{
    CREATE_NEW_AUTO_OBJ(X9MovieClipEvent,);
}
X9MovieClipEvent::X9MovieClipEvent():X9Event("X9MovieClipEvent")
{
    x9_setCtor(MovieClipEvent);
}
void X9MovieClipEvent::removed()
{
    X9Event::removed();
}
void X9MovieClipEvent::initObject(const vector<X9ValueObject*>& vs)
{
    runSuperCtor("Event",vs);
}