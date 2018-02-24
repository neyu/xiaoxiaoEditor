//
//  X9Rect.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/5.
//
//

#include "X9Rect.h"
#include "X9Point.h"
#include "../../XMath.h"
//

X9ValueObject* base_rect_getClosest(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->isNumber() && values[1]->isNumber(),"getClosest");
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    float ax = values[0]->getNumber();
    float ay = values[1]->getNumber();
    vector<float> arr;
    arr.push_back(ax-rect->rect.getMinX());
    arr.push_back(rect->rect.getMaxX()-ax);
    arr.push_back(ay-rect->rect.getMinY());
    arr.push_back(rect->rect.getMaxY()-ay);
    float v = arr[0];
    int idx = 0;
    for (int i = 0; i<arr.size(); i++) {
        if (arr[i] < v) {
            idx = i;
            v = arr[i];
        }
    }
    switch (idx) {
        case 0:
            //l
            ax = rect->rect.getMinX();
            break;
        case 1:
            //r
            ax = rect->rect.getMaxX();
            break;
        case 2:
            //d
            ay = rect->rect.getMinY();
            break;
        case 3:
            //t
            ay = rect->rect.getMaxY();
            break;
    }
    return X9ValueObject::createWithObject(X9Point::newPoint(target->getLibrary(),ax,ay));
}

X9ValueObject* base_rect_lineInRectValue(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 4 && values[0]->isNumber() && values[1]->isNumber() && values[2]->isNumber() && values[3]->isNumber(),"intersectLine Error!!!");
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    Vec2 a(values[0]->getNumber(),values[1]->getNumber());
    Vec2 b(values[2]->getNumber(),values[3]->getNumber());
    vector<float> value;
    if (a.x == b.x) {
        value.push_back(XMath::umix(a.y, b.y, rect->rect.origin.y));
        value.push_back(XMath::umix(a.y, b.y, rect->rect.origin.y+rect->rect.size.height));
    }else if(a.y == b.y)
    {
        value.push_back(XMath::umix(a.x, b.x, rect->rect.origin.x));
        value.push_back(XMath::umix(a.x, b.x, rect->rect.origin.x+rect->rect.size.width));
    }else
    {
        float _l = rect->rect.origin.x;
        float _b = rect->rect.origin.y;
        float _r = _l+rect->rect.size.width;
        float _t = _b+rect->rect.size.height;
        Vec2 arr[4][2];
        //tl,tr
        arr[0][0].x = _l;
        arr[0][0].y = _t;
        arr[0][1].x = _r;
        arr[0][1].y = _t;
        //bl,br
        arr[1][0].x = _l;
        arr[1][0].y = _b;
        arr[1][1].x = _r;
        arr[1][1].y = _b;
        //tl,bl
        arr[2][0].x = _l;
        arr[2][0].y = _t;
        arr[2][1].x = _l;
        arr[2][1].y = _b;
        //tr,br
        arr[3][0].x = _r;
        arr[3][0].y = _t;
        arr[3][1].x = _r;
        arr[3][1].y = _b;
        for (int i = 0; i<4; i++) {
            Vec2 v = XMath::beelineIntersectPointValue(a,b,arr[i][0],arr[i][1]);
            if (v.y >= 0 && v.y <= 1) {
                value.push_back(v.x);
            }
        }
    }
    float v = value[0];
    for (int i = 1; i<value.size(); i++) {
        if (value[i] < v) {
            v = value[i];
        }
    }
    return X9ValueObject::createWithNumber(v);
}

