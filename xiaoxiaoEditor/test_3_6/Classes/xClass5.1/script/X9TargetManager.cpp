//
//  X9TargetManager.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/22.
//
//

#include "X9TargetManager.h"
#include "baseClasses/X9DisplayObject.h"
#include "baseClasses/event/X9KeyboardEvent.h"


//MARK::X9intervalObject
X9intervalObject::X9intervalObject(int ID,bool loop,X9FunctionValueObject* callback, float interval, const vector<X9ValueObject*>& values)
{
    this->ID = ID;
    time = 0;
    this->isLoop = loop;
    callback->use();
    this->callback = callback;
    this->interval = interval;
    this->values = values;
    for (int i = 0; i<values.size(); i++) {
        values[i]->use();
    }
}
X9intervalObject::~X9intervalObject()
{
    log("~X9intervalObject()");
    callback->nonuse();
    for (int i = 0; i<values.size(); i++) {
        values[i]->nonuse();
    }
    values.clear();
}

//MARK::X9TargetManager
X9TargetManager::X9TargetManager(X9Library* library)
{
    _maxIntervalID = 0;
    _maxTimeoutID = 0;
    _library = library;
}
X9TargetManager::~X9TargetManager()
{
    for (int i = 0; i<intervals.size(); i++) {
        X9_SAFE_DELETE(intervals[i]);
    }
    intervals.clear();
    for (int i = 0; i<timeouts.size(); i++) {
        X9_SAFE_DELETE(timeouts[i]);
    }
    timeouts.clear();
}



void X9TargetManager::addUpdateTarget(X9Object* obj)
{
    for (auto it = remove_updateTargets.begin(); it != remove_updateTargets.end(); it++) {
        if (obj == *it) {
            remove_updateTargets.erase(it);
            break;
        }
    }
    for (int i = 0; i<updateTargets.size(); i++) {
        if(updateTargets[i] == obj) return;
    }
    updateTargets.push_back(obj);
}
void X9TargetManager::removeUpdateTarget(X9Object* obj)
{
    remove_updateTargets.push_back(obj);
}
void X9TargetManager::addActionTarget(X9DisplayObject* obj)
{
    for (auto it = remove_actionTarbets.begin(); it != remove_actionTarbets.end(); it++) {
        if (obj == *it) {
            remove_actionTarbets.erase(it);
            break;
        }
    }
    for (int i = 0; i<actionTarbets.size(); i++) {
        if (actionTarbets[i] == obj) return;
    }
    actionTarbets.push_back(obj);
}
void X9TargetManager::removeActionTarget(X9DisplayObject* obj)
{
    remove_actionTarbets.push_back(obj);
}
void X9TargetManager::addKeyTarget(X9DisplayObject* obj,bool isKeyDown)
{
    if (isKeyDown) {
        keyDownTargets.push_back(obj);
    }else{
        keyUpTargets.push_back(obj);
    }
}
void X9TargetManager::removeKeyTarget(X9DisplayObject* obj,bool isKeyDown)
{
    vector<X9DisplayObject*>& _vect = isKeyDown ? keyDownTargets : keyUpTargets;
    for (auto it = _vect.begin(); it != _vect.end(); it++) {
        if (*it == obj) {
            _vect.erase(it);
            break;
        }
    }
}
int X9TargetManager::addInterval(X9FunctionValueObject* callback,float interval,const vector<X9ValueObject*>& values)
{
    timerTargets[callback->target] = true;
    intervals.push_back(new X9intervalObject(++_maxIntervalID,true,callback,interval,values));
    return intervals.back()->ID;
}
int X9TargetManager::addTimeout(X9FunctionValueObject* callback,float interval,const vector<X9ValueObject*>& values)
{
    timeouts.push_back(new X9intervalObject(++_maxTimeoutID,true,callback,interval,values));
    return timeouts.back()->ID;
}
void X9TargetManager::clearInterval(int ID)
{
    for (auto it = intervals.begin(); it != intervals.end(); it++) {
        if ((*it)->ID == ID) {
            remove_intervals.push_back(*it);
            break;
        }
    }
}
void X9TargetManager::clearTimeout(int ID)
{
    for (auto it = timeouts.begin(); it != timeouts.end(); it++) {
        if ((*it)->ID == ID) {
            remove_timeouts.push_back(*it);
            break;
        }
    }
}
void X9TargetManager::clearTimerTarget(X9RunObject* obj)
{
    if (timerTargets.find(obj) != timerTargets.end()) {
        timerTargets.erase(obj);
        for (int i = 0; i<intervals.size(); i++) {
            if (intervals[i]->callback->target == obj) {
                remove_intervals.push_back(intervals[i]);
            }
        }
        for (int i = 0; i<timeouts.size(); i++) {
            if (timeouts[i]->callback->target == obj) {
                remove_timeouts.push_back(intervals[i]);
            }
        }
        log("clearTimerTarget(), %d, %d",(int)remove_intervals.size(),(int)remove_timeouts.size());
    }
}
//#define REMOVE_TargetLIST(_N_) \
//for (int i = 0;i<remove_##_N_.size(); i++) { \
//    for (auto it = _N_.begin(); it != _N_.end(); it++) { \
//        if (remove_##_N_[i] == *it) { \
//            _N_.erase(it); \
//            break; \
//        } \
//    } \
//} \
//remove_##_N_.clear()

