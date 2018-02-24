//
//  X9ScriptData.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/25.
//
//

#ifndef X9ScriptData_hpp
#define X9ScriptData_hpp

#include "../X9AutoObject.h"
#include "XJsonMacrocoding.h"
#include "../X9Path.h"

class X9RunObject;
class X9ValueObject;
class X9Library;
class X9Player;

class X9Object;
class X9BaseGlobal;
class X9Array;
class X9EventDispatcher;
class X9DisplayObject;
class X9Node;
class X9BaseScene;
class X9Event;
class X9Image;
class X9Point;
class X9Vec3;
class X9Size;
class X9Rect;
class X9Math;
class X9Label;
class X9Color;
class X9MovieClip;
class X9Particle;
class X9Slider;
class X9ScriptClassData;
class X9ScriptData
{
public:
    typedef enum {
        NONE = 1,
        EXPRESSION,
        EVALUATION,
        COMMAND,
        OPERATOR,
        VALUE,
        STRING,
        NUMBER,
        BOOL,
        CLASS,
        SCRIPT,
        ARGS,
        NEW,
        
        EXTENDS,
        DELETE,
        VAR,
        _CONST,
        BREAK,
        RETURN_,
        FUNCTION,
        IF,
        ELSE_IF,
        ELSE,
        WHILE,
        SWITCH,
        CASE,
        DEFAULT,
        FOR,
        FOR_IN,
        FOR_EACH,
    }CodeType;
    static map<string,CodeType> TempTypes;
public:
    X9ScriptData* parentData;
    X9ScriptClassData* rootData;
    CodeType type;
    CodeType valueType;
    union {
        bool b;
        float n;
        string* s;
    }value;
    int lineIndex;
    vector<string> pNames;
    vector<X9ScriptData*> arguments;
    vector<X9ScriptData*> children;
public:
    const string& getStringValue();
    float getNumberValue();
    bool getBoolValue();
    void setStringValue(const string& value);
    void setNumberValue(float value);
    void setBoolValue(bool value);
    virtual string getErrorInfo();
    virtual string toString();
public:
    X9ScriptData():valueType(NONE){};
    X9ScriptData(const rapidjson::Value &jsonData, X9ScriptData* parentData, X9ScriptClassData* rootData);
    ~X9ScriptData();
    void setScript(const rapidjson::Value &jsonData, X9ScriptData* parentData, X9ScriptClassData* rootData);
};
class X9ScriptClassData : public X9ScriptData
{
public:
    bool isFramework;
    bool isBaseClass;
    X9Library* library;
    string className;
    string baseExtendsName;
    vector<string> extends;
    map<string,X9ScriptData*> propertyMap;
    vector<X9ScriptData*> propertys;
    map<string,X9ValueObject*> constValues;
    map<string,X9ScriptData*> functions;
    map<string,vector<string>> baseFunctionKeys;
    map<string,string> baseSetKeys;
    map<string,string> baseGetKeys;
    std::function<X9RunObject*(X9ScriptClassData*,const vector<X9ValueObject*>)> createFunction;
public:
    X9ScriptClassData(const string& className, const string& extends,X9Library* library);
    X9ScriptClassData(const string& path,X9Library* library);
    ~X9ScriptClassData();
//    X9ValueObject* getConstValue(const string& name);
    void neatenExtendsAndFunctions();
//    void setConstValues();
};
//class X9AutoObject
//{
////    int _useCount;
//    bool isUse;
//public:
//    static map<X9AutoObject*,int> _memory;
//    static map<X9AutoObject*,int> _removeMemory;
//    static void recycleUpdate();
//    static void clearMemory();
//public:
//    X9AutoObject();
//    ~X9AutoObject();
//    void use();
//    void nonuse();
//    int getUseCount();
//    virtual void removed(){};
////    virtual void realRemoved(){};
//};
class X9FunctionValueObject : public X9AutoObject
{
public:
    X9RunObject* target;
    bool isSuper;
    bool isTargetEnabled;
    string functionName;
    static X9FunctionValueObject* create();
    static X9FunctionValueObject* create(X9RunObject* t,const string& fName, bool isSuper);
    X9ValueObject* runFunction(const vector<X9ValueObject*>& values);
    X9ValueObject* runFunction(X9ValueObject* v1);
    X9ValueObject* runFunction(X9ValueObject* v1,X9ValueObject* v2);
    X9ValueObject* runFunction(X9ValueObject* v1,X9ValueObject* v2,X9ValueObject* v3);
    X9ScriptData* getFunctionData();
    virtual void removed()override;
    virtual void use()override;
private:
    X9FunctionValueObject();
    ~X9FunctionValueObject();
};