X9ValueObject* base_rect_containsPoint(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    if (values.size() == 1 && values[0]->isObject<X9Point*>()) {
        return X9ValueObject::createWithBool(rect->rect.containsPoint(values[0]->getObject<X9Point*>()->point));
    }
    if (values.size() == 2 && values[0]->isNumber() && values[1]->isNumber()) {
        return X9ValueObject::createWithBool(rect->rect.containsPoint(Vec2(values[0]->getNumber(),values[1]->getNumber())));
    }
    X9ASSERT(false,"containsPoint Error!!!");
}
void baseSet_rect_x(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set x Error!!!");
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    rect->rect.origin.x = value->getNumber();
}
X9ValueObject* baseGet_rect_x(X9RunObject* target)
{
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    return X9ValueObject::createWithNumber(rect->rect.origin.x);
}
void baseSet_rect_y(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set y Error!!!");
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    rect->rect.origin.y = value->getNumber();
}
X9ValueObject* baseGet_rect_y(X9RunObject* target)
{
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    return X9ValueObject::createWithNumber(rect->rect.origin.y);
}
void baseSet_rect_width(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set width Error!!!");
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    rect->rect.size.width = value->getNumber();
}
X9ValueObject* baseGet_rect_width(X9RunObject* target)
{
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    return X9ValueObject::createWithNumber(rect->rect.size.width);
}
void baseSet_rect_height(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set height Error!!!");
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    rect->rect.size.height = value->getNumber();
}
X9ValueObject* baseGet_rect_height(X9RunObject* target)
{
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    return X9ValueObject::createWithNumber(rect->rect.size.height);
}
X9ValueObject* baseGet_rect_minX(X9RunObject* target)
{
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    return X9ValueObject::createWithNumber(rect->rect.origin.x);
}
X9ValueObject* baseGet_rect_minY(X9RunObject* target)
{
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    return X9ValueObject::createWithNumber(rect->rect.origin.y);
}
X9ValueObject* baseGet_rect_maxX(X9RunObject* target)
{
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    return X9ValueObject::createWithNumber(rect->rect.origin.x+rect->rect.size.width);
}
X9ValueObject* baseGet_rect_maxY(X9RunObject* target)
{
    X9Rect* rect = dynamic_cast<X9Rect*>(target);
    return X9ValueObject::createWithNumber(rect->rect.origin.y+rect->rect.size.height);
}
void X9Rect::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(rect_,getClosest);
    x9_AddBaseFunc(rect_,lineInRectValue);
    x9_AddBaseFunc(rect_,containsPoint);
    x9_AddBaseSGet(rect_,x);
    x9_AddBaseSGet(rect_,y);
    x9_AddBaseSGet(rect_,width);
    x9_AddBaseSGet(rect_,height);
    x9_AddBaseGet(rect_,minX);
    x9_AddBaseGet(rect_,minY);
    x9_AddBaseGet(rect_,maxX);
    x9_AddBaseGet(rect_,maxY);
}
void X9Rect::setConstValues(X9ScriptClassData* classData)
{
    
}
X9Rect* X9Rect::newRect(X9Library* library, float x, float y, float w, float h)
{
    X9Rect* rect = dynamic_cast<X9Rect*>(library->createObject("Rect"));
    rect->rect.setRect(x, y, w, h);
    return rect;
}
X9Rect* X9Rect::newRect(X9Library* library, const Rect& _rect)
{
    X9Rect* rect = dynamic_cast<X9Rect*>(library->createObject("Rect"));
    rect->rect = _rect;
    return rect;
}
X9Rect* X9Rect::create()
{
    CREATE_NEW_AUTO_OBJ(X9Rect,);
}
X9Rect::X9Rect():X9Object("X9Rect")
{
    x9_setCtor(Rect);
}

void X9Rect::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() <= 4,"new Rect Error!!!");
    if (vs.size() == 1 && vs[0]->isObject<X9Rect*>()) {
        rect = vs[0]->getObject<X9Rect*>()->rect;
    }else{
        for (int i = 0; i<vs.size(); i++) {
            X9ASSERT(vs[i]->isNumber(),"new Rect Error!!!");
        }
        rect.origin.x = vs.size() > 0 ? vs[0]->getNumber() : 0;
        rect.origin.y = vs.size() > 1 ? vs[1]->getNumber() : 0;
        rect.size.width = vs.size() > 2 ? vs[2]->getNumber() : 0;
        rect.size.height = vs.size() > 3 ? vs[3]->getNumber() : 0;
    }
}
string X9Rect::toString()
{
    std::ostringstream oss;
    oss<<"[Rect x:";
    oss<<rect.origin.x;
    oss<<", y:";
    oss<<rect.origin.y;
    oss<<", w:";
    oss<<rect.size.width;
    oss<<", h:";
    oss<<rect.size.height;
    oss<<"]";
    return oss.str();
}