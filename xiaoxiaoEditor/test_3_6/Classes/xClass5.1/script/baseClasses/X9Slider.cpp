//
//  X9BaseSlider.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/2.
//
//

#include "X9Slider.h"
#include "X9Size.h"

X9ValueObject* base_slider_addChild(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9DisplayObject*>(),"addChild Error!!!");
    X9Slider* slider = dynamic_cast<X9Slider*>(target);
    slider->_contentNode->addElement(values[0]->getObject<X9DisplayObject*>());
    return values[0]->clone();
}
X9ValueObject* base_slider_addChildAt(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->isObject<X9DisplayObject*>() && values[1]->isNumber(),"addChildAt Error!!!");
    X9Slider* slider = dynamic_cast<X9Slider*>(target);
    slider->_contentNode->addElementAt(values[0]->getObject<X9DisplayObject*>(),values[1]->getNumber());
    return values[0]->clone();
}
X9ValueObject* base_slider_removeChild(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9DisplayObject*>(),"removeChild Error!!!");
    X9Slider* slider = dynamic_cast<X9Slider*>(target);
    slider->_contentNode->removeElement(values[0]->getObject<X9DisplayObject*>());
    return values[0]->clone();
}
X9ValueObject* base_slider_removeChildAt(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"removeChildAt Error!!!");
    X9Slider* slider = dynamic_cast<X9Slider*>(target);
    X9DisplayObject* child = slider->_contentNode->getElement(values[0]->getNumber());
    slider->_contentNode->removeElementByIndex(values[0]->isNumber());
    return X9ValueObject::createWithObject(child);
}
X9ValueObject* base_slider_removeChildren(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() <= 2,"removeChildren Error!!!");
    X9Slider* slider = dynamic_cast<X9Slider*>(target);
    if (slider->_contentNode->elements.empty()) {
        return nullptr;
    }
    int stIdx = 0;
    int edIdx = slider->_contentNode->elements.size();
    if (values.size() > 0) {
        X9ASSERT(values[0]->isNumber(),"removeChildren Error!!!");
        stIdx = values[0]->getNumber();
        X9ASSERT(stIdx >= 0 && stIdx < slider->_contentNode->elements.size(),"removeChildren Error!!!");
    }
    if (values.size() == 2) {
        X9ASSERT(values[1]->isNumber(),"removeChildren Error!!!");
        edIdx = values[1]->getNumber();
        X9ASSERT(edIdx > stIdx && edIdx <= slider->_contentNode->elements.size(),"removeChildren Error!!!");
    }
    slider->_contentNode->removeElements(stIdx,edIdx);
    return nullptr;
}
X9ValueObject* base_slider_getChild(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"getChild Error!!!");
    X9Slider* slider = dynamic_cast<X9Slider*>(target);
    int index = values[0]->getNumber();
    X9ASSERT(index >= 0 && index < slider->_contentNode->elements.size(),"getChild Error!!!");
    return X9ValueObject::createWithObject(slider->_contentNode->getElement(index));
}
X9ValueObject* base_slider_getChildIndex(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->getObject<X9DisplayObject*>(),"getChildIndex Error!!!");
    X9Slider* slider = dynamic_cast<X9Slider*>(target);
    return X9ValueObject::createWithNumber(slider->_contentNode->getElementIndex(values[0]->getObject<X9DisplayObject*>()));
}
X9ValueObject* base_slider_setChildIndex(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->getObject<X9DisplayObject*>() && values[1]->isNumber(),"setChildIndex Error!!!");
    X9Slider* slider = dynamic_cast<X9Slider*>(target);
    slider->_contentNode->setElementIndex(values[0]->getObject<X9DisplayObject*>(), values[1]->isNumber());
    return nullptr;
}


void baseSet_slider_size(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isObject<X9Size*>(),"set size Error!!!");
    dynamic_cast<X9Slider*>(target)->resetSize(value->getObject<X9Size*>()->size);
}
X9ValueObject* baseGet_slider_size(X9RunObject* target)
{
    X9Slider* slider = dynamic_cast<X9Slider*>(target);
    return X9ValueObject::createWithObject(X9Size::newSize(target->getLibrary(),slider->_size.width,slider->_size.height));
}
void baseSet_slider_contentSize(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isObject<X9Size*>(),"set size Error!!!");
    dynamic_cast<X9Slider*>(target)->resetContentSize(value->getObject<X9Size*>()->size);
}
X9ValueObject* baseGet_slider_contentSize(X9RunObject* target)
{
    X9Slider* slider = dynamic_cast<X9Slider*>(target);
    return X9ValueObject::createWithObject(X9Size::newSize(target->getLibrary(),slider->_contentSize.width,slider->_contentSize.height));
}

