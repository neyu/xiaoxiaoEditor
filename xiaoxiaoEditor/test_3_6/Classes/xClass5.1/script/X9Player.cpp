//
//  X9Player.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/25.
//
//

#include "X9Player.h"
#include "X9SoundPlayer.h"
#include "baseClasses/X9Event.h"
#include "baseClasses/X9BaseScene.h"
#include "../animation/X9FLAnimationData.h"

X9Value* X9Value::createNone()
{
    CREATE_NEW_AUTO_OBJ(X9Value,);
}
X9Value* X9Value::createNumber(float value)
{
    X9Value* vo = createNone();
    vo->setNumber(value);
    return vo;
}
X9Value* X9Value::createBool(bool value)
{
    X9Value* vo = createNone();
    vo->setBool(value);
    return vo;
}
X9Value* X9Value::createString(const string& value)
{
    X9Value* vo = createNone();
    vo->setString(value);
    return vo;
}
X9Value::X9Value():X9AutoObject("X9Value")
{
    type = T_NONE;
}
void X9Value::removed()
{
    clearString();
    X9AutoObject::removed();
}
void X9Value::clearString()
{
    if (type == T_STRING) {
        X9_SAFE_DELETE(value.s);
    }
    type = T_NONE;
}
void X9Value::setNumber(float value)
{
    clearString();
    type = T_NUMBER;
    this->value.f = value;
}
void X9Value::setBool(bool value)
{
    clearString();
    type = T_BOOL;
    this->value.b = value;
}
void X9Value::setString(const string& value)
{
    clearString();
    type = T_STRING;
    this->value.s = new string(value);
}
X9ValueObject* X9Value::getValueObject()
{
    switch (type) {
        case T_NONE:
            return X9ValueObject::create();
        case T_NUMBER:
            return X9ValueObject::createWithNumber(getNumber());
        case T_BOOL:
            return X9ValueObject::createWithBool(getBool());
        case T_STRING:
            return X9ValueObject::createWithString(getString());
    }
}
X9Player* X9Player::create(const string &path,float screenWidth, float screenHeight)
{
    X9Player* mc = new X9Player();
    if (mc && mc->init(path,screenWidth, screenHeight)) {
        mc->autorelease();
        return mc;
    }
    X9_SAFE_DELETE(mc);
    return nullptr;
}
map<EventKeyboard::KeyCode,string> X9Player::_keyCodes;
X9Player::X9Player()
{
    if (_keyCodes.empty()) {
        _keyCodes[EventKeyboard::KeyCode::KEY_SPACE] = "space";
        _keyCodes[EventKeyboard::KeyCode::KEY_SHIFT] = "shift";
        _keyCodes[EventKeyboard::KeyCode::KEY_ALT] = "alt";
        _keyCodes[EventKeyboard::KeyCode::KEY_TAB] = "tab";
        _keyCodes[EventKeyboard::KeyCode::KEY_CTRL] = "ctrl";
        _keyCodes[EventKeyboard::KeyCode::KEY_COMMA] = "ctrl";
        _keyCodes[EventKeyboard::KeyCode::KEY_ENTER] = "enter";
        _keyCodes[EventKeyboard::KeyCode::KEY_UP_ARROW] = "up";
        _keyCodes[EventKeyboard::KeyCode::KEY_DOWN_ARROW] = "down";
        _keyCodes[EventKeyboard::KeyCode::KEY_LEFT_ARROW] = "left";
        _keyCodes[EventKeyboard::KeyCode::KEY_RIGHT_ARROW] = "right";
        _keyCodes[EventKeyboard::KeyCode::KEY_0] = "0";
        _keyCodes[EventKeyboard::KeyCode::KEY_1] = "1";
        _keyCodes[EventKeyboard::KeyCode::KEY_2] = "2";
        _keyCodes[EventKeyboard::KeyCode::KEY_3] = "3";
        _keyCodes[EventKeyboard::KeyCode::KEY_4] = "4";
        _keyCodes[EventKeyboard::KeyCode::KEY_5] = "5";
        _keyCodes[EventKeyboard::KeyCode::KEY_6] = "6";
        _keyCodes[EventKeyboard::KeyCode::KEY_7] = "7";
        _keyCodes[EventKeyboard::KeyCode::KEY_8] = "8";
        _keyCodes[EventKeyboard::KeyCode::KEY_9] = "9";
        _keyCodes[EventKeyboard::KeyCode::KEY_Q] = "q";
        _keyCodes[EventKeyboard::KeyCode::KEY_W] = "w";
        _keyCodes[EventKeyboard::KeyCode::KEY_E] = "e";
        _keyCodes[EventKeyboard::KeyCode::KEY_R] = "r";
        _keyCodes[EventKeyboard::KeyCode::KEY_T] = "t";
        _keyCodes[EventKeyboard::KeyCode::KEY_Y] = "y";
        _keyCodes[EventKeyboard::KeyCode::KEY_U] = "u";
        _keyCodes[EventKeyboard::KeyCode::KEY_I] = "i";
        _keyCodes[EventKeyboard::KeyCode::KEY_O] = "o";
        _keyCodes[EventKeyboard::KeyCode::KEY_P] = "p";
        _keyCodes[EventKeyboard::KeyCode::KEY_A] = "a";
        _keyCodes[EventKeyboard::KeyCode::KEY_S] = "s";
        _keyCodes[EventKeyboard::KeyCode::KEY_D] = "d";
        _keyCodes[EventKeyboard::KeyCode::KEY_F] = "f";
        _keyCodes[EventKeyboard::KeyCode::KEY_G] = "g";
        _keyCodes[EventKeyboard::KeyCode::KEY_H] = "h";
        _keyCodes[EventKeyboard::KeyCode::KEY_J] = "j";
        _keyCodes[EventKeyboard::KeyCode::KEY_K] = "k";
        _keyCodes[EventKeyboard::KeyCode::KEY_L] = "l";
        _keyCodes[EventKeyboard::KeyCode::KEY_Z] = "z";
        _keyCodes[EventKeyboard::KeyCode::KEY_X] = "x";
        _keyCodes[EventKeyboard::KeyCode::KEY_C] = "c";
        _keyCodes[EventKeyboard::KeyCode::KEY_V] = "v";
        _keyCodes[EventKeyboard::KeyCode::KEY_B] = "b";
        _keyCodes[EventKeyboard::KeyCode::KEY_N] = "n";
        _keyCodes[EventKeyboard::KeyCode::KEY_M] = "m";
    }
    _isLife = true;
    _touchListener = nullptr;
    _keyboardListener = nullptr;
    _currentTouchNode = nullptr;
    library = nullptr;
    currentScene = nullptr;
    nextSceneName = "";
    for (auto it = _keyCodes.begin(); it != _keyCodes.end(); it++) {
        keyDownMap[it->second] = false;
    }
    backgroundLayer = nullptr;
    borderLayer1 = nullptr;
    borderLayer2 = nullptr;
}
X9Player::~X9Player()
{
    _isLife = false;
    if(currentScene)
    {
        currentScene->nonuse();
        currentScene = nullptr;
    }
    _currentTouchNode = nullptr;
    if (_touchListener) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);
        _touchListener = nullptr;
    }
    if (_keyboardListener) {
        Director::getInstance()->getEventDispatcher()->removeEventListener(_keyboardListener);
        _keyboardListener = nullptr;
    }
    delete targetManager;
    delete library;
    delete path;
    X9AutoObject::recycleUpdate();
    X9AutoObject::clearMemory();
}
bool X9Player::init(const string &path,float screenWidth, float screenHeight)
{
    if (Node::init()) {
        _playerSize.width = screenWidth;
        _playerSize.height = screenHeight;
        this->path = new X9Path(path);
        X9FLAnimationLibrary::getInstance()->setPath(this->path);
        library = new X9Library(this->path,this);
        targetManager = new X9TargetManager(library);
        soundPlayer = library->soundPlayer;
        scheduleUpdate();
        _currentTouchNode = nullptr;
        auto touchListener = EventListenerTouchAllAtOnce::create();
        touchListener->onTouchesBegan = CC_CALLBACK_2(X9Player::onTouchesBegan, this);
        touchListener->onTouchesMoved = CC_CALLBACK_2(X9Player::onTouchesMoved, this);
        touchListener->onTouchesEnded = CC_CALLBACK_2(X9Player::onTouchesEnded, this);
        touchListener->onTouchesCancelled = CC_CALLBACK_2(X9Player::onTouchesCancelled, this);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);
        _touchListener = touchListener;
        
        auto keyListener = EventListenerKeyboard::create();
        keyListener->onKeyPressed = CC_CALLBACK_2(X9Player::onKeyPressed, this);
        keyListener->onKeyReleased = CC_CALLBACK_2(X9Player::onKeyReleased, this);
        Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
        _keyboardListener = keyListener;
        gotoScene("Main",true);
        return true;
    }
    return false;
}
void X9Player::update(float delay)
{
    library->timer+= delay;
    targetManager->updateAllTarget(delay);
    X9AutoObject::recycleUpdate();
    targetManager->clearTargets();
    if(nextSceneName != "")
    {
        gotoScene(nextSceneName, true);
    }
}
void X9Player::setAppCommand(const string& name,std::function<X9Value*(X9Value*)> command)
{
    appCommands[name] = command;
}
void X9Player::removeAppCommand(const string& name)
{
    if (appCommands.find(name) != appCommands.end()) {
        appCommands.erase(name);
    }
}
X9Value* X9Player::runAppCommand(const string& name, X9Value* value)
{
    if (appCommands.find(name) != appCommands.end()) {
        return appCommands[name](value);
    }
    return X9Value::createNone();
}
X9Value* X9Player::runGameCommand(const string& name, const vector<X9Value*>& values)
{
//    if (appCommands.find(name) != appCommands.end()) {
//        return appCommands[name](value);
//    }
    return X9Value::createNone();
}
void X9Player::addTouchNodes(X9Node* node, vector<X9Node*>* touchNodes)
{
    if (node->touchEnabled) {
        touchNodes->push_back(node);
    }
    for (auto it = node->elements.begin(); it != node->elements.end(); it++) {
        X9Node* elementNode = dynamic_cast<X9Node*>(*it);
        if (elementNode && elementNode->getNode()->isVisible()) {
            addTouchNodes(elementNode,touchNodes);
        }
    }
//    vector<X9Node*> nodes;
//    nodes.push_back(currentScene);
//    for (int i = 0; i<nodes.size(); i++) {
//        for (auto it = nodes[i]->elements.begin();it != nodes[i]->elements.end(); it++) {
//            X9Node* nodeChild = dynamic_cast<X9Node*>(*it);
//            if (nodeChild && nodeChild->getNode()->isVisible()) {
//                if (nodeChild->touchChildCount > 0) {
//                    nodes.push_back(nodeChild);
//                }
//                if (nodeChild->touchEnabled) {
//                    touchNodes->push_back(nodeChild);
//                }
//            }
//        }
//    }
}
void X9Player::madeTouchEvent(X9Node::X9MainTouchType type,const vector<Touch*>& touchs)
{
    touchRootLocations.resize(touchs.size());
    touchPlayerLocations.resize(touchs.size());
    vector<X9Node::TouchPoint> nodeLocations(touchs.size());
    for (int i = 0; i<touchs.size(); i++) {
        touchRootLocations[i].location = touchs[i]->getLocation();
        touchRootLocations[i].id = touchs[i]->getID();
        touchPlayerLocations[i].location = convertTouchToNodeSpace(touchs[i]);
        touchPlayerLocations[i].id = touchs[i]->getID();
    }
    if (type == X9Node::X9MT_BEGIN && !_currentTouchNode) {
        vector<X9Node*> touchNodes;
        addTouchNodes(currentScene,&touchNodes);
        if (touchNodes.empty()) {
            _currentTouchNode = nullptr;
        }else{
            for (int i = touchNodes.size()-1; i>=0; i--) {
                X9Node* node = touchNodes[i];
                for (int k = 0; k<touchs.size(); k++) {
                    Vec2 p = node->getNode()->convertTouchToNodeSpace(touchs[k]);
                    if (node->touchRect.containsPoint(p)) {
                        _currentTouchNode = node;
                        break;
                    }
                }
                if (_currentTouchNode) {
                    break;
                }
            }
        }
    }
    if (_currentTouchNode) {
        for (int i = 0; i<touchs.size(); i++) {
            nodeLocations[i].location.set(_currentTouchNode->getNode()->convertTouchToNodeSpace(touchs[i]));
        }
        _currentTouchNode->_sendTouch(type, nodeLocations);
    }
}
void X9Player::onTouchesBegan(const std::vector<Touch*>& touchs, Event* event)
{
    madeTouchEvent(X9Node::X9MT_BEGIN,touchs);
}
void X9Player::onTouchesMoved(const std::vector<Touch*>& touchs, Event* event)
{
    madeTouchEvent(X9Node::X9MT_MOVED,touchs);
}
void X9Player::onTouchesEnded(const std::vector<Touch*>& touchs, Event* event)
{
    madeTouchEvent(X9Node::X9MT_ENDED,touchs);
    _currentTouchNode = nullptr;
}
void X9Player::onTouchesCancelled(const std::vector<Touch*>& touchs, Event* event)
{
    madeTouchEvent(X9Node::X9MT_CANCELLED,touchs);
}

