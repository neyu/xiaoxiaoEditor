//
//  X9ScriptData.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/25.
//
//

#include "X9ScriptData.h"
#include "X9RunScript.h"
#include "baseClasses/X9Object.h"
#include "baseClasses/X9BaseGlobal.h"
#include "baseClasses/X9HitTile.h"
#include "baseClasses/X9UserDefault.h"
#include "baseClasses/X9BaseScene.h"
#include "baseClasses/X9Array.h"
#include "baseClasses/X9Event.h"
#include "baseClasses/X9EventDispatcher.h"
#include "baseClasses/X9DisplayObject.h"
#include "baseClasses/X9Node.h"
#include "baseClasses/X9MovieClip.h"
#include "baseClasses/X9KeyFrameScriptNode.h"
#include "baseClasses/X9Particle.h"
#include "baseClasses/X9Image.h"
#include "baseClasses/X9Scale9Image.h"
#include "baseClasses/X9Math.h"
#include "baseClasses/event/X9TouchEvent.h"
#include "baseClasses/event/X9MovieClipEvent.h"
#include "baseClasses/event/X9KeyboardEvent.h"
#include "baseClasses/X9Point.h"
#include "baseClasses/X9AniPosFrame.h"
#include "baseClasses/X9Vec3.h"
#include "baseClasses/X9Size.h"
#include "baseClasses/X9Rect.h"
#include "baseClasses/X9BezierList.h"
#include "baseClasses/X9Label.h"
#include "baseClasses/X9Color.h"
#include "baseClasses/X9StateButton.h"
#include "baseClasses/action/X9Action.h"
#include "baseClasses/action/X9CallBackAction.h"
#include "baseClasses/action/X9UpdateAction.h"
#include "baseClasses/action/X9EaseAction.h"
#include "baseClasses/action/X9Sequence.h"
#include "baseClasses/action/X9Spawn.h"
#include "baseClasses/action/X9ValueTo.h"
#include "baseClasses/action/X9MoveTo.h"
#include "baseClasses/action/X9MoveBy.h"
#include "baseClasses/action/X9ScaleTo.h"
#include "baseClasses/action/X9ScaleBy.h"
#include "baseClasses/action/X9RotationTo.h"
#include "baseClasses/action/X9RotationBy.h"
#include "baseClasses/action/X9SkewTo.h"
#include "baseClasses/action/X9SkewBy.h"
#include "baseClasses/action/X9VisibleTo.h"
#include "baseClasses/action/X9ColorTo.h"
#include "baseClasses/action/X9AlphaTo.h"
#include "X9Player.h"
#include "X9SoundPlayer.h"

//MARK::X9ScriptData
map<string,X9ScriptData::CodeType> X9ScriptData::TempTypes = map<string,CodeType>();

