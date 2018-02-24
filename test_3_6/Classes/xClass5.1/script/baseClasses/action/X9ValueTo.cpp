//
//  X9ValueTo.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9ValueTo.h"
#include "X9DisplayObject.h"
#include "../../../XMath.h"

void X9ValueTo::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9ValueTo::setConstValues(X9ScriptClassData* classData)
{
    
}

X9ValueTo* X9ValueTo::newValueTo(X9Library* library,float time, const string& name, float value)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithNumber(time));
    values.push_back(X9ValueObject::createWithString(name));
    values.push_back(X9ValueObject::createWithNumber(value));
    return dynamic_cast<X9ValueTo*>(library->createObject("ValueTo", values));
}

X9_CPP_CREATE(ValueTo,Action)

void X9ValueTo::removed()
{
    X9Action::removed();
}
void X9ValueTo::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() == 3 && vs[0]->isNumber() && vs[1]->isString() && vs[2]->isNumber(),"new ValueTo Error!!!");
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(vs[0]->clone());
    runSuperCtor("Action",timeVs);
    name = vs[1]->getString();
    toValue = vs[2]->getNumber();
}
void X9ValueTo::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
    fromValue = target->getValue(MemberType::MT_PROPERTY, name)->getNumber();
}
void X9ValueTo::updateAction(float v)
{
    target->setValue(MemberType::MT_PROPERTY, name, X9ValueObject::createWithNumber(XMath::mix(fromValue, toValue, v)));
}
X9Action* X9ValueTo::clone()
{
    return newValueTo(getLibrary(),time,name,toValue);
}