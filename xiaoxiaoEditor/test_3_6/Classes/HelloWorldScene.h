#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "csAppClient.h"
#include "xClass5.1/XBezierList.h"
//#include "test/BaseTestLayer.h"
class X9Player;
class HelloWorld : public cocos2d::Layer
{
    Size winSize;
    
    Node* stageGroup;
    X9Player* x9player;
    
    float dotSpace;
    Node* dotGroup;
    DrawNode* dotItem;
    
    Node* itemGroup;
    float _ox;
    float _mx;
    vector<string> getFilePathAtVec(string filePath);
    void gotoGame(const string& path);
//    float startDragTime;
    vector<Vec3> dragSpeeds;
    bool isAutoDraging;
    float autoMoveTime;
    static long _startTimeSec;
    int _listIndex;
    int _maxListIndex;
public:
    ~HelloWorld();
    static float getTimer();
    typedef enum {
        DRAG_BEGIN = 1,
        DRAG_MOVED,
        DRAG_ENDED
    }DragType;
    
    XBezierList* _blist;
    DrawNode* _bdot;
public:
    static cocos2d::Scene* createScene();
    virtual bool init()override;
    CREATE_FUNC(HelloWorld);
    
    void onKeyDown(EventKeyboard::KeyCode keyCode, Event* event);
    bool onToucheBegin(Touch* t, Event* e);
    void onToucheMove(Touch* t, Event* e);
    void onToucheEnd(Touch* t, Event* e);
    void onDrag(DragType type,const Vec2& p, float time);
};
class IconNode : public Node
{
    Node* stage;
    std::function<void(const string&)> callBack;
    std::function<void(HelloWorld::DragType,const Vec2&, float)> dragCallBack;
    string path;
    Rect touchRect;
    ClippingNode* clip;
    Vec2 _beginTouchLocation;
    Vec2 _oldMoveLocation;
    bool _isDrag;
public:
    static IconNode* create(const string& path,Node* stage,std::function<void(const string&)> callBack, std::function<void(HelloWorld::DragType,const Vec2&, float)> dragCallBack);
    IconNode();
    ~IconNode();
    bool init(const string& path,Node* stage,std::function<void(const string&)> callBack, std::function<void(HelloWorld::DragType,const Vec2&, float)> dragCallBack);
    bool onToucheBegin(Touch* t, Event* e);
    void onToucheMove(Touch* t, Event* e);
    void onToucheEnd(Touch* t, Event* e);
};

#endif // __HELLOWORLD_SCENE_H__
