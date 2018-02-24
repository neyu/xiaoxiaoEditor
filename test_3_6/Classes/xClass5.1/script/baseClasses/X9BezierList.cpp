//
//  X9Bezier.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/12/29.
//
//

#include "X9BezierList.h"
#include "X9Point.h"
#include "../../XMath.h"
#include "X9Array.h"

X9ValueObject* base_bezier_getAngle(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"getRadian Error!!!");
    X9BezierList* bezier = dynamic_cast<X9BezierList*>(target);
    return X9ValueObject::createWithNumber(bezier->getAngle(values[0]->getNumber()));
}
X9ValueObject* base_bezier_getPoint(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"getPoint Error!!!");
    X9BezierList* bezier = dynamic_cast<X9BezierList*>(target);
    return X9ValueObject::createWithObject(X9Point::newPoint(target->getLibrary(), bezier->getPoint(values[0]->getNumber())));
}
X9ValueObject* base_bezier_getAngleByDistance(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"getAngleByDistance Error!!!");
    X9BezierList* bezier = dynamic_cast<X9BezierList*>(target);
    return X9ValueObject::createWithNumber(bezier->getAngleByDistance(values[0]->getNumber()));
}
X9ValueObject* base_bezier_getPointByDistance(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"getPointByDistance Error!!!");
    X9BezierList* bezier = dynamic_cast<X9BezierList*>(target);
    return X9ValueObject::createWithObject(X9Point::newPoint(target->getLibrary(), bezier->getPointByDistance(values[0]->getNumber())));
}
X9ValueObject* base_bezier_getClosest(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() > 0 && values.size() < 3,"getClosest Error!!!");
    X9BezierList* bezier = dynamic_cast<X9BezierList*>(target);
    Vec2 p;
    if(values.size() == 1)
    {
        X9ASSERT(values[0]->isObject<X9Point*>(),"getClosest Error!!!");
        p = values[0]->getObject<X9Point*>()->point;
    }else
    {
        X9ASSERT(values[0]->isNumber() && values[1]->isNumber(),"getClosest Error!!!");
        p.x = values[0]->getNumber();
        p.y = values[1]->getNumber();
    }
    return X9ValueObject::createWithNumber(bezier->getClosest(p));
}
X9ValueObject* base_bezier_getSegmentLength(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"getSegmentLength Error!!!");
    X9BezierList* bezier = dynamic_cast<X9BezierList*>(target);
    return X9ValueObject::createWithNumber(bezier->getSegmentLength(values[0]->getNumber()));
}
X9ValueObject* base_bezier_getTimeByDistance(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"getTimeByDistance Error!!!");
    X9BezierList* bezier = dynamic_cast<X9BezierList*>(target);
    return X9ValueObject::createWithNumber(bezier->getTimeByDistance(values[0]->getNumber()));
}


X9ValueObject* baseGet_bezier_start(X9RunObject* target)
{
    X9BezierList* bl = dynamic_cast<X9BezierList*>(target);
    if (bl->isBezier) {
        return X9ValueObject::createWithObject(X9Point::newPoint(target->getLibrary(), bl->bezierList->_list.front().start));
    }
    return X9ValueObject::createWithObject(X9Point::newPoint(target->getLibrary(), bl->lineA));
}
X9ValueObject* baseGet_bezier_end(X9RunObject* target)
{
    X9BezierList* bl = dynamic_cast<X9BezierList*>(target);
    if (bl->isBezier) {
        return X9ValueObject::createWithObject(X9Point::newPoint(target->getLibrary(), bl->bezierList->_list.back().end));
    }
    return X9ValueObject::createWithObject(X9Point::newPoint(target->getLibrary(), bl->lineB));
}
X9ValueObject* baseGet_bezier_length(X9RunObject* target)
{
    X9BezierList* bl = dynamic_cast<X9BezierList*>(target);
    return X9ValueObject::createWithNumber(bl->length);
}
void X9BezierList::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(bezier_,getAngle);
    x9_AddBaseFunc(bezier_,getPoint);
    x9_AddBaseFunc(bezier_,getAngleByDistance);
    x9_AddBaseFunc(bezier_,getPointByDistance);
    x9_AddBaseFunc(bezier_,getClosest);
    x9_AddBaseFunc(bezier_,getSegmentLength);
    x9_AddBaseFunc(bezier_,getTimeByDistance);
    
    x9_AddBaseGet(bezier_,start);
    x9_AddBaseGet(bezier_,end);
    x9_AddBaseGet(bezier_,length);
}
void X9BezierList::setConstValues(X9ScriptClassData* classData)
{
    
}
float X9BezierList::PRECISION = 1e-10;
X9BezierList* X9BezierList::create()
{
    CREATE_NEW_AUTO_OBJ(X9BezierList,);
}
X9BezierList::X9BezierList():X9Object("X9BezierList")
{
    x9_setCtor(BezierList);
    isBezier = false;
    bezierList = new XBezierList();
}
X9BezierList::~X9BezierList()
{
    delete bezierList;
}

