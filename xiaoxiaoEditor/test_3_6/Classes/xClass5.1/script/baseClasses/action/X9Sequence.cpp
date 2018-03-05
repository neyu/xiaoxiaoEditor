//
//  X9Sequence.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9Sequence.h"
#include "../X9DisplayObject.h"
#include "../X9Array.h"
#include "../../../XMath.h"

X9ValueObject* base_sequence_addAction(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() >= 1 && values[0]->isObject<X9Action*>(),"addAction Error!!!");
    X9Sequence* sequence = dynamic_cast<X9Sequence*>(target);
    X9ASSERT(!sequence->isRuning,"addAction Error!!!");
    for (int i = 0; i<values.size(); i++) {
        X9ASSERT(values[1]->isObject<X9Action*>(),"addAction Error!!!");
        sequence->actions.push_back(values[1]->getObject<X9Action*>());
        sequence->actions.back()->use();
    }
    return X9ValueObject::create();
}
X9ValueObject* base_sequence_addActions(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Array*>(),"addAction Error!!!");
    X9Sequence* sequence = dynamic_cast<X9Sequence*>(target);
    
    X9ASSERT(!sequence->isRuning,"addAction Error!!!");
    for (int i = 0; i<values[0]->getObject<X9Array*>()->children.size(); i++) {
        X9ASSERT(values[0]->getObject<X9Array*>()->children[i]->isObject<X9Action*>(),"addAction Error!!!");
        sequence->actions.push_back(values[0]->getObject<X9Array*>()->children[i]->getObject<X9Action*>());
        sequence->actions.back()->use();
    }
    return X9ValueObject::create();
}
void X9Sequence::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(sequence_,addAction);
    x9_AddBaseFunc(sequence_,addActions);
}
void X9Sequence::setConstValues(X9ScriptClassData* classData)
{
    
}

X9_CPP_CREATE(Sequence,Action)

void X9Sequence::removed()
{
    for (int i = 0; i<actions.size(); i++) {
        actions[i]->nonuse();
    }
    actions.clear();
    X9Action::removed();
}
void X9Sequence::initObject(const vector<X9ValueObject*>& vs)
{
    vector<X9ValueObject*> timeVs;
    for (int i = 0; i<vs.size(); i++) {
        X9ASSERT(vs[i]->isObject<X9Action*>(),"new Sequence Error!!!");
        actions.push_back(vs[i]->getObject<X9Action*>());
        actions[i]->use();
    }
    timeVs.push_back(X9ValueObject::createWithNumber(0));
    runSuperCtor("Action",timeVs);
}
void X9Sequence::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
    actions[0]->setTarget(target);
    runIdx = 0;
    float t = 0;
    for (int i = 0; i<actions.size(); i++) {
        actions[i]->startTime = t;
        t += actions[i]->time;
    }
}
void X9Sequence::makeTime()
{
    time = 0;
    for (int i = 0; i<actions.size(); i++) {
        actions[i]->makeTime();
        time += actions[i]->time;
    }
}
void X9Sequence::updateAction(float v)
{
    float t = v*time;
    for (int i = runIdx; i<actions.size(); i++) {
        X9Action* act = actions[i];
        float at = act->startTime+act->time;
        if (t >= at) {
            act->updateAction(1);
            runIdx = i+1;
            if (runIdx < actions.size()) {
                actions[runIdx]->setTarget(target);
            }
        }else{
            act->updateAction(XMath::umix(act->startTime, at, t));
            break;
        }
    }
}
X9Action* X9Sequence::clone()
{
    X9Sequence* act = dynamic_cast<X9Sequence*>(getLibrary()->createObject("Sequence"));
    for (int i = 0; i<actions.size(); i++) {
        act->actions.push_back(actions[i]->clone());
        act->actions[i]->use();
    }
    return act;
}