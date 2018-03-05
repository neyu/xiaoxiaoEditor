//
//  X9Object.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/25.
//
//

#include "X9Object.h"
#include "X9RunScript.h"
#include "X9Array.h"
#include "X9Player.h"
//#include "<#header#>"

/**
 * @function:wath
 * @param:string name
 * @param:Function call function
 * @return:null
 *
 */
X9ValueObject* base_obj_watch(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9Object* obj = dynamic_cast<X9Object*>(target);
    X9ASSERT(values.size() == 2 && values[0]->isString() && values[1]->isFunction(),"watch Error!!!");
    obj->addWatch(values[0]->getString(), values[1]);
    return nullptr;
}
/**
 * @function:removeWatch (function:removeWatch(),function:removeWatch(name),function:removeWatch(name,func))
 * @param:string name
 * @param:Function call function
 * @return:null
 *
 */
X9ValueObject* base_obj_removeWatch(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9Object* obj = dynamic_cast<X9Object*>(target);
    X9ASSERT(values.size() <= 2,"removeWatch Error!!!");
    if (values.empty()) {
        obj->removeWatch();
    }else if(values.size() == 1)
    {
        X9ASSERT(values[0]->isString(),"removeWatch Error!!!");
        obj->removeWatch(values[0]->getString());
    }else{
        X9ASSERT(values[0]->isString() && values[1]->isFunction(),"removeWatch Error!!!");
        obj->removeWatch(values[0]->getString(),values[1]);
    }
    return nullptr;
}
/**
 * @function:get
 * @param:string name
 * @return:value
 *
 */
X9ValueObject* base_obj_get(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && (values[0]->isString() || values[0]->isNumber()),"get Error!!!");
    X9Object* obj = dynamic_cast<X9Object*>(target);
    const string& name = values[0]->getString();
    if (obj->propertys.find(name) != obj->propertys.end()) {
        return obj->propertys[name]->clone();
    }
    if (obj->getClassData()->baseGetKeys.find(name) != obj->getClassData()->baseGetKeys.end()) {
        return obj->getLibrary()->baseGetFunctions[obj->getClassData()->baseGetKeys[name]][name](obj)->clone();
    }
    return X9ValueObject::create();
}
/**
 * @function:set
 * @param:string name
 * @param:* name
 * @return:null
 *
 */
X9ValueObject* base_obj_set(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && (values[0]->isString() || values[0]->isNumber()),"at Error!!!");
    X9Object* obj = dynamic_cast<X9Object*>(target);
    const string& name = values[0]->getString();
    if (obj->propertys.find(name) == obj->propertys.end()) {
        obj->propertys[name] = values[1];
        obj->propertys[name]->use();
    }else if (obj->getClassData()->baseSetKeys.find(name) != obj->getClassData()->baseSetKeys.end()) {
        obj->getLibrary()->baseSetFunctions[obj->getClassData()->baseSetKeys[name]][name](obj,values[1]);
    }else{
        obj->propertys[name]->setValueObject(values[1]);
    }
    return nullptr;
}
X9ValueObject* base_obj_del(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && (values[0]->isString() || values[0]->isNumber()),"at Error!!!");
    X9Object* obj = dynamic_cast<X9Object*>(target);
    obj->deleteValue(values[0]->getString());
    return nullptr;
}
/**
 * @function:has
 * @param:string name
 * @return:bool
 *
 */