X9ScriptData::X9ScriptData(const rapidjson::Value &jsonData, X9ScriptData* parentData, X9ScriptClassData* rootData)
{
    if (TempTypes.empty()) {
        TempTypes["NONE"]=NONE;
        TempTypes["expression"]=EXPRESSION;
        TempTypes["evaluation"]=EVALUATION;
        TempTypes["command"]=COMMAND;
        TempTypes["operator"]=OPERATOR;
        TempTypes["value"]=VALUE;
        TempTypes["string"]=STRING;
        TempTypes["number"]=NUMBER;
        TempTypes["bool"]=BOOL;
        TempTypes["class"]=CLASS;
        TempTypes["script"]=SCRIPT;
        TempTypes["args"]=ARGS;
        TempTypes["new"]=NEW;
        
        TempTypes["extends"]=EXTENDS;
        TempTypes["delete"]=DELETE;
        TempTypes["var"]=VAR;
        TempTypes["const"] = _CONST;
        TempTypes["break"]=BREAK;
        TempTypes["return"]=RETURN_;
        TempTypes["function"]=FUNCTION;
        TempTypes["if"]=IF;
        TempTypes["elseif"]=ELSE_IF;
        TempTypes["else"]=ELSE;
        TempTypes["while"]=WHILE;
        TempTypes["switch"]=SWITCH;
        TempTypes["case"]=CASE;
        TempTypes["default"]=DEFAULT;
        TempTypes["for"]=FOR;
        TempTypes["forin"]=FOR_IN;
        TempTypes["foreach"]=FOR_EACH;
    }
    setScript(jsonData,parentData,rootData);
    
//    this->parentData = parentData;
//    this->rootData = rootData;
//    valueType = NONE;
//    type = (CodeType)jsonData["t"].GetInt();
//    if (jsonData.HasMember("v")) {
//        if (jsonData["v"].IsString()) {
//            valueType = STRING;
//            value.s = new string(jsonData["v"].GetString());
//        }else if(jsonData["v"].IsBool()) {
//            valueType = BOOL;
//            value.b = jsonData["v"].GetBool();
//        }else{
//            valueType = NUMBER;
//            value.n = jsonData["v"].GetDouble();
//        }
//    }
//    if (type == CodeType::NEW) {
//        rootData->library->addClassData(*value.s);
//    }
//    lineIndex = -1;
//    if (jsonData.HasMember("l")) {
//        lineIndex = jsonData["l"].GetInt();
//    }
//    if (jsonData.HasMember("p")) {
//        for (int i = 0; i<jsonData["p"].Size(); i++) {
//            pNames.push_back(jsonData["p"][i].GetString());
//        }
//    }
//    if (jsonData.HasMember("a")) {
//        for (int i = 0; i<jsonData["a"].Size(); i++) {
//            arguments.push_back(new X9ScriptData(jsonData["a"][i],this,rootData));
//        }
//    }
//    if (jsonData.HasMember("c")) {
//        for (int i = 0; i<jsonData["c"].Size(); i++) {
//            children.push_back(new X9ScriptData(jsonData["c"][i],this,rootData));
//        }
//    }
}
void X9ScriptData::setScript(const rapidjson::Value &jsonData, X9ScriptData* parentData, X9ScriptClassData* rootData)
{
    this->parentData = parentData;
    this->rootData = rootData;
    valueType = NONE;
    //    CCASSERT(TempTypes.find(jsonData["t"].GetString()) != TempTypes.end(),"Script Type Error!!!");
    //    type = TempTypes[jsonData["t"].GetString()];
    type = (CodeType)jsonData["t"].GetInt();
    if (jsonData.HasMember("v")) {
        if (jsonData["v"].IsString()) {
            valueType = STRING;
            value.s = new string(jsonData["v"].GetString());
        }else if(jsonData["v"].IsBool()) {
            valueType = BOOL;
            value.b = jsonData["v"].GetBool();
        }else{
            valueType = NUMBER;
            value.n = jsonData["v"].GetDouble();
        }
    }
    if (type == CodeType::NEW) {
        rootData->library->addClassData(*value.s);
    }
    lineIndex = -1;
    if (jsonData.HasMember("l")) {
        lineIndex = jsonData["l"].GetInt();
    }
    if (jsonData.HasMember("p")) {
        for (int i = 0; i<jsonData["p"].Size(); i++) {
            pNames.push_back(jsonData["p"][i].GetString());
        }
    }
    if (jsonData.HasMember("a")) {
        for (int i = 0; i<jsonData["a"].Size(); i++) {
            arguments.push_back(new X9ScriptData(jsonData["a"][i],this,rootData));
        }
    }
    if (jsonData.HasMember("c")) {
        for (int i = 0; i<jsonData["c"].Size(); i++) {
            children.push_back(new X9ScriptData(jsonData["c"][i],this,rootData));
        }
    }
}
X9ScriptData::~X9ScriptData()
{
    for (int i = 0; i<arguments.size(); i++) {
        X9_SAFE_DELETE(arguments[i]);
    }
    for (int i = 0; i<children.size(); i++) {
        X9_SAFE_DELETE(children[i]);
    }
    if (valueType == STRING) {
        X9_SAFE_DELETE(value.s);
    }
}
const string& X9ScriptData::getStringValue()
{
    return *value.s;
}
float X9ScriptData::getNumberValue()
{
    return value.n;
}
bool X9ScriptData::getBoolValue()
{
    return value.b;
}
void X9ScriptData::setStringValue(const string& str)
{
    if (valueType == CodeType::STRING) {
        delete value.s;
    }
    valueType = CodeType::STRING;
    value.s = new string(str);
}
void X9ScriptData::setNumberValue(float num)
{
    if (valueType == CodeType::STRING) {
        delete value.s;
    }
    valueType = CodeType::NUMBER;
    value.n = num;
}
void X9ScriptData::setBoolValue(bool b)
{
    if (valueType == CodeType::STRING) {
        delete value.s;
    }
    valueType = CodeType::NUMBER;
    value.b = b;
}
string X9ScriptData::getErrorInfo()
{
    int idx = -1;
    X9ScriptData* p = this;
    while (p) {
        if (idx == -1 && p->lineIndex != -1) {
            idx = p->lineIndex;
        }
        if (!p->parentData) {
            X9ScriptClassData* cData = dynamic_cast<X9ScriptClassData*>(p);
            char str[32] = {0};
            sprintf(str, "class %s, lineIndex:%d", cData->className.c_str(),idx+1);
            return str;
        }
        p = p->parentData;
    }
    return "";
}
string X9ScriptData::toString()
{
    switch (type) {
        case CodeType::NONE:
            return "null";
        case CodeType::EXPRESSION:
        {
            std::ostringstream oss;
            for (int i = 0; i<arguments.size(); i++) {
                if (i > 0) {
                    oss<<" ";
                }
                oss<<arguments[i]->toString();
            }
            return oss.str();
        }
        case CodeType::EVALUATION:
        {
            std::ostringstream oss;
            for (int i = 0; i<pNames.size(); i++) {
                oss<<pNames[i]<<".";
            }
            oss<<getStringValue()<<" = "<<arguments[0]->toString();
            return oss.str();
        }
        case CodeType::COMMAND:
        {
            std::ostringstream oss;
            for (int i = 0; i<pNames.size(); i++) {
                oss<<pNames[i]<<".";
            }
            oss<<getStringValue()<<"(";
            for (int i = 0; i<arguments.size(); i++) {
                if (i > 0) {
                    oss<<", ";
                }
                oss<<arguments[i]->toString();
            }
            oss<<")";
            return oss.str();
        }
        case CodeType::OPERATOR:
            return getStringValue();
        case CodeType::VALUE:
        {
            std::ostringstream oss;
            for (int i = 0; i<pNames.size(); i++) {
                oss<<pNames[i]<<".";
            }
            oss<<getStringValue();
            return oss.str();
        }
        case CodeType::STRING:
            return "\""+getStringValue()+"\"";
        case CodeType::NUMBER:
        {
            std::ostringstream oss;
            oss<<getNumberValue();
            return oss.str();
        }
        case CodeType::BOOL:
            return getBoolValue() ? "true" : "false";
        case CodeType::CLASS:
            return "class "+getStringValue()+" extends:"+pNames[0];
        case CodeType::SCRIPT:
            return "SCRIPT";
        case CodeType::ARGS:
            return "ARGS";
        case CodeType::NEW:
        {
            std::ostringstream oss;
            oss<<"new "<<getStringValue()<<"(";
            for (int i = 0; i<arguments.size(); i++) {
                if (i > 0) {
                    oss<<", ";
                }
                oss<<arguments[i]->toString();
            }
            oss<<")";
            return oss.str();
        }
        case CodeType::EXTENDS:
            return "EXTENDS";
        case CodeType::DELETE:
            return "delete "+arguments[0]->toString();
        case CodeType::VAR:
            return "var "+getStringValue()+" = "+arguments[0]->toString();
        case CodeType::_CONST:
            return "const "+getStringValue()+" = "+arguments[0]->toString();
        case CodeType::BREAK:
            return "break";
        case CodeType::RETURN_:
        {
            if (arguments.empty()) {
                return "return";
            }
            return "return "+arguments[0]->toString();
        }
        case CodeType::FUNCTION:
        {
            std::ostringstream oss;
            oss<<"function "<<getStringValue()<<"(";
            for (int i = 0; i<arguments.size(); i++) {
                if (i > 0) {
                    oss<<", ";
                }
                oss<<arguments[i]->getStringValue();
            }
            oss<<")";
            return oss.str();
        }
        case CodeType::IF:
            return "if("+arguments[0]->toString()+")";
        case CodeType::ELSE_IF:
            return "else if("+arguments[0]->toString()+")";
        case CodeType::ELSE:
            return "else";
        case CodeType::WHILE:
            return "while("+arguments[0]->toString()+")";
        case CodeType::SWITCH:
            return "switch("+arguments[0]->toString()+")";
        case CodeType::CASE:
            return "case "+arguments[0]->toString()+":";
        case CodeType::DEFAULT:
            return "default:";
        case CodeType::FOR:
            return "for("+arguments[0]->toString()+"; "+arguments[1]->toString()+"; "+arguments[2]->toString()+")";
        case CodeType::FOR_IN:
            return "for(var "+getStringValue()+" in "+arguments[0]->toString()+")";
        case CodeType::FOR_EACH:
            return "for each(var "+getStringValue()+" in "+arguments[0]->toString()+")";
    }
    return "";
}
//MARK::X9ScriptClassData
X9ScriptClassData::X9ScriptClassData(const string& className, const string& extends,X9Library* library)
{
    isFramework = false;
    isBaseClass = true;
    this->className = className;
    if (extends != "") {
        this->extends.push_back(extends);
    }
    baseExtendsName = extends;
    this->library = library;
//    this->neatenExtendsAndFunctions();
}
X9ScriptClassData::X9ScriptClassData(const string& path,X9Library* library)
{
    loadJsonDoc(_doc,path);
    this->library = library;
    setScript(_doc, nullptr, this);
//    X9ScriptData(_doc,nullptr,this);
    isBaseClass = false;
    className = getStringValue();
    library->classes[className] = this;
    CCASSERT(!pNames.empty(),"Class extends Error!!!");
    extends.push_back(pNames[0]);
    pNames.clear();
    for(int i = 0;i<children.size();i++)
    {
        if (children[i]->type == VAR)
        {
            propertyMap[children[i]->getStringValue()] = children[i];
            propertys.push_back(children[i]);
        }
        else if (children[i]->type == _CONST)
        {
            CCASSERT(children[i]->arguments.size() == 1,"const value Error!!!");
            switch (children[i]->arguments[0]->type) {
                case NUMBER:
                    constValues[children[i]->getStringValue()] = X9ValueObject::createWithNumber(children[i]->arguments[0]->getNumberValue());
                    break;
                case BOOL:
                    constValues[children[i]->getStringValue()] = X9ValueObject::createWithBool(children[i]->arguments[0]->getBoolValue());
                    break;
                case STRING:
                    constValues[children[i]->getStringValue()] = X9ValueObject::createWithString(children[i]->arguments[0]->getStringValue());
                    break;
                default:
                    CCASSERT(false,"const value type Error!!!");
                    break;
            }
            constValues[children[i]->getStringValue()]->use();
        }else if(children[i]->type == FUNCTION)
        {
            functions[children[i]->getStringValue()] = children[i];
        }else
        {
            CCASSERT(false,"Class Script Error!!!");
        }
    }
    neatenExtendsAndFunctions();
    createFunction = library->classes[baseExtendsName]->createFunction;
    if (extends[0] == "BaseScene" && _doc.HasMember("ns")) {
        string nextScene = _doc["ns"].GetString();
        library->_scenes.push_back(nextScene);
        library->addClassData(nextScene);
    }
}
X9ScriptClassData::~X9ScriptClassData()
{
    for (auto it = constValues.begin(); it != constValues.end(); it++) {
        it->second->nonuse();
    }
}
//X9ValueObject* X9ScriptClassData::getConstValue(const string& name)
//{
//    if (constValues.find(name) == constValues.end()) {
//        for (int i = 0; i<children.size(); i++) {
//            if (children[i]->type == _CONST && children[i]->getStringValue() == name) {
//                
//                break;
//            }
//        }
//    }
//    return constValues[name]->clone();
//}
void X9ScriptClassData::neatenExtendsAndFunctions()
{
    map<string,bool> _temp;
    for(int i = 0;i<extends.size();i++) {
        CCASSERT(_temp.find(extends[i]) == _temp.end(),"Class extends Error!!!");
        _temp[extends[i]] = true;
        library->addClassData(extends[i]);
        X9ScriptClassData* cData = library->classes[extends[i]];
        for (int k = 0; k<cData->extends.size(); k++) {
            extends.push_back(cData->extends[k]);
            if(k > 0) i++;
        }
    }
    if (isBaseClass) {
        for (auto it = library->baseFunctions[className].begin(); it != library->baseFunctions[className].end(); it++) {
            baseFunctionKeys[it->first] = vector<string>();
            baseFunctionKeys[it->first].push_back(className);
        }
        for (auto it = library->baseSetFunctions[className].begin(); it != library->baseSetFunctions[className].end(); it++) {
            baseSetKeys[it->first] = className;
        }
        for (auto it = library->baseGetFunctions[className].begin(); it != library->baseGetFunctions[className].end(); it++) {
            baseGetKeys[it->first] = className;
        }
    }
//    log("--------%s",className.c_str());
    for(int i = 0;i<extends.size();i++) {
        X9ScriptClassData* cData = library->classes[extends[i]];
        if (baseExtendsName == "" && cData->isBaseClass) {
            baseExtendsName = cData->className;
        }
        for (int k = 0; k<cData->children.size(); k++) {
            const string& vName = cData->children[k]->getStringValue();
            if (cData->children[k]->type == VAR)
            {
                if (propertyMap.find(vName) == propertyMap.end()) {
                    propertyMap[vName] = cData->children[k];
                    propertys.push_back(cData->children[k]);
                }
            }
            else if (cData->children[k]->type == _CONST)
            {
                if (constValues.find(vName) == constValues.end()) {
                    switch (cData->children[k]->arguments[0]->type) {
                        case NUMBER:
                            constValues[vName] = X9ValueObject::createWithNumber(cData->children[k]->arguments[0]->getNumberValue());
                            break;
                        case BOOL:
                            constValues[vName] = X9ValueObject::createWithBool(cData->children[k]->arguments[0]->getBoolValue());
                            break;
                        case STRING:
                            constValues[vName] = X9ValueObject::createWithString(cData->children[k]->arguments[0]->getStringValue());
                            break;
                        default:
                            break;
                    }
                    constValues[vName]->use();
                }
            }else if(cData->children[k]->type == FUNCTION)
            {
                if(functions.find(vName) == functions.end())
                {
                    functions[vName] = cData->children[k];
                }
            }
        }
        if (cData->isBaseClass) {
            const string& acName = cData->className;
            for (auto it = library->baseFunctions[acName].begin(); it != library->baseFunctions[acName].end(); it++) {
                if (baseFunctionKeys.find(it->first) == baseFunctionKeys.end()) {
                    baseFunctionKeys[it->first] = vector<string>();
                }
                baseFunctionKeys[it->first].push_back(acName);
            }
            for (auto it = library->baseSetFunctions[acName].begin(); it != library->baseSetFunctions[acName].end(); it++) {
                if (baseSetKeys.find(it->first) == baseSetKeys.end()) {
                    baseSetKeys[it->first] = acName;
                }
            }
            for (auto it = library->baseGetFunctions[acName].begin(); it != library->baseGetFunctions[acName].end(); it++) {
                if (baseGetKeys.find(it->first) == baseGetKeys.end()) {
                    baseGetKeys[it->first] = acName;
                }
            }
        }
    }
}
//void X9ScriptClassData::setConstValues()
//{
//    
//}

