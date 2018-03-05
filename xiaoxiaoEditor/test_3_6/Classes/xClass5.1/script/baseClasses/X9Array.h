//
//  X9Array.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/28.
//
//

#ifndef X9Array_hpp
#define X9Array_hpp

#include "X9Object.h"


class X9Array : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    vector<X9ValueObject*> children;
public:
    static X9Array* newArray(X9Library* library);
    static X9Array* create();
    X9Array();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    
    void push(X9ValueObject* v);
    void unshift(X9ValueObject* v);
    void pushNumber(float value);
    void pushBool(bool value);
    void pushString(const string& value);
    void pushObject(X9Object* value);
    void unshiftNumber(float value);
    void unshiftBool(bool value);
    void unshiftString(const string& value);
    void unshiftObject(X9Object* value);
    X9ValueObject* pop();
    X9ValueObject* shift();
    virtual string getJsonString()override;
    virtual string toString()override;
};

#endif /* X9Array_hpp */
