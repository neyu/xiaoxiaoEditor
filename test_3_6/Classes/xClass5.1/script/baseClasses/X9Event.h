//
//  X9Event.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/1.
//
//

#ifndef X9Event_hpp
#define X9Event_hpp

#include "X9Object.h"

class X9Event : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    string eventType;
    X9ValueObject* targetValueObj;
public:
    static X9Event* newEvent(X9Library* library,const string& type);
    static X9Event* create();
    X9Event(const string& key);
    
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void use()override;
    virtual void nonuse()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    void setTarget(X9Object* obj);
    virtual string toString()override;
};

#endif /* X9Event_hpp */
