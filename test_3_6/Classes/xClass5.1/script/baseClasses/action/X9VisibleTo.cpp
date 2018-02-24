//
//  X9VisibleTo.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9VisibleTo.h"
#include "../X9DisplayObject.h"

void X9VisibleTo::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9VisibleTo::setConstValues(X9ScriptClassData* classData)
{
    
}

X9VisibleTo* X9VisibleTo::newVisibleTo(X9Library* library,float time, bool value)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithNumber(time));
    values.push_back(X9ValueObject::createWithBool(value));
    return dynamic_cast<X9VisibleTo*>(library->createObject("VisibleTo", values));
}

X9_CPP_CREATE(VisibleTo,Action)

void X9VisibleTo::removed()
{
    X9Action::removed();
}
void X9VisibleTo::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() == 2 && vs[0]->isNumber() && vs[1]->isBool(),"new MoveTo Error!!!");
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(vs[0]->clone());
    runSuperCtor("Action",timeVs);
    to = vs[1]->getBool();
}
void X9VisibleTo::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
}
void X9VisibleTo::updateAction(float v)
{
    if (v >= 1) {
        target->getNode()->setVisible(to);
    }
}
X9Action* X9VisibleTo::clone()
{
    return newVisibleTo(getLibrary(),time,to);
}
