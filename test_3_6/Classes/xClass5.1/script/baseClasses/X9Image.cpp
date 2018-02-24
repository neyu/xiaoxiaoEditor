//
//  X9Image.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/5.
//
//

#include "X9Image.h"

X9ValueObject* base_setAnchorPoint(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2,"setAnchrPoint Error!!!");
    X9Image* image = dynamic_cast<X9Image*>(target);
    image->getNode()->setAnchorPoint(Vec2(values[0]->getNumber(),values[1]->getNumber()));
    return nullptr;
}
void baseSet_image_width(X9RunObject* target, X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"get width Error!!!");
    X9Image* img = dynamic_cast<X9Image*>(target);
    img->getNode()->setScaleX(value->getNumber()/img->getNode()->getContentSize().width);
}
X9ValueObject* baseGet_image_width(X9RunObject* target)
{
    X9Image* img = dynamic_cast<X9Image*>(target);
    return X9ValueObject::createWithNumber(img->getNode()->getContentSize().width*img->getNode()->getScaleX());
}
void baseSet_image_height(X9RunObject* target, X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"get width Error!!!");
    X9Image* img = dynamic_cast<X9Image*>(target);
    img->getNode()->setScaleY(value->getNumber()/img->getNode()->getContentSize().height);
}
X9ValueObject* baseGet_image_height(X9RunObject* target)
{
    X9Image* img = dynamic_cast<X9Image*>(target);
    return X9ValueObject::createWithNumber(img->getNode()->getContentSize().height*img->getNode()->getScaleY());
}
X9ValueObject* baseGet_image_imageWidth(X9RunObject* target)
{
    X9Image* img = dynamic_cast<X9Image*>(target);
    return X9ValueObject::createWithNumber(img->getNode()->getContentSize().width);
}
X9ValueObject* baseGet_image_imageHeight(X9RunObject* target)
{
    X9Image* img = dynamic_cast<X9Image*>(target);
    return X9ValueObject::createWithNumber(img->getNode()->getContentSize().height);
}


void baseSet_image_source(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isString(),"set fileName Error!!!");
    X9Image* image = dynamic_cast<X9Image*>(target);
    image->setImageFile(value->getString());
}
X9ValueObject* baseGet_image_source(X9RunObject* target)
{
    X9Image* image = dynamic_cast<X9Image*>(target);
    return X9ValueObject::createWithString(image->_source);
}

X9Image* X9Image::newImage(X9Library* library, const string& name)
{
    vector<X9ValueObject*> vs;
    if (name != "") {
        vs.push_back(X9ValueObject::createWithString(name));
    }
    return dynamic_cast<X9Image*>(library->createObject("Image",vs));
}
X9Image* X9Image::create()
{
    CREATE_NEW_AUTO_OBJ(X9Image,"X9Image");
}
X9Image::X9Image(const string& key):X9DisplayObject(key)
{
    x9_setCtor(Image);
    _mc = nullptr;
}
void X9Image::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(,setAnchorPoint);
    
    x9_AddBaseSGet(image_,width);
    x9_AddBaseSGet(image_,height);
    x9_AddBaseGet(image_,imageWidth);
    x9_AddBaseGet(image_,imageHeight);
    x9_AddBaseSGet(image_,source);
}
void X9Image::setConstValues(X9ScriptClassData* classData)
{
    
}
void X9Image::removed()
{
    CC_SAFE_RELEASE(_node);
    _mc = nullptr;
    _glow = 0;
    _blendMode = "normal";
    X9DisplayObject::removed();
}
void X9Image::initObject(const vector<X9ValueObject*>& vs)
{
//    _mc = new Sprite;
//    _mc->init();
    _source = "";
    _glow = 0;
    _blendMode = "normal";
    if (!vs.empty()) {
        X9ASSERT(vs.size() == 1 && vs[0]->isString(),"new Image initObject Error!!!");
        _source = vs[0]->getString();
        _mc = Sprite::create(getLibrary()->path->getResourcePath(_source));
    }else{
        _mc = Sprite::create();
    }
    _mc->retain();
//    _mc->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _node = _mc;
}
void X9Image::setImageFile(const string& source, const Rect& rect)
{
    _mc->setTexture(getLibrary()->path->getResourcePath(source));
    _source = source;
}
void X9Image::setBlendMode(const string& blendMode)
{
    X9ASSERT(false,"not complete Error!!!");
}
void X9Image::setGlow(float glow)
{
    X9ASSERT(false,"not complete Error!!!");
}