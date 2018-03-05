//
//  X9SkewBy.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9SkewBy.h"
#include "../X9DisplayObject.h"
#include "../../../XMath.h"

void X9SkewBy::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9SkewBy::setConstValues(X9ScriptClassData* classData)
{
    
}

X9SkewBy* X9SkewBy::newSkewBy(X9Library* library,float time, float x, float y)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithNumber(time));
    values.push_back(X9ValueObject::createWithNumber(x));
    values.push_back(X9ValueObject::createWithNumber(y));
    return dynamic_cast<X9SkewBy*>(library->createObject("SkewBy", values));
}

X9_CPP_CREATE(SkewBy,Action)

void X9SkewBy::removed()
{
    X9Action::removed();
}
void X9SkewBy::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() == 3 && vs[0]->isNumber() && vs[1]->isNumber() && vs[2]->isNumber(),"new MoveTo Error!!!");
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(vs[0]->clone());
    runSuperCtor("Action",timeVs);
    to.x = vs[1]->getNumber();
    to.y = vs[2]->getNumber();
}
void X9SkewBy::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
    from.x = target->getNode()->getRotationSkewX();
    from.y = target->getNode()->getRotationSkewY();
    to += from;
}
void X9SkewBy::updateAction(float v)
{
    Vec2 rv = XMath::mix(from, to, v);
    target->getNode()->setRotationSkewX(rv.x);
    target->getNode()->setRotationSkewY(rv.y);
}
X9Action* X9SkewBy::clone()
{
    return newSkewBy(getLibrary(),time,to.x,to.y);
}

