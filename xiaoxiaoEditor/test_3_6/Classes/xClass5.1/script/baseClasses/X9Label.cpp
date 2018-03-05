//
//  X9Label.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/10.
//
//

#include "X9Label.h"
#include "X9Size.h"

void baseSet_label_text(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isString(),"set text Error!!!");
    X9Label* label = dynamic_cast<X9Label*>(target);
    label->_label->setString(value->getString());
}
X9ValueObject* baseGet_label_text(X9RunObject* target)
{
    X9Label* label = dynamic_cast<X9Label*>(target);
    return X9ValueObject::createWithString(label->_label->getString());
}
void baseSet_label_labelSize(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isObject<X9Size*>(),"set fontSize Error!!!");
    X9Label* label = dynamic_cast<X9Label*>(target);
    const Size& size = value->getObject<X9Size*>()->size;
    label->_label->setDimensions(size.width, size.height);
}
X9ValueObject* baseGet_label_labelSize(X9RunObject* target)
{
    X9Label* label = dynamic_cast<X9Label*>(target);
    return X9ValueObject::createWithObject(X9Size::newSize(target->getLibrary(),label->_label->getDimensions()));
}
void baseSet_label_fontSize(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set fontSize Error!!!");
    X9Label* label = dynamic_cast<X9Label*>(target);
    TTFConfig ttfConfig = label->_label->getTTFConfig();
    ttfConfig.fontSize = value->getNumber();
    label->_label->setTTFConfig(ttfConfig);
}
X9ValueObject* baseGet_label_fontSize(X9RunObject* target)
{
    X9Label* label = dynamic_cast<X9Label*>(target);
    return X9ValueObject::createWithNumber(label->_label->getTTFConfig().fontSize);
}
void baseSet_label_hAlign(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isString(),"set hAlignment Error!!!");
    X9Label* label = dynamic_cast<X9Label*>(target);
    TextHAlignment hAlignment = TextHAlignment::LEFT;
    if (value->getString() == "center") {
        hAlignment = TextHAlignment::CENTER;
    }else if(value->getString() == "right") {
        hAlignment = TextHAlignment::RIGHT;
    }
    label->_label->setAlignment(hAlignment);
}
X9ValueObject* baseGet_label_hAlign(X9RunObject* target)
{
    X9Label* label = dynamic_cast<X9Label*>(target);
    switch (label->_label->getHorizontalAlignment()) {
        case TextHAlignment::LEFT:
            return X9ValueObject::createWithString("left");
        case TextHAlignment::CENTER:
            return X9ValueObject::createWithString("center");
        case TextHAlignment::RIGHT:
            return X9ValueObject::createWithString("right");
    }
}

X9Label* X9Label::create()
{
    CREATE_NEW_AUTO_OBJ(X9Label,);
}
X9Label::X9Label():X9DisplayObject("X9Label")
{
    x9_setCtor(Label);
    _label = nullptr;
}
void X9Label::setBaseFunctions(X9Library* library, const string& className)
{
//    x9_AddBaseFunc(label_,setAlignment);
//    x9_AddBaseFunc(getImageWidth);
//    x9_AddBaseFunc(getImageHeight);
    //
    x9_AddBaseSGet(label_,text);
    x9_AddBaseSGet(label_,labelSize);
    x9_AddBaseSGet(label_,fontSize);
    x9_AddBaseSGet(label_,hAlign);
}
void X9Label::setConstValues(X9ScriptClassData* classData)
{
    
}
void X9Label::removed()
{
    CC_SAFE_RELEASE(_label);
    X9DisplayObject::removed();
}
/**
 * @class:Label
 * @param:string text
 * @param:... Label(txt,fontSize), Label(txt,fontName,fontSize), Label(txt,fontSize,width,height), Label(txt,fontName,fontSize,width,height), Label(txt,fontSize,width,height,hAlignment,vAlignment), Label(txt,fontName,fontSize,width,height,hAlignment,vAlignment)
 *
 */
