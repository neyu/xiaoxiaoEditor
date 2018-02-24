//
//  X9RunScript.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/26.
//
//

#include "X9RunScript.h"
#include "X9Player.h"
#include "baseClasses/X9Array.h"
#include "baseClasses/X9BaseGlobal.h"
#include "../table/X9TableLoader.h"

#include "baseClasses/action/X9EaseAction.h"
#include "../XMath.h"
//#include "baseClasses/action/X9Sequence.h"
//#include "baseClasses/action/X9Spawn.h"
//#include "baseClasses/action/X9MoveBy.h"
//#include "baseClasses/action/X9MoveTo.h"
struct _TempExpressionObject
{
    union{
        X9ValueObject* v;
        X9ScriptData* d;
    }value;
    X9ScriptClassData* cData;
    bool isValue;
    _TempExpressionObject(X9ValueObject* value,X9ScriptClassData* cData)
    {
        this->value.v = value;
        isValue = true;
        this->cData = cData;
    }
    _TempExpressionObject(X9ScriptData* data,X9ScriptClassData* cData)
    {
        this->value.d = data;
        isValue = false;
        this->cData = cData;
    }
    X9ValueObject* getValue(X9RunObject* runObj)
    {
        if(!isValue) {
            isValue = true;
            value.v = X9RunScript::run(runObj, value.d,cData);
        }
        return value.v;
    }
};
X9ValueObject* type_expression(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    vector<_TempExpressionObject> values;
    for (int i = 0; i<script->arguments.size(); i++) {
        if (script->arguments[i]->type == X9ScriptData::OPERATOR) {
            
            X9ASSERT(values.size() >= 2,"expression Error!!!");
            X9ValueObject* a = values[values.size()-2].getValue(runObj);
            X9ValueObject* b = values[values.size()-1].getValue(runObj);
            values.erase(values.end()-2,values.end());
            values.push_back(_TempExpressionObject(X9RunScript::operatorCommands[script->arguments[i]->getStringValue()](runObj,a,b),currentClass));
        }else{
            values.push_back(_TempExpressionObject(script->arguments[i],currentClass));
        }
    }
    X9ASSERT(values.size() == 1,"expression Error!!!");
    return values[0].value.v;
}
X9ValueObject* type_evaluation(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    const string& name = script->getStringValue();
//    if (!script->pNames.empty()) {
//        X9Library* library = dynamic_cast<X9Object*>(runObj->_target)->classData->library;
//        X9ASSERT(library->classes.find(script->pNames[0]) == library->classes.end(),"evaluation Error!!!");
//    }
    X9RunObject* tempObj = runObj->getUseRunObject(name, script->pNames);
    X9RunObject::MemberType mt = tempObj->checkValue(name,false);
    if (mt == X9RunObject::MT_NONE) {
        X9Object* obj = dynamic_cast<X9Object*>(tempObj->_target);
        X9ASSERT(obj->_isExoteric,"expression Error!!!");
        obj->setValue(X9RunObject::MT_PROPERTY, name, X9RunScript::run(runObj, script->arguments[0],currentClass));
        return X9ValueObject::create();
    }
    X9ASSERT(mt == X9RunObject::MT_PROPERTY || mt == X9RunObject::MT_VALUE || mt == X9RunObject::MT_SGET,"expression Error!!!");
    tempObj->setValue(mt, name, X9RunScript::run(runObj, script->arguments[0],currentClass));
    return X9ValueObject::create();
}
X9ValueObject* type_command(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    const string& name = script->getStringValue();
//    if (!script->pNames.empty()) {
//        X9Library* library = dynamic_cast<X9Object*>(runObj->_target)->classData->library;
//        X9ASSERT(library->classes.find(script->pNames[0]) == library->classes.end(),"command Error!!!");
//    }
    if (script->pNames.empty() && X9RunScript::keywordCommands.find(name) != X9RunScript::keywordCommands.end()) {
        vector<X9ValueObject*> values;
        for (int i = 0; i<script->arguments.size(); i++) {
            values.push_back(X9RunScript::run(runObj, script->arguments[i],currentClass));
        }
        return X9RunScript::keywordCommands[name](runObj,values,script);
    }
    X9RunObject* newRunObj = runObj->getUseRunObject(name, script->pNames);
    bool isSuper = !script->pNames.empty() && script->pNames[0] == "super";
    X9RunObject::MemberType mt = newRunObj->checkFunction(name, isSuper);
    X9ASSERT(mt != X9RunObject::MT_NONE && mt != X9RunObject::MT_CONST,"command Error!!!");
    vector<X9ValueObject*> values;
    for (int i = 0; i<script->arguments.size(); i++) {
        values.push_back(X9RunScript::run(runObj, script->arguments[i],currentClass));
    }
    X9ScriptClassData* cData = currentClass;
    if (mt == X9RunObject::MT_SUPER_FUNCTION) {
        bool isOK = false;
        for(int i = 0;i<cData->extends.size();i++)
        {
            X9ScriptClassData* td = runObj->getLibrary()->getClassData(cData->extends[i]);
            if (td->functions.find(name) != td->functions.end()) {
                cData = td;
                isOK = true;
                break;
            }else if(td->baseFunctionKeys.find(name) != td->baseFunctionKeys.end())
            {
                cData = td;
                isOK = true;
                mt = X9RunObject::MT_BASE_FUNCTION;
                break;
            }
        }
        X9ASSERT(isOK,"command Error!!!");
    }
    return newRunObj->runFunction(mt, name, values,cData);
}
X9ValueObject* type_value(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    if(script->valueType == X9ScriptData::CodeType::NONE)
    {
        return X9ValueObject::create();
    }
    const string& name = script->getStringValue();
    if (name == "null") {
        return X9ValueObject::create();
    }
    if (script->pNames.empty()) {
        if (name == "this") {
            return X9ValueObject::createWithObject(runObj->_target);
        }
        if (name == "global") {
            return X9ValueObject::createWithObject(runObj->getLibrary()->global);
        }
    }else if (name == "this" || name == "global") {
        X9ASSERT(false,"value Error!!!");
    }
    X9Library* library = runObj->getLibrary();
    if (script->pNames.size() == 1 && library->classes.find(script->pNames[0]) != library->classes.end()) {
        X9ASSERT(library->classes[script->pNames[0]]->constValues.find(name) != library->classes[script->pNames[0]]->constValues.end(),"value Error!!!");
        return library->classes[script->pNames[0]]->constValues[name]->clone();
    }
//    if (script->pNames.empty()) {
//        X9Library* library = dynamic_cast<X9Object*>(runObj->_target)->classData->library;
//        X9ASSERT(library->classes.find(script->pNames[0]) != library->classes.end(),"value Error!!!");
//    }
    runObj = runObj->getUseRunObject(name, script->pNames);
    X9RunObject::MemberType mt = runObj->checkValue(name,!script->pNames.empty() && script->pNames[0] == "super");
    if (mt == X9RunObject::MemberType::MT_CONST && script->pNames.size() == 1) {
        runObj->getLibrary()->addClassData(script->pNames[0]);
    }
    X9ASSERT(mt != X9RunObject::MT_NONE,"expression Error!!!");
    return runObj->getValue(mt, name);
}
X9ValueObject* type_string(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    return X9ValueObject::createWithString(script->getStringValue());
}
X9ValueObject* type_number(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    return X9ValueObject::createWithNumber(script->getNumberValue());
}
X9ValueObject* type_bool(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    return X9ValueObject::createWithBool(script->getBoolValue());
}
X9ValueObject* type_new(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    X9ASSERT(runObj->getLibrary()->checkClassData(script->getStringValue()),"new Error!!!");
    vector<X9ValueObject*> values;
    for (int i = 0; i<script->arguments.size(); i++) {
        values.push_back(X9RunScript::run(runObj, script->arguments[i],currentClass));
    }
    return runObj->getLibrary()->createValueObject(script->getStringValue(), values);
}
X9ValueObject* type_delete(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    const string& name = script->getStringValue();
    //    if (!script->pNames.empty()) {
    //        X9Library* library = dynamic_cast<X9Object*>(runObj->_target)->classData->library;
    //        X9ASSERT(library->classes.find(script->pNames[0]) == library->classes.end(),"evaluation Error!!!");
    //    }
    X9RunObject* tempObj = runObj->getUseRunObject(name, script->pNames);
    X9RunObject::MemberType mt = tempObj->checkValue(name, false);
    X9ASSERT(mt == X9RunObject::MT_PROPERTY,"delete Error!!!");
    if (mt == X9RunObject::MT_PROPERTY) {
        X9Object* obj = dynamic_cast<X9Object*>(tempObj->_target);
        X9ASSERT(obj->className == "Object","delete Error!!!");
        obj->deleteValue(name);
        return nullptr;
    }
    X9ASSERT(false,"delete Error!!!");
}
X9ValueObject* type_var(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    runObj->setValue(X9RunObject::MT_VALUE,script->getStringValue(), X9RunScript::run(runObj, script->arguments[0],currentClass));
    return X9ValueObject::create();
}
X9ValueObject* type_break(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    runObj->setBreak();
    return X9ValueObject::create();
}
X9ValueObject* type_return(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    if (script->arguments.empty()) {
        runObj->setReturnValue(X9ValueObject::create());
    }else{
        runObj->setReturnValue(X9RunScript::run(runObj, script->arguments[0],currentClass));
    }
    return X9ValueObject::create();
}
X9ValueObject* type_if(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    X9ValueObject* v = X9RunScript::run(runObj, script->arguments[0],currentClass);
    if (v->getBool()) {
        X9RunScript::runList(runObj, script->children,currentClass);
    }
    return v;
}
X9ValueObject* type_elseif(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    X9ValueObject* v = X9RunScript::run(runObj, script->arguments[0],currentClass);
    if (v->getBool()) {
        X9RunScript::runList(runObj, script->children,currentClass);
    }
    return v;
}
X9ValueObject* type_else(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    X9RunScript::runList(runObj, script->children,currentClass);
    return nullptr;
}
X9ValueObject* type_while(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    X9RunObject* tempObj = X9RunObject::createBreak(runObj->_target, runObj);
    while (X9RunScript::run(runObj, script->arguments[0],currentClass)->getBool()) {
        X9RunScript::runList(tempObj, script->children,currentClass);
        if (tempObj->getBreak() || tempObj->getReturnValue()) {
            break;
        }
    }
    return nullptr;
}
X9ValueObject* type_switch(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    X9RunObject* tempObj = X9RunObject::createBreak(runObj->_target, runObj);
    X9ValueObject* value = X9RunScript::run(runObj, script->arguments[0],currentClass);
    if (!script->children.empty()) {
        int stIdx = -1;
        for (int i = 0; i<script->children.size(); i++) {
            if (script->children[i]->type == X9ScriptData::DEFAULT) {
                stIdx = i;
            }else if((*value == X9RunScript::run(runObj, script->children[i]->arguments[0],currentClass))->getBool()){
                stIdx = i;
                break;
            }
        }
        for (int i = stIdx; i<script->children.size(); i++) {
            X9RunScript::runList(tempObj, script->children[i]->children,currentClass);
            if (tempObj->getBreak() || tempObj->getReturnValue()) {
                break;
            }
        }
    }
    return nullptr;
}
X9ValueObject* type_for(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    X9RunObject* tempObj = X9RunObject::createBreak(runObj->_target, runObj);
    for (X9RunScript::run(runObj, script->arguments[0],currentClass); X9RunScript::run(runObj, script->arguments[1],currentClass)->getBool(); X9RunScript::run(runObj, script->arguments[2],currentClass)) {
        X9RunScript::runList(tempObj, script->children,currentClass);
        if (tempObj->getBreak() || tempObj->getReturnValue()) {
            break;
        }
    }
    return nullptr;
}
X9ValueObject* type_forin(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    const string& keyName = script->getStringValue();
    X9ValueObject* objValue = X9RunScript::run(runObj, script->arguments[0],currentClass);
    X9ASSERT(objValue->isObject(),"type_foreach Error!!!");
    X9RunObject* tempObj = X9RunObject::createBreak(runObj->_target, runObj);
    X9Object* obj = objValue->getObject<X9Object*>();
    X9Array* arr = dynamic_cast<X9Array*>(obj);
    if (arr) {
        int num = arr->children.size();
        for (int i = 0; i<num; i++) {
            runObj->setValue(X9RunObject::MT_VALUE, keyName, X9ValueObject::createWithNumber(i));
            X9RunScript::runList(tempObj, script->children,currentClass);
            if (tempObj->getBreak() || tempObj->getReturnValue()) {
                break;
            }
        }
    }else{
        vector<string> keys;
        for (auto it = obj->propertys.begin(); it != obj->propertys.end(); it++) {
            keys.push_back(it->first);
        }
        for (int i = 0; i<keys.size(); i++) {
            runObj->setValue(X9RunObject::MT_VALUE, keyName, X9ValueObject::createWithString(keys[i]));
            X9RunScript::runList(tempObj, script->children,currentClass);
            if (tempObj->getBreak() || tempObj->getReturnValue()) {
                break;
            }
        }
    }
    return nullptr;
}
X9ValueObject* type_foreach(X9RunObject* runObj,X9ScriptData* script, X9ScriptClassData* currentClass)
{
    const string& valueName = script->getStringValue();
    X9ValueObject* objValue = X9RunScript::run(runObj, script->arguments[0],currentClass);
    X9ASSERT(objValue->isObject(),"type_foreach Error!!!");
    X9RunObject* tempObj = X9RunObject::createBreak(runObj->_target, runObj);
    X9Object* obj = objValue->getObject<X9Object*>();
    X9Array* arr = dynamic_cast<X9Array*>(obj);
    if (arr) {
        vector<X9ValueObject*> vs;
        for (int i = 0; i<arr->children.size(); i++) {
            vs.push_back(arr->children[i]->clone());
        }
        for (int i = 0; i<vs.size(); i++) {
            runObj->setValue(X9RunObject::MT_VALUE, valueName, vs[i]);
            X9RunScript::runList(tempObj, script->children,currentClass);
            if (tempObj->getBreak() || tempObj->getReturnValue()) {
                break;
            }
        }
    }else{
        vector<X9ValueObject*> vs;
        for (auto it = obj->propertys.begin(); it != obj->propertys.end(); it++) {
            vs.push_back(it->second->clone());
        }
        for (int i = 0;i<vs.size();i++) {
            runObj->setValue(X9RunObject::MT_VALUE, valueName, vs[i]);
            X9RunScript::runList(tempObj, script->children,currentClass);
            if (tempObj->getBreak() || tempObj->getReturnValue()) {
                break;
            }
        }
    }
    return nullptr;
}

