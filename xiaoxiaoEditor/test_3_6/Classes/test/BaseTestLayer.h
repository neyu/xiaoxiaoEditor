//
//  BaseTestLayer.h
//  xxTest
//
//  Created by xiaoxiaozzq on 15/8/31.
//
//

#ifndef __xxTest__BaseTestLayer__
#define __xxTest__BaseTestLayer__

#include "csAppClient.h"

#define NEW_FUNC(__TYPE__) __TYPE__(std::function<void()> closeCall):BaseTestLayer(closeCall){}

#define PUSH_TESTITEM(__NAME__,__CLASS_NAME__) b_testList.push_back(TestItemName(__NAME__,[](){return new __CLASS_NAME__();}));

#define PUSH_TESTITEM_NAME(__NAME__) b_testList.push_back(TestItemName(#__NAME__,[](){return new __NAME__();}));
//int64 __curTime__ = 0;
//inline int GetCurTime()
//{
//    timeval time;
//    gettimeofday(&time, NULL);
//    int64 millisecs = time.tv_sec;
//    millisecs = millisecs * 1000 + (time.tv_usec / 1000);
//    if(__curTime__ == 0)
//    {
//        __curTime__ = millisecs;
//    }
//    return millisecs - __curTime__;
//};
class BaseTestItemLayer : public Layer
{
public:
    Size winSize;
    int testIndex;
    BaseTestItemLayer(){
        winSize = Director::getInstance()->getWinSize();
    };
    virtual bool onTouchBegin(const Vec2 &location){return false;}
    virtual void onTouchMove(const Vec2 &location){}
    virtual void onTouchEnd(const Vec2 &location){}
};
struct TestItemName
{
    string name;
    std::function<BaseTestItemLayer*()> func;
    TestItemName(string name, std::function<BaseTestItemLayer*()> func):name(name),func(func){}
};
class BaseTestLayer : public Layer
{
    std::function<void()> closeCall;
    void onCallBack(Ref* sender, string t);
    int b_testIndex;
    BaseTestItemLayer* testItemLayer;
    void b_creatTestItem(int index);
    Label* b_label;
public:
    vector<TestItemName> b_testList;
public:
    BaseTestLayer(std::function<void()> closeCall);
    ~BaseTestLayer();
    virtual void onEnter() override;
    bool onToucheBegin(Touch* t, Event* e);
    void onToucheMove(Touch* t, Event* e);
    void onToucheEnd(Touch* t, Event* e);
};

#endif /* defined(__xxTest__BaseTestLayer__) */
