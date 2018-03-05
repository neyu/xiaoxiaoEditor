//
//  X9BaseGlobal.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/26.
//
//

#include "X9BaseGlobal.h"
#include "X9Array.h"
#include "X9Math.h"
#include "X9UserDefault.h"
#include "../X9Player.h"
#include "X9BaseScene.h"

X9ValueObject* base_global_isTouchWorking(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"isTouchWorking Error!!!");
    return X9ValueObject::createWithBool(target->getLibrary()->player->isTouchWorking(values[0]->getNumber()));
}
X9ValueObject* base_global_nextScene(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty() && target->getLibrary()->player->currentScene->nextSceneName != "","nextScene Error!!!");
    target->getLibrary()->player->gotoScene(target->getLibrary()->player->currentScene->nextSceneName);
    return X9ValueObject::create();
}
X9ValueObject* base_global_prevScene(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty() && target->getLibrary()->player->currentScene->prevSceneName != "","prevScene Error!!!");
    target->getLibrary()->player->gotoScene(target->getLibrary()->player->currentScene->prevSceneName);
    return X9ValueObject::create();
}
X9ValueObject* base_global_gotoScene(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isString(),"gotoScene Error!!!");
    target->getLibrary()->player->gotoScene(values[0]->getString());
    return X9ValueObject::create();
}


X9ValueObject* baseGet_global_currentScene(X9RunObject* target)
{
    X9BaseGlobal* obj = dynamic_cast<X9BaseGlobal*>(target);
    return X9ValueObject::createWithObject(obj->getLibrary()->player->currentScene);
}
X9ValueObject* baseGet_global_userDefault(X9RunObject* target)
{
    X9BaseGlobal* obj = dynamic_cast<X9BaseGlobal*>(target);
    return X9ValueObject::createWithObject(obj->userDefault);
}

void X9BaseGlobal::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(global_,isTouchWorking);
    x9_AddBaseFunc(global_,nextScene);
    x9_AddBaseFunc(global_,prevScene);
    x9_AddBaseFunc(global_,gotoScene);
    x9_AddBaseGet(global_,currentScene);
    x9_AddBaseGet(global_,userDefault);
}
void X9BaseGlobal::setConstValues(X9ScriptClassData* classData)
{
    
}
X9BaseGlobal* X9BaseGlobal::create()
{
    CREATE_NEW_AUTO_OBJ(X9BaseGlobal,);
}
X9BaseGlobal::X9BaseGlobal():X9Object("X9BaseGlobal")
{
    x9_setCtor(BaseGlobal);
}

void X9BaseGlobal::removed()
{
//    if (userDefault->_isAutoSave) {
//        userDefault->save();
//    }
    userDefault->nonuse();
    X9Object::removed();
}
/**
 * @class:BaseGlobal
 *
 */
void X9BaseGlobal::initObject(const vector<X9ValueObject*>& vs)
{
    runSuperCtor("Object",vs);
    userDefault = dynamic_cast<X9UserDefault*>(getLibrary()->createObject("UserDefault"));
    userDefault->use();
    setValue(MemberType::MT_PROPERTY, "math", getLibrary()->createValueObject("Math"));
    vector<string> jsonPaths;
    for(auto it = getLibrary()->path->getFrameWorkPaths().begin();it != getLibrary()->path->getFrameWorkPaths().end();it++)
    {
        jsonPaths.push_back((*it)+"/globalObject.json");
    }
    jsonPaths.push_back(getLibrary()->path->getProjectPath()+"globalObject.json");
    for (int i = 0; i<jsonPaths.size(); i++) {
        if(FileUtils::getInstance()->fullPathForFilename(jsonPaths[i]) != "")
        {
            loadJsonDoc(_doc,jsonPaths[i]);
            X9ValueObject* _obj = loadjson(_doc);
            X9ASSERT(_obj->isObject<X9Object*>(),"load globalObject.json Error!!!");
            X9Object* obj = _obj->getObject<X9Object*>();
            for(auto it = obj->propertys.begin();it != obj->propertys.end();it++) {
                setValue(MemberType::MT_PROPERTY, it->first, it->second);
            }
        }
    }
}