X9ValueObject* operator_add(X9RunObject* runObj, X9ValueObject* a, X9ValueObject* b)
{
    return *a + b;
}
X9ValueObject* operator_sub(X9RunObject* runObj, X9ValueObject* a, X9ValueObject* b)
{
    return *a - b;
}
X9ValueObject* operator_mult(X9RunObject* runObj, X9ValueObject* a, X9ValueObject* b)
{
    return *a * b;
}
X9ValueObject* operator_div(X9RunObject* runObj, X9ValueObject* a, X9ValueObject* b)
{
    return *a / b;
}
X9ValueObject* operator_and(X9RunObject* runObj, X9ValueObject* a, X9ValueObject* b)
{
    return *a && b;
}
X9ValueObject* operator_or(X9RunObject* runObj, X9ValueObject* a, X9ValueObject* b)
{
    return *a || b;
}
X9ValueObject* operator_gt(X9RunObject* runObj, X9ValueObject* a, X9ValueObject* b)
{
    return *a > b;
}
X9ValueObject* operator_ge(X9RunObject* runObj, X9ValueObject* a, X9ValueObject* b)
{
    return *a >= b;
}
X9ValueObject* operator_lt(X9RunObject* runObj, X9ValueObject* a, X9ValueObject* b)
{
    return *a < b;
}
X9ValueObject* operator_le(X9RunObject* runObj, X9ValueObject* a, X9ValueObject* b)
{
    return *a <= b;
}
X9ValueObject* operator_eq(X9RunObject* runObj, X9ValueObject* a, X9ValueObject* b)
{
    return *a == b;
}
X9ValueObject* operator_et(X9RunObject* runObj, X9ValueObject* a, X9ValueObject* b)
{
    return *a != b;
}
X9ValueObject* operator_mod(X9RunObject* runObj, X9ValueObject* a, X9ValueObject* b)
{
    return *a % b;
}


