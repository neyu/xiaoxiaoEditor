//
//  X9UserDefined.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/14.
//
//

#include "X9UserDefault.h"
#include "../X9Player.h"


X9ValueObject* base_userDefined_save(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"removeData Error!!!");
    X9UserDefault* obj = dynamic_cast<X9UserDefault*>(target);
    obj->save();
    return nullptr;
}
X9ValueObject* base_userDefined_clear(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"clear Error!!!");
    X9UserDefault* obj = dynamic_cast<X9UserDefault*>(target);
    obj->clear();
    return nullptr;
}
X9ValueObject* baseGet_userDefault_data(X9RunObject* target)
{
    X9UserDefault* obj = dynamic_cast<X9UserDefault*>(target);
    return X9ValueObject::createWithObject(obj->_data);
}
void baseSet_userDefault_isAutoSave(X9RunObject* target, X9ValueObject* value)
{
    X9ASSERT(value->isBool(),"isAutoSave Error!!!");
    X9UserDefault* obj = dynamic_cast<X9UserDefault*>(target);
    obj->_isAutoSave = value->getBool();
}
X9ValueObject* baseGet_userDefault_isAutoSave(X9RunObject* target)
{
    X9UserDefault* obj = dynamic_cast<X9UserDefault*>(target);
    return X9ValueObject::createWithBool(obj->_isAutoSave);
}
void X9UserDefault::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(userDefined_,save);
    x9_AddBaseFunc(userDefined_,clear);
    
    x9_AddBaseGet(userDefault_,data);
    x9_AddBaseSGet(userDefault_,isAutoSave);
}
void X9UserDefault::setConstValues(X9ScriptClassData* classData)
{
    
}
X9UserDefault* X9UserDefault::create()
{
    CREATE_NEW_AUTO_OBJ(X9UserDefault,);
}
X9UserDefault::X9UserDefault():X9Object("X9UserDefault")
{
    _isAutoSave = true;
    _data = nullptr;
    _key = "x9UserDefault";
    x9_setCtor(UserDefault);
}

void X9UserDefault::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.empty(),"new UserDefined Error!!!");
    _isAutoSave = true;
    _key = "x9UserDefault_"+getPlayer()->path->pathName;
    _data = loadjson(UserDefault::getInstance()->getStringForKey(_key.c_str(), "{}"))->getObject<X9Object*>();
    _data->use();
}
void X9UserDefault::removed()
{
    if (_isAutoSave) {
        save();
    }
    _isAutoSave = true;
    _data->nonuse();
    _data = nullptr;
    
    X9Object::removed();
}
void X9UserDefault::save()
{
    UserDefault::getInstance()->setStringForKey(_key.c_str(), _data->getJsonString());
    UserDefault::getInstance()->flush();
}
void X9UserDefault::clear()
{
    for (auto it = _data->propertys.begin(); it != _data->propertys.end(); it++) {
        _data->deleteValue(it->first);
    }
    _data->propertys.clear();
}