//
//  X9Math.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/7.
//
//

#include "X9Math.h"
#include "X9Point.h"
#include "../../XMath.h"
//#include "../x9Utils.h"

X9ValueObject* base_abs(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(abs(values[0]->getNumber()));
}
X9ValueObject* base_acos(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(acos(values[0]->getNumber()));
}
X9ValueObject* base_asin(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(asin(values[0]->getNumber()));
}
X9ValueObject* base_atan(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(atan(values[0]->getNumber()));
}
X9ValueObject* base_atan2(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->isNumber() && values[1]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(atan2(values[0]->getNumber(),values[1]->getNumber()));
}
X9ValueObject* base_ceil(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(ceil(values[0]->getNumber()));
}
X9ValueObject* base_cos(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(cos(values[0]->getNumber()));
}
X9ValueObject* base_exp(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(exp(values[0]->getNumber()));
}
X9ValueObject* base_floor(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(floor(values[0]->getNumber()));
}
X9ValueObject* base_log(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(log(values[0]->getNumber()));
}
X9ValueObject* base_max(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(!values.empty() && values[0]->isNumber(),"sin Error!!!");
    float n = values[0]->getNumber();
    for (int i = 1; i<values.size(); i++) {
        X9ASSERT(values[i]->isNumber(),"sin Error!!!");
        n = MAX(n,values[i]->getNumber());
    }
    return X9ValueObject::createWithNumber(n);
}
X9ValueObject* base_min(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(!values.empty() && values[0]->isNumber(),"sin Error!!!");
    float n = values[0]->getNumber();
    for (int i = 1; i<values.size(); i++) {
        X9ASSERT(values[i]->isNumber(),"sin Error!!!");
        n = MIN(n,values[i]->getNumber());
    }
    return X9ValueObject::createWithNumber(n);
}
X9ValueObject* base_pow(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->isNumber() && values[1]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(pow(values[0]->getNumber(),values[1]->getNumber()));
}
X9ValueObject* base_round(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(round(values[0]->getNumber()));
}
X9ValueObject* base_sin(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(sin(values[0]->getNumber()));
}
X9ValueObject* base_sqrt(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(sqrt(values[0]->getNumber()));
}
X9ValueObject* base_tan(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"sin Error!!!");
    return X9ValueObject::createWithNumber(tan(values[0]->getNumber()));
}
X9ValueObject* base_mix(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 3 && values[2]->isNumber(),"mix Error!!!");
    float v = values[2]->getNumber();
    if(values[0]->isNumber() && values[1]->isNumber())
    {
        return X9ValueObject::createWithNumber(XMath::mix(values[0]->getNumber(), values[1]->getNumber(), v));
    }else if(values[0]->isObject<X9Point*>() && values[1]->isObject<X9Point*>())
    {
        return X9ValueObject::createWithObject(X9Point::newPoint(target->getLibrary(), XMath::mix(values[0]->getObject<X9Point*>()->point, values[1]->getObject<X9Point*>()->point, v)));
    }else
    {
        X9ASSERT(false,"mix Error!!!");
    }
}
X9ValueObject* base_umix(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 3 && values[0]->isNumber() && values[1]->isNumber() && values[2]->isNumber(),"umix Error!!!");
    return X9ValueObject::createWithNumber(XMath::umix(values[0]->getNumber(), values[1]->getNumber(), values[2]->getNumber()));
}




X9ValueObject* baseGet_math_PI(X9RunObject* target)
{
    return X9ValueObject::createWithNumber(M_PI);
}
X9ValueObject* baseGet_math_PI2(X9RunObject* target)
{
    return X9ValueObject::createWithNumber(M_PI*2);
}
X9ValueObject* baseGet_math_PI_2(X9RunObject* target)
{
    return X9ValueObject::createWithNumber(M_PI_2);
}
X9ValueObject* baseGet_math_E(X9RunObject* target)
{
    return X9ValueObject::createWithNumber(M_E);
}
X9ValueObject* baseGet_math_LN10(X9RunObject* target)
{
    return X9ValueObject::createWithNumber(M_LN10);
}
X9ValueObject* baseGet_math_LN2(X9RunObject* target)
{
    return X9ValueObject::createWithNumber(M_LN2);
}
X9ValueObject* baseGet_math_LOG10E(X9RunObject* target)
{
    return X9ValueObject::createWithNumber(M_LOG10E);
}
X9ValueObject* baseGet_math_LOG2E(X9RunObject* target)
{
    return X9ValueObject::createWithNumber(M_LOG2E);
}
X9ValueObject* baseGet_math_SQRT1_2(X9RunObject* target)
{
    return X9ValueObject::createWithNumber(M_SQRT1_2);
}
X9ValueObject* baseGet_math_SQRT2(X9RunObject* target)
{
    return X9ValueObject::createWithNumber(M_SQRT2);
}
void X9Math::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(,abs);
    x9_AddBaseFunc(,acos);
    x9_AddBaseFunc(,asin);
    x9_AddBaseFunc(,atan);
    x9_AddBaseFunc(,atan2);
    x9_AddBaseFunc(,ceil);
    x9_AddBaseFunc(,cos);
    x9_AddBaseFunc(,exp);
    x9_AddBaseFunc(,floor);
    x9_AddBaseFunc(,log);
    x9_AddBaseFunc(,max);
    x9_AddBaseFunc(,min);
    x9_AddBaseFunc(,pow);
    x9_AddBaseFunc(,round);
    x9_AddBaseFunc(,sin);
    x9_AddBaseFunc(,sqrt);
    x9_AddBaseFunc(,tan);
    
    x9_AddBaseFunc(,mix);
    x9_AddBaseFunc(,umix);
    
    
    x9_AddBaseGet(math_,PI);
    x9_AddBaseGet(math_,PI2);
    x9_AddBaseGet(math_,PI_2);
    x9_AddBaseGet(math_,E);
    x9_AddBaseGet(math_,LN10);
    x9_AddBaseGet(math_,LN2);
    x9_AddBaseGet(math_,LOG10E);
    x9_AddBaseGet(math_,LOG2E);
    x9_AddBaseGet(math_,SQRT1_2);
    x9_AddBaseGet(math_,SQRT2);
}
void X9Math::setConstValues(X9ScriptClassData* classData)
{
//    x9_AddConstValue(Number, PI, M_PI);
//    x9_AddConstValue(Number, PI2, M_PI*2);
//    x9_AddConstValue(Number, PI_2, M_PI_2);
//    x9_AddConstValue(Number, E, M_E);
//    x9_AddConstValue(Number, LN10, M_LN10);
//    x9_AddConstValue(Number, LN2, M_LN2);
//    x9_AddConstValue(Number, LOG10E, M_LOG10E);
//    x9_AddConstValue(Number, LOG2E, M_LOG2E);
//    x9_AddConstValue(Number, SQRT1_2, M_SQRT1_2);
//    x9_AddConstValue(Number, SQRT2, M_SQRT2);
}
X9Math* X9Math::create()
{
    CREATE_NEW_AUTO_OBJ(X9Math,);
}
X9Math::X9Math():X9Object("X9Math")
{
    x9_setCtor(Math);
}
void X9Math::initObject(const vector<X9ValueObject*>& vs)
{
    
}