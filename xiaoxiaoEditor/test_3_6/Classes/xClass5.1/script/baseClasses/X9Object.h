//
//  X9Object.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/25.
//
//

#ifndef X9Object_hpp
#define X9Object_hpp

#include "../X9ScriptData.h"

#define x9_AddBaseFunc(_C_,_N_) library->baseFunctions[className][#_N_] = base_##_C_##_N_

#define x9_AddBaseSGet(_C_,_N_) \
library->baseSetFunctions[className][#_N_] = baseSet_##_C_##_N_; \
library->baseGetFunctions[className][#_N_] = baseGet_##_C_##_N_

#define x9_AddBaseGet(_C_,_N_) library->baseGetFunctions[className][#_N_] = baseGet_##_C_##_N_

#define x9_AddConstValue(_T_,_N_,_V_) \
classData->constValues[#_N_] = X9ValueObject::createWith##_T_(_V_); \
classData->constValues[#_N_]->use()

#define x9_setCtor(_N_) ctors[#_N_] = CC_CALLBACK_1(X9##_N_::initObject, this)


#define X9_CPP_CREATE(_N_,_B_) \
X9##_N_* X9##_N_::create() { \
CREATE_NEW_AUTO_OBJ(X9##_N_,); \
} \
X9##_N_::X9##_N_():X9##_B_(#_N_) { \
x9_setCtor(_N_); \
}



#define X9_CPP_CREATE_KEY(_N_,_B_) \
X9##_N_* X9##_N_::create() { \
CREATE_NEW_AUTO_OBJ(X9##_N_,#_N_); \
} \
X9##_N_::X9##_N_(const string& key):X9##_B_(key) { \
x9_setCtor(_N_); \
}


class X9Object : public X9RunObject
{
    struct WatchCommand
    {
        vector<X9ValueObject*> funcs;
        bool isWatching;
        bool isRemoved;
        bool isRemoveAlled;
        WatchCommand();
        ~WatchCommand();
        void addFunc(X9ValueObject* func);
        void removeFunc(X9ValueObject* func);
        bool checkFunc(X9ValueObject* func);
    };
    map<string,WatchCommand> _watchCommands;
    void _setValueWatch(MemberType mt, const string& name, X9ValueObject* value);
    void initObject(const vector<X9ValueObject*>& vs);
protected:    float _displayTimeSpeed;
    float _realTimeSpeed;
protected:
    X9ValueObject* loadjson(const string& jsonStr);
    X9ValueObject* loadjson(const rapidjson::Value &jsonData);
public:
    string _currentCtorName;
    bool _isExoteric;
    bool isRunRemovedCall;
    map<string,std::function<void(const vector<X9ValueObject*>)>> ctors;
    void runSuperCtor(const string& name,const vector<X9ValueObject*>& vs);
    virtual void runCtor(const vector<X9ValueObject*>& vs){initObject(vs);};
public:
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    string className;
    X9ScriptClassData* classData;
    map<string,X9ValueObject*> propertys;
public:
    static X9Object* newObject(X9Library* library);
    static X9Object* create();
    X9Object(const string& key);
    void initScript(X9ScriptClassData* classData,const vector<X9ValueObject*>& vs);
    virtual void removed()override;
    virtual void setValue(MemberType mt, const string& name, X9ValueObject* value)override;
    void removedCall();
    bool has(const string& name);
    void deleteValue(const string& name);
    void addWatch(const string& name,X9ValueObject* func);
    void removeWatch(const string& name = "",X9ValueObject* func = nullptr);
    virtual void update(float delay){};
    float getTimeSpeed(){return _realTimeSpeed;};
    float getDisplayedTimeSpeed(){return _displayTimeSpeed;};
    void addNumberValue(const string& name, float value);
    void addBoolValue(const string& name, bool value);
    void addStringValue(const string& name, const string& value);
    void addObjectValue(const string& name, X9Object* value);
    void addNoneValue(const string& name);
    virtual string getJsonString()override;
//    virtual X9Object* clone();
    virtual string toString();
};

#endif /* X9Object_hpp */
