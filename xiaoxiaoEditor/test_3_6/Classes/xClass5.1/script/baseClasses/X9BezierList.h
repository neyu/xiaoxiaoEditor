//
//  X9Bezier.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/12/29.
//
//

#ifndef X9Bezier_hpp
#define X9Bezier_hpp

#include "X9Object.h"
#include "../../XBezierList.h"
class X9BezierList : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
    static float PRECISION;
public:
    XBezierList* bezierList;
    Vec2 lineA;
    Vec2 lineB;
    bool isBezier;
    float length;
public:
    static X9BezierList* create();
    X9BezierList();
    ~X9BezierList();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    
    
    float getAngle(float time);
    Vec2 getPoint(float time);
    float getAngleByDistance(float distance);
    Vec2 getPointByDistance(float distance);
    float getClosest(const Vec2& p);
    float getSegmentLength(float time);
    float getTimeByDistance(float distance);
    
};


#endif /* X9Bezier_hpp */
