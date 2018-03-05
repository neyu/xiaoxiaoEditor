//
//  X9DisplayObject.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/1.
//
//

#include "X9DisplayObject.h"
#include "X9Node.h"
#include "X9Color.h"
#include "X9Point.h"
#include "X9Player.h"
#include "action/X9Action.h"
#include "X9Event.h"

X9ValueObject* base_setPosition(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->isNumber() && values[1]->isNumber(),"setPosition Error!!!");
    X9DisplayObject* display = dynamic_cast<X9DisplayObject*>(target);
    display->getNode()->setPosition(values[0]->getNumber(), values[1]->getNumber());
    return X9ValueObject::create();
}
X9ValueObject* base_localToGlobal(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9DisplayObject* display = dynamic_cast<X9DisplayObject*>(target);
    Vec2 p;
    if (values.size() == 1) {
        X9ASSERT(values[0]->isObject<X9Point*>(),"localToGlobal Error!!!");
        X9Point* pp = values[0]->getObject<X9Point*>();
        p.x = pp->point.x;
        p.y = pp->point.y;
    }else if(values.size() == 2)
    {
        X9ASSERT(values[0]->isNumber() && values[1]->isNumber(),"localToGlobal Error!!!");
        p.x = values[0]->getNumber();
        p.y = values[1]->getNumber();
    }else{
        X9ASSERT(values.empty(),"localToGlobal Error!!!");
    }
    p = display->getLibrary()->player->convertToNodeSpace(display->getNode()->convertToWorldSpace(p));
    return X9ValueObject::createWithObject(X9Point::newPoint(display->getLibrary(),p.x, p.y));
}
X9ValueObject* base_globalToLocal(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9DisplayObject* display = dynamic_cast<X9DisplayObject*>(target);
    Vec2 p;
    if (values.size() == 1) {
        X9ASSERT(values[0]->isObject<X9Point*>(),"localToGlobal Error!!!");
        X9Point* pp = values[0]->getObject<X9Point*>();
        p.x = pp->point.x;
        p.y = pp->point.y;
    }else if(values.size() == 2)
    {
        X9ASSERT(values[0]->isNumber() && values[1]->isNumber(),"localToGlobal Error!!!");
        p.x = values[0]->getNumber();
        p.y = values[1]->getNumber();
    }else{
        X9ASSERT(values.empty(),"localToGlobal Error!!!");
    }
    p = display->getNode()->convertToNodeSpace(display->getLibrary()->player->convertToWorldSpace(p));
    return X9ValueObject::createWithObject(X9Point::newPoint(display->getLibrary(),p.x, p.y));
}
//X9ValueObject* base_getParent(X9RunObject* target,const vector<X9ValueObject*>& values)
//{
//    X9DisplayObject* display = dynamic_cast<X9DisplayObject*>(target);
//    X9ASSERT(display->_parentNode,"getParent Error!!!");
//    return X9ValueObject::createWithObject(display->_parentNode);
//}
X9ValueObject* base_removeSelf(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9DisplayObject* display = dynamic_cast<X9DisplayObject*>(target);
    X9ASSERT(display->_parentNode,"removeSelf Error!!!");
    display->_parentNode->removeElement(display);
    return nullptr;
}
X9ValueObject* base_runAction(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9DisplayObject* display = dynamic_cast<X9DisplayObject*>(target);
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"runAction Error!!!");
    X9ASSERT(display->_parentNode,"removeSelf Error!!!");
    display->runAction(values[0]->getObject<X9Action*>());
    return nullptr;
}
X9ValueObject* base_stopAction(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9DisplayObject* display = dynamic_cast<X9DisplayObject*>(target);
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"stopAction Error!!!");
    display->stopAction(values[0]->getObject<X9Action*>());
    return nullptr;
}
X9ValueObject* base_stopAllAction(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9DisplayObject* display = dynamic_cast<X9DisplayObject*>(target);
    X9ASSERT(values.empty(),"stopAllAction Error!!!");
    display->stopAllAction();
    return nullptr;
}
//X9ValueObject* base_runAcrion(X9RunObject* target,const vector<X9ValueObject*>& values)
//{
//    X9DisplayObject* display = dynamic_cast<X9DisplayObject*>(target);
//    X9ASSERT(display->_parent,"removeSelf Error!!!");
//    display->_parent->removeElement(display);
//    return nullptr;
//}