//MARK::X9AutoObject
//map<X9AutoObject*,int> X9AutoObject::_memory;
//map<X9AutoObject*,int> X9AutoObject::_removeMemory;
//void X9AutoObject::recycleUpdate()
//{
////    log("memory:%d, clearMemory:%d",(int)_memory.size(),(int)_removeMemory.size());
//    while (!_removeMemory.empty()) {
//        map<X9AutoObject*,int> copyMemory = _removeMemory;
//        _removeMemory.clear();
//        for (auto it = copyMemory.begin(); it != copyMemory.end(); it++) {
//            it->first->removed();
//            delete it->first;
//        }
//    }
//}
//X9AutoObject::X9AutoObject()
//{
//    isUse = false;
//    _removeMemory[this] = 0;
////    _useCount = 0;
//}
//X9AutoObject::~X9AutoObject()
//{
//    //CCASSERT(false,"Error!!!");
//}
//void X9AutoObject::use()
//{
////    _useCount++;
//    if (isUse) {
//        _memory[this]++;
//    }else{
//        _removeMemory.erase(this);
//        _memory[this] = 1;
//        isUse = true;
//    }
//}
//void X9AutoObject::nonuse()
//{
//    CCASSERT(isUse && _memory[this] > 0,"run command Error!!!");
////    _useCount--;
//    _memory[this]--;
//    if (_memory[this] == 0) {
//        isUse = false;
//        _memory.erase(this);
//        _removeMemory[this] = 0;
//    }
//}
//int X9AutoObject::getUseCount()
//{
//    if (isUse) {
//        return _memory[this];
//    }
//    return 0;
//}
//void X9AutoObject::clearMemory()
//{
//    for (auto it = _memory.begin(); it != _memory.end(); it++) {
//        delete it->first;
//    }
//    _memory.clear();
//    for (auto it = _removeMemory.begin(); it != _removeMemory.end(); it++) {
//        delete it->first;
//    }
//    _removeMemory.clear();
//}

//MARK::X9ValueObject
X9ValueObject* X9ValueObject::create()
{
    CREATE_NEW_AUTO_OBJ(X9ValueObject,);
}
X9ValueObject* X9ValueObject::createWithBool(bool value)
{
    X9ValueObject* obj = create();
    obj->setBool(value);
    return obj;
}
X9ValueObject* X9ValueObject::createWithNumber(float value)
{
    X9ValueObject* obj = create();
    obj->setNumber(value);
    return obj;
}
X9ValueObject* X9ValueObject::createWithString(const string& value)
{
    X9ValueObject* obj = create();
    obj->setString(value);
    return obj;
}
X9ValueObject* X9ValueObject::createWithObject(X9RunObject* value)
{
    X9ValueObject* obj = create();
    obj->setObject(value);
    return obj;
}
X9ValueObject* X9ValueObject::createWithFunction(X9RunObject* target, const string& fName, bool isSuper)
{
    X9ValueObject* obj = create();
    obj->setFunction(target, fName, isSuper);
    return obj;
}
X9ValueObject::X9ValueObject():X9AutoObject("X9ValueObject")
{
    type = NONE;
//    setValueObject = CC_CALLBACK_1(X9ValueObject::_setValueObject, this);
}
void X9ValueObject::removed()
{
    clear();
//    for (int i = 0; i<_watchCall.size(); i++) {
//        _watchCall[i]->nonuse();
//    }
//    _watchCall.clear();
//    setValueObject = CC_CALLBACK_1(X9ValueObject::_setValueObject, this);
    X9AutoObject::removed();
}
void X9ValueObject::use()
{
//    log("X9ValueObject::use()");
    X9AutoObject::use();
}
void X9ValueObject::nonuse()
{
//    log("X9ValueObject::nonuse()");
    X9AutoObject::nonuse();
}
string X9ValueObject::getJsonString()
{
    switch (type) {
        case STRING:
            return "\""+getString()+"\"";
        case BOOL:
        case NUMBER:
            return getString();
        case OBJECT:
            return getObject()->getJsonString();
        case FUNCTION:
            return "{}";
        default:
            break;
    }
    return "";
}
//void X9ValueObject::_setValueObject(X9ValueObject* v)
//{
//    switch (v->type) {
//        case NONE:
//            clear();
//            break;
//        case NUMBER:
//            setNumber(v->getNumber());
//            break;
//        case BOOL:
//            setBool(v->getBool());
//            break;
//        case STRING:
//            setString(v->getString());
//            break;
//        case OBJECT:
//            setObject(v->getObject());
//            break;
//        case FUNCTION:
//            setFunction(v->getFunction()->target, v->getFunction()->functionName);
//            break;
//        default:
//            break;
//    }
//}
void X9ValueObject::setValueObject(X9ValueObject* v)
{
    switch (v->type) {
        case NONE:
            clear();
            break;
        case NUMBER:
            setNumber(v->getNumber());
            break;
        case BOOL:
            setBool(v->getBool());
            break;
        case STRING:
            setString(v->getString());
            break;
        case OBJECT:
            setObject(v->getObject());
            break;
        case FUNCTION:
            setFunction(v->getFunction()->target, v->getFunction()->functionName, v->getFunction()->isSuper);
            break;
        default:
            break;
    }
}

