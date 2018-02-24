//
//  X9Color.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/10.
//
//

#ifndef X9Color_hpp
#define X9Color_hpp

#include "X9Object.h"

class X9Color : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    Color3B _color;
public:
    static X9Color* newColor(X9Library* library, int r = 255, int g = 255, int b = 255);
    static X9Color* create();
    X9Color();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
};

#endif /* X9Color_hpp */
