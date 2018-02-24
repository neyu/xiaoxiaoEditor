//
//  X9Spawn.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9Spawn.h"
#include "../X9DisplayObject.h"
#include "../X9Array.h"

X9ValueObject* base_spawn_addAction(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() >= 1 && values[0]->isObject<X9Action*>(),"addAction Error!!!");
    X9Spawn* spawn = dynamic_cast<X9Spawn*>(target);
    X9ASSERT(!spawn->isRuning,"addAction Error!!!");
    for (int i = 0; i<values.size(); i++) {
        X9ASSERT(values[1]->isObject<X9Action*>(),"addAction Error!!!");
        spawn->actions.push_back(values[1]->getObject<X9Action*>());
        spawn->actions.back()->use();
    }
    return X9ValueObject::create();
}
X9ValueObject* base_spawn_addActions(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Array*>(),"addAction Error!!!");
    X9Spawn* spawn = dynamic_cast<X9Spawn*>(target);
    
    X9ASSERT(!spawn->isRuning,"addAction Error!!!");
    for (int i = 0; i<values[0]->getObject<X9Array*>()->children.size(); i++) {
        X9ASSERT(values[0]->getObject<X9Array*>()->children[i]->isObject<X9Action*>(),"addAction Error!!!");
        spawn->actions.push_back(values[0]->getObject<X9Array*>()->children[i]->getObject<X9Action*>());
        spawn->actions.back()->use();
    }
    return X9ValueObject::create();
}

void X9Spawn::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(spawn_,addAction);
    x9_AddBaseFunc(spawn_,addActions);
}
void X9Spawn::setConstValues(X9ScriptClassData* classData)
{
    
}

X9_CPP_CREATE(Spawn,Action)

void X9Spawn::removed()
{
    for (int i = 0; i<actions.size(); i++) {
        actions[i]->nonuse();
    }
    actions.clear();
    X9Action::removed();
}
void X9Spawn::initObject(const vector<X9ValueObject*>& vs)
{
    vector<X9ValueObject*> timeVs;
    float t = 0;
    for (int i = 0; i<vs.size(); i++) {
        X9ASSERT(vs[i]->isObject<X9Action*>(),"new Spawn Error!!!");
        actions.push_back(vs[i]->getObject<X9Action*>());
        actions[i]->use();
        t = MAX(t,actions[i]->time);
    }
    playings.resize(actions.size());
    timeVs.push_back(X9ValueObject::createWithNumber(t));
    runSuperCtor("Action",timeVs);
}
void X9Spawn::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
    for (int i = 0; i<actions.size(); i++) {
        actions[i]->setTarget(target);
        actions[i]->startTime = 0;
        playings[i] = true;
    }
}
void X9Spawn::makeTime()
{
    time = 0;
    for (int i = 0; i<actions.size(); i++) {
        actions[i]->makeTime();
        time = MAX(time,actions[i]->time);
    }
}
void X9Spawn::updateAction(float v)
{
    float t = v*time;
    for (int i = 0; i<actions.size(); i++) {
        X9Action* act = actions[i];
        if (playings[i]) {
            if (t >= act->time) {
                playings[i] = false;
                act->updateAction(1);
            }else{
                act->updateAction(t/act->time);
            }
        }
    }
}
X9Action* X9Spawn::clone()
{
    X9Spawn* act = dynamic_cast<X9Spawn*>(getLibrary()->createObject("Spawn"));
    for (int i = 0; i<actions.size(); i++) {
        act->actions.push_back(actions[i]->clone());
        act->actions[i]->use();
    }
    return act;
}