//void X9ValueObject::_setValueObjectWatch(X9ValueObject* v)
//{
//    X9ValueObject* oldValue = clone();
//    switch (v->type) {
//        case NONE:
//            clear();
//            break;
//        case NUMBER:
//            setNumber(v->getNumber());
//            break;
//        case BOOL:
//            setBool(v->getBool());
//            break;
//        case STRING:
//            setString(v->getString());
//            break;
//        case OBJECT:
//            setObject(v->getObject());
//            break;
//        case FUNCTION:
//            setFunction(v->getFunction()->target, v->getFunction()->functionName);
//            break;
//        default:
//            break;
//    }
//}
void X9ValueObject::setNumber(float v)
{
    clear();
    type = NUMBER;
    value.n = v;
}
void X9ValueObject::setString(const string& v)
{
    clear();
    type = STRING;
    value.s = new string(v);
}
void X9ValueObject::setBool(bool v)
{
    clear();
    type = BOOL;
    value.b = v;
}
void X9ValueObject::setObject(X9RunObject* v)
{
    clear();
    type = OBJECT;
    value.o = v;
    value.o->use();
}
void X9ValueObject::setFunction(X9RunObject* t,const string& fName, bool isSuper)
{
    clear();
    type = FUNCTION;
    value.f = X9FunctionValueObject::create(t, fName, isSuper);
    value.f->use();
}
X9ValueObject* X9ValueObject::clone()
{
    switch (type) {
        case NONE:
            return create();
        case NUMBER:
            return createWithNumber(getNumber());
        case BOOL:
            return createWithBool(getBool());
        case STRING:
            return createWithString(getString());
        case OBJECT:
            return createWithObject(getObject());
        case FUNCTION:
            return createWithFunction(getFunction()->target, getFunction()->functionName, getFunction()->isSuper);
    }
}
float X9ValueObject::getNumber()
{
    switch (type) {
        case BOOL:
            return value.b ? 1 : 0;
        case NUMBER:
            return value.n;
        case STRING:
        case FUNCTION:
        case OBJECT:
        case NONE:
            return 0;
    }
}
string X9ValueObject::getString()
{
    switch (type) {
        case BOOL:
            return value.b ? "true" : "false";
        case NUMBER:
        {
            std::ostringstream oss;
            oss<<value.n;
            return oss.str();
        }
        case STRING:
            return *value.s;
        case FUNCTION:
            return "Function";
        case OBJECT:
            return dynamic_cast<X9Object*>(value.o)->toString();
        case NONE:
            return "null";
    }
}
bool X9ValueObject::getBool()
{
    switch (type) {
        case BOOL:
            return value.b;
        case NUMBER:
            return value.n != 0;
        case STRING:
            return *value.s != "";
        case FUNCTION:
        case OBJECT:
            return true;
        case NONE:
            return false;
    }
}
X9RunObject* X9ValueObject::getObject()
{
    if (type == OBJECT) {
        return value.o;
    }
    CCASSERT(false,"getObject Error!!!");
}
//template<class check_typeA>
//check_typeA X9ValueObject::getObject()
//{
//    return dynamic_cast<check_typeA>(getObject());
//}
X9FunctionValueObject* X9ValueObject::getFunction()
{
    if (type == FUNCTION) {
        return value.f;
    }
    CCASSERT(false,"getFunction Error!!!");
}

bool X9ValueObject::isValue()
{
    return type == NUMBER || type == STRING || type == BOOL;
}
bool X9ValueObject::isNumber()
{
    return type == NUMBER;
}
bool X9ValueObject::isString()
{
    return type == STRING;
}
bool X9ValueObject::isBool()
{
    return type == BOOL;
}
bool X9ValueObject::isObject()
{
    return type == OBJECT;
}
bool X9ValueObject::isFunction()
{
    return type == FUNCTION;
}
bool X9ValueObject::isNone()
{
    return type == NONE;
}
void X9ValueObject::clear()
{
    switch (type) {
        case STRING:
            X9_SAFE_DELETE(value.s);
            break;
        case FUNCTION:
            value.f->nonuse();
            break;
        case OBJECT:
            value.o->nonuse();
            break;
        default:
            break;
    }
    type = NONE;
}
//void X9ValueObject::addWatch(X9ValueObject* func)
//{
//    for (int i = 0; i<_watchCall.size(); i++) {
//        if ((*func == _watchCall[i])->getBool()) {
//            return;
//        }
//    }
//    if (_watchCall.empty()) {
//        setValueObject = CC_CALLBACK_1(X9ValueObject::_setValueObjectWatch, this);
//    }
//    func->use();
//    _watchCall.push_back(func);
//}
//void X9ValueObject::removeWatch(X9ValueObject* func)
//{
//    if (func == nullptr) {
//        for (int i = 0; i<_watchCall.size(); i++) {
//            _watchCall[i]->nonuse();
//        }
//        _watchCall.clear();
//        setValueObject = CC_CALLBACK_1(X9ValueObject::_setValueObject, this);
//    }else{
//        for (auto it = _watchCall.begin(); it != _watchCall.end(); it++) {
//            if ((*func == *it)->getBool()) {
//                (*it)->nonuse();
//                _watchCall.erase(it);
//                if (_watchCall.empty()) {
//                    setValueObject = CC_CALLBACK_1(X9ValueObject::_setValueObject, this);
//                }
//                break;
//            }
//        }
//    }
//}
X9ValueObject* X9ValueObject::operator+(X9ValueObject* v)
{
    if(isString() || v->isString())
    {
        return X9ValueObject::createWithString(getString()+v->getString());
    }
    if (isObject() && v->isObject()) {
        X9Point* a = this->getObject<X9Point*>();
        X9Point* b = v->getObject<X9Point*>();
        if (a && b) {
            return X9ValueObject::createWithObject(*a+b);
        }
    }
    return X9ValueObject::createWithNumber(getNumber()+v->getNumber());
}
X9ValueObject* X9ValueObject::operator-(X9ValueObject* v)
{
    if (isObject() && v->isObject()) {
        X9Point* a = this->getObject<X9Point*>();
        X9Point* b = v->getObject<X9Point*>();
        if (a && b) {
            return X9ValueObject::createWithObject(*a-b);
        }
    }
    return X9ValueObject::createWithNumber(getNumber()-v->getNumber());
}
X9ValueObject* X9ValueObject::operator*(X9ValueObject* v)
{
    return X9ValueObject::createWithNumber(getNumber()*v->getNumber());
}
X9ValueObject* X9ValueObject::operator/(X9ValueObject* v)
{
    return X9ValueObject::createWithNumber(getNumber()/v->getNumber());
}
X9ValueObject* X9ValueObject::operator&&(X9ValueObject* v)
{
    return X9ValueObject::createWithBool(getBool() && v->getBool());
}
X9ValueObject* X9ValueObject::operator||(X9ValueObject* v)
{
    return X9ValueObject::createWithBool(getBool() || v->getBool());
}
X9ValueObject* X9ValueObject::operator==(X9ValueObject* v)
{
    if (type == v->type) {
        switch (type) {
            case STRING:
                return X9ValueObject::createWithBool(getString() == v->getString());
                break;
            case NUMBER:
                return X9ValueObject::createWithBool(getNumber() == v->getNumber());
                break;
            case BOOL:
                return X9ValueObject::createWithBool(getBool() == v->getBool());
                break;
            case OBJECT:
                return X9ValueObject::createWithBool(getObject() == v->getObject());
                break;
            case FUNCTION:
            {
                X9FunctionValueObject* f1 = getFunction();
                X9FunctionValueObject* f2 = v->getFunction();
                return X9ValueObject::createWithBool(f1->target == f2->target && f1->functionName == f2->functionName);
                break;
            }
            case NONE:
                return X9ValueObject::createWithBool(true);
                break;
        }
    }
    return X9ValueObject::createWithBool(false);
}
X9ValueObject* X9ValueObject::operator!=(X9ValueObject* v)
{
    if (type == v->type) {
        switch (type) {
            case STRING:
                return X9ValueObject::createWithBool(getString() != v->getString());
                break;
            case NUMBER:
                return X9ValueObject::createWithBool(getNumber() != v->getNumber());
                break;
            case BOOL:
                return X9ValueObject::createWithBool(getBool() != v->getBool());
                break;
            case OBJECT:
                return X9ValueObject::createWithBool(getObject() != v->getObject());
                break;
            case FUNCTION:
            {
                X9FunctionValueObject* f1 = getFunction();
                X9FunctionValueObject* f2 = v->getFunction();
                return X9ValueObject::createWithBool(f1->target != f2->target || f1->functionName != f2->functionName);
                break;
            }
            case NONE:
                return X9ValueObject::createWithBool(false);
                break;
        }
    }
    return X9ValueObject::createWithBool(true);
}
X9ValueObject* X9ValueObject::operator>=(X9ValueObject* v)
{
    return X9ValueObject::createWithBool(getNumber()>=v->getNumber());
}
X9ValueObject* X9ValueObject::operator>(X9ValueObject* v)
{
    return X9ValueObject::createWithBool(getNumber()>v->getNumber());
}
X9ValueObject* X9ValueObject::operator<=(X9ValueObject* v)
{
    return X9ValueObject::createWithBool(getNumber()<=v->getNumber());
}
X9ValueObject* X9ValueObject::operator<(X9ValueObject* v)
{
    return X9ValueObject::createWithBool(getNumber()<v->getNumber());
}
X9ValueObject* X9ValueObject::operator%(X9ValueObject* v)
{
    return X9ValueObject::createWithNumber(getNumber()-floor(getNumber()/v->getNumber())*v->getNumber());
}


