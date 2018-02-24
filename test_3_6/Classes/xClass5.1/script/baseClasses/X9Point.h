//
//  X9Point.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/5.
//
//

#ifndef X9Point_hpp
#define X9Point_hpp

#include "X9Object.h"

class X9Point : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    Vec2 point;
public:
    static X9Point* newPoint(X9Library* library, float x = 0, float y = 0);
    static X9Point* newPoint(X9Library* library, const Vec2& p);
    static X9Point* create();
    X9Point();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    
    X9Point* operator+(X9Point* v);
    X9Point* operator-(X9Point* v);
    
    virtual string toString()override;
};

#endif /* X9Point_hpp */
