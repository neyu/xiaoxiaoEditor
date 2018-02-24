//
//  X9MoveTo.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9MoveTo.h"
#include "../X9DisplayObject.h"
#include "../../../XMath.h"

void X9MoveTo::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9MoveTo::setConstValues(X9ScriptClassData* classData)
{
    
}

X9MoveTo* X9MoveTo::newMoveTo(X9Library* library,float time, float x, float y)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithNumber(time));
    values.push_back(X9ValueObject::createWithNumber(x));
    values.push_back(X9ValueObject::createWithNumber(y));
    return dynamic_cast<X9MoveTo*>(library->createObject("MoveTo", values));
}

X9_CPP_CREATE(MoveTo,Action)

void X9MoveTo::removed()
{
    X9Action::removed();
}
void X9MoveTo::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() == 3 && vs[0]->isNumber() && vs[1]->isNumber() && vs[2]->isNumber(),"new MoveTo Error!!!");
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(vs[0]->clone());
    runSuperCtor("Action",timeVs);
    to.x = vs[1]->getNumber();
    to.y = vs[2]->getNumber();
}
void X9MoveTo::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
    from.x = target->getNode()->getPositionX();
    from.y = target->getNode()->getPositionY();
}
void X9MoveTo::updateAction(float v)
{
    target->getNode()->setPosition(XMath::mix(from, to, v));
}
X9Action* X9MoveTo::clone()
{
    return newMoveTo(getLibrary(),time,to.x,to.y);
}