X9ValueObject* keyword_breakPoint(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    int a = 0;
    log("breakPoint:%d",a);
    return nullptr;
}
X9ValueObject* keyword_super(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9Object* obj = dynamic_cast<X9Object*>(runObj->_target);
    X9ScriptClassData* superClassData = obj->getLibrary()->getClassData(obj->_currentCtorName);
    if (!superClassData->extends.empty()) {
        obj->_currentCtorName = superClassData->extends[0];
        obj->runSuperCtor(superClassData->extends[0],values);
    }
    return nullptr;
}
X9ValueObject* keyword_trace(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    std::ostringstream oss;
    //    oss<<num;
    //    std::cout<<oss.str()<<std::endl;
    for (int i = 0; i<values.size(); i++) {
        if (i > 0) {
            oss<<", ";
        }
        oss<<values[i]->getString();
    }
    
//    log("X9Script(%s)----->> %s",script->getErrorInfo().c_str(),oss.str().c_str());
    log("X9Script----->> %s",oss.str().c_str());
    //    string str = "";
    //    for (int i = 0; i<values.size(); i++) {
    //        if (i > 0) {
    //            str = str+", ";
    //        }
    //        str += values[i]->getString();
    //    }
    //    log("X9Script::%s",str.c_str());
    return nullptr;
}
X9ValueObject* keyword_IF(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() <= 3 && ((int)values.size())%2 == 1,"keyword_random Error!!!");
    for (int i = 1; i<values.size(); i+=2) {
        bool b = values[i-1]->getBool();
        if (b) {
            return values[i]->clone();
        }else if(i == values.size()-2){
            return values[i+1]->clone();
        }
    }
    return nullptr;
}