//template<class T>
//bool checkValueObject(X9ValueObject* value)
//{
//    if (value->isObject()) {
//        return dynamic_cast<T>(value->getObject()) != nullptr;
//    }
//    return false;
//}


//MARK::X9FunctionValueObject
X9FunctionValueObject* X9FunctionValueObject::create()
{
    CREATE_NEW_AUTO_OBJ(X9FunctionValueObject,);
}
X9FunctionValueObject* X9FunctionValueObject::create(X9RunObject* t,const string& fName, bool isSuper)
{
    X9FunctionValueObject* obj = create();
    obj->isTargetEnabled = true;
    t->addFunctionValue(obj);
    obj->target = t;
    obj->isSuper = isSuper;
    //    t->use();
    obj->functionName = fName;
    return obj;
}
X9FunctionValueObject::X9FunctionValueObject():X9AutoObject("X9FunctionValueObject")
{
    isSuper = false;
    target = nullptr;
    functionName = "";
}
X9FunctionValueObject::~X9FunctionValueObject()
{
}
void X9FunctionValueObject::removed()
{
    isSuper = false;
    //    if (target) {
    //        target->nonuse();
    //    }
    if (isTargetEnabled) {
        target->removeFunctionValue(this);
    }
    target = nullptr;
    functionName = "";
    X9AutoObject::removed();
}
void X9FunctionValueObject::use()
{
    X9AutoObject::use();
}
X9ValueObject* X9FunctionValueObject::runFunction(const vector<X9ValueObject*>& values)
{
    X9RunObject::MemberType mt = target->checkFunction(functionName,isSuper);
    X9ASSERT(isTargetEnabled,"not found target! Error!!!");
    X9ASSERT(mt != X9RunObject::MT_NONE,"run command Error!!!");
    return target->runFunction(mt, functionName, values,target->getClassData());
}
X9ValueObject* X9FunctionValueObject::runFunction(X9ValueObject* v1)
{
    X9RunObject::MemberType mt = target->checkFunction(functionName,isSuper);
    X9ASSERT(isTargetEnabled,"not found target! Error!!!");
    X9ASSERT(mt != X9RunObject::MT_NONE,"run command Error!!!");
    vector<X9ValueObject*> values;
    values.push_back(v1->clone());
    return target->runFunction(mt, functionName, values,target->getClassData());
}
X9ValueObject* X9FunctionValueObject::runFunction(X9ValueObject* v1,X9ValueObject* v2)
{
    X9RunObject::MemberType mt = target->checkFunction(functionName,isSuper);
    X9ASSERT(isTargetEnabled,"not found target! Error!!!");
    X9ASSERT(mt != X9RunObject::MT_NONE,"run command Error!!!");
    vector<X9ValueObject*> values;
    values.push_back(v1->clone());
    values.push_back(v2->clone());
    return target->runFunction(mt, functionName, values,target->getClassData());
}
X9ValueObject* X9FunctionValueObject::runFunction(X9ValueObject* v1,X9ValueObject* v2,X9ValueObject* v3)
{
    X9RunObject::MemberType mt = target->checkFunction(functionName,isSuper);
    X9ASSERT(isTargetEnabled,"not found target! Error!!!");
    X9ASSERT(mt != X9RunObject::MT_NONE,"run command Error!!!");
    vector<X9ValueObject*> values;
    values.push_back(v1->clone());
    values.push_back(v2->clone());
    values.push_back(v3->clone());
    return target->runFunction(mt, functionName, values,target->getClassData());
}
X9ScriptData* X9FunctionValueObject::getFunctionData()
{
    return nullptr;
}
//MARK::X9RunObject
X9RunObject* X9RunObject::create()
{
    CREATE_NEW_AUTO_OBJ(X9RunObject,"X9RunObject");
}
X9RunObject* X9RunObject::create(X9RunObject* target,X9RunObject* parentObj)
{
    X9RunObject* obj = create();
    obj->_target = target;
    obj->_breakObj = parentObj->_breakObj;
    obj->_topObj = parentObj->_topObj;
    
    return obj;
}
X9RunObject* X9RunObject::createTop(X9RunObject* target,X9RunObject* parentObj)
{
    X9RunObject* obj = X9RunObject::create(target,parentObj);
    obj->_topObj = obj;
    return obj;
}
X9RunObject* X9RunObject::createBreak(X9RunObject* target,X9RunObject* parentObj)
{
    X9RunObject* obj = X9RunObject::create(target,parentObj);
    obj->_breakObj = obj;
    return obj;
}
X9RunObject::X9RunObject(const string& key):X9AutoObject(key)
{
    _target = nullptr;
    _breakObj = nullptr;
    _topObj = nullptr;
    _returnValue = nullptr;
    _isBreak = false;
}
void X9RunObject::removed()
{
    _target = nullptr;
    _breakObj = nullptr;
    _topObj = nullptr;
    _returnValue = nullptr;
    _isBreak = false;
    if (!values.empty()) {
        for (auto it = values.begin(); it != values.end(); it++) {
            it->second->nonuse();
        }
    }
    values.clear();
    X9AutoObject::removed();
}
void X9RunObject::clearValues()
{
    for (auto it = values.begin(); it != values.end(); it++) {
//        it->second->nonuseAndRecycle();
        it->second->nonuse();
    }
    values.clear();
}
void X9RunObject::setBreak()
{
    _breakObj->_isBreak = true;
}
bool X9RunObject::getBreak()
{
    if (_breakObj) {
        return _breakObj->_isBreak;
    }
    return false;
}
X9ScriptClassData* X9RunObject::getClassData()
{
    return dynamic_cast<X9Object*>(_target)->classData;
}
X9Library* X9RunObject::getLibrary()
{
    return getClassData()->library;
}
X9Player* X9RunObject::getPlayer()
{
    return getClassData()->library->player;
}
bool X9RunObject::checkClassType(const string& className)
{
    if (_target != this) {
        return false;
    }
    X9Object* obj = dynamic_cast<X9Object*>(this);
    if (obj) {
        if (obj->className == className) {
            return true;
        }
        for (int i = 0; i<obj->classData->extends.size(); i++) {
            if (obj->classData->extends[i] == className) {
                return true;
            }
        }
    }
    return false;
}
void X9RunObject::setReturnValue(X9ValueObject* value)
{
    _topObj->_returnValue = value;
}
X9ValueObject* X9RunObject::getReturnValue()
{
    return _topObj->_returnValue;
}
X9RunObject* X9RunObject::getUseRunObject(const string& valueName,const vector<string>& pnames)
{
    if(pnames.empty())
    {
        X9RunObject::MemberType mt = _topObj->checkValue(valueName,false);
        if (mt == X9RunObject::MT_NONE) {
            if (_target->checkValue(valueName,false) != MT_NONE) {
                return _target;
            }
            return getLibrary()->global;
        }
        return _topObj;
    }
    X9RunObject* runObj = _topObj;
        int stIdx = 0;
        bool isSuper = pnames[0] == "super";
        if (pnames[0] == "this" || pnames[0] == "super") {
            runObj = _target;
            stIdx = 1;
        }else if(pnames[0] == "global") {
            runObj = dynamic_cast<X9Object*>(_target)->classData->library->global;
            stIdx = 1;
        }else
        {
            X9RunObject::MemberType mt = runObj->checkValue(pnames[0],isSuper);
            if (mt == X9RunObject::MT_NONE) {
                mt = getLibrary()->global->checkValue(pnames[0],isSuper);
//                if (mt == X9RunObject::MT_NONE) {
//                    try {
//                        throw "Error!!";
//                    } catch (char* msg) {
//                        cout << "string::" << msg << endl;
//                    } catch (int i) {
//                        cout << "int::" << i << endl;
//                    }
//                }
                X9ASSERT(mt != X9RunObject::MT_NONE,"getUseRunObject Error!!!");
                runObj = getLibrary()->global;
    //            stIdx = 1;
            }
        }
        for (int i = stIdx; i<pnames.size(); i++) {
            X9RunObject::MemberType mt = runObj->checkValue(pnames[i],isSuper);
            X9ASSERT(mt != X9RunObject::MT_NONE,"getUseRunObject Error!!!");
            X9ValueObject* v = runObj->getValue(mt, pnames[i]);
            X9ASSERT(v->isObject(),"getUseRunObject Error!!!");
            runObj = v->getObject();
        }
    return runObj;
}
X9RunObject::MemberType X9RunObject::checkValue(const string& name,bool isSuper)
{
    //temporary variable 
    if (_topObj && _topObj->values.find(name) != _topObj->values.end()) {
        return MT_VALUE;
    }
    X9Object* target = dynamic_cast<X9Object*>(_target);
    //self variable 
    if(target->propertys.find(name) != target->propertys.end())
    {
        return MT_PROPERTY;
    }
    X9ScriptClassData* cData = target->classData;
    //set
    if (cData->baseGetKeys.find(name) != cData->baseGetKeys.end()) {
        return MT_SGET;
    }
//    if (cData->baseGetKeys.find(name) != cData->baseGetKeys.end()) {
//        return MT_GET;
//    }
    //const variable 
    if (cData->constValues.find(name) != cData->constValues.end()) {
        return MT_CONST;
    }
    //method variable 
    if (cData->functions.find(name) != cData->functions.end()) {
        if (isSuper) {
            return MT_SUPER_FUNCTION;
        }
        return MT_FUNCTION;
    }
    //base method variable 
    if (cData->baseFunctionKeys.find(name) != cData->baseFunctionKeys.end()) {
        if (isSuper) {
            return MT_BASE_SUPER_FUNCTION;
        }
        return MT_BASE_FUNCTION;
    }
    
    return MT_NONE;
}
X9ValueObject* X9RunObject::getValue(MemberType mt, const string& name)
{
    switch (mt) {
        case MT_SGET:
            return getLibrary()->baseGetFunctions[getClassData()->baseGetKeys[name]][name](_target);
        case MT_VALUE:
            return _topObj->values[name]->clone();
        case MT_PROPERTY:
        {
            X9Object* target = dynamic_cast<X9Object*>(_target);
            return target->propertys[name]->clone();
        }
        case MT_CONST:
        {
            X9ScriptClassData* cData = dynamic_cast<X9Object*>(_target)->classData;
            return cData->constValues[name]->clone();
        }
        case MT_SUPER_FUNCTION:
            return X9ValueObject::createWithFunction(_target, name,true);
        case MT_FUNCTION:
            return X9ValueObject::createWithFunction(_target, name,false);
        case MT_BASE_SUPER_FUNCTION:
            return X9ValueObject::createWithFunction(_target, name,true);
        case MT_BASE_FUNCTION:
            return X9ValueObject::createWithFunction(_target, name,false);
        case MT_NONE:
            X9ASSERT(false,"getValue Error!!!");
            break;
    }
}
void X9RunObject::setValue(MemberType mt, const string& name, X9ValueObject* value)
{
    switch (mt) {
        case MT_SGET:
            getLibrary()->baseSetFunctions[getClassData()->baseSetKeys[name]][name](_target,value);
            break;
        case MT_VALUE:
            if(_topObj->values.find(name) != _topObj->values.end()) {
                _topObj->values[name]->setValueObject(value);
            }else{
                value->use();
                _topObj->values[name] = value;
            }
            break;
        case MT_PROPERTY:
        {
            X9Object* target = dynamic_cast<X9Object*>(_target);
            if (target->propertys.find(name) != target->propertys.end()) {
                target->propertys[name]->setValueObject(value);
            }else{
                value->use();
                target->propertys[name] = value;
//                target->propertyNames.push_back(name);
            }
            break;
        }
        default:
            X9ASSERT(false,"setValue Error!!!");
            break;
    }
//    if (_target == this) {
//        X9Object* target = dynamic_cast<X9Object*>(this);
//        if (target->propertys.find(name) != target->propertys.end()) {
//            target->propertys[name]->setValueObject(value);
//        }else{
//            value->use();
//            target->propertys[name] = value;
//        }
//    }else
//    {
//        if(_topObj->values.find(name) != _topObj->values.end()) {
//            _topObj->values[name]->setValueObject(value);
//        }else{
//            value->use();
//            _topObj->values[name] = value;
//        }
//    }
}

