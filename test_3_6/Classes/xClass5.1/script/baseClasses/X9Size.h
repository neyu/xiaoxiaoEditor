//
//  X9Size.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/10.
//
//

#ifndef X9Size_hpp
#define X9Size_hpp

#include "X9Object.h"

class X9Size : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    Size size;
public:
    static X9Size* newSize(X9Library* library, float w = 0, float h = 0);
    static X9Size* newSize(X9Library* library, const Size& size);
    static X9Size* create();
    X9Size();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    
    virtual string toString()override;
};

#endif /* X9Size_hpp */
