//
//  X9Size.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/10.
//
//

#include "X9Size.h"
void baseSet_size_width(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set width Error!!!");
    X9Size* size = dynamic_cast<X9Size*>(target);
    size->size.width = value->getNumber();
}
X9ValueObject* baseGet_size_width(X9RunObject* target)
{
    X9Size* size = dynamic_cast<X9Size*>(target);
    return X9ValueObject::createWithNumber(size->size.width);
}
void baseSet_size_height(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set height Error!!!");
    X9Size* size = dynamic_cast<X9Size*>(target);
    size->size.height = value->getNumber();
}
X9ValueObject* baseGet_size_height(X9RunObject* target)
{
    X9Size* size = dynamic_cast<X9Size*>(target);
    return X9ValueObject::createWithNumber(size->size.height);
}
void X9Size::setBaseFunctions(X9Library* library, const string& className)
{
    //    x9_AddBaseFunc(testFunc);
    x9_AddBaseSGet(size_,width);
    x9_AddBaseSGet(size_,height);
}
void X9Size::setConstValues(X9ScriptClassData* classData)
{
    
}
X9Size* X9Size::newSize(X9Library* library, float w, float h)
{
    X9Size* size = dynamic_cast<X9Size*>(library->createObject("Size"));
    size->size.width = w;
    size->size.height = h;
    return size;
}
X9Size* X9Size::newSize(X9Library* library, const Size& size)
{
    return newSize(library,size.width,size.height);
}
X9Size* X9Size::create()
{
    CREATE_NEW_AUTO_OBJ(X9Size,);
}
X9Size::X9Size():X9Object("X9Size")
{
    x9_setCtor(Size);
    size.width = 0;
    size.height = 0;
}

void X9Size::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() <= 2,"new Size Error!!!");
    if (vs.size() == 1 && vs[0]->isObject<X9Size*>()) {
        size = vs[0]->getObject<X9Size*>()->size;
    }else{
        for (int i = 0; i<vs.size(); i++) {
            X9ASSERT(vs[i]->isNumber(),"new Size Error!!!");
        }
        size.width = vs.size() > 0 ? vs[0]->getNumber() : 0;
        size.height = vs.size() > 1 ? vs[1]->getNumber() : 0;
    }
}
string X9Size::toString()
{
    std::ostringstream oss;
    oss<<"[Size w:";
    oss<<size.width;
    oss<<", h:";
    oss<<size.height;
    oss<<"]";
    return oss.str();
}