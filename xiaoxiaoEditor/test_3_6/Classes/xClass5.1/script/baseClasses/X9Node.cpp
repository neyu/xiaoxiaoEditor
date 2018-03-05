//
//  X9Node.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/1.
//
//

#include "X9Node.h"
#include "X9Point.h"
#include "X9Rect.h"
#include "X9Size.h"
#include "X9Array.h"
#include "event/X9TouchEvent.h"
#include "../X9Player.h"



X9ValueObject* base_cacheAsBitmap(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9Node* node = dynamic_cast<X9Node*>(target);
    X9ASSERT(values.size() >= 1 && values.size() <= 2 && values[0]->isBool(),"cacheAsBitmap Error!!!");
    X9ASSERT(values[0]->getBool() && values.size() == 2 && values[1]->isObject<X9Size*>(),"cacheAsBitmap Error!!!");
    bool value = values[0]->getBool();
    if (!value) {
        node->setCacheAsBitmap(false);
    }else{
        X9ASSERT(values.size() == 2 && values[1]->isObject<X9Size*>(),"cacheAsBitmap Error!!!");
        node->setCacheAsBitmap(true,values[1]->getObject<X9Size*>()->size);
    }
    return nullptr;
}
X9ValueObject* base_setMask(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9Node* node = dynamic_cast<X9Node*>(target);
    X9ASSERT(!values.empty() && values.size() <= 2,"setMask Error!!!");
    X9ASSERT(values[0]->isObject<X9DisplayObject*>(),"setMask Error!!!");
    float a = 0.5;
    if (values.size() == 2) {
        X9ASSERT(values[1]->isNumber(),"setMask Error!!!");
        a = values[1]->getNumber();
    }
    node->setMask(values[0]->getObject<X9DisplayObject*>(),a);
    return nullptr;
}
X9ValueObject* base_getMask(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"getMask Error!!!");
    X9Node* node = dynamic_cast<X9Node*>(target);
    if (node->maskNode) {
        return node->maskValueObj->clone();
    }
    return X9ValueObject::create();
}
X9ValueObject* base_setMaskAlpha(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"setMaskAlpha Error!!!");
    X9Node* node = dynamic_cast<X9Node*>(target);
    if (node->maskNode) {
        node->maskNode->setAlphaThreshold(values[0]->getNumber());
    }
    return nullptr;
}
X9ValueObject* base_getMaskAlpha(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"getMaskAlpha Error!!!");
    X9Node* node = dynamic_cast<X9Node*>(target);
    if (node->maskNode) {
        return X9ValueObject::createWithNumber(node->maskNode->getAlphaThreshold());
    }
    return X9ValueObject::createWithNumber(-1);
}
X9ValueObject* base_clearMask(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"clearMask Error!!!");
    X9Node* node = dynamic_cast<X9Node*>(target);
    node->clearMask();
    return nullptr;
}
X9ValueObject* base_startUpdate(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"startUpdate Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    if (obj->isCanUpdate) {
        obj->startUpdate();
    }
    return nullptr;
}
X9ValueObject* base_stopUpdate(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"stopUpdate Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    if (obj->isCanUpdate) {
        obj->stopUpdate();
    }
    return nullptr;
}
X9ValueObject* base_addChild(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9DisplayObject*>(),"addChild Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    obj->addElement(values[0]->getObject<X9DisplayObject*>());
    return values[0]->clone();
}
X9ValueObject* base_addChildAt(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->isObject<X9DisplayObject*>() && values[1]->isNumber(),"addChildAt Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    obj->addElementAt(values[0]->getObject<X9DisplayObject*>(),values[1]->getNumber());
    return values[0]->clone();
}
X9ValueObject* base_removeChild(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9DisplayObject*>(),"removeChild Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    obj->removeElement(values[0]->getObject<X9DisplayObject*>());
    return values[0]->clone();
}
X9ValueObject* base_removeChildAt(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"removeChildAt Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    X9DisplayObject* child = obj->getElement(values[0]->getNumber());
    obj->removeElementByIndex(values[0]->isNumber());
    return X9ValueObject::createWithObject(child);
}
X9ValueObject* base_removeChildren(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() <= 2,"removeChildren Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    if (obj->elements.empty()) {
        return nullptr;
    }
    int stIdx = 0;
    int edIdx = obj->elements.size();
    if (values.size() > 0) {
        X9ASSERT(values[0]->isNumber(),"removeChildren Error!!!");
        stIdx = values[0]->getNumber();
        X9ASSERT(stIdx >= 0 && stIdx < obj->elements.size(),"removeChildren Error!!!");
    }
    if (values.size() == 2) {
        X9ASSERT(values[1]->isNumber(),"removeChildren Error!!!");
        edIdx = values[1]->getNumber();
        X9ASSERT(edIdx > stIdx && edIdx <= obj->elements.size(),"removeChildren Error!!!");
    }
    obj->removeElements(stIdx,edIdx);
    return nullptr;
}
X9ValueObject* base_getChild(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"getChild Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    int index = values[0]->getNumber();
    X9ASSERT(index >= 0 && index < obj->elements.size(),"getChild Error!!!");
    return X9ValueObject::createWithObject(obj->getElement(index));
}
X9ValueObject* base_getChildIndex(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->getObject<X9DisplayObject*>(),"getChildIndex Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    return X9ValueObject::createWithNumber(obj->getElementIndex(values[0]->getObject<X9DisplayObject*>()));
}
X9ValueObject* base_setChildIndex(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->getObject<X9DisplayObject*>() && values[1]->isNumber(),"setChildIndex Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    obj->setElementIndex(values[0]->getObject<X9DisplayObject*>(), values[1]->getNumber());
    return nullptr;
}
X9ValueObject* base_getChildren(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"getChildren Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    X9Array* arr = X9Array::newArray(target->getLibrary());
    for(int i = 0;i<obj->elements.size();i++)
    {
        arr->pushObject(obj->elements[i]);
    }
    return X9ValueObject::createWithObject(arr);
}
X9ValueObject* base_getChildrenByType(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isString(),"getChildrenByType Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    X9Array* arr = X9Array::newArray(target->getLibrary());
    string type = values[0]->getString();
    for(int i = 0;i<obj->elements.size();i++)
    {
        X9Object* mc = obj->elements[i];
        if(mc->className == type) {
            arr->pushObject(mc);
        }else{
            for(int k = 0;k<mc->classData->extends.size();k++)
            {
                if (mc->classData->extends[k] == type) {
                    arr->pushObject(mc);
                    break;
                }
            }
        }
    }
    return X9ValueObject::createWithObject(arr);
}
X9ValueObject* base_setTouchEnabled(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 || values.size() == 2 || values.size() == 5,"setTouchEnabled Error!!!");
    X9ASSERT(values[0]->isBool(),"setTouchEnabled Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    if (values[0]->getBool()) {
        if (values.size() == 2) {
            X9ASSERT(values[1]->isObject<X9Rect*>(),"setTouchEnabled Error!!!");
            X9Rect* rect = values[1]->getObject<X9Rect*>();
            obj->setTouchEnabled(true,rect->rect);
        }else{
            X9ASSERT(values[1]->isNumber() && values[2]->isNumber() && values[3]->isNumber() && values[4]->isNumber(),"setTouchEnabled Error!!!");
            obj->setTouchEnabled(true,Rect(values[1]->getNumber(), values[2]->getNumber(), values[3]->getNumber(), values[4]->getNumber()));
        }
    }else{
        obj->setTouchEnabled(false);
    }
    return nullptr;
}
X9ValueObject* base_setTouchRect(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 || values.size() == 4,"setTouchRect Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    if (values.size() == 1) {
        X9ASSERT(values[0]->isObject<X9Rect*>(),"setTouchRect Error!!!");
        X9Rect* rect = values[0]->getObject<X9Rect*>();
        obj->setTouchRect(rect->rect);
    }else{
        X9ASSERT(values[0]->isNumber() && values[1]->isNumber() && values[2]->isNumber() && values[3]->isNumber(),"setTouchRect Error!!!");
        obj->setTouchRect(Rect(values[0]->getNumber(), values[1]->getNumber(), values[2]->getNumber(), values[3]->getNumber()));
    }
    return nullptr;
}
X9ValueObject* base_getTouchRect(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"getTouchRect Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    vector<X9ValueObject*> vs;
    vs.push_back(X9ValueObject::createWithNumber(obj->touchRect.origin.x));
    vs.push_back(X9ValueObject::createWithNumber(obj->touchRect.origin.y));
    vs.push_back(X9ValueObject::createWithNumber(obj->touchRect.size.width));
    vs.push_back(X9ValueObject::createWithNumber(obj->touchRect.size.height));
    return target->getLibrary()->createValueObject("Rect", vs);
}
X9ValueObject* base_getTouchLocation(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() <= 1,"getTouchLocation Error!!!");
    X9Node* obj = dynamic_cast<X9Node*>(target);
    int ID = 0;
    if (values.size() == 1) {
        X9ASSERT(values[0]->isNumber(),"getTouchLocation Error!!!");
        ID = values[0]->getNumber();
    }
    return X9ValueObject::createWithObject(X9Point::newPoint(target->getLibrary(),obj->getTouchLocation(ID)));
}
X9ValueObject* base_startDrag(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() >= 2 && values.size() <= 3 && values[0]->isBool() && values[1]->isObject<X9Rect*>(),"stopDrag Error!!!");
    X9Node* node = dynamic_cast<X9Node*>(target);
    Rect bounds = Rect::ZERO;
    if (values.size() == 3) {
        bounds = values[2]->getObject<X9Rect*>()->rect;
    }
    node->startDrag(values[0]->getBool(), values[1]->getObject<X9Rect*>()->rect, bounds);
    return nullptr;
}
X9ValueObject* base_stopDrag(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"stopDrag Error!!!");
    X9Node* node = dynamic_cast<X9Node*>(target);
    node->stopDrag();
    return nullptr;
}
//void baseSet_node_size(X9RunObject* target, X9ValueObject* value)
//{
//    X9ASSERT(value->isObject() && dynamic_cast<X9Size*>(value->getObject()),"set size Error!!!");
//    X9Node* node = dynamic_cast<X9Node*>(target);
//    X9Size* size = dynamic_cast<X9Size*>(value->getObject());
//    node->_size = size->size;
//}
//X9ValueObject* baseGet_node_size(X9RunObject* target)
//{
//    X9Node* node = dynamic_cast<X9Node*>(target);
//    return X9ValueObject::createWithObject(X9Size::newSize(target->getLibrary(),node->_size.width,node->_size.height));
//}
void baseSet_node_horizontalSliderEnalbed(X9RunObject* target, X9ValueObject* value)
{
    X9ASSERT(value->isBool(),"set horizontalSliderEnalbed Error!!!");
    X9Node* node = dynamic_cast<X9Node*>(target);
    node->_horizontalSliderEnalbed = value->getBool();
}
X9ValueObject* baseGet_node_horizontalSliderEnalbed(X9RunObject* target)
{
    X9Node* node = dynamic_cast<X9Node*>(target);
    return X9ValueObject::createWithBool(node->_horizontalSliderEnalbed);
}
void baseSet_node_verticalSliderEnalbed(X9RunObject* target, X9ValueObject* value)
{
    X9ASSERT(value->isBool(),"set verticalSliderEnalbed Error!!!");
    X9Node* node = dynamic_cast<X9Node*>(target);
    node->_verticalSliderEnalbed = value->getBool();
}
X9ValueObject* baseGet_node_verticalSliderEnalbed(X9RunObject* target)
{
    X9Node* node = dynamic_cast<X9Node*>(target);
    return X9ValueObject::createWithBool(node->_verticalSliderEnalbed);
}
X9ValueObject* baseGet_node_isCacheAsBitmap(X9RunObject* target)
{
    X9Node* node = dynamic_cast<X9Node*>(target);
    return X9ValueObject::createWithBool(node->_isCacheAsBitmap);
}
void X9Node::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(,cacheAsBitmap);
    x9_AddBaseFunc(,setMask);
    x9_AddBaseFunc(,getMask);
    x9_AddBaseFunc(,setMaskAlpha);
    x9_AddBaseFunc(,getMaskAlpha);
    x9_AddBaseFunc(,clearMask);
    x9_AddBaseFunc(,startUpdate);
    x9_AddBaseFunc(,stopUpdate);
    x9_AddBaseFunc(,addChild);
    x9_AddBaseFunc(,addChildAt);
    x9_AddBaseFunc(,removeChild);
    x9_AddBaseFunc(,removeChildAt);
    x9_AddBaseFunc(,removeChildren);
    x9_AddBaseFunc(,getChild);
    x9_AddBaseFunc(,getChildIndex);
    x9_AddBaseFunc(,setChildIndex);
    x9_AddBaseFunc(,getChildren);
    x9_AddBaseFunc(,getChildrenByType);
    x9_AddBaseFunc(,setTouchEnabled);
    x9_AddBaseFunc(,setTouchRect);
    x9_AddBaseFunc(,getTouchRect);
    x9_AddBaseFunc(,getTouchLocation);
    x9_AddBaseFunc(,startDrag);
    x9_AddBaseFunc(,stopDrag);
    