void baseSet_x(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set x Error!!!");
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    obj->getNode()->setPositionX(value->getNumber());
}
X9ValueObject* baseGet_x(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    return X9ValueObject::createWithNumber(obj->getNode()->getPositionX());
}
void baseSet_y(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set y Error!!!");
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    obj->getNode()->setPositionY(value->getNumber());
}
X9ValueObject* baseGet_y(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    return X9ValueObject::createWithNumber(obj->getNode()->getPositionY());
}
void baseSet_scaleX(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set scaleX Error!!!");
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    obj->getNode()->setScaleX(value->getNumber());
}
X9ValueObject* baseGet_scaleX(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    return X9ValueObject::createWithNumber(obj->getNode()->getScaleX());
}
void baseSet_scaleY(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set scaleY Error!!!");
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    obj->getNode()->setScaleY(value->getNumber());
}
X9ValueObject* baseGet_scaleY(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    return X9ValueObject::createWithNumber(obj->getNode()->getScaleY());
}
void baseSet_skewX(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set skewX Error!!!");
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    obj->getNode()->setRotationSkewX(value->getNumber());
}
X9ValueObject* baseGet_skewX(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    return X9ValueObject::createWithNumber(obj->getNode()->getRotationSkewX());
}
void baseSet_skewY(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set skewY Error!!!");
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    obj->getNode()->setRotationSkewY(value->getNumber());
}
X9ValueObject* baseGet_skewY(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    return X9ValueObject::createWithNumber(obj->getNode()->getRotationSkewY());
}
void baseSet_rotation(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set rotation Error!!!");
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
//    obj->getNode()->setRotation(value->getNumber());
    obj->getNode()->setRotationSkewX(value->getNumber());
    obj->getNode()->setRotationSkewY(value->getNumber());
}
X9ValueObject* baseGet_rotation(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    return X9ValueObject::createWithNumber(obj->getNode()->getRotationSkewY());
}
void baseSet_alpha(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set alpha Error!!!");
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    obj->getNode()->setOpacity(MIN(255,MAX(0,value->getNumber()*255)));
}
X9ValueObject* baseGet_alpha(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    return X9ValueObject::createWithNumber(obj->getNode()->getOpacity()/255);
}
void baseSet_color(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isObject<X9Color*>(),"set color Error!!!");
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    obj->getNode()->setColor(value->getObject<X9Color*>()->_color);
}
X9ValueObject* baseGet_color(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    const Color3B& color = obj->getNode()->getColor();
    return X9ValueObject::createWithObject(X9Color::newColor(obj->getLibrary(),color.r, color.g, color.b));
}
void baseSet_visible(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isBool(),"set visible Error!!!");
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    obj->getNode()->setVisible(value->getBool());
}
X9ValueObject* baseGet_visible(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    return X9ValueObject::createWithBool(obj->getNode()->isVisible());
}
void baseSet_cascadeTimeSpeedEnabled(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isBool(),"set cascadeTimeSpeedEnabled Error!!!");
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    obj->setCascadeTimeSpeedEnabled(value->getBool());
}
X9ValueObject* baseGet_cascadeTimeSpeedEnabled(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    return X9ValueObject::createWithBool(obj->isCascadeTimeSpeedEnabled());
}
void baseSet_timeSpeed(X9RunObject* target, X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set timeSpeed Error!!!");
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    obj->setTimeSpeed(value->getNumber());
}
X9ValueObject* baseGet_timeSpeed(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    return X9ValueObject::createWithBool(obj->getTimeSpeed());
}
X9ValueObject* baseGet_displayTimeSpeed(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    return X9ValueObject::createWithBool(obj->getDisplayedTimeSpeed());
}
X9ValueObject* baseGet_parent(X9RunObject* target)
{
    X9DisplayObject* obj = dynamic_cast<X9DisplayObject*>(target);
    return X9ValueObject::createWithObject(obj->_parentNode);
}
void X9DisplayObject::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(,setPosition);
    x9_AddBaseFunc(,localToGlobal);
    x9_AddBaseFunc(,globalToLocal);
//    x9_AddBaseFunc(,getParent);
    x9_AddBaseFunc(,removeSelf);
    x9_AddBaseFunc(,runAction);
    x9_AddBaseFunc(,stopAction);
    x9_AddBaseFunc(,stopAllAction);
