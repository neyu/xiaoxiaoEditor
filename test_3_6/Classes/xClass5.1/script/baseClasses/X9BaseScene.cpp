//
//  X9BaseScene.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/20.
//
//

#include "X9BaseScene.h"
#include "../X9Player.h"
#include "X9Color.h"

X9ValueObject* baseGet_scene_sceneWidth(X9RunObject* target)
{
    X9BaseScene* obj = dynamic_cast<X9BaseScene*>(target);
    return X9ValueObject::createWithNumber(obj->_sceneWidth);
}
X9ValueObject* baseGet_scene_sceneHeight(X9RunObject* target)
{
    X9BaseScene* obj = dynamic_cast<X9BaseScene*>(target);
    return X9ValueObject::createWithNumber(obj->_sceneHeight);
}
X9ValueObject* baseGet_scene_isLastScene(X9RunObject* target)
{
    X9BaseScene* obj = dynamic_cast<X9BaseScene*>(target);
    return X9ValueObject::createWithBool(obj->nextSceneName == "");
}
X9ValueObject* baseGet_scene_isFirstScene(X9RunObject* target)
{
    X9BaseScene* obj = dynamic_cast<X9BaseScene*>(target);
    return X9ValueObject::createWithBool(obj->prevSceneName == "");
}
X9ValueObject* baseGet_scene_isFillScreen(X9RunObject* target)
{
    X9BaseScene* obj = dynamic_cast<X9BaseScene*>(target);
    return X9ValueObject::createWithBool(obj->_isFillScreen);
}
void baseSet_scene_backgroundColor(X9RunObject* target, X9ValueObject* value)
{
    X9ASSERT(value->isObject<X9Color*>(),"set backgroundColor Error!!!");
    X9BaseScene* obj = dynamic_cast<X9BaseScene*>(target);
    obj->_backgroundColor = value->getObject<X9Color*>()->_color;
}
X9ValueObject* baseGet_scene_backgroundColor(X9RunObject* target)
{
    X9BaseScene* obj = dynamic_cast<X9BaseScene*>(target);
    return X9ValueObject::createWithObject(X9Color::newColor(target->getLibrary(),obj->_backgroundColor.r,obj->_backgroundColor.g,obj->_backgroundColor.b));
}
void X9BaseScene::setBaseFunctions(X9Library* library, const string& className)
{
    //    x9_AddBaseFunc(main_,test);
    x9_AddBaseGet(scene_,sceneWidth);
    x9_AddBaseGet(scene_,sceneHeight);
    x9_AddBaseGet(scene_,isLastScene);
    x9_AddBaseGet(scene_,isFirstScene);
    
    x9_AddBaseGet(scene_,isFillScreen);
    x9_AddBaseSGet(scene_,backgroundColor);
}
void X9BaseScene::setConstValues(X9ScriptClassData* classData)
{
    
}
X9BaseScene* X9BaseScene::create()
{
    CREATE_NEW_AUTO_OBJ(X9BaseScene,);
}
X9BaseScene::X9BaseScene():X9Node("X9BaseScene")
{
    _isInStage = true;
    nextSceneName = "";
    prevSceneName = "";
    x9_setCtor(BaseScene);
}

//void X9BaseScene::removed()
//{
//    X9Node::removed();
//}
void X9BaseScene::initObject(const vector<X9ValueObject*>& vs)
{
    getPlayer()->currentScene = this;
    _backgroundColor = Color3B::BLACK;
    _isFillScreen = false;
    getLibrary()->defaultFontName = "droid_sans_fallback.ttf";
    runSuperCtor("Node",vs);
    X9ASSERT(vs.size() >= 2 && vs.size() <= 4 && vs[0]->isNumber() && vs[1]->isNumber(),"new BaseScene Error!!!");
    _sceneWidth = vs[0]->getNumber();
    _sceneHeight = vs[1]->getNumber();
    if (vs.size() == 3) {
        X9ASSERT(vs[2]->isString(),"new BaseScene Error!!!");
        getLibrary()->defaultFontName = vs[4]->getString();
    }
    if(vs.size() >= 4)
    {
        X9ASSERT(vs[3]->isBool(),"new BaseScene Error!!!");
        _isFillScreen = vs[3]->getBool();
    }
}