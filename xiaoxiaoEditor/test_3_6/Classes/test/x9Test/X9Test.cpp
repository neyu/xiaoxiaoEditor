//
//  X9Test.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/5.
//
//

#include "X9Test.h"
#include "../../xClass5.1/animation/X9FLAnimation.h"


void X9Test::onEnter()
{
//    string path = FileUtils::getInstance()->fullPathForFilename("x9project");
//    vector<string> dir = getFilePathAtVec(path);
//    for (auto it = dir.begin(); it != dir.end(); it++) {
//        string p = FileUtils::getInstance()->fullPathForFilename("x9project/"+(*it)+"/game");
//        if (p == "") {
//            dir.erase(it);
//        }
//    }
//    b_testList.push_back(TestItemName("X9Test_script2",[](){return new X9Test_script2();}));
    PUSH_TESTITEM_NAME(X9Test_script2)
    BaseTestLayer::onEnter();
}
vector<string> X9Test::getFilePathAtVec(string filePath)
{
    std::vector<std::string> path_vec;
#ifdef WIN32

#else
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    int i=0;
    
    if((dp=opendir(filePath.c_str()))==NULL)
    {
        return path_vec;
    }
    chdir(filePath.c_str());
    
    while((entry=readdir(dp))!=NULL&&i<255)
    {
        stat(entry->d_name,&statbuf);
        log("name:%s",entry->d_name);
        if(!S_ISDIR(statbuf.st_mode))
            continue;
        string p = StringUtils::format("%s",entry->d_name);
        if (p == "." || p == "..") {
            continue;
        }
        path_vec.push_back(p);
    }
#endif
    return path_vec;
}
void X9Test_script1::onEnter()
{
    BaseTestItemLayer::onEnter();
//    X9FLAnimationLibrary::getInstance()->addAniPath("x9/resources/animations/");
    X9FLAnimation* ani = X9FLAnimation::create("ani1",Color3B::BLACK,10);
    ani->use();
    ani->setPosition(winSize/2);
    addChild(ani);
    ani->play();
    scheduleUpdate();
    vector<int> arr;
    for (int i = 0; i<10; i++) {
        arr.push_back(i);
    }
    for (auto it = arr.begin(); it!=arr.end(); it++) {
        if (*it == 3) {
            arr.erase(it);
        }
        log("%d",*it);
    }
}
void X9Test_script1::update(float delay)
{
    X9AutoObject::recycleUpdate();
}
void X9Test_script2::onEnter()
{
    BaseTestItemLayer::onEnter();
//    auto dispatcher = Director::getInstance()->getEventDispatcher();
//    
//    auto touchListener = EventListenerTouchAllAtOnce::create();
//    touchListener->onTouchesBegan = CC_CALLBACK_2(X9Test_script1::onTouchesBegan, this);
//    touchListener->onTouchesMoved = CC_CALLBACK_2(X9Test_script1::onTouchesMoved, this);
//    touchListener->onTouchesEnded = CC_CALLBACK_2(X9Test_script1::onTouchesEnded, this);
//    touchListener->onTouchesCancelled = CC_CALLBACK_2(X9Test_script1::onTouchesCancelled, this);
//    dispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
//    double num = 32.010000;
//    
//    printf("%g\n",num);
//    printf("%f",num);
    player = X9Player::create("zby_test1",winSize.width,winSize.height);
    player->setAppCommand("test", CC_CALLBACK_1(X9Test_script2::test, this));
    addChild(player);
}

X9Value* X9Test_script2::test(X9Value* value)
{
    CCASSERT(value->isString(),"appCommand tset Error!!!");
    log("Player::AppCommand::%s",value->getString().c_str());
    return X9Value::createString("啦啦啦啦");
}
void X9Test_script2::onTouchesBegan(const std::vector<Touch*>& touchs, Event* event)
{
    std::ostringstream oss;
    oss<<"onTouchesBegan:";
    oss<<touchs.size();
    for (int i = 0; i<touchs.size(); i++) {
        oss<<", ";
        oss<<touchs[i]->getID();
    }
    log("%s",oss.str().c_str());
}
void X9Test_script2::onTouchesMoved(const std::vector<Touch*>& touchs, Event* event)
{
    std::ostringstream oss;
    oss<<"onTouchesMoved:";
    oss<<touchs.size();
    for (int i = 0; i<touchs.size(); i++) {
        oss<<", ";
        oss<<touchs[i]->getID();
    }
    log("%s",oss.str().c_str());
}
void X9Test_script2::onTouchesEnded(const std::vector<Touch*>& touchs, Event* event)
{
    std::ostringstream oss;
    oss<<"onTouchesEnded:";
    oss<<touchs.size();
    for (int i = 0; i<touchs.size(); i++) {
        oss<<", ";
        oss<<touchs[i]->getID();
    }
    log("%s",oss.str().c_str());
}
void X9Test_script2::onTouchesCancelled(const std::vector<Touch*>& touchs, Event* event)
{
    
}