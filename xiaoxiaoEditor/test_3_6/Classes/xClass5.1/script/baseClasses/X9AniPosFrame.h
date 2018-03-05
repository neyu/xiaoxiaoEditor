//
//  X9AniPosFrame.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/12/12.
//
//

#ifndef X9AniPosFrame_hpp
#define X9AniPosFrame_hpp


#include "X9Object.h"
class X9PosFrame;
class X9AniPosFrame : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    bool isEmpty;
    Vec2 a;
    Vec2 b;
    float r;
    bool reverse;
public:
    static X9AniPosFrame* newAniPosFrame(X9Library* library, X9PosFrame* posFrame);
    static X9AniPosFrame* newAniPosFrame(X9Library* library);
//    static X9AniPosFrame* newPoint(X9Library* library, const Vec2& p);
    static X9AniPosFrame* create();
    X9AniPosFrame();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    
    virtual string toString()override;
};


#endif /* X9AniPosFrame_hpp */
