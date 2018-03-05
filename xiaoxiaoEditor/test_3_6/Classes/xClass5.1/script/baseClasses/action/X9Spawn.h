//
//  X9Spawn.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9Spawn_hpp
#define X9Spawn_hpp

#include "X9Action.h"
class X9Spawn : public X9Action
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    vector<X9Action*> actions;
    vector<bool> playings;
public:
    static X9Spawn* create();
    X9Spawn();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual void setTarget(X9DisplayObject* target)override;
    virtual void makeTime()override;
    virtual void updateAction(float v)override;
    virtual X9Action* clone()override;
};

#endif /* X9Spawn_hpp */
