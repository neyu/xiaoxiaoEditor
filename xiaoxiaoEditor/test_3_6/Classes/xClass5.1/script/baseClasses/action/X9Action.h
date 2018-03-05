//
//  X9Action.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#ifndef X9Action_hpp
#define X9Action_hpp



#include "X9Action.h"
#define ACT_CLASS_1(_N_) \
class _N_ : public X9Action{ \
    void initObject(const vector<X9ValueObject*>& vs); \
public:

#define ACT_CLASS_2(_N_) \
public: \
    static _N_* create(); \
    _N_(); \
    static void setBaseFunctions(X9Library* library, const string& className); \
    static void setConstValues(X9ScriptClassData* classData); \
    virtual void removed()override; \
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);}; \
    virtual void setTarget(X9DisplayObject* target)override; \
    virtual void updateAction(float v)override; \
    virtual X9Action* clone()override; \
};


#include "X9Object.h"
class X9DisplayObject;
class X9Action : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    X9DisplayObject* target;
    float time;
    float startTime;
    bool isRuning;
public:
    static X9Action* newAction(X9Library* library, float time);
    static X9Action* create();
    X9Action(const string& key);
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual void setTarget(X9DisplayObject* target);
    virtual void makeTime(){};
    virtual void updateAction(float v){};
    virtual X9Action* clone();
};

#endif /* X9Action_hpp */
