//
//  X9Vec3.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/2.
//
//

#ifndef X9Vec3_hpp
#define X9Vec3_hpp

#include "X9Object.h"

class X9Vec3 : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    Vec3 v;
public:
    static X9Vec3* newVec3(X9Library* library, float x = 0, float y = 0, float z = 0);
    static X9Vec3* create();
    X9Vec3();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    
    X9Vec3* operator+(X9Vec3* p);
    X9Vec3* operator-(X9Vec3* p);
    
    virtual string toString()override;
};
#endif /* X9Vec3_hpp */