class X9RunObject : public X9AutoObject
{
public:
    X9RunObject* _target;
    X9RunObject* _topObj;
protected:
    X9RunObject* _breakObj;
    X9ValueObject* _returnValue;
    bool _isBreak;
    vector<X9FunctionValueObject*> _functions;
public:
    typedef enum {
        MT_NONE = 1,
        MT_SGET,
        MT_VALUE,
        MT_PROPERTY,
        MT_CONST,
        MT_FUNCTION,
        MT_SUPER_FUNCTION,
        MT_BASE_FUNCTION,
        MT_BASE_SUPER_FUNCTION,
    }MemberType;
    //    typedef enum {
    //        SET_NONE = 1,
    //        SET_VAR,
    //        SET_THIS,
    //        SET_GLOBAL
    //    }SetTargetType;
public:
    map<string,X9ValueObject*> values;
public:
    static X9RunObject* create();
    static X9RunObject* create(X9RunObject* target,X9RunObject* parentObj);
    static X9RunObject* createTop(X9RunObject* target,X9RunObject* parentObj);
    static X9RunObject* createBreak(X9RunObject* target,X9RunObject* parentObj);
public:
    void setBreak();
    bool getBreak();
    X9ScriptClassData* getClassData();
    X9Library* getLibrary();
    X9Player* getPlayer();
    bool checkClassType(const string& className);
    void setReturnValue(X9ValueObject* value);
    X9ValueObject* getReturnValue();
    X9RunObject* getUseRunObject(const string& valueName,const vector<string>& pnames);
    virtual MemberType checkValue(const string& name,bool isSuper);
    virtual X9ValueObject* getValue(MemberType mt, const string& name);
    virtual void setValue(MemberType mt, const string& name, X9ValueObject* value);
    void addFunctionValue(X9FunctionValueObject* funcValue);
    void removeFunctionValue(X9FunctionValueObject* funcValue);
    virtual MemberType checkFunction(const string& name,bool isSuper);
    virtual X9ValueObject* runFunction(MemberType mt, const string& name, const vector<X9ValueObject*>& values, X9ScriptClassData* currentClass);
    
    virtual string getJsonString(){return "";};
    void clearValues();
protected:
    X9RunObject(const string& key);
    virtual void removed()override;
};

class X9ValueObject : public X9AutoObject
{
//    void _setValueObject(X9ValueObject* v);
//    void _setValueObjectWatch(X9ValueObject* v);
//    vector<X9ValueObject*> _watchCall;
public:
    typedef enum {
        NONE = 1,
        NUMBER,
        BOOL,
        STRING,
        OBJECT,
        FUNCTION,
    }ValueType;
private:
    ValueType type;
    union {
        bool b;
        float n;
        string* s;
        X9RunObject* o;
        X9FunctionValueObject* f;
    }value;
public:
    void setValueObject(X9ValueObject*);
//    std::function<void(X9ValueObject*)> setValueObject;
    void setNumber(float v);
    void setString(const string& v);
    void setBool(bool v);
    void setObject(X9RunObject* v);
    void setFunction(X9RunObject* t,const string& fName, bool isSuper);
public:
    float getNumber();
    string getString();
    bool getBool();
    X9RunObject* getObject();
    template<class T>
    T getObject()
    {
        return dynamic_cast<T>(getObject());
    }
    X9FunctionValueObject* getFunction();
    X9ValueObject* clone();
public:
    ValueType getType(){return type;};
    bool isValue();
    bool isNumber();
    bool isString();
    bool isBool();
    bool isObject();
    template<class T>
    bool isObject()
    {
        if (isObject()) {
            return dynamic_cast<T>(getObject()) != nullptr;
        }
        return false;
    }
    bool isFunction();
    bool isNone();
public:
    static X9ValueObject* create();
    static X9ValueObject* createWithBool(bool value);
    static X9ValueObject* createWithNumber(float value);
    static X9ValueObject* createWithString(const string& value);
    static X9ValueObject* createWithObject(X9RunObject* value);
    static X9ValueObject* createWithFunction(X9RunObject* target, const string& fName, bool isSuper);
    virtual void removed()override;
    virtual void use()override;
    virtual void nonuse()override;
//    void addWatch(X9ValueObject* func);
//    void removeWatch(X9ValueObject* func = nullptr);
    
    virtual string getJsonString();
private:
    void clear();
    X9ValueObject();
public:
    X9ValueObject* operator+(X9ValueObject* v);
    X9ValueObject* operator-(X9ValueObject* v);
    X9ValueObject* operator*(X9ValueObject* v);
    X9ValueObject* operator/(X9ValueObject* v);
    X9ValueObject* operator&&(X9ValueObject* v);
    X9ValueObject* operator||(X9ValueObject* v);
    X9ValueObject* operator==(X9ValueObject* v);
    X9ValueObject* operator!=(X9ValueObject* v);
    X9ValueObject* operator>=(X9ValueObject* v);
    X9ValueObject* operator>(X9ValueObject* v);
    X9ValueObject* operator<=(X9ValueObject* v);
    X9ValueObject* operator<(X9ValueObject* v);
    X9ValueObject* operator%(X9ValueObject* v);
};