//    x9_AddBaseSGet(node_,size);
    x9_AddBaseSGet(node_,horizontalSliderEnalbed);
    x9_AddBaseSGet(node_,verticalSliderEnalbed);
    x9_AddBaseGet(node_,isCacheAsBitmap);
}
void X9Node::setConstValues(X9ScriptClassData* classData)
{
    
}
X9Node* X9Node::newNode(X9Library* library)
{
    return dynamic_cast<X9Node*>(library->createObject("Node"));
}
X9Node* X9Node::create()
{
    CREATE_NEW_AUTO_OBJ(X9Node,"X9Node");
}
X9Node::X9Node(const string& key):X9DisplayObject(key)
{
    _isCacheAsBitmap = false;
    _isOnlyTouchEvent = false;
    _beginSliderDistance = 10;
    _horizontalSliderEnalbed = false;
    _verticalSliderEnalbed = false;
    _sliderNode = nullptr;
    isCanUpdate = false;
    isUpdate = false;
    touchEnabled = false;
//    touchChildCount = 0;
    x9_setCtor(Node);
}
//void X9Node::onEnter()
//{
//    Node::onEnter();
//    if (isUpdate) {
//        scheduleUpdate();
//    }
//}
void X9Node::removed()
{
    removeElements();
    _isOnlyTouchEvent = false;
    _horizontalSliderEnalbed = false;
    _verticalSliderEnalbed = false;
    _sliderNode = nullptr;
    isCanUpdate = false;
    isUpdate = false;
    touchEnabled = false;
//    touchChildCount = 0;
//    for (auto it = elements.begin(); it != elements.end(); it++) {
//        (*it)->nonuse();
//    }
//    elements.clear();
    clearMask();
//    getNode()->removeAllChildren();
    CC_SAFE_RELEASE(_node);
    X9DisplayObject::removed();
}
void X9Node::use()
{
    if (this->className == "Node") {
//        log("Node::use()");
    }
    X9DisplayObject::use();
}
void X9Node::nonuse()
{
    if (this->className == "Node") {
//        log("Node::nonuse()");
    }
    X9DisplayObject::nonuse();
}
void X9Node::initObject(const vector<X9ValueObject*>& vs)
{
    runSuperCtor("DisplayObject",vs);
//    log("from:%s, X9Node::initObject()",className.c_str());
    _oldTouchInsides.resize(10);
    for (int i = 0; i<10; i++) {
        _oldTouchInsides[i] = true;
    }
    _horizontalSliderEnalbed = false;
    _verticalSliderEnalbed = false;
    _isDrag = false;
    _node = Node::create();
    _node->setCascadeOpacityEnabled(true);
    _node->setCascadeColorEnabled(true);
    _node->retain();
//    touchChildCount = 0;
    maskNode = nullptr;
    touchEnabled = false;
//    X9DisplayObject::initObject(vs);
    isCanUpdate = classData->functions.find("update") != classData->functions.end();
}
void X9Node::setIsInStage(bool value)
{
    if (_isInStage != value) {
        X9DisplayObject::setIsInStage(value);
        for (auto it = elements.begin(); it != elements.end(); it++) {
            (*it)->setIsInStage(value);
        }
    }
}
void X9Node::update(float delay)
{
    if(classData->functions.find("update") != classData->functions.end()) {
        vector<X9ValueObject*> values;
        values.push_back(X9ValueObject::createWithNumber(delay));
        runFunction(MT_FUNCTION, "update", values,classData);
    }
}
void X9Node::addedToStage()
{
    X9DisplayObject::addedToStage();
    if (isUpdate) {
        startUpdate();
    }
//    if(touchEnabled || !_touchNodes.empty())
//    {
//        _parentNode->addTouchChild(this);
//    }
}
void X9Node::removedFromStage()
{
//    if (touchEnabled || !_touchNodes.empty()) {
//        _parentNode->removeTouchChild(this);
//    }
    X9DisplayObject::removedFromStage();
    stopUpdate();
}
X9Node* X9Node::getSliderNode(bool bx, bool by, float x, float y)
{
    if ((_horizontalSliderEnalbed || _verticalSliderEnalbed) && (_horizontalSliderEnalbed == bx || _verticalSliderEnalbed == by)) {
        if (bx && by && _horizontalSliderEnalbed != _verticalSliderEnalbed) {
            if (x > y == _verticalSliderEnalbed) {
                if (_parentNode != nullptr) {
                    X9Node* n = _parentNode->getSliderNode(!_horizontalSliderEnalbed, !_verticalSliderEnalbed, x, y);
                    if (n) return n;
                }
            }
        }
        return this;
    }
    if (_parentNode != nullptr) {
        return _parentNode->getSliderNode(bx, by, x, y);
    }
    return nullptr;
}
void X9Node::_sendTouch(X9MainTouchType type, const vector<TouchPoint>& locations)
{
    for (int i = 0; i<locations.size(); i++) {
        const TouchPoint& tp = locations[i];
        string typeStr;
        X9MainTouchType _t = type;
        switch (type) {
            case X9MT_BEGIN:
                if (tp.id == 0 && !_isOnlyTouchEvent) {
                    if (_horizontalSliderEnalbed || _verticalSliderEnalbed) {
                        _sliderNode = this;
                        _isReadyGetSliderNode = false;
                        _sendSliderTouch(type,tp.location);
                    }else{
                        _isReadyGetSliderNode = true;
                        _sliderNode = nullptr;
                    }
                    _beginTouchLocation = tp.location;
                }
                typeStr = X9TouchEvent::TOUCH_BEGIN;
                _oldTouchInsides[i] = true;
                break;
            case X9MT_MOVED:
                if (tp.id == 0 && !_isOnlyTouchEvent) {
                    if(_isReadyGetSliderNode) {
                        float ax = abs(tp.location.x-_beginTouchLocation.x);
                        float ay = abs(tp.location.y-_beginTouchLocation.y);
                        bool bx = ax >= _beginSliderDistance;
                        bool by = ay >= _beginSliderDistance;
                        if (bx >= _beginSliderDistance || by >= _beginSliderDistance) {
                            _isReadyGetSliderNode = false;
                            X9Node* tpSliderNode = getSliderNode(bx,by,ax,ay);
                            if (tpSliderNode) {
                                readyDispatchTouchEvent(X9MT_CANCELLED,X9TouchEvent::TOUCH_CANCELLED,tp.location,getPlayer()->touchPlayerLocations[i].location,tp.id);
                                _sliderNode = tpSliderNode;
                                _sliderNode->_sendSliderTouch(X9MT_BEGIN,_oldTouchLocation);
                            }
                        }
                    }
                    if (_sliderNode) {
                        _sliderNode->_sendSliderTouch(type,tp.location);
                    }
                }
                if (touchRect.containsPoint(tp.location)) {
                    if (!_oldTouchInsides[i]) {
                        _oldTouchInsides[i] = true;
                        readyDispatchTouchEvent(X9MT_OVER,X9TouchEvent::TOUCH_OVER,tp.location,getPlayer()->touchPlayerLocations[i].location,tp.id);
                    }
                    typeStr = X9TouchEvent::TOUCH_MOVED;
                }else{
                    if (_oldTouchInsides[i]) {
                        _oldTouchInsides[i] = false;
                        readyDispatchTouchEvent(X9MT_OUT,X9TouchEvent::TOUCH_OUT,tp.location,getPlayer()->touchPlayerLocations[i].location,tp.id);
                    }
                    _t = X9MT_OUTSIDE_MOVED;
                    typeStr = X9TouchEvent::TOUCH_OUTSIDE_MOVED;
                }
                break;
            case X9MT_ENDED:
                if (touchRect.containsPoint(tp.location)) {
                    typeStr = X9TouchEvent::TOUCH_ENDED;
                }else{
                    typeStr = X9TouchEvent::TOUCH_OUTSIDE_ENDED;
                    _t = X9MT_OUTSIDE_ENDED;
                }
                if (tp.id == 0 && _sliderNode && !_isOnlyTouchEvent) {
                    _sliderNode->_sendSliderTouch(type,tp.location);
                }
                break;
            case X9MT_CANCELLED:
                typeStr = X9TouchEvent::TOUCH_CANCELLED;
                _t = X9MT_CANCELLED;
                break;
            default:
                return;
        }
        if (tp.id == 0) {
            _oldTouchLocation = tp.location;
        }
        readyDispatchTouchEvent(_t,typeStr,tp.location,getPlayer()->touchPlayerLocations[i].location,tp.id);
    }
}
void X9Node::readyDispatchTouchEvent(X9MainTouchType type, const string& typeStr, const Vec2& location, const Vec2& playerLocation,int ID)
{
    if (ID == 0 && _sliderNode) {
        return;
    }
    dispatchTouchEvent(type,typeStr,location,playerLocation,ID);
}
void X9Node::dispatchTouchEvent(X9MainTouchType type, const string& typeStr, const Vec2& location, const Vec2& playerLocation,int ID)
{
//    log("dispatchTouchEvent::%s",typeStr.c_str());
    X9TouchEvent* event = X9TouchEvent::newTouchEvent(getLibrary(), typeStr, ID, location, playerLocation);
//    X9Event* event = X9Event::newEvent(getLibrary(), typeStr);
//    event->addNumberValue("x", location.x);
//    event->addNumberValue("y", location.y);
//    event->addNumberValue("id", ID);
    dispatchEvent(event);
    if (_isDrag && ID == 0) {
        switch (type) {
            case X9MT_BEGIN:
            {
                _dragOP.x = getNode()->getPositionX();
                _dragOP.y = getNode()->getPositionY();
                _dragMP = _parentNode->getTouchLocation(0);
//                X9Event* dragEvent = X9Event::newEvent(getLibrary(), "dragBegin");
                event->eventType = X9TouchEvent::DRAG_BEGIN;
                dispatchEvent(event);
                break;
            }
            case X9MT_MOVED:
            case X9MT_OUTSIDE_MOVED:
            {
                Vec2 pp = _parentNode->getTouchLocation(0);
                float ax;
                float ay;
                if (_isDragLockCenter) {
                    ax = pp.x;
                    ay = pp.y;
                }else{
                    ax = _dragOP.x+pp.x-_dragMP.x;
                    ay = _dragOP.y+pp.y-_dragMP.y;
                }
                if (_dragRounds.size.width > 0 || _dragRounds.size.height > 0) {
                    ax = MAX(_dragRounds.origin.x, MIN(_dragRounds.origin.x+_dragRounds.size.width,ax));
                    ay = MAX(_dragRounds.origin.y, MIN(_dragRounds.origin.y+_dragRounds.size.height,ay));
                }
                getNode()->setPosition(ax, ay);
                event->eventType = X9TouchEvent::DRAG_MOVED;
                dispatchEvent(event);
                break;
            }
            case X9MT_ENDED:
            case X9MT_OUTSIDE_ENDED:
            case X9MT_CANCELLED:
            {
                event->eventType = X9TouchEvent::DRAG_ENDED;
                dispatchEvent(event);
                break;
            }
            default:
                break;
        }
    }
}
//void X9Node::addTouchChild(X9Node* node)
//{
//    if (_touchNodes.empty() && _parentNode) {
//        _parentNode->addTouchChild(this);
//    }
//    _touchNodes.push_back(node);
//}
//void X9Node::removeTouchChild(X9Node* node)
//{
//    for (auto it = _touchNodes.begin(); it != _touchNodes.end(); it++) {
//        if (node == *it) {
//            _touchNodes.erase(it);
//            break;
//        }
//    }
//    if (_touchNodes.empty() && _parentNode) {
//        _parentNode->removeTouchChild(this);
//    }
//}
void X9Node::setTouchEnabled(bool enabled, const Rect& rect)
{
    bool te = enabled && !rect.equals(Rect::ZERO);
    if (touchEnabled != te) {
        touchEnabled = te;
        touchRect = rect;
//        if(_parentNode)
//        {
//            if (touchEnabled) {
//                _parentNode->addTouchChild(this);
//            }else{
//                _parentNode->removeTouchChild(this);
//            }
//        }
    }
}
void X9Node::setTouchRect(const Rect& rect)
{
    touchRect = rect;
}
const Rect& X9Node::getTouchRect()
{
    return touchRect;
}
Vec2 X9Node::getTouchLocation(int ID)
{
    return getNode()->convertToNodeSpace(getLibrary()->player->getRootTouchLocation(ID));
}
void X9Node::setCacheAsBitmap(bool value,const Size& size)
{
    _isCacheAsBitmap = value;
    _cacheAsBitmapSize = size;
}
void X9Node::setMask(X9DisplayObject* node, float alpha)
{
    if (maskNode) {
        maskValueObj->setObject(node);
        maskNode->setStencil(node->getNode());
        maskNode->setAlphaThreshold(alpha);
    }else{
        maskNode = ClippingNode::create(node->getNode());
        maskNode->setAlphaThreshold(alpha);
        _node->removeAllChildren();
        for (int i = 0; i<elements.size(); i++) {
            maskNode->addChild(elements[i]->getNode());
        }
        _node->addChild(maskNode);
        maskValueObj = X9ValueObject::createWithObject(node);
        maskValueObj->use();
    }
}
void X9Node::clearMask()
{
    if (maskNode) {
        maskNode->removeAllChildren();
        for (int i = 0; i<elements.size(); i++) {
            _node->addChild(elements[i]->getNode());
        }
        _node->removeChild(maskNode);
        maskNode = nullptr;
        if(maskValueObj) maskValueObj->nonuse();
        maskValueObj = nullptr;
    }
}
void X9Node::startUpdate()
{
    if (isUpdate) {
        return;
    }
    getPlayer()->targetManager->addUpdateTarget(this);
}
void X9Node::stopUpdate()
{
    if (isUpdate) {
       getPlayer()->targetManager->removeUpdateTarget(this);
    }
}
void X9Node::addElement(X9DisplayObject* element)
{
    addElementAt(element, elements.size());
}
void X9Node::addElementAt(X9DisplayObject* element, int index)
{
    X9ASSERT(element && index >= 0 && index <= elements.size(),"addElementAt Error!!!");
    Node* pNode = _node;
    if (maskNode) {
        pNode = maskNode;
    }
    element->use();
    pNode->addChild(element->getNode(),index);
    element->setParentNode(this, index);
    element->setIsInStage(_isInStage);
    if (index == elements.size()) {
        elements.push_back(element);
    }else{
        elements.insert(elements.begin()+index, element);
        for (int i = index+1; i<elements.size(); i++) {
            elements[i]->_childIndex = i;
            elements[i]->getNode()->setLocalZOrder(i);
        }
    }
    if (_cascadeTimeSpeetEnabled)
    {
        updateCascadeTimeSpeed();
    }
}
void X9Node::setElementIndex(X9DisplayObject* element, int index)
{
    X9ASSERT(index >= 0 && index <= elements.size(),"addElementAt Error!!!");
    int oldIndex = getElementIndex(element);
    X9ASSERT(oldIndex >= 0,"setElementIndex Error!!!");
    if (oldIndex != index) {
        elements.erase(elements.begin()+oldIndex);
        elements.insert(elements.begin()+index, element);
        for (int i = MIN(oldIndex,index); i<elements.size(); i++) {
            elements[i]->_childIndex = i;
            elements[i]->getNode()->setLocalZOrder(i);
        }
    }
}
X9DisplayObject* X9Node::getElement(int index)
{
    X9ASSERT(index >= 0 && index <= elements.size(),"getElement Error!!!");
    return elements[index];
}
int X9Node::getElementIndex(X9DisplayObject* element)
{
    if (element->_parentNode && element->_parentNode == this) {
        return element->_childIndex;
    }
    return -1;
}
void X9Node::removeElement(X9DisplayObject* element)
{
    X9ASSERT(element->_parentNode && element->_parentNode == this,"getElementIndex Error!!!");
    removeElementByIndex(element->_childIndex);
}
void X9Node::removeElementByIndex(int index)
{
    X9ASSERT(index >= 0 && index <= elements.size(),"getElement Error!!!");
    Node* pNode = _node;
    if (maskNode) {
        pNode = maskNode;
    }
    elements[index]->setIsInStage(false);
    elements[index]->setParentNode(nullptr);
//    elements[index]->removedFromStage();
    pNode->removeChild(elements[index]->getNode());
    elements.erase(elements.begin()+index);
    elements[index]->nonuse();
    for (int i = index; i<elements.size(); i++) {
        elements[i]->_childIndex = i;
        elements[i]->getNode()->setLocalZOrder(i);
    }
}
void X9Node::removeElements(int stIdx,int edIdx)
{
    Node* pNode = _node;
    if (maskNode) {
        pNode = maskNode;
    }
    if (!elements.empty()) {
        if (edIdx == -1) {
            edIdx = elements.size();
        }
        X9ASSERT(stIdx >= 0 && stIdx < elements.size(),"getElement Error!!!");
        X9ASSERT(edIdx > stIdx && edIdx <= elements.size(),"getElement Error!!!");
        if (stIdx == 0 && edIdx == elements.size()) {
            for (int i = 0; i<elements.size(); i++) {
                elements[i]->setIsInStage(false);
                elements[i]->setParentNode(nullptr);
                pNode->removeChild(elements[i]->getNode());
                elements[i]->nonuse();
            }
            elements.clear();
        }else{
            for (int i = stIdx; i<edIdx; i++) {
                elements[i]->setIsInStage(false);
                elements[i]->setParentNode(nullptr);
                pNode->removeChild(elements[i]->getNode());
                elements[i]->nonuse();
            }
            elements.erase(elements.begin()+stIdx, elements.begin()+edIdx);
        }
    }
}
void X9Node::updateDisplayedTimeSpeed(float parentSpeed)
{
    X9DisplayObject::updateDisplayedTimeSpeed(parentSpeed);
    if (_cascadeTimeSpeetEnabled) {
        for (auto element : elements) {
            element->updateDisplayedTimeSpeed(_displayTimeSpeed);
        }
    }
}
void X9Node::disableCascadeTimeSpeed()
{
    X9DisplayObject::disableCascadeTimeSpeed();
    for (auto element : elements) {
        element->updateDisplayedTimeSpeed(1);
    }
}

void X9Node::startDrag(bool lockCenter, const Rect& touchRect, const Rect& bounds)
{
    _isDragLockCenter = lockCenter;
    _isDrag = true;
    _dragRounds = bounds;
    setTouchEnabled(true,touchRect);
    _isOnlyTouchEvent = true;
}
void X9Node::stopDrag()
{
    _isDrag = false;
    setTouchEnabled(false);
    _isOnlyTouchEvent = false;
}