//
//  X9SkewTo.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9SkewTo.h"
#include "../X9DisplayObject.h"
#include "../../../XMath.h"

void X9SkewTo::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9SkewTo::setConstValues(X9ScriptClassData* classData)
{
    
}

X9SkewTo* X9SkewTo::newSkewTo(X9Library* library,float time, float x, float y)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithNumber(time));
    values.push_back(X9ValueObject::createWithNumber(x));
    values.push_back(X9ValueObject::createWithNumber(y));
    return dynamic_cast<X9SkewTo*>(library->createObject("SkewTo", values));
}

X9_CPP_CREATE(SkewTo,Action)

void X9SkewTo::removed()
{
    X9Action::removed();
}
void X9SkewTo::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() == 3 && vs[0]->isNumber() && vs[1]->isNumber() && vs[2]->isNumber(),"new MoveTo Error!!!");
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(vs[0]->clone());
    runSuperCtor("Action",timeVs);
    to.x = vs[1]->getNumber();
    to.y = vs[2]->getNumber();
}
void X9SkewTo::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
    from.x = target->getNode()->getRotationSkewX();
    from.y = target->getNode()->getRotationSkewY();
}
void X9SkewTo::updateAction(float v)
{
    Vec2 rv = XMath::mix(from, to, v);
    target->getNode()->setRotationSkewX(rv.x);
    target->getNode()->setRotationSkewY(rv.y);
}
X9Action* X9SkewTo::clone()
{
    return newSkewTo(getLibrary(),time,to.x,to.y);
}
