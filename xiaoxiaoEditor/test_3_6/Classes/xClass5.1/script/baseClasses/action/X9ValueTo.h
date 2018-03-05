//
//  X9ValueTo.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9ValueTo_hpp
#define X9ValueTo_hpp

#include "X9Action.h"
class X9ValueTo : public X9Action
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    string name;
    float fromValue;
    float toValue;
public:
    static X9ValueTo* newValueTo(X9Library* library,float time, const string& name, float value);
    static X9ValueTo* create();
    X9ValueTo();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual void setTarget(X9DisplayObject* target)override;
    virtual void updateAction(float v)override;
    virtual X9Action* clone()override;
};


#endif /* X9ValueTo_hpp */