void X9Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (_keyCodes.find(keyCode) != _keyCodes.end()) {
        keyDownMap[_keyCodes[keyCode]] = true;
        targetManager->onKeyHandler(true, _keyCodes[keyCode]);
//        for (auto it = library->keyDownTargets.begin(); it != library->keyDownTargets.end(); it++) {
//            if (it->first->getIsInStage()) {
//                X9Event* event = X9Event::newEvent(library,"keyDown", it->first);
//                event->addStringValue("keyCode", _keyCodes[keyCode]);
//                it->first->dispatchEvent(event);
//            }
//        }
    }
}
void X9Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (_keyCodes.find(keyCode) != _keyCodes.end()) {
        keyDownMap[_keyCodes[keyCode]] = false;
        targetManager->onKeyHandler(false, _keyCodes[keyCode]);
//        for (auto it = library->keyDownTargets.begin(); it != library->keyDownTargets.end(); it++) {
//            if (it->first->getIsInStage()) {
//                X9Event* event = X9Event::newEvent(library,"keyUp", it->first);
//                event->addStringValue("keyCode", _keyCodes[keyCode]);
//                it->first->dispatchEvent(event);
//            }
//        }
    }
}
const Vec2& X9Player::getRootTouchLocation(int ID)
{
    for (int i = 0; i<touchRootLocations.size(); i++) {
        if (touchRootLocations[i].id == ID) {
            return touchRootLocations[i].location;
        }
    }
    return Vec2::ZERO;
}
bool X9Player::isTouchWorking(int ID)
{
    for (int i = 0; i<touchRootLocations.size(); i++) {
        if (touchRootLocations[i].id == ID) {
            return true;
        }
    }
    return false;
}
void X9Player::gotoScene(const string& name, bool realGoto)
{
    if (currentScene) {
        currentScene->setIsInStage(false);
        removeChild(currentScene->getNode());
        currentScene->nonuse();
        currentScene = nullptr;
    }
    if (realGoto) {
        currentScene = library->createScene(name);
        currentScene->use();
        addChild(currentScene->getNode());
        currentScene->setIsInStage(true);
        sceneSize.width = currentScene->_sceneWidth;
        sceneSize.height = currentScene->_sceneHeight;
        float sw = _playerSize.width;
        float sh = _playerSize.height;
        if(currentScene->_isFillScreen) {
            currentScene->getNode()->setScale(_playerSize.width/sceneSize.width,_playerSize.height/sceneSize.height);
        }else{
            float s = MIN(_playerSize.width/sceneSize.width,_playerSize.height/sceneSize.height);
            sw = sceneSize.width*s;
            sh = sceneSize.height*s;
            currentScene->getNode()->setScale(s);
            currentScene->getNode()->setPosition((_playerSize.width-sw)/2,(_playerSize.height-sh)/2);
        }
        nextSceneName = "";
        if (backgroundLayer == nullptr) {
            backgroundLayer = LayerColor::create(Color4B::BLACK);
            borderLayer1 = LayerColor::create(Color4B::BLACK);
            borderLayer2 = LayerColor::create(Color4B::BLACK);
            backgroundLayer->setAnchorPoint(Vec2::ZERO);
            borderLayer1->setAnchorPoint(Vec2::ZERO);
            borderLayer2->setAnchorPoint(Vec2::ZERO);
            addChild(backgroundLayer, -100);
            addChild(borderLayer1, 100);
            addChild(borderLayer2, 100);
        }
        backgroundLayer->setColor(currentScene->_backgroundColor);
        backgroundLayer->setContentSize(_playerSize);
        borderLayer1->setVisible(!currentScene->_isFillScreen);
        borderLayer2->setVisible(!currentScene->_isFillScreen);
        if (!currentScene->_isFillScreen) {
            borderLayer1->setPosition(0, 0);
            if (sw < _playerSize.width) {
                Size bSize((_playerSize.width-sw)/2,_playerSize.height);
                borderLayer1->setContentSize(bSize);
                borderLayer2->setContentSize(bSize);
                borderLayer2->setPosition(_playerSize.width-(_playerSize.width-sw)/2, 0);
            }else{
                Size bSize(_playerSize.width,(_playerSize.height-sh)/2);
                borderLayer1->setContentSize(bSize);
                borderLayer2->setContentSize(bSize);
                borderLayer2->setPosition(0,_playerSize.height-(_playerSize.height-sh)/2);
            }
        }
    }else{
        nextSceneName = name;
    }
}