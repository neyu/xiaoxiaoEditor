//
//  X9Vec3.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/2.
//
//

#include "X9Vec3.h"

void baseSet_vec3_x(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set x Error!!!");
    X9Vec3* point = dynamic_cast<X9Vec3*>(target);
    point->v.x = value->getNumber();
}
X9ValueObject* baseGet_vec3_x(X9RunObject* target)
{
    X9Vec3* point = dynamic_cast<X9Vec3*>(target);
    return X9ValueObject::createWithNumber(point->v.x);
}
void baseSet_vec3_y(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set y Error!!!");
    X9Vec3* point = dynamic_cast<X9Vec3*>(target);
    point->v.y = value->getNumber();
}
X9ValueObject* baseGet_vec3_y(X9RunObject* target)
{
    X9Vec3* point = dynamic_cast<X9Vec3*>(target);
    return X9ValueObject::createWithNumber(point->v.y);
}
void baseSet_vec3_z(X9RunObject* target,X9ValueObject* value)
{
    X9ASSERT(value->isNumber(),"set z Error!!!");
    X9Vec3* point = dynamic_cast<X9Vec3*>(target);
    point->v.z = value->getNumber();
}
X9ValueObject* baseGet_vec3_z(X9RunObject* target)
{
    X9Vec3* point = dynamic_cast<X9Vec3*>(target);
    return X9ValueObject::createWithNumber(point->v.z);
}
void X9Vec3::setBaseFunctions(X9Library* library, const string& className)
{
    //    x9_AddBaseFunc(testFunc);
    x9_AddBaseSGet(vec3_,x);
    x9_AddBaseSGet(vec3_,y);
    x9_AddBaseSGet(vec3_,z);
}
void X9Vec3::setConstValues(X9ScriptClassData* classData)
{
    
}
X9Vec3* X9Vec3::newVec3(X9Library* library, float x, float y, float z)
{
    X9Vec3* point = dynamic_cast<X9Vec3*>(library->createObject("Vec3"));
    point->v.x = x;
    point->v.y = y;
    point->v.z = z;
    return point;
}
X9Vec3* X9Vec3::create()
{
    CREATE_NEW_AUTO_OBJ(X9Vec3,);
}
X9Vec3::X9Vec3():X9Object("X9Vec3")
{
    x9_setCtor(Vec3);
    v.x = 0;
    v.y = 0;
    v.z = 0;
}

void X9Vec3::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() <= 3,"new Vec3 Error!!!");
    if(vs.size() == 1 && vs[0]->isObject<X9Vec3*>()) {
        v = vs[0]->getObject<X9Vec3*>()->v;
    }else{
        for (int i = 0; i<vs.size(); i++) {
            X9ASSERT(vs[i]->isNumber(),"new Size Error!!!");
        }
        v.x = vs.size() > 0 ? vs[0]->getNumber() : 0;
        v.y = vs.size() > 1 ? vs[1]->getNumber() : 0;
        v.z = vs.size() > 2 ? vs[2]->getNumber() : 0;
    }
}

X9Vec3* X9Vec3::operator+(X9Vec3* p)
{
    return newVec3(getLibrary(),v.x+p->v.x,v.y+p->v.y,v.z+p->v.z);
}
X9Vec3* X9Vec3::operator-(X9Vec3* p)
{
    return newVec3(getLibrary(),v.x-p->v.x,v.y-p->v.y,v.z-p->v.z);
}
string X9Vec3::toString()
{
    std::ostringstream oss;
    oss<<"[Vec3 x:";
    oss<<v.x;
    oss<<", y:";
    oss<<v.y;
    oss<<", z:";
    oss<<v.z;
    oss<<"]";
    return oss.str();
}