//X9Slider* X9Slider::newSlider(X9Library* library)
//{
//    vector<X9ValueObject*> vs;
//    if (name != "") {
//        vs.push_back(X9ValueObject::createWithString(name));
//    }
//    return dynamic_cast<X9Image*>(library->createObject("Image",vs));
//}
X9Slider* X9Slider::create()
{
    CREATE_NEW_AUTO_OBJ(X9Slider,"X9Slider");
}
X9Slider::X9Slider(const string& key):X9Node(key)
{
    _horizontalValue = 0;
    _verticalValue = 0;
    _contentNode = nullptr;
    _stencil = nullptr;
    x9_setCtor(Slider);
}
void X9Slider::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(slider_,addChild);
    x9_AddBaseFunc(slider_,addChildAt);
    x9_AddBaseFunc(slider_,removeChild);
    x9_AddBaseFunc(slider_,removeChildAt);
    x9_AddBaseFunc(slider_,removeChildren);
    x9_AddBaseFunc(slider_,getChild);
    x9_AddBaseFunc(slider_,getChildIndex);
    x9_AddBaseFunc(slider_,setChildIndex);
//    x9_AddBaseFunc(,setAnchrPoint);
    //
    x9_AddBaseSGet(slider_,size);
    x9_AddBaseSGet(slider_,contentSize);
//    x9_AddBaseSGet(image_,height);
//    x9_AddBaseGet(image_,imageWidth);
//    x9_AddBaseGet(image_,imageHeight);
//    x9_AddBaseSGet(image_,source);
}
void X9Slider::setConstValues(X9ScriptClassData* classData)
{
    
}
void X9Slider::removed()
{
    _contentNode = nullptr;
    _stencil = nullptr;
    X9Node::removed();
}
/**
 * @class:Slider
 * @param:Size slider size
 * @param:Node content node(or null)
 * @param:Size content size
 * @param:bool horizontal enalbed
 * @param:bool vertical enalbed
 * 
 */
void X9Slider::initObject(const vector<X9ValueObject*>& vs)
{
    
    X9ASSERT(vs.size() >= 3 && vs.size() <= 5
             && vs[0]->isObject<X9Size*>()
             && vs[1]->isObject<X9Node*>()
             && vs[2]->isObject<X9Size*>(),"new Slider initObject Error!!!");
    
    _horizontalSliderEnalbed = true;
    _verticalSliderEnalbed = true;
    if (vs.size() == 3) {
        X9ASSERT(vs[2]->isBool(),"new Slider initObject Error!!!");
        _horizontalSliderEnalbed = vs[2]->getBool();
    }
    if (vs.size() == 4) {
        X9ASSERT(vs[3]->isBool(),"new Slider initObject Error!!!");
        _verticalSliderEnalbed = vs[3]->getBool();
    }
    _size = vs[0]->getObject<X9Size*>()->size;
    _contentSize = vs[2]->getObject<X9Size*>()->size;
    _contentNode = vs[1]->getObject<X9Node*>();
    if (!_contentNode) {
        _contentNode = dynamic_cast<X9Node*>(getLibrary()->createObject("Node"));
    }
    _stencil = DrawNode::create();
    _stencil->drawRect(Vec2::ZERO, Vec2(100,100), Color4F(1,1,1,1));
    _stencil->setScale(_size.width/100, _size.height/100);
    
    maskNode = ClippingNode::create(_stencil);
    maskNode->setAlphaThreshold(0.5);
    _node->addChild(maskNode);
    addElement(_contentNode);
    _contentNode->addNumberValue("x", 0);
    _contentNode->addNumberValue("y", _size.height-_contentSize.height);
    setTouchEnabled(true,Rect(0,0,_size.width,_size.height));
}
void X9Slider::_sendSliderTouch(X9MainTouchType type, const Vec2& location)
{
    
}
void X9Slider::resetSize(const Size& size)
{
    _size = size;
}
void X9Slider::resetContentSize(const Size& size)
{
    _contentSize = size;
}