#define _rand() ((float)rand() / (float)(RAND_MAX))
X9ValueObject* keyword_random(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() <= 2,"keyword_random Error!!!");
    if (values.empty()) {
        return X9ValueObject::createWithNumber(_rand());
    }
    if (values.size() == 1) {
        X9ASSERT(values[0]->isNumber(),"keyword_random Error!!!");
        return X9ValueObject::createWithNumber(_rand()*values[0]->getNumber());
    }
    X9ASSERT(values[1]->isNumber(),"keyword_random Error!!!");
    float a = values[0]->getNumber();
    float b = values[1]->getNumber();
    return X9ValueObject::createWithNumber(a+(b-a)*_rand());
}
X9ValueObject* keyword_randomAt(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 || values.size() == 2,"keyword_randomAt Error!!!");
    X9ASSERT(values[0]->isNumber(),"keyword_randomAt Error!!!");
    int a = values[0]->getNumber();
    if (values.size() == 1) {
        return X9ValueObject::createWithNumber((int)(_rand()*a));
    }
    X9ASSERT(values[1]->isNumber(),"keyword_randomAt Error!!!");
    int b = values[1]->getNumber();
    return X9ValueObject::createWithNumber((int)(a+(b-a+1)*_rand()));
}
X9ValueObject* keyword_getTimer(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return X9ValueObject::createWithNumber(runObj->getLibrary()->timer);
}
X9ValueObject* keyword_setTimeOut(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() >= 2 && values[0]->isFunction() && values[1]->isNumber(),"keyword_setTimeOut Error!!!");
    int ID = -1;
    if (values.size() == 2) {
        ID = runObj->getPlayer()->targetManager->addTimeout(values[0]->getFunction(), values[1]->getNumber(), runObj->getLibrary()->_emptyValues);
    }else{
        vector<X9ValueObject*> vs;
        for (int i = 2; i < values.size(); i++) {
            vs.push_back(values[i]);
        }
        ID = runObj->getPlayer()->targetManager->addTimeout(values[0]->getFunction(), values[1]->getNumber(), vs);
    }
    return X9ValueObject::createWithNumber(ID);
}
X9ValueObject* keyword_clearTimeOut(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"keyword_clearTimeOut Error!!!");
    runObj->getPlayer()->targetManager->clearTimeout(values[0]->getNumber());
    return nullptr;
}
X9ValueObject* keyword_setInterval(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() >= 2 && values[0]->isFunction() && values[1]->isNumber(),"keyword_setInterval Error!!!");
    int ID = -1;
    if (values.size() == 2) {
        ID = runObj->getPlayer()->targetManager->addInterval(values[0]->getFunction(), values[1]->getNumber(), runObj->getLibrary()->_emptyValues);
    }else{
        vector<X9ValueObject*> vs;
        for (int i = 2; i < values.size(); i++) {
            vs.push_back(values[i]);
        }
        ID = runObj->getPlayer()->targetManager->addInterval(values[0]->getFunction(), values[1]->getNumber(), vs);
    }
    return X9ValueObject::createWithNumber(ID);
}
X9ValueObject* keyword_clearInterval(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"keyword_clearInterval Error!!!");
    runObj->getPlayer()->targetManager->clearInterval(values[0]->getNumber());
    return nullptr;
}
X9ValueObject* __keyword_loadJson__child(X9Library* library, const rapidjson::Value &jsonData)
{
    X9ValueObject* value = nullptr;
    if (jsonData.IsObject()) {
        X9Object* obj = dynamic_cast<X9Object*>(library->createObject("Object"));
        for (auto it = jsonData.MemberonBegin(); it != jsonData.MemberonEnd(); it++) {
            obj->propertys[it->name.GetString()] = __keyword_loadJson__child(library,it->value);
        }
        value = X9ValueObject::createWithObject(obj);
    }else if(jsonData.IsArray()) {
        X9Array* arr = dynamic_cast<X9Array*>(library->createObject("Array"));
        for (int i = 0; i<jsonData.Size(); i++) {
            arr->children.push_back(__keyword_loadJson__child(library,jsonData[i]));
        }
        value = X9ValueObject::createWithObject(arr);
    }else if(jsonData.IsString()) {
        value = X9ValueObject::createWithString(jsonData.GetString());
    }else if(jsonData.IsNumber()) {
        value = X9ValueObject::createWithNumber(jsonData.GetDouble());
    }else if(jsonData.IsBool()) {
        value = X9ValueObject::createWithBool(jsonData.GetBool());
    }else{
        X9ASSERT(false,"keyword_loadJsonChild Error!!!");
    }
    value->use();
    return value;
}
X9ValueObject* keyword_loadJson(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isString(),"keyword_loadJson Error!!!");
    loadJsonDoc(_doc,runObj->getLibrary()->path->getDataPath(values[0]->getString()));
    X9ValueObject* value = __keyword_loadJson__child(runObj->getLibrary(),_doc);
    value->nonuse();
    return value;
}
X9ValueObject* keyword_loadTable(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isString(),"keyword_loadTable Error!!!");
    string path = runObj->getLibrary()->path->getDataPath(values[0]->getString());
    X9ASSERT(path != "","keyword_loadTable Error!!!");
    X9TableItem* item = X9TableLoader::load(path);
    X9Array* arr = X9Array::newArray(runObj->getLibrary());
    for (int i = 0; i<item->values.size(); i++) {
        X9Object* obj = X9Object::newObject(runObj->getLibrary());
        for (int k = 0; k<item->values[i].size(); k++) {
            switch (item->types[k]) {
                case X9TableItem::Type::NUMBER:
                    obj->addNumberValue(item->names[k], item->values[i][k]->value.f);
                    break;
                case X9TableItem::Type::BOOL:
                    obj->addBoolValue(item->names[k], item->values[i][k]->value.b);
                    break;
                case X9TableItem::Type::STRING:
                    obj->addStringValue(item->names[k], *item->values[i][k]->value.s);
                    break;
            }
        }
        arr->pushObject(obj);
    }
    delete item;
    return X9ValueObject::createWithObject(arr);
}
X9ValueObject* keyword_appCommand(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT((values.size() == 1 || values.size() == 2) && values[0]->isString(),"keyword_appCommand Error!!!");
    const string& name = values[0]->getString();
    X9Value* value;
    if (values.size() == 2) {
        X9ASSERT(values[1]->isString() || values[1]->isNumber() || values[1]->isBool(),"keyword_appCommand Error!!!");
        if (values[1]->isString()) {
            value = X9Value::createString(values[1]->getString());
        }else if(values[1]->isNumber()) {
            value = X9Value::createNumber(values[1]->getNumber());
        }else{
            value = X9Value::createBool(values[1]->getBool());
        }
    }else{
        value = X9Value::createNone();
    }
    return runObj->getLibrary()->player->runAppCommand(name, value)->getValueObject();
}
X9ValueObject* keyword_isKeyDown(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isString(),"keyword_isKeyDown Error!!!");
    const string& key = values[0]->getString();
    if(runObj->getLibrary()->player->keyDownMap.find(key) == runObj->getLibrary()->player->keyDownMap.end())
    {
        return X9ValueObject::createWithBool(false);
    }
    return X9ValueObject::createWithBool(runObj->getLibrary()->player->keyDownMap[key]);
}
X9ValueObject* keyword_newClass(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() >= 1 && values[0]->isString(),"keyword_newClass Error!!!");
    runObj->getLibrary()->addClassData(values[0]->getString());
    if (values.size() == 1) {
        return runObj->getLibrary()->createValueObject(values[0]->getString());
    }else{
        vector<X9ValueObject*> vs = values;
        vs.erase(vs.begin());
        return runObj->getLibrary()->createValueObject(values[0]->getString(), vs);
    }
    return X9ValueObject::create();
}
X9ValueObject* keyword_keystringCall(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 2 && values[0]->isObject() && values[1]->isString(),"keyword_keystringCall Error!!!");
    X9Object* target = values[0]->getObject<X9Object*>();
    string str = values[1]->getString();
    const size_t n = str.find(":");
    string callName = str;
    vector<X9ValueObject*> vs;
    if (n != string::npos) {
        callName = str.substr(0,n);
        string vstr = str.substr(n+1);
        vector<string> keys;
        XMath::split(vstr, ",", keys);
        for (int i = 0; i<keys.size(); i++) {
            vs.push_back(X9ValueObject::createWithString(keys[i]));
        }
    }
    return target->runFunction(X9RunObject::MemberType::MT_FUNCTION, callName, vs,target->getClassData());
}
X9ValueObject* keyword_apply(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 2 && values[0]->isFunction() && values[1]->isObject<X9Array*>(),"keyword_applyCall Error!!!");
    X9FunctionValueObject* func = values[0]->getFunction();
    X9Array* arr = values[1]->getObject<X9Array*>();
    vector<X9ValueObject*> vs;
    for (int i = 0; i<arr->children.size(); i++) {
        vs.push_back(arr->children[i]->clone());
    }
    return func->runFunction(vs);
}

