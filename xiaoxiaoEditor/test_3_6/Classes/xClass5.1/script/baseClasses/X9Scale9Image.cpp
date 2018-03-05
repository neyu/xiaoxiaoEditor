//
//  X9Scale9Image.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 18/1/18.
//
//

#include "X9Scale9Image.h"
#include "X9Rect.h"

X9ValueObject* base_scale9Image_setAnchorPoint(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2,"setAnchrPoint Error!!!");
    X9Scale9Image* image = dynamic_cast<X9Scale9Image*>(target);
    image->getNode()->setAnchorPoint(Vec2(values[0]->getNumber(),values[1]->getNumber()));
    return nullptr;
}
void baseSet_scale9Image_width(X9RunObject* target, X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"get width Error!!!");
    X9Scale9Image* img = dynamic_cast<X9Scale9Image*>(target);
    img->getNode()->setContentSize(Size(value->getNumber(),img->getNode()->getContentSize().height));
}
X9ValueObject* baseGet_scale9Image_width(X9RunObject* target)
{
    X9Scale9Image* img = dynamic_cast<X9Scale9Image*>(target);
    return X9ValueObject::createWithNumber(img->getNode()->getContentSize().width);
}
void baseSet_scale9Image_height(X9RunObject* target, X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"get width Error!!!");
    X9Scale9Image* img = dynamic_cast<X9Scale9Image*>(target);
    img->getNode()->setContentSize(Size(img->getNode()->getContentSize().width,value->getNumber()));
}
X9ValueObject* baseGet_scale9Image_height(X9RunObject* target)
{
    X9Scale9Image* img = dynamic_cast<X9Scale9Image*>(target);
    return X9ValueObject::createWithNumber(img->getNode()->getContentSize().height);
}
void baseSet_scale9Image_scaleX(X9RunObject* target, X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"get scaleX Error!!!");
    X9Scale9Image* img = dynamic_cast<X9Scale9Image*>(target);
    ui::Scale9Sprite* mc = dynamic_cast<ui::Scale9Sprite*>(img->getNode());
    const Size& size = mc->getSprite()->getTexture()->getContentSize();
    img->getNode()->setContentSize(Size(value->getNumber()*size.width,img->getNode()->getContentSize().height));
}
X9ValueObject* baseGet_scale9Image_scaleX(X9RunObject* target)
{
    X9Scale9Image* img = dynamic_cast<X9Scale9Image*>(target);
    ui::Scale9Sprite* mc = dynamic_cast<ui::Scale9Sprite*>(img->getNode());
    const Size& size = mc->getSprite()->getTexture()->getContentSize();
    return X9ValueObject::createWithNumber(img->getNode()->getContentSize().width/size.width);
}
void baseSet_scale9Image_scaleY(X9RunObject* target, X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"get scaleY Error!!!");
    X9Scale9Image* img = dynamic_cast<X9Scale9Image*>(target);
    ui::Scale9Sprite* mc = dynamic_cast<ui::Scale9Sprite*>(img->getNode());
    const Size& size = mc->getSprite()->getTexture()->getContentSize();
    img->getNode()->setContentSize(Size(img->getNode()->getContentSize().width,value->getNumber()*size.height));
}
X9ValueObject* baseGet_scale9Image_scaleY(X9RunObject* target)
{
    X9Scale9Image* img = dynamic_cast<X9Scale9Image*>(target);
    ui::Scale9Sprite* mc = dynamic_cast<ui::Scale9Sprite*>(img->getNode());
    const Size& size = mc->getSprite()->getTexture()->getContentSize();
    return X9ValueObject::createWithNumber(img->getNode()->getContentSize().height/size.height);
}
//X9ValueObject* baseGet_scale9Image_imageWidth(X9RunObject* target)
//{
//    X9Scale9Image* img = dynamic_cast<X9Scale9Image*>(target);
//    return X9ValueObject::createWithNumber(img->getNode()->getContentSize().width);
//}
//X9ValueObject* baseGet_scale9Image_imageHeight(X9RunObject* target)
//{
//    X9Scale9Image* img = dynamic_cast<X9Scale9Image*>(target);
//    return X9ValueObject::createWithNumber(img->getNode()->getContentSize().height);
//}


X9ValueObject* baseGet_scale9Image_source(X9RunObject* target)
{
    X9Scale9Image* image = dynamic_cast<X9Scale9Image*>(target);
    return X9ValueObject::createWithString(image->_source);
}

X9Scale9Image* X9Scale9Image::newScale9Image(X9Library* library, const string& name, const Rect& rect)
{
    vector<X9ValueObject*> vs;
    vs.push_back(X9ValueObject::createWithString(name));
    vs.push_back(X9ValueObject::createWithObject(X9Rect::newRect(library,rect)));
    return dynamic_cast<X9Scale9Image*>(library->createObject("Scale9Image",vs));
}
X9Scale9Image* X9Scale9Image::create()
{
    CREATE_NEW_AUTO_OBJ(X9Scale9Image,"X9Scale9Image");
}
X9Scale9Image::X9Scale9Image(const string& key):X9DisplayObject(key)
{
    x9_setCtor(Scale9Image);
    _mc = nullptr;
}
void X9Scale9Image::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(scale9Image_,setAnchorPoint);
    
    x9_AddBaseSGet(scale9Image_,width);
    x9_AddBaseSGet(scale9Image_,height);
    x9_AddBaseSGet(scale9Image_,scaleX);
    x9_AddBaseSGet(scale9Image_,scaleY);
//    x9_AddBaseGet(scale9Image_,imageWidth);
//    x9_AddBaseGet(scale9Image_,imageHeight);
    x9_AddBaseGet(scale9Image_,source);
}
void X9Scale9Image::setConstValues(X9ScriptClassData* classData)
{
    
}
void X9Scale9Image::removed()
{
    CC_SAFE_RELEASE(_node);
    _mc = nullptr;
    _glow = 0;
    _blendMode = "normal";
    X9DisplayObject::removed();
}
void X9Scale9Image::initObject(const vector<X9ValueObject*>& vs)
{
    //    _mc = new Sprite;
    //    _mc->init();
    X9ASSERT(vs.size() == 2 && vs[0]->isString() && vs[1]->isObject<X9Rect*>(),"new Scale9Image initObject Error!!!");
    _source = vs[0]->getString();
    _glow = 0;
    _blendMode = "normal";
    _mc = ui::Scale9Sprite::create(vs[1]->getObject<X9Rect*>()->rect, getLibrary()->path->getResourcePath(_source));
    
    _mc->retain();
    //    _mc->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
    _node = _mc;
}
//void X9Scale9Image::setImageFile(const string& source, const Rect& rect)
//{
//    if (_mc) {
//        
//        _mc->release();
//    }
//    _mc->setTexture(getLibrary()->path->getResourcePath(source));
//    _source = source;
//}
void X9Scale9Image::setBlendMode(const string& blendMode)
{
    X9ASSERT(false,"not complete Error!!!");
}