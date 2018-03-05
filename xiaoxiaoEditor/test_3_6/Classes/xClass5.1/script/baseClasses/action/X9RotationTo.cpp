//
//  X9RotationTo.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9RotationTo.h"

#include "../X9DisplayObject.h"
#include "../../../XMath.h"

void X9RotationTo::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9RotationTo::setConstValues(X9ScriptClassData* classData)
{
    
}

X9RotationTo* X9RotationTo::newRotationTo(X9Library* library,float time, float r)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithNumber(time));
    values.push_back(X9ValueObject::createWithNumber(r));
    return dynamic_cast<X9RotationTo*>(library->createObject("RotationTo", values));
}

X9_CPP_CREATE(RotationTo,Action)

void X9RotationTo::removed()
{
    X9Action::removed();
}
void X9RotationTo::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() == 2 && vs[0]->isNumber() && vs[1]->isNumber(),"new MoveTo Error!!!");
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(vs[0]->clone());
    runSuperCtor("Action",timeVs);
    to = vs[1]->getNumber();
}
void X9RotationTo::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
    from = target->getNode()->getRotationSkewX();
}
void X9RotationTo::updateAction(float v)
{
    float rv = XMath::mix(from, to, v);
    target->getNode()->setRotationSkewX(rv);
    target->getNode()->setRotationSkewY(rv);
}
X9Action* X9RotationTo::clone()
{
    return newRotationTo(getLibrary(),time,to);
}