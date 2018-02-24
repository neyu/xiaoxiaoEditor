//
//  X9Player.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/25.
//
//

#ifndef X9Player_hpp
#define X9Player_hpp

//#include "X9ScriptData.h"
#include "baseClasses/X9Node.h"
#include "../X9Path.h"
#include "X9TargetManager.h"
class X9SoundPlayer;
class X9Value: public X9AutoObject
{
    typedef enum {
        T_NUMBER = 1,
        T_BOOL,
        T_STRING,
        T_NONE,
    }ValueType;
    union{
        float f;
        bool b;
        string* s;
    }value;
    ValueType type;
    void clearString();
public:
    static X9Value* createNone();
    static X9Value* createNumber(float value);
    static X9Value* createBool(bool value);
    static X9Value* createString(const string& value);
    X9Value();
    virtual void removed()override;
    void setNumber(float value);
    void setBool(bool value);
    void setString(const string& value);
    void setNone();
    const string& getString(){return *value.s;};
    float getNumber(){return value.f;};
    bool getBool(){return value.b;};
    bool isNumber(){return type == T_NUMBER;};
    bool isBool(){return type == T_BOOL;};
    bool isString(){return type == T_STRING;};
    bool isNone(){return type == T_NONE;};
    X9ValueObject* getValueObject();
};
class X9Player : public Node
{
    bool _isLife;
    static map<EventKeyboard::KeyCode,string> _keyCodes;
    EventListenerTouchAllAtOnce* _touchListener;
    EventListenerKeyboard* _keyboardListener;
    EventListenerAcceleration* _accelerationListener;
    X9Node* _currentTouchNode;
    X9Library* library;
    Size _playerSize;
    X9SoundPlayer* soundPlayer;
    bool isTouching;
    vector<X9Node::TouchPoint> touchRootLocations;
    void addTouchNodes(X9Node* node, vector<X9Node*>* touchNodes);
    
    void madeTouchEvent(X9Node::X9MainTouchType type,const vector<Touch*>& touchs);
    map<string,std::function<X9Value*(X9Value*)>> appCommands;
    LayerColor* backgroundLayer;
    LayerColor* borderLayer1;
    LayerColor* borderLayer2;
public:
    vector<X9Node::TouchPoint> touchPlayerLocations;
    X9Path* path;
    map<string,bool> keyDownMap;
    Size sceneSize;
    X9TargetManager* targetManager;
private:
    void onTouchesBegan(const std::vector<Touch*>& touchs, Event* event);
    void onTouchesMoved(const std::vector<Touch*>& touchs, Event* event);
    void onTouchesEnded(const std::vector<Touch*>& touchs, Event* event);
    void onTouchesCancelled(const std::vector<Touch*>& touchs, Event* event);
    
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    virtual void update(float delay)override;
    string nextSceneName;
public:
    X9BaseScene* currentScene;
    static X9Player* create(const string& path,float screenWidth, float screenHeight);
    X9Player();
    ~X9Player();
    bool init(const string& path,float screenWidth, float screenHeight);
    void setAppCommand(const string& name,std::function<X9Value*(X9Value*)> command);
    void removeAppCommand(const string& name);
    X9Value* runAppCommand(const string& name, X9Value* value);
    X9Value* runGameCommand(const string& name, const vector<X9Value*>& values);
    const Vec2& getRootTouchLocation(int ID);
    bool isTouchWorking(int ID);
    void gotoScene(const string& name, bool realGoto = false);
    bool isLife(){return _isLife;}
};

#endif /* X9Player_hpp */
