//
//  X9MoveBy.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9MoveBy.h"
#include "../X9DisplayObject.h"
#include "../../../XMath.h"

void X9MoveBy::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9MoveBy::setConstValues(X9ScriptClassData* classData)
{
    
}

X9MoveBy* X9MoveBy::newMoveBy(X9Library* library,float time, float x, float y)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithNumber(time));
    values.push_back(X9ValueObject::createWithNumber(x));
    values.push_back(X9ValueObject::createWithNumber(y));
    return dynamic_cast<X9MoveBy*>(library->createObject("MoveBy", values));
}

X9_CPP_CREATE(MoveBy,Action)

void X9MoveBy::removed()
{
    X9Action::removed();
}
void X9MoveBy::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() == 3 && vs[0]->isNumber() && vs[1]->isNumber() && vs[2]->isNumber(),"new MoveTo Error!!!");
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(vs[0]->clone());
    runSuperCtor("Action",timeVs);
    to.x = vs[1]->getNumber();
    to.y = vs[2]->getNumber();
}
void X9MoveBy::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
    from.x = target->getNode()->getPositionX();
    from.y = target->getNode()->getPositionY();
    to += from;
}
void X9MoveBy::updateAction(float v)
{
    target->getNode()->setPosition(XMath::mix(from, to, v));
}
X9Action* X9MoveBy::clone()
{
    return newMoveBy(getLibrary(),time,to.x,to.y);
}