//        indexOf(str,idx):Int
X9ValueObject* keyword_strIndexOf(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() >= 2 && values.size() <= 3 && values[0]->isString() && values[1]->isString(),"keyword_strIndexOf Error!!!");
    string str = values[0]->getString();
    string key = values[1]->getString();
    int idx = 0;
    if (values.size() == 3) {
        X9ASSERT(values[1]->isNumber(),"keyword_strIndexOf Error!!!");
        idx = MAX(0,values[2]->getNumber());
    }
    size_t n = str.find(key,idx);
    if (n == string::npos) {
        return X9ValueObject::createWithNumber(-1);
    }
    return X9ValueObject::createWithNumber(n);
}
//        split(str):Array
X9ValueObject* keyword_strSplit(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 2 && values[0]->isString() && values[1]->isString(),"keyword_strSplit Error!!!");
    string str = values[0]->getString();
    string key = values[1]->getString();
    
    return X9ValueObject::create();
}
//        parseNumber(str):Number
X9ValueObject* keyword_parseNumber(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isString(),"keyword_parseNumber Error!!!");
    return X9ValueObject::createWithNumber(atof(values[0]->getString().c_str()));
}
//        parseInt(str,radix):Number
string _parseIntStr1 = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
string _parseIntStr2 = "0123456789abcdefghijklmnopqrstuvwxyz";
X9ValueObject* keyword_parseInt(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 2 && values[0]->isString() && values[1]->isNumber(),"keyword_parseInt Error!!!");
    float n = values[1]->getNumber();
    X9ASSERT(XMath::mod(n, 1) == 0,"keyword_parseInt Error!!!");
    if (n == 10) {
        return X9ValueObject::createWithNumber(atof(values[0]->getString().c_str()));
    }
    string str = values[0]->getString();
    if (str == "") {
        return X9ValueObject::createWithNumber(0);
    }
    int result = 0;
    int k = 1;
    for (int i = str.size()-1; i>=0; i--,k++) {
        str[i];
        auto idx = _parseIntStr1.find(str[i]);
        if (idx == string::npos) {
            idx = _parseIntStr2.find(str[i]);
        }
        X9ASSERT(idx != string::npos,"keyword_parseInt Error!!!");
        int m = (int)idx;
        X9ASSERT(m < n,"keyword_parseInt Error!!!");
        result += m*pow(n,k);
    }
    return X9ValueObject::createWithNumber(result);
}
//        parseString(num):String
X9ValueObject* keyword_parseString(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"keyword_parseString Error!!!");
    return X9ValueObject::createWithString(values[0]->getString());
}
X9ValueObject* keyword_typeof(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1,"keyword_typeof Error!!!");
    switch (values[0]->getType()) {
        case X9ValueObject::NONE:
            return X9ValueObject::createWithString("null");
        case X9ValueObject::STRING:
            return X9ValueObject::createWithString("string");
        case X9ValueObject::NUMBER:
            return X9ValueObject::createWithString("number");
        case X9ValueObject::BOOL:
            return X9ValueObject::createWithString("bool");
        case X9ValueObject::FUNCTION:
            return X9ValueObject::createWithString("Function");
        case X9ValueObject::OBJECT:
        {
            X9Object* obj = values[0]->getObject<X9Object*>();
            return X9ValueObject::createWithString("["+obj->className+"]");
        }
    }
    return nullptr;
}
X9ValueObject* keyword_Rect(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() <= 4,"keyword_Rect Error!!!");
    return runObj->getLibrary()->createValueObject("Rect", values);
}


