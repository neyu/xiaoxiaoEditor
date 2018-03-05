//
//  X9ColorTo.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9ColorTo.h"
#include "../X9DisplayObject.h"
#include "../../../XMath.h"

void X9ColorTo::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9ColorTo::setConstValues(X9ScriptClassData* classData)
{
    
}

X9ColorTo* X9ColorTo::newColorTo(X9Library* library,float time, float R, float G, float B)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithNumber(time));
    values.push_back(X9ValueObject::createWithNumber(R));
    values.push_back(X9ValueObject::createWithNumber(G));
    values.push_back(X9ValueObject::createWithNumber(B));
    return dynamic_cast<X9ColorTo*>(library->createObject("ColorTo", values));
}

X9_CPP_CREATE(ColorTo,Action)

void X9ColorTo::removed()
{
    X9Action::removed();
}
void X9ColorTo::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() == 4 && vs[0]->isNumber() && vs[1]->isNumber() && vs[2]->isNumber() && vs[3]->isNumber(),"new MoveTo Error!!!");
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(vs[0]->clone());
    runSuperCtor("Action",timeVs);
    to.r = MAX(0,MIN(255,vs[1]->getNumber()));
    to.g = MAX(0,MIN(255,vs[2]->getNumber()));
    to.b = MAX(0,MIN(255,vs[3]->getNumber()));
}
void X9ColorTo::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
    from = target->getNode()->getColor();
}
void X9ColorTo::updateAction(float v)
{
    float ar = MAX(0,MIN(255,XMath::mix(from.r,to.r,v)));
    float ag = MAX(0,MIN(255,XMath::mix(from.g,to.b,v)));
    float ab = MAX(0,MIN(255,XMath::mix(from.g,to.b,v)));
    target->getNode()->setColor(Color3B(ar,ag,ab));
}
X9Action* X9ColorTo::clone()
{
    return newColorTo(getLibrary(),time,to.r,to.g,to.b);
}