//    x9_AddBaseFunc(runAcrion);
    
    x9_AddBaseSGet(,x);
    x9_AddBaseSGet(,y);
    x9_AddBaseSGet(,scaleX);
    x9_AddBaseSGet(,scaleY);
    x9_AddBaseSGet(,skewX);
    x9_AddBaseSGet(,skewY);
    x9_AddBaseSGet(,rotation);
    x9_AddBaseSGet(,alpha);
    x9_AddBaseSGet(,color);
    x9_AddBaseSGet(,visible);
    x9_AddBaseSGet(,cascadeTimeSpeedEnabled);
    x9_AddBaseSGet(,timeSpeed);
    x9_AddBaseGet(,displayTimeSpeed);
    x9_AddBaseGet(,parent);
}
void X9DisplayObject::setConstValues(X9ScriptClassData* classData)
{
    
}
X9DisplayObject* X9DisplayObject::create()
{
    CREATE_NEW_AUTO_OBJ(X9DisplayObject,"X9DisplayObject");
}
X9DisplayObject::X9DisplayObject(const string& key):X9EventDispatcher(key)
{
//    _size = Size::ZERO;
    _keyDownEventCount = 0;
    _keyUpEventCount = 0;
    _cascadeTimeSpeetEnabled = true;
    _isInStage = false;
    _parentNode = nullptr;
    _childIndex = -1;
    _node = nullptr;
    x9_setCtor(DisplayObject);
}
void X9DisplayObject::removed()
{
//    _size = Size::ZERO;
    _cascadeTimeSpeetEnabled = true;
    _isInStage = false;
    _parentNode = nullptr;
    _childIndex = -1;
    _node = nullptr;
    if (_keyDownEventCount > 0) {
        getPlayer()->targetManager->removeKeyTarget(this, true);
    }
    if (_keyUpEventCount > 0) {
        getPlayer()->targetManager->removeKeyTarget(this, false);
    }
    _keyDownEventCount = 0;
    _keyUpEventCount = 0;
    X9EventDispatcher::removed();
}
void X9DisplayObject::initObject(const vector<X9ValueObject*>& vs)
{
//    log("from:%s, X9DisplayObject::initObject()",className.c_str());
}
void X9DisplayObject::setIsInStage(bool value)
{
    if(_isInStage != value)
    {
        _isInStage = value;
        if (value) {
            addedToStage();
//            X9Event* event = X9Event::newEvent(getLibrary(), "addedToStage");
//            dispatchEvent(event);
        }else{
//            X9Event* event = X9Event::newEvent(getLibrary(), "removedToStage");
//            dispatchEvent(event);
            removedFromStage();
            _parentNode = nullptr;
            _childIndex = -1;
        }
    }
}
void X9DisplayObject::setParentNode(X9Node* parentNode, int childIndex)
{
    _parentNode = parentNode;
    _childIndex = childIndex;
}
void X9DisplayObject::removedFromStage()
{
    stopAllAction();
}
void X9DisplayObject::checkAddEventListener(const string& type)
{
    if (type == "keyDown") {
        if (_keyDownEventCount == 0) {
            getPlayer()->targetManager->addKeyTarget(this, true);
        }
        _keyDownEventCount++;
    }else if(type == "keyUp") {
        if (_keyUpEventCount == 0) {
            getPlayer()->targetManager->addKeyTarget(this, false);
        }
        _keyUpEventCount++;
    }
}
void X9DisplayObject::checkRemoveEventListener(const string& type)
{
    if (type == "keyDown") {
        if (_keyDownEventCount > 0) {
            _keyDownEventCount--;
            if (_keyDownEventCount == 0) {
                getPlayer()->targetManager->removeKeyTarget(this, true);
            }
        }
    }else if(type == "keyUp") {
        if (_keyUpEventCount > 0) {
            _keyUpEventCount--;
            if (_keyUpEventCount == 0) {
                getPlayer()->targetManager->removeKeyTarget(this, false);
            }
        }
    }
}
void X9DisplayObject::setTimeSpeed(float speed)
{
    _displayTimeSpeed = _realTimeSpeed = speed;
    
    updateCascadeTimeSpeed();
}

void X9DisplayObject::updateDisplayedTimeSpeed(float parentSpeed)
{
    _displayTimeSpeed = _realTimeSpeed * parentSpeed;
    timeSpeedChanged();
}

void X9DisplayObject::setCascadeTimeSpeedEnabled(bool enabled)
{
    if (_cascadeTimeSpeetEnabled == enabled)
    {
        return;
    }
    
    _cascadeTimeSpeetEnabled = enabled;
    
    if (enabled)
    {
        updateCascadeTimeSpeed();
    }
    else
    {
        disableCascadeTimeSpeed();
    }
}

void X9DisplayObject::updateCascadeTimeSpeed()
{
    float parentTimeSpeed = 1;
    
    if (_parentNode != nullptr && _parentNode->isCascadeTimeSpeedEnabled())
    {
        parentTimeSpeed = _parentNode->getDisplayedTimeSpeed();
    }
    
    updateDisplayedTimeSpeed(parentTimeSpeed);
}

void X9DisplayObject::disableCascadeTimeSpeed()
{
    _displayTimeSpeed = _realTimeSpeed;
}
void X9DisplayObject::updateAction(float delay)
{
    for (int i = 0; i<_actions.size(); i++) {
        _actions[i]->startTime += delay;
        float v = _actions[i]->startTime/_actions[i]->time;
        
        if (_actions[i]->time == 0 || v >= 1) {
            _actions[i]->updateAction(1);
            _removedActions.push_back(_actions[i]);
        }else{
            _actions[i]->updateAction(v);
        }
    }
    //
    for (int i = 0; i<_removedActions.size(); i++) {
        for (auto it = _actions.begin(); it != _actions.end(); it++) {
            if (*it == _removedActions[i]) {
                _removedActions[i]->nonuse();
                _actions.erase(it);
                break;
            }
        }
    }
    _removedActions.clear();
    if (_actions.empty()) {
        getPlayer()->targetManager->removeActionTarget(this);
    }
}
void X9DisplayObject::runAction(X9Action* action)
{
    if (_actions.empty()) {
        getPlayer()->targetManager->addActionTarget(this);
    }
    _actions.push_back(action);
    action->use();
    action->makeTime();
    action->setTarget(this);
    action->startTime = 0;
}
void X9DisplayObject::stopAction(X9Action* action)
{
    for (int i = 0; i<_actions.size(); i++) {
        if (action == _actions[i]) {
            _removedActions.push_back(action);
        }
    }
}
void X9DisplayObject::stopAllAction()
{
    _removedActions = _actions;
}