X9ValueObject* keyword_sequence(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("Sequence", values);
}
X9ValueObject* keyword_spawn(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("Spawn", values);
}
X9ValueObject* keyword_callbackAct(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("CallBackAction", values);
}
X9ValueObject* keyword_updateAct(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("UpdateAction", values);
}
X9ValueObject* keyword_easeAct(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("EaseAction", values);
}
X9ValueObject* keyword_delayAct(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("Action", values);
}



X9ValueObject* keyword_easeSineIn(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeSineIn Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "sineIn", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_easeSineOut(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeSineOut Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "sineOut", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_easeSineInOut(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeSineInOut Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "sineInOut", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_easeBounceIn(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeBounceIn Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "bounceIn", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_easeBounceOut(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeBounceOut Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "bounceOut", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_easeBounceInOut(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeBounceInOut Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "bounceInOut", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_easeElasticIn(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeElasticIn Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "elasticIn", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_easeElasticOut(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeElasticOut Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "elasticOut", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_easeElasticInOut(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeElasticInOut Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "elasticInOut", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_easeExponentialIn(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeExponentialIn Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "exponentialIn", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_easeExponentialOut(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeExponentialOut Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "exponentialOut", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_easeExponentialInOut(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeExponentialInOut Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "exponentialInOut", values[0]->getObject<X9Action*>()));
}

X9ValueObject* keyword_easeBackIn(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeBackIn Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "backIn", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_easeBackOut(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeBackOut Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "backOut", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_easeBackInOut(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    X9ASSERT(values.size() == 1 && values[0]->isObject<X9Action*>(),"keyword_easeBackInOut Error!!!");
    return X9ValueObject::createWithObject(X9EaseAction::newEaseAction(runObj->getLibrary(), "backInOut", values[0]->getObject<X9Action*>()));
}
X9ValueObject* keyword_valueTo(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("ValueTo", values);
}
X9ValueObject* keyword_moveTo(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("MoveTo", values);
}
X9ValueObject* keyword_moveBy(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("MoveBy", values);
}
X9ValueObject* keyword_scaleTo(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("ScaleTo", values);
}
X9ValueObject* keyword_scaleBy(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("ScaleBy", values);
}
X9ValueObject* keyword_rotationTo(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("RotationTo", values);
}
X9ValueObject* keyword_rotationBy(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("RotationBy", values);
}
X9ValueObject* keyword_skewTo(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("SkewTo", values);
}
X9ValueObject* keyword_skewBy(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("SkewBy", values);
}
X9ValueObject* keyword_visibleTo(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("VisibleTo", values);
}
X9ValueObject* keyword_colorTo(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("ColorTo", values);
}
X9ValueObject* keyword_alphaTo(X9RunObject* runObj,const vector<X9ValueObject*>& values, X9ScriptData* script)
{
    return runObj->getLibrary()->createValueObject("AlphaTo", values);
}

map<X9ScriptData::CodeType,std::function<X9ValueObject*(X9RunObject*,X9ScriptData*,X9ScriptClassData*)>> X9RunScript::typeCommands;
map<string,std::function<X9ValueObject*(X9RunObject*,const vector<X9ValueObject*>&,X9ScriptData*)>> X9RunScript::keywordCommands;
map<string, std::function<X9ValueObject*(X9RunObject*,X9ValueObject*,X9ValueObject*)>> X9RunScript::operatorCommands;
void X9RunScript::initScript()
{
    if (typeCommands.empty()) {
        
//        typeCommands[X9ScriptData::NONE] = type_none;
        typeCommands[X9ScriptData::EXPRESSION] = type_expression;
        typeCommands[X9ScriptData::EVALUATION] = type_evaluation;
        typeCommands[X9ScriptData::COMMAND] = type_command;
//        typeCommands[X9ScriptData::OPERATOR] = type_operator;
        typeCommands[X9ScriptData::VALUE] = type_value;
        typeCommands[X9ScriptData::STRING] = type_string;
        typeCommands[X9ScriptData::NUMBER] = type_number;
        typeCommands[X9ScriptData::BOOL] = type_bool;
//        typeCommands[X9ScriptData::CLASS] = type_class;
//        typeCommands[X9ScriptData::SCRIPT] = type_script;
//        typeCommands[X9ScriptData::ARGS] = type_args;
        typeCommands[X9ScriptData::NEW] = type_new;
        
//        typeCommands[X9ScriptData::EXTENDS] = type_extends;
        typeCommands[X9ScriptData::DELETE] = type_delete;
        typeCommands[X9ScriptData::VAR] = type_var;
//        typeCommands[X9ScriptData::_CONST] = type_const;
        typeCommands[X9ScriptData::BREAK] = type_break;
        typeCommands[X9ScriptData::RETURN_] = type_return;
//        typeCommands[X9ScriptData::FUNCTION] = type_function;
        typeCommands[X9ScriptData::IF] = type_if;
        typeCommands[X9ScriptData::ELSE_IF] = type_elseif;
        typeCommands[X9ScriptData::ELSE] = type_else;
        typeCommands[X9ScriptData::WHILE] = type_while;
        typeCommands[X9ScriptData::SWITCH] = type_switch;
//        typeCommands[X9ScriptData::CASE] = type_case;
        //        typeCommands[X9ScriptData::DEFAULT] = type_default;
        typeCommands[X9ScriptData::FOR] = type_for;
        typeCommands[X9ScriptData::FOR_IN] = type_forin;
        typeCommands[X9ScriptData::FOR_EACH] = type_foreach;
        
        keywordCommands["breakPoint"] = keyword_breakPoint;
        keywordCommands["super"] = keyword_super;
        keywordCommands["trace"] = keyword_trace;
        keywordCommands["IF"] = keyword_IF;
        keywordCommands["random"] = keyword_random;
        keywordCommands["randomAt"] = keyword_randomAt;
        keywordCommands["getTimer"] = keyword_getTimer;
        keywordCommands["setTimeOut"] = keyword_setTimeOut;
        keywordCommands["clearTimeOut"] = keyword_clearTimeOut;
        keywordCommands["setInterval"] = keyword_setInterval;
        keywordCommands["clearInterval"] = keyword_clearInterval;
        keywordCommands["loadJson"] = keyword_loadJson;
        keywordCommands["loadTable"] = keyword_loadTable;
        keywordCommands["appCommand"] = keyword_appCommand;
        keywordCommands["isKeyDown"] = keyword_isKeyDown;
        keywordCommands["newClass"] = keyword_newClass;
        keywordCommands["keystringCall"] = keyword_keystringCall;
        keywordCommands["apply"] = keyword_apply;
        
//        keywordCommands["Rect"] = keyword_loadTable;
        
        //        indexOf(str,key,idx):int
        keywordCommands["strIndexOf"] = keyword_strIndexOf;
        //        slice(st,end):String
//        keywordCommands["strSlice"] = keyword_strSlice;
        //        split(str):Array
        keywordCommands["strSplit"] = keyword_strSplit;
        //        parseNumber(str):Number
        keywordCommands["parseNumber"] = keyword_parseNumber;
        //        parseInt(str,radix):Number
        keywordCommands["parseInt"] = keyword_parseInt;
        //        parseString(num):String
        keywordCommands["parseString"] = keyword_parseString;
        
        
        
        operatorCommands["+"] = operator_add;
        operatorCommands["-"] = operator_sub;
        operatorCommands["*"] = operator_mult;
        operatorCommands["/"] = operator_div;
        operatorCommands["&&"] = operator_and;
        operatorCommands["||"] = operator_or;
        operatorCommands[">"] = operator_gt;
        operatorCommands[">="] = operator_ge;
        operatorCommands["<"] = operator_lt;
        operatorCommands["<="] = operator_le;
        operatorCommands["=="] = operator_eq;
        operatorCommands["!="] = operator_et;
        operatorCommands["%"] = operator_mod;
//        operatorCommands["!~"] = operator_is;
        
        
        keywordCommands["sequence"] = keyword_sequence;
        keywordCommands["spawn"] = keyword_spawn;
        keywordCommands["callbackAct"] = keyword_callbackAct;
        keywordCommands["updateAct"] = keyword_updateAct;
        keywordCommands["easeAct"] = keyword_easeAct;
        keywordCommands["delayAct"] = keyword_delayAct;
        
        keywordCommands["easeBackIn"] = keyword_easeBackIn;
        keywordCommands["easeBackOut"] = keyword_easeBackOut;
        keywordCommands["easeBackInOut"] = keyword_easeBackInOut;
        keywordCommands["easeSineIn"] = keyword_easeSineIn;
        keywordCommands["easeSineOut"] = keyword_easeSineOut;
        keywordCommands["easeSineInOut"] = keyword_easeSineInOut;
        keywordCommands["easeBounceIn"] = keyword_easeBounceIn;
        keywordCommands["easeBounceOut"] = keyword_easeBounceOut;
        keywordCommands["easeBounceInOut"] = keyword_easeBounceInOut;
        keywordCommands["easeElasticIn"] = keyword_easeElasticIn;
        keywordCommands["easeElasticOut"] = keyword_easeElasticOut;
        keywordCommands["easeElasticInOut"] = keyword_easeElasticInOut;
        keywordCommands["easeExponentialIn"] = keyword_easeExponentialIn;
        keywordCommands["easeExponentialOut"] = keyword_easeExponentialOut;
        keywordCommands["easeExponentialInOut"] = keyword_easeExponentialInOut;
        
        keywordCommands["valueTo"] = keyword_valueTo;
        keywordCommands["moveTo"] = keyword_moveTo;
        keywordCommands["moveBy"] = keyword_moveBy;
        keywordCommands["scaleTo"] = keyword_scaleTo;
        keywordCommands["scaleBy"] = keyword_scaleBy;
        keywordCommands["rotationTo"] = keyword_rotationTo;
        keywordCommands["rotationBy"] = keyword_rotationBy;
        keywordCommands["skewTo"] = keyword_skewTo;
        keywordCommands["skewBy"] = keyword_skewBy;
        keywordCommands["visibleTo"] = keyword_visibleTo;
        keywordCommands["colorTo"] = keyword_colorTo;
        keywordCommands["alphaTo"] = keyword_alphaTo;
    }
}
X9ValueObject* X9RunScript::run(X9RunObject* runObj, X9ScriptData* script, X9ScriptClassData* currentClass)
{
    X9Library::currentCodeData = script;
    return typeCommands[script->type](runObj,script,currentClass);
//    X9ValueObject* result = nullptr;
//    try {
//        result = typeCommands[script->type](runObj,script);
//    } catch (char* msg) {
//        cout << script->getErrorInfo().c_str() << msg << endl;
//    } catch (int i) {
//        cout << script->getErrorInfo().c_str() << i << endl;
//    }
//    return result;
}
void X9RunScript::runList(X9RunObject* runObj, const vector<X9ScriptData*> scripts, X9ScriptClassData* currentClass)
{
    bool isIf = false;
    X9ValueObject* prevValue = nullptr;
    for (int i = 0; i<scripts.size(); i++) {
        if (scripts[i]->type == X9ScriptData::ELSE || scripts[i]->type == X9ScriptData::ELSE_IF) {
            X9ASSERT(isIf,"else location Error!!!");
            isIf = scripts[i]->type == X9ScriptData::ELSE_IF;
            if (!prevValue->getBool()) {
                prevValue = run(runObj, scripts[i],currentClass);
            }
        }else {
            isIf = scripts[i]->type == X9ScriptData::IF;
            prevValue = run(runObj, scripts[i],currentClass);
        }
        if (runObj->getBreak() || runObj->getReturnValue()) {
            break;
        }
    }
}