void X9TargetManager::updateAllTarget(float delay)
{
    for (int i = 0; i<actionTarbets.size(); i++) {
        actionTarbets[i]->updateAction(delay*actionTarbets[i]->getDisplayedTimeSpeed());
    }
    for (auto it = intervals.begin(); it != intervals.end(); it++) {
        X9intervalObject* obj = *it;
        obj->time += delay;
        if (obj->time >= obj->interval) {
            obj->time -= floor(obj->time/obj->interval)*obj->interval;
            obj->callback->runFunction(obj->values);
        }
    }
    for (auto it = timeouts.begin(); it != timeouts.end(); it++) {
        X9intervalObject* obj = *it;
        obj->time += delay;
        if (obj->time >= obj->interval) {
            obj->callback->runFunction(obj->values);
            remove_timeouts.push_back(*it);
        }
    }
    for (int i = 0; i<updateTargets.size(); i++) {
        X9Object* obj = updateTargets[i];
        obj->update(delay*obj->getDisplayedTimeSpeed());
    }
//    for (auto it = updateTargets.begin(); it != updateTargets.end(); it++) {
//        X9Object* obj = *it;
//        obj->update(delay*obj->getDisplayedTimeSpeed());
//    }
}

void X9TargetManager::clearTargets()
{
    if (!remove_intervals.empty() || !remove_timeouts.empty()) {
        log("X9TargetManager::clearTargets()");
    }
    
    for (int i = 0;i<remove_intervals.size(); i++) { \
        for (auto it = intervals.begin(); it != intervals.end(); it++) {
            if (remove_intervals[i] == *it) {
                delete (*it);
                intervals.erase(it);
                break;
            }
        }
    }
    remove_intervals.clear();
    for (int i = 0;i<remove_timeouts.size(); i++) { \
        for (auto it = timeouts.begin(); it != timeouts.end(); it++) {
            if (remove_timeouts[i] == *it) {
                delete (*it);
                timeouts.erase(it);
                break;
            }
        }
    }
    remove_timeouts.clear();
    for (int i = 0;i<remove_updateTargets.size(); i++) { \
        for (auto it = updateTargets.begin(); it != updateTargets.end(); it++) {
            if (remove_updateTargets[i] == *it) {
                updateTargets.erase(it);
                break;
            }
        }
    }
    remove_updateTargets.clear();
    for (int i = 0;i<remove_actionTarbets.size(); i++) { \
        for (auto it = actionTarbets.begin(); it != actionTarbets.end(); it++) {
            if (remove_actionTarbets[i] == *it) {
                actionTarbets.erase(it);
                break;
            }
        }
    }
    remove_actionTarbets.clear();
    
}
void X9TargetManager::onKeyHandler(bool isDown, const string& keyCode)
{
    if(isDown) {
        for (auto it = keyDownTargets.begin(); it != keyDownTargets.end(); it++) {
            X9DisplayObject* obj = *it;
            if (obj->getIsInStage() && obj->getNode()->isVisible()) {
                X9Event* event = X9KeyboardEvent::newKeyboardEvent(_library,X9KeyboardEvent::KEY_DOWN,keyCode);
                obj->dispatchEvent(event);
            }
        }
    }else{
        for (auto it = keyUpTargets.begin(); it != keyUpTargets.end(); it++) {
            X9DisplayObject* obj = *it;
            if (obj->getIsInStage() && obj->getNode()->isVisible()) {
                X9Event* event = X9KeyboardEvent::newKeyboardEvent(_library,X9KeyboardEvent::KEY_UP,keyCode);
                obj->dispatchEvent(event);
            }
        }
    }
}