X9ValueObject* base_obj_has(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && (values[0]->isString() || values[0]->isNumber()),"has Error!!!");
    const string& name = values[0]->getString();
    X9Object* obj = dynamic_cast<X9Object*>(target);
    return X9ValueObject::createWithBool(obj->has(name));
}
X9ValueObject* base_obj_getPropertyNames(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"getPropertyNames Error!!!");
    X9Object* obj = dynamic_cast<X9Object*>(target);
    X9Array* arr = X9Array::newArray(target->getLibrary());
    for(auto it = obj->propertys.begin();it != obj->propertys.end();it++)
    {
        arr->pushString(it->first);
    }
    for(auto it = obj->getClassData()->baseSetKeys.begin();it != obj->getClassData()->baseSetKeys.end();it++)
    {
        arr->pushString(it->first);
    }
    return X9ValueObject::createWithObject(arr);
}
///**
// * @function:hasOwnConst
// * @param:string name
// * @return:bool
// *
// */
//X9ValueObject* base_obj_hasOwnConst(X9RunObject* target, const vector<X9ValueObject*>& values)
//{
//    X9ASSERT(values.size() == 1 && values[0]->isString(),"hasOwnConst Error!!!");
//    const string& name = values[0]->getString();
//    X9Object* obj = dynamic_cast<X9Object*>(target);
//    return X9ValueObject::createWithBool(obj->classData->constValues.find(name) != obj->classData->constValues.end());
//}
///**
// * @function:hasOwnFunction
// * @param:string name
// * @return:bool
// *
// */
//X9ValueObject* base_obj_hasOwnFunction(X9RunObject* target, const vector<X9ValueObject*>& values)
//{
//    X9ASSERT(values.size() == 1 && values[0]->isString(),"hasOwnFunction Error!!!");
//    const string& name = values[0]->getString();
//    X9Object* obj = dynamic_cast<X9Object*>(target);
//    if (obj->classData->functions.find(name) == obj->classData->functions.end()) {
//        return X9ValueObject::createWithBool(obj->classData->baseFunctionKeys.find(name) != obj->classData->baseFunctionKeys.end());
//    }
//    return X9ValueObject::createWithBool(true);
//}
//X9ValueObject* base_obj_clone(X9RunObject* target, const vector<X9ValueObject*>& values)
//{
//    X9ASSERT(values.size() == 1 && values[0]->isString(),"hasOwnFunction Error!!!");
//    const string& name = values[0]->getString();
//    X9Object* obj = dynamic_cast<X9Object*>(target);
//    return X9ValueObject::createWithObject(obj->clone());
//}
X9ValueObject* base_obj_test(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    log("Object_test");
    return nullptr;
}

//X9ValueObject* baseGet_obj_timeSpeed(X9RunObject* target)
//{
//    X9Object* obj = dynamic_cast<X9Object*>(target);
//    return X9ValueObject::createWithNumber(obj->getTimeSpeed());
//}
//X9ValueObject* baseGet_obj_displayTimeSpeed(X9RunObject* target)
//{
//    X9Object* obj = dynamic_cast<X9Object*>(target);
//    return X9ValueObject::createWithNumber(obj->getDisplayedTimeSpeed());
//}
X9ValueObject* baseGet_obj_isExoteric(X9RunObject* target)
{
    X9Object* obj = dynamic_cast<X9Object*>(target);
    return X9ValueObject::createWithBool(obj->_isExoteric);
}
void baseSet_obj_isExoteric(X9RunObject* target, X9ValueObject* value)
{
    X9ASSERT(value->isBool(),"set isExoteric Error!!!");
    X9Object* obj = dynamic_cast<X9Object*>(target);
    obj->_isExoteric = value->getBool();
}
X9Object::WatchCommand::WatchCommand()
{
    isRemoved = false;
    isWatching = false;
    isRemoveAlled = false;
}
X9Object::WatchCommand::~WatchCommand()
{
    for (int i = 0; i<funcs.size(); i++) {
        funcs[i]->nonuse();
    }
    funcs.clear();
}
void X9Object::WatchCommand::addFunc(X9ValueObject* func)
{
    func->use();
    funcs.push_back(func);
}
void X9Object::WatchCommand::removeFunc(X9ValueObject* func)
{
    for (auto it = funcs.begin(); it != funcs.end(); it++) {
        if((**it == func)->getBool())
        {
            (*it)->nonuse();
            funcs.erase(it);
            break;
        }
    }
}
bool X9Object::WatchCommand::checkFunc(X9ValueObject* func)
{
    for (int i = 0; i<funcs.size(); i++) {
        if ((*funcs[i] == func)->getBool()) {
            return true;
        }
    }
    return false;
}
void X9Object::setBaseFunctions(X9Library* library, const string& className)
{
    //x9_AddBaseFunc(obj_,test);
    x9_AddBaseFunc(obj_,watch);
    x9_AddBaseFunc(obj_,removeWatch);
    x9_AddBaseFunc(obj_,get);
    x9_AddBaseFunc(obj_,set);
    x9_AddBaseFunc(obj_,del);
    x9_AddBaseFunc(obj_,has);
    x9_AddBaseFunc(obj_,getPropertyNames);
//    x9_AddBaseFunc(obj_,hasOwnConst);
//    x9_AddBaseFunc(obj_,hasOwnFunction);
//    x9_AddBaseFunc(obj_,clone);
    
    x9_AddBaseGet(obj_,isExoteric);
//    x9_AddBaseGet(obj_,timeSpeed);
//    x9_AddBaseGet(obj_,displayTimeSpeed);
}