void X9RunObject::addFunctionValue(X9FunctionValueObject* funcValue)
{
    _functions.push_back(funcValue);
}
void X9RunObject::removeFunctionValue(X9FunctionValueObject* funcValue)
{
    for (auto it = _functions.begin(); it != _functions.end(); it++) {
        if (*it == funcValue) {
            _functions.erase(it);
            break;
        }
    }
    _functions.push_back(funcValue);
}
X9RunObject::MemberType X9RunObject::checkFunction(const string& name, bool isSuper)
{
    //temporary variable
    if (_topObj && _topObj->values.find(name) != _topObj->values.end()) {
        if (_topObj->values[name]->isFunction()) {
            return MT_VALUE;
        }
        return MT_NONE;
    }
    X9Object* target = dynamic_cast<X9Object*>(_target);
    //self variable 
    if(target->propertys.find(name) != target->propertys.end())
    {
        if (target->propertys[name]->isFunction()) {
            return MT_PROPERTY;
        }
        return MT_NONE;
    }
    X9ScriptClassData* cData = target->classData;
    //SGet
    if (cData->baseGetKeys.find(name) != cData->baseGetKeys.end()) {
        return MT_SGET;
    }
    //method variable
    if (cData->functions.find(name) != cData->functions.end()) {
        if (isSuper) {
            return MT_SUPER_FUNCTION;
        }
        return MT_FUNCTION;
    }
    //base method variable
    if (cData->baseFunctionKeys.find(name) != cData->baseFunctionKeys.end()) {
        if (isSuper) {
            return MT_BASE_SUPER_FUNCTION;
        }
        return MT_BASE_FUNCTION;
    }
    return MT_NONE;
}
X9ValueObject* X9RunObject::runFunction(MemberType mt, const string& name, const vector<X9ValueObject*>& values, X9ScriptClassData* currentClass)
{
//    X9ValueObject* funcValue = getValue(mt, name);
//    X9ASSERT(funcValue->type == X9ValueObject::FUNCTION,"function call Error!!!");
    X9ScriptClassData* cData = nullptr;
    switch (mt) {
        case MT_SGET:
        case MT_VALUE:
        case MT_PROPERTY:
            return getValue(mt, name)->getFunction()->runFunction(values);
        case MT_BASE_SUPER_FUNCTION:
        {
            cData = dynamic_cast<X9Object*>(_target)->classData;
            X9ASSERT(cData->baseFunctionKeys[name].size() > 1,"runFunction Error!!!");
            cData = cData->library->classes[cData->baseFunctionKeys[name][1]];
        }
        case MT_BASE_FUNCTION:
        {
            if (cData == nullptr) {
                cData = dynamic_cast<X9Object*>(_target)->classData;
            }
            const string& cName = cData->baseFunctionKeys[name][0];
            return cData->library->baseFunctions[cName][name](_target,values);
        }
        case MT_SUPER_FUNCTION:
        {
            cData = currentClass;
        }
        case MT_FUNCTION:
        {
            if (cData == nullptr) {
                cData = dynamic_cast<X9Object*>(_target)->classData;
            }
            X9ScriptData* funcData = cData->functions[name];
            X9RunObject* funcRunObj = X9RunObject::createTop(_target, this);
            for (int i = 0; i<funcData->arguments.size(); i++) {
                if (i == funcData->arguments.size()-1 && funcData->arguments[i]->type == X9ScriptData::ARGS) {
                    X9Array* arr = dynamic_cast<X9Array*>(dynamic_cast<X9Object*>(_target)->classData->library->createObject("Array"));
                    funcRunObj->values[funcData->arguments[i]->getStringValue()] = X9ValueObject::createWithObject(arr);
                    for (int k = i; k<values.size(); k++) {
                        arr->push(values[k]);
                    }
                }else {
                    X9ASSERT(values.size() > i,"function arguments Error!!!");
                    values[i]->use();
                    funcRunObj->values[funcData->arguments[i]->getStringValue()] = values[i];
                }
            }
            X9RunScript::runList(funcRunObj, funcData->children,cData);
            funcRunObj->clearValues();
            if (funcRunObj->getReturnValue() == nullptr) {
                return X9ValueObject::create();
            }
            return funcRunObj->getReturnValue();
        }
        case MT_NONE:
        case MT_CONST:
            X9ASSERT(cData->baseFunctionKeys[name].size() > 1,"not find function");
            break;
    }
}

