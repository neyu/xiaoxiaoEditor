//
//  X9DisplayObject.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/1.
//
//

#ifndef X9DisplayObject_hpp
#define X9DisplayObject_hpp

#include "X9EventDispatcher.h"
class X9Node;
class X9Action;
class X9DisplayObject : public X9EventDispatcher
{
    void initObject(const vector<X9ValueObject*>& vs);
    int _keyDownEventCount;
    int _keyUpEventCount;
protected:
    Node* _node;
    bool _isInStage;
    bool _cascadeTimeSpeetEnabled;
    virtual void timeSpeedChanged(){};
public:
//    Size _size;
    int _childIndex;
    X9Node* _parentNode;
    vector<X9Action*> _actions;
    vector<X9Action*> _removedActions;
    virtual void setIsInStage(bool value);
    void setParentNode(X9Node* parentNode, int childIndex = -1);
    virtual void addedToStage(){};
    virtual void removedFromStage();
    
    virtual void checkAddEventListener(const string& type)override;
    virtual void checkRemoveEventListener(const string& type)override;
    Node* getNode(){return _node;}
    bool getIsInStage(){return _isInStage;};
public:
    bool isCascadeTimeSpeedEnabled(void) const{return _cascadeTimeSpeetEnabled;};
    void setTimeSpeed(float speed);
    virtual void updateDisplayedTimeSpeed(float parentSpeed);
    void setCascadeTimeSpeedEnabled(bool enabled);
    void updateCascadeTimeSpeed();
    virtual void disableCascadeTimeSpeed();
public:
    static X9DisplayObject* create();
    X9DisplayObject(const string& key);
    
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    void updateAction(float delay);
    void runAction(X9Action* action);
    void stopAction(X9Action* action);
    void stopAllAction();
};

#endif /* X9DisplayObject_hpp */
