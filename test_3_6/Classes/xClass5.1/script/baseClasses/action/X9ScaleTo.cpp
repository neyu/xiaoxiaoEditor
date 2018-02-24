//
//  X9ScaleTo.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9ScaleTo.h"
#include "../X9DisplayObject.h"
#include "../../../XMath.h"

void X9ScaleTo::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9ScaleTo::setConstValues(X9ScriptClassData* classData)
{
    
}

X9ScaleTo* X9ScaleTo::newScaleTo(X9Library* library,float time, float x, float y)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithNumber(time));
    values.push_back(X9ValueObject::createWithNumber(x));
    values.push_back(X9ValueObject::createWithNumber(y));
    return dynamic_cast<X9ScaleTo*>(library->createObject("ScaleTo", values));
}

X9_CPP_CREATE(ScaleTo,Action)

void X9ScaleTo::removed()
{
    X9Action::removed();
}
void X9ScaleTo::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() == 3 && vs[0]->isNumber() && vs[1]->isNumber() && vs[2]->isNumber(),"new MoveTo Error!!!");
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(vs[0]->clone());
    runSuperCtor("Action",timeVs);
    to.x = vs[1]->getNumber();
    to.y = vs[2]->getNumber();
}
void X9ScaleTo::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
    from.x = target->getNode()->getScaleX();
    from.y = target->getNode()->getScaleY();
}
void X9ScaleTo::updateAction(float v)
{
    Vec2 sv = XMath::mix(from, to, v);
    target->getNode()->setScale(sv.x,sv.y);
}
X9Action* X9ScaleTo::clone()
{
    return newScaleTo(getLibrary(),time,to.x,to.y);
}