void X9Object::setConstValues(X9ScriptClassData* classData)
{
}
X9Object* X9Object::newObject(X9Library* library)
{
    return dynamic_cast<X9Object*>(library->createObject("Object"));
}
X9Object* X9Object::create()
{
    CREATE_NEW_AUTO_OBJ(X9Object,"X9Object");
}
X9Object::X9Object(const string& key):X9RunObject(key)
{
    _isExoteric = false;
    _displayTimeSpeed = 1;
    _realTimeSpeed = 1;
    _target = this;
    x9_setCtor(Object);
}
/**
 * @class:Object
 * @return:null
 *
 */
void X9Object::initScript(X9ScriptClassData* classData,const vector<X9ValueObject*>& vs)
{
    if (classData->className == "Object") {
        _isExoteric = true;
    }
    isRunRemovedCall = true;
    _target = this;
    this->className = classData->className;
    _currentCtorName = this->className;
    this->classData = classData;
    for (int i = 0; i<classData->propertys.size(); i++) {
        propertys[classData->propertys[i]->getStringValue()] = X9RunScript::run(this, classData->propertys[i]->arguments[0],classData);
        propertys[classData->propertys[i]->getStringValue()]->use();
    }
    if (classData->isBaseClass) {
        runCtor(vs);
    }else if (classData->functions.find(className) != classData->functions.end()) {
        runFunction(MT_FUNCTION, className, vs,classData);
    }
}
void X9Object::runSuperCtor(const string& name,const vector<X9ValueObject*>& vs)
{
    if (ctors.find(name) != ctors.end()) {
        ctors[name](vs);
    }else{
        runFunction(MT_FUNCTION, name, vs,classData);
    }
}
void X9Object::initObject(const vector<X9ValueObject*>& vs)
{
//    log("from:%s, X9Object::initObject()",className.c_str());
}
void X9Object::removedCall()
{
//    if (isRunRemovedCall && has("@removed")) {
//        vector<X9ValueObject*> vs;
//        runFunction(MemberType::MT_FUNCTION, "@removed", vs, classData);
//    }
}
void X9Object::removed()
{
    removedCall();
    _isExoteric = false;
    _displayTimeSpeed = 1;
    _realTimeSpeed = 1;
    removeWatch();
    if (getPlayer()->isLife()) {
        getPlayer()->targetManager->clearTimerTarget(this);
    }
    for (auto it = propertys.begin(); it != propertys.end(); it++) {
        it->second->nonuse();
    }
    propertys.clear();
    X9RunObject::removed();
}
void X9Object::_setValueWatch(MemberType mt, const string& name, X9ValueObject* value)
{
    if (_watchCommands.find(name) == _watchCommands.end()) {
        X9RunObject::setValue(mt, name, value);
        return;
    }
    X9ValueObject* oldValue;
    bool isWatch = mt == MT_PROPERTY || mt == MT_SGET;
    if (isWatch) {
        oldValue = getValue(mt, name);
    }
    X9RunObject::setValue(mt, name, value);
    if (isWatch) {
        if (!_watchCommands[name].isWatching) {
            _watchCommands[name].isWatching = true;
            vector<X9ValueObject*> vs;
            vs.push_back(X9ValueObject::createWithObject(this));
            vs.push_back(X9ValueObject::createWithString(name));
            vs.push_back(oldValue->clone());
            vs.push_back(value->clone());
            for (auto it = _watchCommands[name].funcs.begin(); it != _watchCommands[name].funcs.end(); it++) {
                (*it)->getFunction()->runFunction(vs);
            }
            _watchCommands[name].isWatching = false;
            if (_watchCommands[name].isRemoved) {
                removeWatch(name);
            }
            if (_watchCommands[name].isRemoveAlled) {
                removeWatch();
            }
        }
    }
}
void X9Object::setValue(MemberType mt, const string& name, X9ValueObject* value)
{
    if (_watchCommands.empty()) {
        X9RunObject::setValue(mt, name, value);
    }else{
        _setValueWatch(mt,name,value);
    }
}
bool X9Object::has(const string& name)
{
    
    bool isHas = propertys.find(name) != propertys.end();
    if(!isHas) {
        isHas = classData->constValues.find(name) != classData->constValues.end();
    }
    if (!isHas) {
        isHas = classData->baseFunctionKeys.find(name) != classData->baseFunctionKeys.end();
    }
    if (!isHas) {
        isHas = classData->functions.find(name) != classData->functions.end();
    }
    return isHas;
}
void X9Object::deleteValue(const string& name)
{
    if (propertys.find(name) != propertys.end()) {
        removeWatch(name);
        propertys[name]->nonuse();
        propertys.erase(name);
    }
}
void X9Object::addWatch(const string& name,X9ValueObject* func)
{
    if (_watchCommands.find(name) == _watchCommands.end()) {
        _watchCommands[name] = WatchCommand();
    }
    if (!_watchCommands[name].checkFunc(func)) {
        _watchCommands[name].addFunc(func);
    }
}
void X9Object::removeWatch(const string& name,X9ValueObject* func)
{
    if (name == "") {
        for (auto it = _watchCommands.begin(); it != _watchCommands.end(); it++) {
            if (it->second.isWatching) {
                it->second.isRemoveAlled = true;
                return;
            }
        }
        _watchCommands.clear();
    }else if(func == nullptr)
    {
        if (_watchCommands.find(name) != _watchCommands.end() && _watchCommands[name].isWatching) {
            _watchCommands[name].isRemoved = true;
            return;
        }
        _watchCommands.erase(name);
    }else if(_watchCommands.find(name) != _watchCommands.end())
    {
        _watchCommands[name].removeFunc(func);
    }
}
void X9Object::addNumberValue(const string& name, float value)
{
    setValue(MemberType::MT_PROPERTY, name, X9ValueObject::createWithNumber(value));
}
void X9Object::addBoolValue(const string& name, bool value)
{
    setValue(MemberType::MT_PROPERTY, name, X9ValueObject::createWithBool(value));
}
void X9Object::addStringValue(const string& name, const string& value)
{
    setValue(MemberType::MT_PROPERTY, name, X9ValueObject::createWithString(value));
}
void X9Object::addObjectValue(const string& name, X9Object* value)
{
    setValue(MemberType::MT_PROPERTY, name, X9ValueObject::createWithObject(value));
}
void X9Object::addNoneValue(const string& name)
{
    setValue(MemberType::MT_PROPERTY, name, X9ValueObject::create());
}
string X9Object::getJsonString()
{
    string str = "{";
    for (auto it = propertys.begin(); it != propertys.end(); it++) {
        if (it != propertys.begin()) {
            str += ",";
        }
        str += "\""+it->first+"\":"+it->second->getJsonString();
    }
    return str+"}";
}
//X9Object* X9Object::clone()
//{
//    return newObject(getLibrary());
//}
X9ValueObject* X9Object::loadjson(const string& jsonStr)
{
    rapidjson::Document _doc;
    _doc.Parse<0>(jsonStr.c_str());
    return loadjson(_doc);
}
X9ValueObject* X9Object::loadjson(const rapidjson::Value &jsonData)
{
    if(jsonData.IsString())
    {
        return X9ValueObject::createWithString(jsonData.GetString());
    }else if(jsonData.IsNumber())
    {
        return X9ValueObject::createWithNumber(jsonData.GetDouble());
    }else if(jsonData.IsBool())
    {
        return X9ValueObject::createWithBool(jsonData.GetBool());
    }else if(jsonData.IsArray())
    {
        X9Array* arr = X9Array::newArray(getLibrary());
        for (int i = 0; i<jsonData.Size(); i++) {
            arr->push(loadjson(jsonData[i]));
        }
        return X9ValueObject::createWithObject(arr);
    }else if(jsonData.IsObject())
    {
        X9Object* obj = X9Object::newObject(getLibrary());
        for (auto it = jsonData.MemberonBegin(); it != jsonData.MemberonEnd(); it++) {
            obj->setValue(MemberType::MT_PROPERTY, it->name.GetString(), loadjson(it->value));
        }
        return X9ValueObject::createWithObject(obj);
    }
    X9ASSERT(false,"loadjson() type Error!!!");
}
string X9Object::toString()
{
    return "[class "+className+"]";
}