//template<class T>
//bool checkValueObject(X9ValueObject* value);










template<class T>
bool checkValueObject(X9ValueObject* value)
{
    if (value->isObject()) {
        return dynamic_cast<T>(value->getObject()) != nullptr;
    }
    return false;
}

//class X9Path
//{
//    vector<string> _frameWorkPaths;
//    map<string,string> _scriptMap;
//    map<string,string> _resourceMap;
//    map<string,string> _animationMap;
//    map<string,string> _fontMap;
//    map<string,string> _shaderMap;
//    map<string,string> _dataMap;
//    string _path;
//    string getFullPathForName(const string& dir, const string& name);
//public:
//    string globalScriptPath;
//    string mainScriptPath;
//    string scriptPath;
//    string fontPath;
//    string resourcePath;
//    string animationPath;
//    X9Path(const string& path);
//    const string& getDataPath(const string& name);
//    const string& getScriptPath(const string& name);
//    const string& getResourcePath(const string& name);
//    const string& getAnimationPath(const string& name);
//    const string& getFontPath(const string& name);
//    const string& getShaderPath(const string& name);
//};
//class X9intervalObject
//{
//public:
//    int ID;
//    float time;
//    X9FunctionValueObject* callback;
//    vector<X9ValueObject*> values;
//    float interval;
//    bool isLoop;
//public:
//    X9intervalObject(int ID,bool loop,X9FunctionValueObject* callback, float interval, const vector<X9ValueObject*>& values);
//    ~X9intervalObject();
//};
#define X9ASSERT(cond,msg) \
if(!(cond)) X9Library::throwCodeInfo(msg); \
CCASSERT(cond,msg)


//#define X9_ERROR_STR(_B_,_S_,...) \
//if(X9Library::__errorStr == "" && !(_B_)) { \
//char bufIdx[128] = { 0 }; \
//sprintf(bufIdx, _S_,##__VA_ARGS__); \
//X9Library::__errorStr = bufIdx; \
//}

class X9SoundPlayer;
class X9Library
{
//    int _maxIntervalID;
//    int _maxTimeoutID;
public:
    static X9ScriptData* currentCodeData;
    static void throwCodeInfo(const string& msg);
public:
    vector<X9ValueObject*> _emptyValues;
    vector<string> _scenes;
//    string path;
    X9Path* path;
    X9BaseGlobal* global;
    X9Player* player;
    string defaultFontName;
    X9SoundPlayer* soundPlayer;
//    string mainName;
    float timer;
    map<string,X9ScriptClassData*> classes;
    map<string,map<string,std::function<X9ValueObject*(X9RunObject*,const vector<X9ValueObject*>&)>>> baseFunctions;
    map<string,map<string,std::function<void(X9RunObject*,X9ValueObject*)>>> baseSetFunctions;
    map<string,map<string,std::function<X9ValueObject*(X9RunObject*)>>> baseGetFunctions;
//    vector<X9Object*> updateTargets;
//    map<X9DisplayObject*,int> keyDownTargets;
//    map<X9DisplayObject*,int> keyUpTargets;
//    vector<X9intervalObject*> intervals;
//    vector<X9intervalObject*> timeouts;
public:
//    static string __errorStr;
    X9Library(X9Path* path,X9Player* player);
    ~X9Library();
    void addClassData(const string& name);
    X9ScriptClassData* getClassData(const string& name);
    bool checkClassData(const string& name);
    X9RunObject* createObject(const string& name,const vector<X9ValueObject*> values);
    X9RunObject* createObject(const string& name);
    X9ValueObject* createValueObject(const string& name,const vector<X9ValueObject*> values);
    X9ValueObject* createValueObject(const string& name);
    X9BaseScene* createScene(const string& name);
//    void addUpdateTarget(X9Object* obj);
//    void removeUpdateTarget(X9Object* obj);
//    void addKeyTarget(X9DisplayObject* obj,bool isKeyDown);
//    void removeKeyTarget(X9DisplayObject* obj,bool isKeyDown);
//    void removeAllKeyTarget(X9DisplayObject* obj);
//    int addInterval(X9FunctionValueObject* callback,float interval,const vector<X9ValueObject*>& values);
//    int addTimeout(X9FunctionValueObject* callback,float interval,const vector<X9ValueObject*>& values);
//    void clearInterval(int ID);
//    void clearTimeout(int ID);
};

#endif /* X9ScriptData_hpp */
