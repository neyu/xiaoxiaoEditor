//
//  X9Rect.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/5.
//
//

#ifndef X9Rect_hpp
#define X9Rect_hpp

#include "X9Object.h"

class X9Rect : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    Rect rect;
//    float x;
//    float y;
//    float width;
//    float height;
public:
    static X9Rect* newRect(X9Library* library, float x = 0, float y = 0, float w = 0, float h = 0);
    static X9Rect* newRect(X9Library* library, const Rect& rect);
    static X9Rect* create();
    X9Rect();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual string toString()override;
};

#endif /* X9Rect_hpp */
