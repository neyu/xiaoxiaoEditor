//
//  X9AutoObject.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/21.
//
//

#include "X9AutoObject.h"
#include "script/X9ScriptData.h"
#include "script/baseClasses/X9Node.h"


//MARK::X9AutoObject
map<string,vector<X9AutoObject*>> X9AutoObject::_typePoolMemory = map<string,vector<X9AutoObject*>>();
map<string,map<X9AutoObject*,int>> X9AutoObject::_typeMemory = map<string,map<X9AutoObject*,int>>();
map<string,map<X9AutoObject*,int>> X9AutoObject::_typeRemoveMemory = map<string,map<X9AutoObject*,int>>();
int X9AutoObject::getPoolObjectCount(const string& key)
{
    if (_typePoolMemory.find(key) == _typePoolMemory.end()) {
        return 0;
    }
    return _typePoolMemory[key].size();
}
X9AutoObject* X9AutoObject::createPoolObject(const string& key)
{
    X9AutoObject* obj = _typePoolMemory[key].back();
//    obj->_isInPool = false;
    _typePoolMemory[key].erase(_typePoolMemory[key].end()-1);
    _typeRemoveMemory[key][obj] = 0;
    return obj;
}
void X9AutoObject::recycleUpdate()
{
    while (true) {
        bool isBreak = true;
        for (auto it1 = _typeRemoveMemory.begin(); it1 != _typeRemoveMemory.end(); it1++) {
            if (it1->second.empty()) {
                continue;
            }
            map<X9AutoObject*,int> copyMemory = it1->second;
            it1->second.clear();
            for (auto it2 = copyMemory.begin(); it2 != copyMemory.end(); it2++) {
                it2->first->removed();
                _typePoolMemory[it1->first].push_back(it2->first);
                isBreak = false;
            }
        }
        if (isBreak) {
            break;
        }
    }
}
X9AutoObject::X9AutoObject(const string& key):_key(key)
{
    //log("create(\"%s\")",key.c_str());
    _useCount = 0;
    _isUse = false;
//    _isInPool = false;
    if (_typeRemoveMemory.find(key) == _typeRemoveMemory.end()) {
        _typeRemoveMemory[key] = map<X9AutoObject*,int>();
    }
    if (_typeMemory.find(key) == _typeMemory.end()) {
        _typeMemory[key] = map<X9AutoObject*,int>();
    }
    if (_typePoolMemory.find(key) == _typePoolMemory.end()) {
        _typePoolMemory[key] = vector<X9AutoObject*>();
    }
    _typeRemoveMemory[key][this] = 0;
}
X9AutoObject::~X9AutoObject()
{
    //CCASSERT(false,"Error!!!");
}
void X9AutoObject::use()
{
    if (_isUse) {
        _typeMemory[_key][this]++;
    }else{
        _typeRemoveMemory[_key].erase(this);
        _typeMemory[_key][this] = 1;
        _isUse = true;
    }
    _useCount++;
}
void X9AutoObject::nonuse()
{
    CCASSERT(_isUse && _typeMemory[_key][this] > 0,"nonuse command Error!!!");
    _typeMemory[_key][this]--;
    if (_typeMemory[_key][this] == 0) {
        _isUse = false;
        _typeMemory[_key].erase(this);
        _typeRemoveMemory[_key][this] = 0;
    }
    _useCount--;
}
void X9AutoObject::recycle()
{
    CCASSERT(!_isUse,"recycle command Error!!!");
    removed();
    _typeRemoveMemory[_key].erase(this);
//    _isInPool = true;
    _typePoolMemory[_key].push_back(this);
}
void X9AutoObject::nonuseAndRecycle()
{
    nonuse();
    recycle();
}
int X9AutoObject::getUseCount()
{
    if (_isUse) {
        return _typeMemory[_key][this];
    }
    return 0;
}
void X9AutoObject::clearMemory()
{
//    map<X9AutoObject*,int> a = _typeMemory["X9Node"];
//    for(auto it = _typeMemory["X9Node"].begin();it != _typeMemory["X9Node"].end();it++)
//    {
//        X9Node* node = dynamic_cast<X9Node*>(it->first);
//        log("",node->getClassData()->getStringValue());
//    }
    for (auto it1 = _typeMemory.begin(); it1 != _typeMemory.end(); it1++) {
//        CCASSERT(it1->second.empty(),"Error!!!");
        if(!it1->second.empty()) {
            log("_typeMemory:%s, %d",it1->first.c_str(),(int)it1->second.size());
        }
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
            delete it2->first;
        }
    }
    for (auto it1 = _typeRemoveMemory.begin(); it1 != _typeRemoveMemory.end(); it1++) {
//        CCASSERT(it1->second.empty(),"Error!!!");
        if(!it1->second.empty()) {
            log("_typeRemoveMemory:%s, %d",it1->first.c_str(),(int)it1->second.size());
        }
        for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
            delete it2->first;
        }
    }
    for (auto it = _typePoolMemory.begin(); it != _typePoolMemory.end(); it++) {
        if(!it->second.empty()) {
            log("_typePoolMemory:%s, %d",it->first.c_str(),(int)it->second.size());
        }
        for (int i = 0; i<it->second.size(); i++) {
//            log("%d",it->second[i]->_useCount);
            delete it->second[i];
        }
    }
    _typeRemoveMemory.clear();
    _typeMemory.clear();
    _typePoolMemory.clear();
}
