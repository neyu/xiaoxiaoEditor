//
//  X9Color.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/10.
//
//

#include "X9Color.h"

X9ValueObject* base_color_set(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Color*>(),"set Error!!!");
    X9Color* color = dynamic_cast<X9Color*>(target);
    color->_color = values[0]->getObject<X9Color*>()->_color;
    return nullptr;
}

void baseSet_color_r(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set x Error!!!");
    X9Color* color = dynamic_cast<X9Color*>(target);
    color->_color.r = value->getNumber();
}
X9ValueObject* baseGet_color_r(X9RunObject* target)
{
    X9Color* color = dynamic_cast<X9Color*>(target);
    return X9ValueObject::createWithNumber(color->_color.r);
}
void baseSet_color_g(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set x Error!!!");
    X9Color* color = dynamic_cast<X9Color*>(target);
    color->_color.g = value->getNumber();
}
X9ValueObject* baseGet_color_g(X9RunObject* target)
{
    X9Color* color = dynamic_cast<X9Color*>(target);
    return X9ValueObject::createWithNumber(color->_color.g);
}
void baseSet_color_b(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set x Error!!!");
    X9Color* color = dynamic_cast<X9Color*>(target);
    color->_color.b = value->getNumber();
}
X9ValueObject* baseGet_color_b(X9RunObject* target)
{
    X9Color* color = dynamic_cast<X9Color*>(target);
    return X9ValueObject::createWithNumber(color->_color.b);
}
void X9Color::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(color_,set);
    x9_AddBaseSGet(color_,r);
    x9_AddBaseSGet(color_,g);
    x9_AddBaseSGet(color_,b);
}
void X9Color::setConstValues(X9ScriptClassData* classData)
{
    x9_AddConstValue(Object, WHITE, newColor(classData->library,255, 255, 255));
    x9_AddConstValue(Object, YELLOW, newColor(classData->library,255, 255, 0));
    x9_AddConstValue(Object, BLUE, newColor(classData->library,0, 0, 255));
    x9_AddConstValue(Object, GREEN, newColor(classData->library,0, 255, 0));
    x9_AddConstValue(Object, RED, newColor(classData->library,255, 0, 0));
    x9_AddConstValue(Object, MAGENTA, newColor(classData->library,255, 255, 255));
    x9_AddConstValue(Object, BLACK, newColor(classData->library,0, 0, 0));
    x9_AddConstValue(Object, ORANGE, newColor(classData->library,255, 127, 0));
    x9_AddConstValue(Object, GRAY, newColor(classData->library,166, 166, 166));
}

X9Color* X9Color::newColor(X9Library* library, int r, int g, int b)
{
    X9Color* color = dynamic_cast<X9Color*>(library->createObject("Color"));
    color->_color.r = r;
    color->_color.g = g;
    color->_color.b = b;
    return color;
}
X9Color* X9Color::create()
{
    CREATE_NEW_AUTO_OBJ(X9Color,);
}
X9Color::X9Color():X9Object("X9Color")
{
    x9_setCtor(Color);
    _color = Color3B::WHITE;
}

void X9Color::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.empty() || vs.size() == 3,"new Color Error!!!");
    if (vs.size() == 3) {
        _color.r = vs[0]->getNumber();
        _color.g = vs[1]->getNumber();
        _color.b = vs[2]->getNumber();
    }else{
        _color = Color3B::WHITE;
    }
}