void X9Label::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() <= 7,"new Label initObject Error!!!");
    string txt = "";
//    string fontName = "droid_sans_fallback.ttf";
    string fontName = getLibrary()->defaultFontName;
    float fontSize = 32;
    Size size;
    TextHAlignment hAlignment = TextHAlignment::LEFT;
    TextVAlignment vAlignment = TextVAlignment::TOP;
    if(vs.size() == 1) {
        X9ASSERT(vs[0]->isString(),"new Label initObject Error!!!");
        txt = vs[0]->getString();
    }else if (vs.size() == 2) {
        X9ASSERT(vs[0]->isString() && vs[1]->isNumber(),"new Label initObject Error!!!");
        txt = vs[0]->getString();
        fontSize = vs[1]->getNumber();
    }else if(vs.size() == 3) {
        X9ASSERT(vs[0]->isString() && vs[1]->isString() && vs[2]->isNumber(),"new Label initObject Error!!!");
        txt = vs[0]->getString();
        fontName = vs[1]->getString();
        fontSize = vs[2]->getNumber();
    }else if(vs.size() == 4) {
        X9ASSERT(vs[0]->isString() && vs[1]->isNumber() && vs[2]->isNumber() && vs[3]->isNumber(),"new Label initObject Error!!!");
        txt = vs[0]->getString();
        fontSize = vs[1]->getNumber();
        size.width = vs[2]->getNumber();
        size.height = vs[3]->getNumber();
    }else if(vs.size() == 5) {
        X9ASSERT(vs[0]->isString() && vs[1]->isString() && vs[2]->isNumber() && vs[3]->isNumber() && vs[4]->isNumber(),"new Label initObject Error!!!");
        txt = vs[0]->getString();
        fontName = vs[1]->getString();
        fontSize = vs[2]->getNumber();
        size.width = vs[3]->getNumber();
        size.height = vs[4]->getNumber();
    }else if(vs.size() == 6) {
        X9ASSERT(vs[0]->isString() && vs[1]->isNumber() && vs[2]->isNumber() && vs[3]->isNumber() && vs[4]->isString() && vs[5]->isString(),"new Label initObject Error!!!");
        txt = vs[0]->getString();
        fontSize = vs[1]->getNumber();
        size.width = vs[2]->getNumber();
        size.height = vs[3]->getNumber();
        if(vs[4]->getString() == "center")
        {
            hAlignment = TextHAlignment::CENTER;
        }else if(vs[4]->getString() == "right")
        {
            hAlignment = TextHAlignment::RIGHT;
        }
        if(vs[5]->getString() == "center")
        {
            vAlignment = TextVAlignment::CENTER;
        }else if(vs[5]->getString() == "bottom")
        {
            vAlignment = TextVAlignment::BOTTOM;
        }
    }else if(vs.size() == 7)
    {
        X9ASSERT(vs[0]->isString() && vs[1]->isString() && vs[2]->isNumber() && vs[3]->isNumber() && vs[4]->isNumber() && vs[5]->isString() && vs[6]->isString(),"new Label initObject Error!!!");
        txt = vs[0]->getString();
        fontName = vs[1]->getString();
        fontSize = vs[2]->getNumber();
        size.width = vs[3]->getNumber();
        size.height = vs[4]->getNumber();
        if(vs[5]->getString() == "center")
        {
            hAlignment = TextHAlignment::CENTER;
        }else if(vs[5]->getString() == "right")
        {
            hAlignment = TextHAlignment::RIGHT;
        }
        if(vs[6]->getString() == "center")
        {
            vAlignment = TextVAlignment::CENTER;
        }else if(vs[6]->getString() == "bottom")
        {
            vAlignment = TextVAlignment::BOTTOM;
        }
    }
    _label = Label::createWithTTF(txt, getLibrary()->path->getFontPath(fontName), fontSize, size, hAlignment, vAlignment);
    _label->setAnchorPoint(Vec2::UNIT_Y);
    _label->retain();
    _node = _label;
    //_label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
}
