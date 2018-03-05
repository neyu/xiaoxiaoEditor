//
//  X9AniPosFrame.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/12/12.
//
//

#include "X9AniPosFrame.h"
#include "X9Point.h"
#include "../../animation/X9FLAnimationData.h"

void baseSet_aniPosFrame_a(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isObject<X9Point*>(),"set a Error!!!");
    X9AniPosFrame* apf = dynamic_cast<X9AniPosFrame*>(target);
    apf->a = value->getObject<X9Point*>()->point;
}
X9ValueObject* baseGet_aniPosFrame_a(X9RunObject* target)
{
    X9AniPosFrame* apf = dynamic_cast<X9AniPosFrame*>(target);
    return X9ValueObject::createWithObject(X9Point::newPoint(target->getLibrary(), apf->a));
}
void baseSet_aniPosFrame_b(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isObject<X9Point*>(),"set b Error!!!");
    X9AniPosFrame* apf = dynamic_cast<X9AniPosFrame*>(target);
    apf->b = value->getObject<X9Point*>()->point;
}
X9ValueObject* baseGet_aniPosFrame_b(X9RunObject* target)
{
    X9AniPosFrame* apf = dynamic_cast<X9AniPosFrame*>(target);
    return X9ValueObject::createWithObject(X9Point::newPoint(target->getLibrary(), apf->b));
}
void baseSet_aniPosFrame_radian(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set radian Error!!!");
    X9AniPosFrame* apf = dynamic_cast<X9AniPosFrame*>(target);
    apf->r = value->getNumber();
}
X9ValueObject* baseGet_aniPosFrame_radian(X9RunObject* target)
{
    X9AniPosFrame* apf = dynamic_cast<X9AniPosFrame*>(target);
    return X9ValueObject::createWithNumber(apf->r);
}
void baseSet_aniPosFrame_rotation(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set rotation Error!!!");
    X9AniPosFrame* apf = dynamic_cast<X9AniPosFrame*>(target);
    apf->r = CC_DEGREES_TO_RADIANS(value->getNumber());
}
X9ValueObject* baseGet_aniPosFrame_rotation(X9RunObject* target)
{
    X9AniPosFrame* apf = dynamic_cast<X9AniPosFrame*>(target);
    return X9ValueObject::createWithNumber(CC_RADIANS_TO_DEGREES(apf->r));
}
void baseSet_aniPosFrame_reverse(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set reverse Error!!!");
    X9AniPosFrame* apf = dynamic_cast<X9AniPosFrame*>(target);
    apf->reverse = value->getNumber();
}
X9ValueObject* baseGet_aniPosFrame_reverse(X9RunObject* target)
{
    X9AniPosFrame* apf = dynamic_cast<X9AniPosFrame*>(target);
    return X9ValueObject::createWithBool(apf->reverse);
}

void X9AniPosFrame::setBaseFunctions(X9Library* library, const string& className)
{
    //    x9_AddBaseFunc(testFunc);
    x9_AddBaseSGet(aniPosFrame_,a);
    x9_AddBaseSGet(aniPosFrame_,b);
    x9_AddBaseSGet(aniPosFrame_,radian);
    x9_AddBaseSGet(aniPosFrame_,rotation);
    x9_AddBaseSGet(aniPosFrame_,reverse);
}
void X9AniPosFrame::setConstValues(X9ScriptClassData* classData)
{
    
}
X9AniPosFrame* X9AniPosFrame::newAniPosFrame(X9Library* library, X9PosFrame* posFrame)
{
    X9AniPosFrame* apf = dynamic_cast<X9AniPosFrame*>(library->createObject("AniPosFrame"));
//    X9AniPosFrame* apf = create();
    apf->isEmpty = posFrame->isEmpty;
    apf->a = posFrame->a;
    apf->b = posFrame->b;
    apf->r = posFrame->r;
    apf->reverse = posFrame->reverse;
    return apf;
}
X9AniPosFrame* X9AniPosFrame::newAniPosFrame(X9Library* library)
{
    X9AniPosFrame* apf = create();
    apf->isEmpty = true;
    return apf;
}
X9AniPosFrame* X9AniPosFrame::create()
{
    CREATE_NEW_AUTO_OBJ(X9AniPosFrame,);
}
X9AniPosFrame::X9AniPosFrame():X9Object("X9AniPosFrame")
{
    x9_setCtor(AniPosFrame);
    a.x = 0;
    a.y = 0;
    b.x = 0;
    b.y = 0;
}

void X9AniPosFrame::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() == 0,"new AniPosFrame Error!!!");
    a.x = 0;
    a.y = 0;
    b.x = 0;
    b.y = 0;
}

string X9AniPosFrame::toString()
{
    std::ostringstream oss;
    oss<<"[ax:";
    oss<<a.x;
    oss<<", ay:";
    oss<<a.y;
    oss<<", bx:";
    oss<<b.y;
    oss<<", by:";
    oss<<b.y;
    oss<<"]";
    return oss.str();
}