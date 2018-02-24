//
//  X9Sequence.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9Sequence_hpp
#define X9Sequence_hpp

#include "X9Action.h"
class X9Sequence : public X9Action
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    vector<X9Action*> actions;
    int runIdx;
public:
    static X9Sequence* create();
    X9Sequence();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual void setTarget(X9DisplayObject* target)override;
    virtual void makeTime()override;
    virtual void updateAction(float v)override;
    virtual X9Action* clone()override;
};

#endif /* X9Sequence_hpp */
