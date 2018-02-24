//
//  X9EventDispatcher.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/1.
//
//

#ifndef X9EventDispatcher_hpp
#define X9EventDispatcher_hpp

#include "X9Object.h"
class X9Event;
class EventFunctionObject
{
public:
    bool isRuning;
    bool isRemoved;
    X9ValueObject* func;
    EventFunctionObject();
    EventFunctionObject(X9ValueObject* func);
    ~EventFunctionObject();
};
class X9EventDispatcher : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    map<string,vector<EventFunctionObject*>> eventFunctions;
public:
    static X9EventDispatcher* create();
    X9EventDispatcher(const string& key);
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    
    virtual void checkAddEventListener(const string& type){};
    virtual void checkRemoveEventListener(const string& type){};
    bool hasEventListener(const string& type);
    void dispatchEvent(X9Event* event);
};

#endif /* X9EventDispatcher_hpp */
