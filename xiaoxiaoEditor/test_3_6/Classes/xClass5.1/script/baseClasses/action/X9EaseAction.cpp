//
//  X9EaseAction.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/28.
//
//

#include "X9EaseAction.h"
#include "../X9DisplayObject.h"
#include "../../../XMath.h"

float x9_act_easeBackIn(float v)
{
    return v * v * (2.70158 * v - 1.70158);
}
float x9_act_easeBackOut(float v)
{
    v = v - 1;
    return v * v * (2.70158 * v + 1.70158) + 1;
}
float x9_act_easeBackInOut(float v)
{
    const float overshoot = 1.70158 * 1.525;
    v = v * 2;
    if (v < 1) return (v * v * ((overshoot + 1) * v - overshoot)) / 2;
    v = v - 2;
    return (v * v * ((overshoot + 1) * v + overshoot)) / 2 + 1;
}
float x9_act_easeBounceOut(float v)
{
    if (v < 1 / 2.75) {
        return 7.5625 * v * v;
    }
    else if (v < 2 / 2.75) {
        v -= 1.5 / 2.75;
        return 7.5625 * v * v + 0.75;
    }
    else if (v < 2.5 / 2.75) {
        v -= 2.25 / 2.75;
        return 7.5625 * v * v + 0.9375;
    }
    
    v-= 2.625 / 2.75;
    return 7.5625 * v * v + 0.984375;
}
float x9_act_easeBounceIn(float v)
{
    return 1 - x9_act_easeBounceOut(1-v);
}
float x9_act_easeBounceInOut(float v)
{
    if (v < 0.5) {
        v = v * 2;
        return x9_act_easeBounceIn(v) * 0.5;
    }else{
        return x9_act_easeBounceOut(v * 2 - 1) * 0.5 + 0.5;
    }
}
float x9_act_easeElasticIn(float v)
{
    
    if (v == 0 || v == 1)
        return v;
    else {
        const float s = 0.3 / 4;
        v = v - 1;
        return -pow(2, 10 * v) * sin( (v-s) * M_PI*2 / 0.3);
    }
}
float x9_act_easeElasticOut(float v)
{
    if (v == 0 || v == 1) {
        return v;
    } else {
        const float s = 0.3 / 4;
        return pow(2, -10 * v) * sin((v-s) *M_PI*2 / 0.3) + 1;
    }
}
float x9_act_easeElasticInOut(float v)
{
    if( v == 0 || v == 1 )
        return v;
    else {
        v = v * 2;
        const float s = 0.3 / 4;
        
        v = v -1;
        if( v < 0 )
            return-0.5 * pow(2, 10 * v) * sin((v - s) * M_PI*2 / 0.3);
        else
            return pow(2, -10 * v) * sin((v - s) * M_PI*2 / 0.3) * 0.5 + 1;
    }
}
float x9_act_easeExponentialIn(float v)
{
    return pow(2, 10 * (v/1 - 1)) - 1 * 0.001;
}
float x9_act_easeExponentialOut(float v)
{
    return -pow(2, -10 * v/1) + 1;
}
float x9_act_easeExponentialInOut(float v)
{
    v*=2;
    if (v < 1)
        v = 0.5 * pow(2, 10 * (v - 1));
    else
        v = 0.5 * (-pow(2, -10 * (v -1) ) + 2);
    return v;
}
float x9_act_easeSineIn(float v)
{
    return -1*cos(v * M_PI_2) +1;
}
float x9_act_easeSineOut(float v)
{
    return sin(v * M_PI_2);
}
float x9_act_easeSineInOut(float v)
{
    return -0.5*(cos(M_PI*v) - 1);
}

map<string,std::function<float(float)>> X9EaseAction::_easeFuncs = map<string,std::function<float(float)>>();
void X9EaseAction::setBaseFunctions(X9Library* library, const string& className)
{
}
void X9EaseAction::setConstValues(X9ScriptClassData* classData)
{
    
}

X9EaseAction* X9EaseAction::newEaseAction(X9Library* library,const string& type, X9Action* act)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithString(type));
    values.push_back(X9ValueObject::createWithObject(act));
    return dynamic_cast<X9EaseAction*>(library->createObject("EaseAction", values));
}
X9EaseAction* X9EaseAction::newEaseAction(X9Library* library,const X9FunctionValueObject* func, X9Action* act)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithFunction(func->target, func->functionName, func->isSuper));
    values.push_back(X9ValueObject::createWithObject(act));
    return dynamic_cast<X9EaseAction*>(library->createObject("EaseAction", values));
}

X9_CPP_CREATE(EaseAction,Action)

void X9EaseAction::removed()
{
    if (!isType) {
        easeFunc->nonuse();
        easeFunc = nullptr;
    }
    act->nonuse();
    act = nullptr;
    X9Action::removed();
}



void X9EaseAction::initObject(const vector<X9ValueObject*>& vs)
{
    if (_easeFuncs.empty()) {
        _easeFuncs["backIn"] = x9_act_easeBackIn;
        _easeFuncs["backOut"] = x9_act_easeBackOut;
        _easeFuncs["backInOut"] = x9_act_easeBackInOut;
        _easeFuncs["bounceIn"] = x9_act_easeBounceIn;
        _easeFuncs["bounceOut"] = x9_act_easeBounceOut;
        _easeFuncs["bounceInOut"] = x9_act_easeBounceInOut;
        _easeFuncs["slasticIn"] = x9_act_easeElasticIn;
        _easeFuncs["slasticOut"] = x9_act_easeElasticOut;
        _easeFuncs["slasticInOut"] = x9_act_easeElasticInOut;
        _easeFuncs["exponentialIn"] = x9_act_easeExponentialIn;
        _easeFuncs["exponentialOut"] = x9_act_easeExponentialOut;
        _easeFuncs["exponentialInOut"] = x9_act_easeExponentialInOut;
        _easeFuncs["sineIn"] = x9_act_easeSineIn;
        _easeFuncs["sineOut"] = x9_act_easeSineOut;
        _easeFuncs["sineInOut"] = x9_act_easeSineInOut;
        
    }
    easeFunc = nullptr;
    X9ASSERT(vs.size() == 2 && vs[1]->isObject<X9Action*>(),"new EaseAction Error!!!");
    if (vs[0]->isString()) {
        isType = true;
        type = vs[0]->getString();
        X9ASSERT(_easeFuncs.find(type) != _easeFuncs.end(),"new EaseAction Error!!!");
    }else{
        X9ASSERT(vs[0]->isFunction(),"new EaseAction Error!!!");
        isType = false;
        easeFunc = vs[0]->getFunction();
        easeFunc->use();
    }
    vector<X9ValueObject*> timeVs;
    timeVs.push_back(X9ValueObject::createWithNumber(0));
    runSuperCtor("Action",timeVs);
    act = vs[1]->getObject<X9Action*>();
    act->use();
}
void X9EaseAction::setTarget(X9DisplayObject* target)
{
    X9Action::setTarget(target);
    act->setTarget(target);
}
void X9EaseAction::makeTime()
{
    act->makeTime();
    time = act->time;
}
void X9EaseAction::updateAction(float v)
{
    if (isType) {
        act->updateAction(_easeFuncs[type](v));
    }else{
        act->updateAction(easeFunc->runFunction(X9ValueObject::createWithNumber(v))->getNumber());
    }
}
X9Action* X9EaseAction::clone()
{
    if (isType) {
        return newEaseAction(getLibrary(),type,act);
    }
    return newEaseAction(getLibrary(),easeFunc,act);
}