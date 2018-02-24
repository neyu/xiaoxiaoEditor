//
//  X9StateButton.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/14.
//
//

#include "X9StateButton.h"
#include "X9Array.h"
#include "X9Rect.h"

void baseSet_sBtn_enabled(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isBool(),"set enabled Error!!!");
    X9StateButton* btn = dynamic_cast<X9StateButton*>(target);
    btn->setTouchEnabled(value->getBool());
}
X9ValueObject* baseGet_sBtn_enabled(X9RunObject* target)
{
    X9StateButton* btn = dynamic_cast<X9StateButton*>(target);
    return X9ValueObject::createWithBool(btn->touchEnabled);
}

X9StateButton* X9StateButton::create()
{
    CREATE_NEW_AUTO_OBJ(X9StateButton,"X9StateButton");
}
X9StateButton::X9StateButton(const string& key):X9Node(key)
{
    _normalNode = nullptr;
    _touchNode = nullptr;
    _disabledNode = nullptr;
    x9_setCtor(StateButton);
}
void X9StateButton::setBaseFunctions(X9Library* library, const string& className)
{
//    x9_AddBaseFunc(slider_,addChild);
//    x9_AddBaseFunc(slider_,addChildAt);
//    x9_AddBaseFunc(slider_,removeChild);
//    x9_AddBaseFunc(slider_,removeChildAt);
//    x9_AddBaseFunc(slider_,removeChildren);
//    x9_AddBaseFunc(slider_,getChild);
//    x9_AddBaseFunc(slider_,getChildIndex);
//    x9_AddBaseFunc(slider_,setChildIndex);
//    
    x9_AddBaseSGet(sBtn_,enabled);
//    x9_AddBaseSGet(slider_,contentSize);
}
void X9StateButton::setConstValues(X9ScriptClassData* classData)
{
    
}
void X9StateButton::removed()
{
    _normalNode->nonuse();
    if (_touchNode != _normalNode) {
        _touchNode->nonuse();
    }
    if (_disabledNode != _normalNode && _disabledNode != _touchNode) {
        _disabledNode->nonuse();
    }
    X9Node::removed();
//    log("_currentNode:useCount:%d",_currentNode->getUseCount());
}
/**
 * @class:X9StateButton
 * @param:Rect touch rect
 * @param:Array? normal contents
 * @param:Array? touch contents
 * @param:Array? disabled contents
 *
 */
void X9StateButton::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() >= 2 && vs.size() <= 4
             && vs[0]->isObject<X9Rect*>()
             && vs[1]->isObject<X9Array*>()
             ,"new StateButton initObject Error!!!");
    runSuperCtor("Node", vs);
    _normalNode = X9Node::newNode(getLibrary());
    _normalNode->use();
    _touchNode = nullptr;
    _disabledNode = nullptr;
    _isTouchContent = false;
    _isDisabledContent = false;
    X9Array* arr1 = vs[1]->getObject<X9Array*>();
    for (int i = 0; i<arr1->children.size(); i++) {
        X9ASSERT(arr1->children[i]->isObject<X9DisplayObject*>(),"new StateButton initObject Error!!!(is not DisplayObject)");
        _normalNode->addElement(arr1->children[i]->getObject<X9DisplayObject*>());
    }
    if (vs.size() >= 3) {
        X9ASSERT(vs[2]->isObject<X9Array*>(),"new StateButton initObject Error!!!");
        _touchNode = X9Node::newNode(getLibrary());
        _touchNode->use();
        _isTouchContent = true;
        X9Array* arr2 = vs[2]->getObject<X9Array*>();
        for (int i = 0; i<arr2->children.size(); i++) {
            X9ASSERT(arr2->children[i]->isObject<X9DisplayObject*>(),"new StateButton initObject Error!!!(is not DisplayObject)");
            _touchNode->addElement(arr2->children[i]->getObject<X9DisplayObject*>());
        }
    }else{
        _touchNode = _normalNode;
    }
    if (vs.size() >= 4) {
        X9ASSERT(vs[3]->isObject<X9Array*>(),"new StateButton initObject Error!!!");
        _disabledNode = X9Node::newNode(getLibrary());
        _disabledNode->use();
        _isDisabledContent = true;
//        _disabledNode = _normalNode;
        X9Array* arr3 = vs[3]->getObject<X9Array*>();
        for (int i = 0; i<arr3->children.size(); i++) {
            X9ASSERT(arr3->children[i]->isObject<X9DisplayObject*>(),"new StateButton initObject Error!!!(is not DisplayObject)");
            _disabledNode->addElement(arr3->children[i]->getObject<X9DisplayObject*>());
        }
    }else{
        _disabledNode = _normalNode;
    }
    _type = SBT_NONE;
    _currentNode = nullptr;
    setTouchEnabled(true,vs[0]->getObject<X9Rect*>()->rect);
}
void X9StateButton::setTouchEnabled(bool enabled, const Rect& rect)
{
    X9Node::setTouchEnabled(enabled,rect);
    if (enabled) {
        setState(SBT_NORMAL);
    }else{
        setState(SBT_DISABLED);
    }
}
void X9StateButton::setState(SBType type)
{
    if (_type != type) {
        _type = type;
        if (_currentNode) {
            removeElement(_currentNode);
        }
        switch (type) {
            case SBT_NORMAL:
                _currentNode = _normalNode;
                break;
            case SBT_TOUCH:
                _currentNode = _touchNode;
                break;
            case SBT_DISABLED:
                _currentNode = _disabledNode;
                break;
            default:
                break;
        }
        addElement(_currentNode);
    }
}
void X9StateButton::dispatchTouchEvent(X9MainTouchType type, const string& typeStr, const Vec2& location, const Vec2& playerLocation, int ID)
{
    if (ID == 0) {
        //log("%s",typeStr.c_str());
        switch (type) {
            case X9Node::X9MT_CANCELLED:
            case X9Node::X9MT_OUT:
            case X9Node::X9MT_ENDED:
                setState(SBT_NORMAL);
                break;
            case X9Node::X9MT_BEGIN:
            case X9Node::X9MT_OVER:
                setState(SBT_TOUCH);
                break;
            default:
                break;
        }
    }
    X9Node::dispatchTouchEvent(type, typeStr, location, playerLocation, ID);
}