//
//  X9Point.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/5.
//
//

#include "X9Point.h"

X9ValueObject* base_point_distance(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 || values.size() == 2,"distance Error!!!");
    X9Point* point = dynamic_cast<X9Point*>(target);
    if(values.size() == 1)
    {
        X9ASSERT(values[0]->isObject<X9Point*>(),"distance Error!!!");
        return X9ValueObject::createWithNumber(point->point.distance(values[0]->getObject<X9Point*>()->point));
    }
    X9ASSERT(values[0]->isNumber() && values[1]->isNumber(),"distance Error!!!");
    return X9ValueObject::createWithNumber(point->point.distance(Vec2(values[0]->getNumber(),values[1]->getNumber())));
}
X9ValueObject* base_point_normalize(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"normalize Error!!!");
    X9Point* point = dynamic_cast<X9Point*>(target);
    float l = sqrt(point->point.x*point->point.x+point->point.y*point->point.y);
    point->point.x *= values[0]->getNumber()/l;
    point->point.y *= values[0]->getNumber()/l;
    return X9ValueObject::create();
}
void baseSet_point_x(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set x Error!!!");
    X9Point* point = dynamic_cast<X9Point*>(target);
    point->point.x = value->getNumber();
}
X9ValueObject* baseGet_point_x(X9RunObject* target)
{
    X9Point* point = dynamic_cast<X9Point*>(target);
    return X9ValueObject::createWithNumber(point->point.x);
}
void baseSet_point_y(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set y Error!!!");
    X9Point* point = dynamic_cast<X9Point*>(target);
    point->point.y = value->getNumber();
}
X9ValueObject* baseGet_point_y(X9RunObject* target)
{
    X9Point* point = dynamic_cast<X9Point*>(target);
    return X9ValueObject::createWithNumber(point->point.y);
}
X9ValueObject* baseGet_point_length(X9RunObject* target)
{
    X9Point* point = dynamic_cast<X9Point*>(target);
    return X9ValueObject::createWithNumber(sqrt(point->point.y*point->point.y+point->point.x*point->point.x));
}
void X9Point::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(point_,distance);
    x9_AddBaseFunc(point_,normalize);
    x9_AddBaseSGet(point_,x);
    x9_AddBaseSGet(point_,y);
    x9_AddBaseGet(point_,length);
}
void X9Point::setConstValues(X9ScriptClassData* classData)
{
    
}
X9Point* X9Point::newPoint(X9Library* library, const Vec2& p)
{
    X9Point* point = dynamic_cast<X9Point*>(library->createObject("Point"));
    point->point.set(p);
    return point;
}
X9Point* X9Point::newPoint(X9Library* library, float x, float y)
{
    X9Point* point = dynamic_cast<X9Point*>(library->createObject("Point"));
    point->point.x = x;
    point->point.y = y;
    return point;
}
X9Point* X9Point::create()
{
    CREATE_NEW_AUTO_OBJ(X9Point,);
}
X9Point::X9Point():X9Object("X9Point")
{
    x9_setCtor(Point);
    point.x = 0;
    point.y = 0;
}

void X9Point::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() <= 2,"new Point Error!!!");
    if(vs.size() == 1 && vs[0]->isObject<X9Point*>())
    {
        point = vs[0]->getObject<X9Point*>()->point;
    }else{
        for (int i = 0; i<vs.size(); i++) {
            X9ASSERT(vs[i]->isNumber(),"new Size Error!!!");
        }
        point.x = vs.size() > 0 ? vs[0]->getNumber() : 0;
        point.y = vs.size() > 1 ? vs[1]->getNumber() : 0;
    }
}

X9Point* X9Point::operator+(X9Point* v)
{
    vector<X9ValueObject*> vs;
    vs.push_back(X9ValueObject::createWithNumber(point.x+v->point.x));
    vs.push_back(X9ValueObject::createWithNumber(point.y+v->point.y));
    return dynamic_cast<X9Point*>(getLibrary()->createObject("Point", vs));
}
X9Point* X9Point::operator-(X9Point* v)
{
    vector<X9ValueObject*> vs;
    vs.push_back(X9ValueObject::createWithNumber(point.x-v->point.x));
    vs.push_back(X9ValueObject::createWithNumber(point.y-v->point.y));
    return dynamic_cast<X9Point*>(getLibrary()->createObject("Point", vs));
}
string X9Point::toString()
{
    std::ostringstream oss;
    oss<<"[Point x:";
    oss<<point.x;
    oss<<", y:";
    oss<<point.y;
    oss<<"]";
    return oss.str();
}