void X9BezierList::initObject(const vector<X9ValueObject*>& vs)
{
    
    X9ASSERT(vs.size() == 1 && vs[0]->isObject<X9Array*>(),"new BezierList Error!!!");
    const vector<X9ValueObject*>& _vs = vs[0]->getObject<X9Array*>()->children;
    X9ASSERT(_vs.size() >= 2,"new BezierList Error!!!");
    
    if(_vs.size() == 2)
    {
        X9ASSERT(_vs[0]->isObject<X9Point*>() && _vs[1]->isObject<X9Point*>(),"new BezierList Error!!!");
        isBezier = false;
        lineA = _vs[0]->getObject<X9Point*>()->point;
        lineB = _vs[1]->getObject<X9Point*>()->point;
        length = lineA.distance(lineB);
    }else if(_vs.size() == 4)
    {
        X9ASSERT(_vs[0]->isNumber() && _vs[1]->isNumber() && _vs[2]->isNumber() && _vs[3]->isNumber(),"new BezierList Error!!!");
        isBezier = false;
        lineA.x = _vs[0]->getNumber();
        lineA.y = _vs[1]->getNumber();
        lineB.x = _vs[2]->getNumber();
        lineB.y = _vs[3]->getNumber();
        length = lineA.distance(lineB);
    }else
    {
        vector<Vec2> points;
        if((int(_vs.size()))%2 == 1)
        {
            for (int i = 0; i<_vs.size(); i++) {
                X9ASSERT(_vs[i]->isObject<X9Point*>(),"new BezierList Error!!!");
                points.push_back(_vs[i]->getObject<X9Point*>()->point);
            }
        }else
        {
            for (int i = 0; i<_vs.size(); i+=2) {
                X9ASSERT(_vs[i]->isNumber() && _vs[i+1]->isNumber(),"new BezierList Error!!!");
                points.push_back(Vec2(_vs[i]->getNumber(),_vs[i+1]->getNumber()));
            }
        }
        if (points.size() == 2) {
            isBezier = false;
            lineA = points[0];
            lineB = points[1];
            length = lineA.distance(lineB);
        }else
        {
            isBezier = true;
            for (int i = 1; i<points.size(); i+=2) {
                bezierList->_list.push_back(XBezier(points[i-1], points[i], points[i+1], true));
            }
            bezierList->reset();
            length = bezierList->length();
        }
    }
}

float X9BezierList::getAngle(float time)
{
    if(isBezier)
    {
        return bezierList->getTangentAngle(time);
    }
    return atan2(lineA.y-lineB.y, lineB.x-lineA.x);
}
Vec2 X9BezierList::getPoint(float v)
{
    if (isBezier) {
        return bezierList->getPoint(v);
    }
    return XMath::mix(lineA, lineB, v);
}
float X9BezierList::getAngleByDistance(float distance)
{
    if(isBezier)
    {
        float t = bezierList->getTimeByDistance(distance);
        return bezierList->getTangentAngle(t);
    }
    return atan2(lineA.y-lineB.y, lineB.x-lineA.x);
}
Vec2 X9BezierList::getPointByDistance(float distance)
{
    if(isBezier) {
        float t = bezierList->getTimeByDistance(distance);
        log("distance:%f, time:%f",distance,t);
        return getPoint(t);
    }
    return XMath::mix(lineA, lineB, distance/length);
}
float X9BezierList::getClosest(const Vec2& fromPoint)
{
    if (isBezier) {
        return bezierList->getClosest(fromPoint);
    }
    if (lineA == lineB) {
        return 0;
    }
    if (lineA.x == lineB.x) {
        return XMath::umix(lineA.y, lineB.y, fromPoint.y);
    }
    if (lineA.y == lineB.y) {
        return XMath::umix(lineA.x, lineB.x, fromPoint.x);
    }
    float A = (lineA.y-lineB.y)/(lineA.x- lineB.x);
    float B = (lineA.y-A*lineA.x);
    float m = fromPoint.x + A*fromPoint.y;
    float x = (m-A*B)/(A*A + 1);
    return XMath::umix(lineA.x, lineB.x, x);
}

float X9BezierList::getSegmentLength(float time)
{
    if (isBezier) {
        return bezierList->getSegmentLength(time);
    }
    return length*time;
}
float X9BezierList::getTimeByDistance(float distance)
{
    if (isBezier) {
        return bezierList->getTimeByDistance(distance);
    }
    return distance/length;
}
