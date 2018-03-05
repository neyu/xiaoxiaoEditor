//
//  X9TargetManager.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/22.
//
//

#ifndef X9TargetManager_hpp
#define X9TargetManager_hpp

#include "x9AppClient.h"

class X9Library;
class X9RunObject;
class X9Object;
class X9DisplayObject;
class X9ValueObject;
class X9FunctionValueObject;

class X9intervalObject
{
public:
    int ID;
    float time;
    X9FunctionValueObject* callback;
    vector<X9ValueObject*> values;
    float interval;
    bool isLoop;
public:
    X9intervalObject(int ID,bool loop,X9FunctionValueObject* callback, float interval, const vector<X9ValueObject*>& values);
    ~X9intervalObject();
};
class X9TargetManager
{
    int _maxIntervalID;
    int _maxTimeoutID;
    X9Library* _library;
private:
    map<X9RunObject*,bool> timerTargets;
    vector<X9Object*> updateTargets;
    vector<X9Object*> remove_updateTargets;
    vector<X9DisplayObject*> keyDownTargets;
    vector<X9DisplayObject*> keyUpTargets;
    vector<X9DisplayObject*> actionTarbets;
    vector<X9DisplayObject*> remove_actionTarbets;
    vector<X9intervalObject*> intervals;
    vector<X9intervalObject*> timeouts;
    vector<X9intervalObject*> remove_intervals;
    vector<X9intervalObject*> remove_timeouts;
public:
    X9TargetManager(X9Library* library);
    ~X9TargetManager();
public:
    void addUpdateTarget(X9Object* obj);
    void removeUpdateTarget(X9Object* obj);
    void addActionTarget(X9DisplayObject* obj);
    void removeActionTarget(X9DisplayObject* obj);
    void addKeyTarget(X9DisplayObject* obj,bool isKeyDown);
    void removeKeyTarget(X9DisplayObject* obj,bool isKeyDown);
//    void removeAllKeyTarget(X9DisplayObject* obj);
    int addInterval(X9FunctionValueObject* callback,float interval,const vector<X9ValueObject*>& values);
    int addTimeout(X9FunctionValueObject* callback,float interval,const vector<X9ValueObject*>& values);
    void clearInterval(int ID);
    void clearTimeout(int ID);
    void clearTimerTarget(X9RunObject* obj);
public:
    void updateAllTarget(float delay);
    void clearTargets();
    void onKeyHandler(bool isDown, const string& keyCode);
};

#endif /* X9TargetManager_hpp */
