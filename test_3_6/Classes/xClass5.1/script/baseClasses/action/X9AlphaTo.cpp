//
//  X9AlphaTo.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9AlphaTo.h"
#include "../X9DisplayObject.h"
#include "../../../XMath.h"

void X9AlphaTo::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9AlphaTo::setConstValues(X9ScriptClassData* classData)
{
    
}

X9AlphaTo* X9AlphaTo::newAlphaTo(X9Library* library,float time, float A)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithNumber(time));
    values.push_back(X9ValueObject::createWithNumber(A));
    return dynamic_cast<X9AlphaTo*>(library->createObject("AlphaTo", values));
}

X9_CPP_CREATE(AlphaTo,Action)

void X9AlphaTo::removed()
{
    X9Action::removed();
}
void X9AlphaTo::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() == 2 && vs[0]->isNumber() && vs[1]->isNumber(),"new AlphaTo Error!!!");
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(vs[0]->clone());
    runSuperCtor("Action",timeVs);
    to = vs[1]->getNumber();
}
void X9AlphaTo::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
    from = target->getNode()->getOpacity()/255.0f;
}
void X9AlphaTo::updateAction(float v)
{
    target->getNode()->setOpacity(MAX(0,MIN(255,XMath::mix(from, to, v)*255)));
}
X9Action* X9AlphaTo::clone()
{
    return newAlphaTo(getLibrary(),time,to);
}

