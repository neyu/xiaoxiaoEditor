//
//  X9Math.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/7.
//
//

#ifndef X9Math_hpp
#define X9Math_hpp

#include "X9Object.h"

class X9Math : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    static X9Math* create();
    X9Math();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
};

#endif /* X9Math_hpp */