//MARK::X9Path
//X9Path::X9Path(const string& path):_path(path)
//{
//    loadJsonDoc(_doc,path+"frameworks/frameworks.json");
//    for (int i = 0; i<_doc.Size(); i++) {
//        _frameWorkPaths.push_back(_doc[i].GetString());
//    }
//    globalScriptPath = _path+"game/Global.x9as";
//    mainScriptPath = _path+"game/Main.x9as";
//    if(FileUtils::getInstance()->fullPathForFilename(globalScriptPath) == "")
//    {
//        globalScriptPath = "";
//    }
//    if(FileUtils::getInstance()->fullPathForFilename(mainScriptPath) == "")
//    {
//        mainScriptPath = "";
//    }
//}
//string X9Path::getFullPathForName(const string& dir, const string& name)
//{
//    string path = _path+"game/"+dir+name;
//    string p = FileUtils::getInstance()->fullPathForFilename(path);
//    for (int i = 0; i<_frameWorkPaths.size() && p == ""; i++) {
//        path = _path+"frameworks/"+dir+name;
//        p = FileUtils::getInstance()->fullPathForFilename(path);
//    }
//    return p;
//}
//const string& X9Path::getDataPath(const string& name)
//{
//    if (_dataMap.find(name) == _dataMap.end()) {
//        string path = getFullPathForName("datas/",name);
//        CCASSERT(path != "","No file found");
//        _dataMap[name] = path;
//    }
//    return _dataMap[name];
//}
//const string& X9Path::getScriptPath(const string& name)
//{
//    if (_scriptMap.find(name) == _scriptMap.end()) {
//        string path = getFullPathForName("scripts/",name);
//        CCASSERT(path != "","No file found");
//        _scriptMap[name] = path;
//    }
//    return _scriptMap[name];
//}
//const string& X9Path::getResourcePath(const string& name)
//{
//    if (_resourceMap.find(name) == _resourceMap.end()) {
//        string path = getFullPathForName("resources/",name);
//        CCASSERT(path != "","No file found");
//        _resourceMap[name] = path;
//    }
//    return _resourceMap[name];
//}
//const string& X9Path::getAnimationPath(const string& name)
//{
//    if (_animationMap.find(name) == _animationMap.end()) {
//        string path = getFullPathForName("resources/animations/",name);
//        CCASSERT(path != "","No file found");
//        _animationMap[name] = path;
//    }
//    return _animationMap[name];
//}
//const string& X9Path::getFontPath(const string& name)
//{
//    if (_fontMap.find(name) == _fontMap.end()) {
//        string path = getFullPathForName("fonts/",name);
//        CCASSERT(path != "","No file found");
//        _fontMap[name] = path;
//    }
//    return _fontMap[name];
//}
//const string& X9Path::getShaderPath(const string& name)
//{
//    if (_shaderMap.find(name) == _shaderMap.end()) {
//        string path = getFullPathForName("shaders/",name);
//        CCASSERT(path != "","No file found");
//        _shaderMap[name] = path;
//    }
//    return _shaderMap[name];
//}
//MARK::X9intervalObject
//X9intervalObject::X9intervalObject(int ID,bool loop,X9FunctionValueObject* callback, float interval, const vector<X9ValueObject*>& values)
//{
//    this->ID = ID;
//    time = 0;
//    this->isLoop = loop;
//    callback->use();
//    this->callback = callback;
//    this->interval = interval;
//    this->values = values;
//    for (int i = 0; i<values.size(); i++) {
//        values[i]->use();
//    }
//}
//X9intervalObject::~X9intervalObject()
//{
//    callback->nonuse();
//    for (int i = 0; i<values.size(); i++) {
//        values[i]->nonuse();
//    }
//    values.clear();
//}
//MARK::X9Library

