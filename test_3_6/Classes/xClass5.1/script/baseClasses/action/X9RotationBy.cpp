//
//  X9RotationBy.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9RotationBy.h"
#include "../X9DisplayObject.h"
#include "../../../XMath.h"

void X9RotationBy::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9RotationBy::setConstValues(X9ScriptClassData* classData)
{
    
}

X9RotationBy* X9RotationBy::newRotationBy(X9Library* library,float time, float r)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithNumber(time));
    values.push_back(X9ValueObject::createWithNumber(r));
    return dynamic_cast<X9RotationBy*>(library->createObject("RotationBy", values));
}

X9_CPP_CREATE(RotationBy,Action)

void X9RotationBy::removed()
{
    X9Action::removed();
}
void X9RotationBy::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() == 2 && vs[0]->isNumber() && vs[1]->isNumber(),"new MoveTo Error!!!");
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(vs[0]->clone());
    runSuperCtor("Action",timeVs);
    to = vs[1]->getNumber();
}
void X9RotationBy::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
    from = target->getNode()->getRotationSkewX();
    to+=from;
}
void X9RotationBy::updateAction(float v)
{
    float rv = XMath::mix(from, to, v);
    target->getNode()->setRotationSkewX(rv);
    target->getNode()->setRotationSkewY(rv);
}
X9Action* X9RotationBy::clone()
{
    return newRotationBy(getLibrary(),time,to);
}