#define AddBaseClass_1(_N_,_E_) \
classes[#_N_] = new X9ScriptClassData(#_N_,#_E_,this); \
baseFunctions[#_N_] = map<string,std::function<X9ValueObject*(X9RunObject*,const vector<X9ValueObject*>&)>>(); \
baseSetFunctions[#_N_] = map<string,std::function<void(X9RunObject*,X9ValueObject*)>>(); \
baseGetFunctions[#_N_] = map<string,std::function<X9ValueObject*(X9RunObject*)>>(); \
X9##_N_::setBaseFunctions(this,#_N_); \
classes[#_N_]->createFunction = [](X9ScriptClassData* cData, const vector<X9ValueObject*>& vs){X9##_N_* obj = X9##_N_::create();
#define AddBaseClass_2(_N_) \
obj->initScript(cData,vs);return obj;}; \
X9##_N_::setConstValues(classes[#_N_]); \
classes[#_N_]->neatenExtendsAndFunctions()

#define AddBaseClass(_N_,_E_) \
AddBaseClass_1(_N_,_E_) \
AddBaseClass_2(_N_)
//#define AddBaseClass(_N_,_E_) \
//classes[#_N_] = new X9ScriptClassData(#_N_,#_E_,this); \
//baseFunctions[#_N_] = map<string,std::function<X9ValueObject*(X9RunObject*,const vector<X9ValueObject*>&)>>(); \
//baseSetFunctions[#_N_] = map<string,std::function<void(X9RunObject*,X9ValueObject*)>>(); \
//baseGetFunctions[#_N_] = map<string,std::function<X9ValueObject*(X9RunObject*)>>(); \
//X9##_N_::setBaseFunctions(this,#_N_); \
//classes[#_N_]->createFunction = [](X9ScriptClassData* cData, const vector<X9ValueObject*>& vs){X9##_N_* obj = X9##_N_::create();obj->initScript(cData,vs);return obj;}; \
//X9##_N_::setConstValues(classes[#_N_]); \
//classes[#_N_]->neatenExtendsAndFunctions()


X9ScriptData* X9Library::currentCodeData = nullptr;
void X9Library::throwCodeInfo(const string& msg)
{
    cout << "==X9code ERROR== " << currentCodeData->getErrorInfo().c_str() << " >>> " << msg.c_str() << endl;
    
    vector<X9ScriptData*> infoDatas;
    X9ScriptData* p = currentCodeData;
    while (p) {
        if (p->parentData && !p->parentData->children.empty()) {
            infoDatas.insert(infoDatas.begin(), p);
        }
        if (!p->parentData && dynamic_cast<X9ScriptClassData*>(p)) {
            infoDatas.insert(infoDatas.begin(), p);
        }
        p = p->parentData;
    }
    for (int i = 0; i<infoDatas.size(); i++) {
        int lineIdx = -1;
        if (infoDatas[i]->parentData) {
            for (int k = 0; k < infoDatas[i]->parentData->children.size(); k++) {
                if (infoDatas[i]->parentData->children[k] == infoDatas[i]) {
                    lineIdx = k+1;
                    break;
                }
            }
        }
        cout << "------ " << lineIdx << ": " << infoDatas[i]->toString() << endl;
    }
}

//string X9Library::__errorStr = "";
X9Library::X9Library(X9Path* path, X9Player* player)
{
//    __errorStr = "";
    currentCodeData = nullptr;
    this->player = player;
    defaultFontName = "droid_sans_fallback.ttf";
    soundPlayer = new X9SoundPlayer(player,this,0.3);
    X9RunScript::initScript();
    timer = 0;
//    _maxIntervalID = 0;
//    _maxTimeoutID = 0;
    this->path = path;
    AddBaseClass(Object,);
    AddBaseClass(Math,Object);
    AddBaseClass(UserDefault,Object);
    AddBaseClass(Point,Object);
    AddBaseClass(AniPosFrame,Object);
    AddBaseClass(Vec3,Object);
    AddBaseClass(Size,Object);
    AddBaseClass(Rect,Object);
    AddBaseClass(BezierList,Object);
    AddBaseClass(Color,Object);
    AddBaseClass(Array,Object);
//    AddBaseClass(BaseGlobal,Object);
    AddBaseClass_1(BaseGlobal,Object);
        cData->library->global = obj;
    AddBaseClass_2(BaseGlobal);
    
    AddBaseClass(Event,Object);
    AddBaseClass(HitTile,Object);
    AddBaseClass(Action,Object);
    AddBaseClass(CallBackAction,Action);
    AddBaseClass(UpdateAction,Action);
    AddBaseClass(EaseAction,Action);
    AddBaseClass(Sequence,Action);
    AddBaseClass(Spawn,Action);
    AddBaseClass(ValueTo,Action);
    AddBaseClass(MoveTo,Action);
    AddBaseClass(MoveBy,Action);
    AddBaseClass(ScaleTo,Action);
    AddBaseClass(ScaleBy,Action);
    AddBaseClass(RotationTo,Action);
    AddBaseClass(RotationBy,Action);
    AddBaseClass(SkewTo,Action);
    AddBaseClass(SkewBy,Action);
    AddBaseClass(VisibleTo,Action);
    AddBaseClass(ColorTo,Action);
    AddBaseClass(AlphaTo,Action);
    AddBaseClass(TouchEvent,Event);
    AddBaseClass(MovieClipEvent,Event);
    AddBaseClass(KeyboardEvent,Event);
    AddBaseClass(EventDispatcher, Object);
    AddBaseClass(DisplayObject, EventDispatcher);
    AddBaseClass(Node, DisplayObject);
    AddBaseClass(StateButton, Node);
    AddBaseClass(KeyFrameScriptNode, Node);
    AddBaseClass(MovieClip, DisplayObject);
    AddBaseClass(Particle, DisplayObject);
    AddBaseClass(Image, DisplayObject);
    AddBaseClass(Scale9Image, DisplayObject);
    AddBaseClass(Label, DisplayObject);
    AddBaseClass(BaseScene,Node);
//    loadJsonDoc(_doc,path.scriptPath+name);
//    vector<X9ScriptClassData*> _tempClasses;
//    for (int i = 0; i<_doc.Size(); i++) {
//        X9ScriptClassData* cData = new X9ScriptClassData(_doc[i],this);
////        _tempClasses.push_back(cData);
//        classes[cData->className] = cData;
//    }
    if (path->globalScriptPath == "") {
        global = dynamic_cast<X9BaseGlobal*>(createObject("BaseGlobal"));
    }else{
        new X9ScriptClassData(path->globalScriptPath,this);
        createObject("Global");
        global->isRunRemovedCall = false;
    }
    global->use();
    CCASSERT(path->firstSceneScriptPath != "","No file found:Main.x9as");
    if (classes.find("Main") == classes.end()) {
        _scenes.push_back("Main");
        new X9ScriptClassData(path->firstSceneScriptPath,this);
    }
//    mainName = "";
//    for (auto it = classes.begin(); it != classes.end(); it++) {
//        it->second->neatenExtendsAndFunctions();
//        if (it->second->baseExtendsName == "BaseGlobal") {
//            CCASSERT(_globalName == "","Global Class Error!!!");
//            _globalName = it->second->className;
//        }
//        if (!it->second->isBaseClass) {
//            if (it->second->baseExtendsName == "BaseMain") {
//                CCASSERT(mainName == "","Main Class Error!!!");
//                mainName = it->second->className;
//            }
//            it->second->createFunction = classes[it->second->baseExtendsName]->createFunction;
//        }
//    }
//    CCASSERT(mainName != "","Main Class Error!!!");
}
X9Library::~X9Library()
{
    global->nonuse();
    global = nullptr;
    for (auto it = classes.begin(); it != classes.end(); it++) {
        X9_SAFE_DELETE(it->second);
    }
    classes.clear();
//    for (int i = 0; i<intervals.size(); i++) {
//        delete intervals[i];
//    }
//    intervals.clear();
//    for (int i = 0; i<timeouts.size(); i++) {
//        delete timeouts[i];
//    }
//    timeouts.clear();
    delete soundPlayer;
}
void X9Library::addClassData(const string& name)
{
    if (!checkClassData(name)) {
        const string& p = path->getScriptPath(name);
        new X9ScriptClassData(p,this);
    }
}
X9ScriptClassData* X9Library::getClassData(const string& name)
{
    if (!checkClassData(name)) {
        addClassData(name);
    }
    return classes[name];
}
bool X9Library::checkClassData(const string& name)
{
    return classes.find(name) != classes.end();
}
X9RunObject* X9Library::createObject(const string& name,const vector<X9ValueObject*> values)
{
    X9ScriptClassData* cData = classes[name];
    return cData->createFunction(cData,values);
}
X9RunObject* X9Library::createObject(const string& name)
{
    return createObject(name,_emptyValues);
}
X9ValueObject* X9Library::createValueObject(const string& name,const vector<X9ValueObject*> values)
{
    return X9ValueObject::createWithObject(createObject(name,values));
}
X9ValueObject* X9Library::createValueObject(const string& name)
{
    return X9ValueObject::createWithObject(createObject(name));
}
X9BaseScene* X9Library::createScene(const string& name)
{
    X9BaseScene* scene = dynamic_cast<X9BaseScene*>(createObject(name));
    for (int i = 0; i<_scenes.size(); i++) {
        if (_scenes[i] == name) {
            string nextName = "";
            string prevName = "";
            if (i > 0) {
                prevName = _scenes[i-1];
            }
            if (i < _scenes.size()-1) {
                nextName = _scenes[i+1];
            }
            scene->nextSceneName = nextName;
            scene->prevSceneName = prevName;
            break;
        }
    }
    return scene;
}
//void X9Library::addUpdateTarget(X9Object* obj)
//{
//    for (int i = 0; i<updateTargets.size(); i++) {
//        if(updateTargets[i] == obj) return;
//    }
//    updateTargets.push_back(obj);
//}
//void X9Library::removeUpdateTarget(X9Object* obj)
//{
//    for (auto it = updateTargets.begin(); it != updateTargets.end(); it++) {
//        if (*it == obj) {
//            updateTargets.erase(it);
//            break;
//        }
//    }
//}
//void X9Library::addKeyTarget(X9DisplayObject* obj,bool isKeyDown)
//{
//    map<X9DisplayObject*,int>& _map = isKeyDown ? keyDownTargets : keyUpTargets;
//    if (_map.find(obj) == _map.end()) {
//        _map[obj] = 0;
//    }
//    _map[obj]++;
//}
//void X9Library::removeKeyTarget(X9DisplayObject* obj,bool isKeyDown)
//{
//    map<X9DisplayObject*,int>& _map = isKeyDown ? keyDownTargets : keyUpTargets;
//    if (_map.find(obj) != _map.end()) {
//        _map[obj]--;
//        if (_map[obj] == 0) {
//            _map.erase(obj);
//        }
//    }
//}
//void X9Library::removeAllKeyTarget(X9DisplayObject* obj)
//{
//    if (keyDownTargets.find(obj) != keyDownTargets.end()) {
//        keyDownTargets.erase(obj);
//    }
//    if (keyUpTargets.find(obj) != keyUpTargets.end()) {
//        keyUpTargets.erase(obj);
//    }
//}
//int X9Library::addInterval(X9FunctionValueObject* callback,float interval,const vector<X9ValueObject*>& values)
//{
//    intervals.push_back(new X9intervalObject(++_maxIntervalID,true,callback,interval,values));
//    return intervals.back()->ID;
//}
//int X9Library::addTimeout(X9FunctionValueObject* callback,float interval,const vector<X9ValueObject*>& values)
//{
//    timeouts.push_back(new X9intervalObject(++_maxTimeoutID,true,callback,interval,values));
//    return timeouts.back()->ID;
//}
//void X9Library::clearInterval(int ID)
//{
//    for (auto it = intervals.begin(); it != intervals.end(); it++) {
//        if ((*it)->ID == ID) {
//            intervals.erase(it);
//            break;
//        }
//    }
//}
//void X9Library::clearTimeout(int ID)
//{
//    for (auto it = timeouts.begin(); it != timeouts.end(); it++) {
//        if ((*it)->ID == ID) {
//            timeouts.erase(it);
//            break;
//        }
//    }
//}