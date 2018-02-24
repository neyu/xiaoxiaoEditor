//
//  BaseTestLayer.cpp
//  xxTest
//
//  Created by xiaoxiaozzq on 15/8/31.
//
//

#include "BaseTestLayer.h"
BaseTestLayer::BaseTestLayer(std::function<void()> closeCall)
{
    this->closeCall = closeCall;
    b_testIndex = 0;
    testItemLayer = nullptr;
}
BaseTestLayer::~BaseTestLayer()
{
    this->closeCall = nullptr;
    b_testList.clear();
}
void BaseTestLayer::onEnter()
{
    Layer::onEnter();
    Size winSize = Director::getInstance()->getWinSize();
    
    b_label = Label::createWithSystemFont("", "Marker Felt", 24);
    b_label->setPosition(winSize.width/2, winSize.height-30);
    addChild(b_label,100);
    
    auto itemL = MenuItemImage::create("b1.png", "b2.png",CC_CALLBACK_1(BaseTestLayer::onCallBack, this,"L"));
    itemL->setPositionX(-winSize.width*0.15);
    auto itemC = MenuItemImage::create("r1.png", "r2.png",CC_CALLBACK_1(BaseTestLayer::onCallBack, this,"C"));
    auto itemR = MenuItemImage::create("f1.png", "f2.png",CC_CALLBACK_1(BaseTestLayer::onCallBack, this,"R"));
    itemR->setPositionX(winSize.width*0.15);
    auto itemB = MenuItemFont::create("Back",CC_CALLBACK_1(BaseTestLayer::onCallBack, this,"B"));
    itemB->setPositionX(winSize.width*0.4);
    auto itemV = MenuItemFont::create("v",CC_CALLBACK_1(BaseTestLayer::onCallBack, this,"v"));
    itemV->setPositionX(winSize.width*0.48);
    auto menu = Menu::create(itemL,itemC,itemR,itemB, itemV, NULL);
    menu->setPosition(winSize.width/2, 40);
    addChild(menu, 100);
    b_creatTestItem(0);
    
    auto listener = EventListenerTouchOneByOne::create();
    //    listener->onT
    listener->onTouchBegan = CC_CALLBACK_2(BaseTestLayer::onToucheBegin, this);
    listener->onTouchMoved = CC_CALLBACK_2(BaseTestLayer::onToucheMove, this);
    listener->onTouchEnded = CC_CALLBACK_2(BaseTestLayer::onToucheEnd, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}
bool BaseTestLayer::onToucheBegin(Touch* t, Event* e)
{
    Vec2 p = t->getLocation();
    if(p.y < 64)
    {
        return false;
    }
    if(testItemLayer)
    {
        bool isReturn = testItemLayer->onTouchBegin(p);
        return isReturn;
    }
    return false;
}
void BaseTestLayer::onToucheMove(Touch* t, Event* e)
{
    if(testItemLayer)
    {
        testItemLayer->onTouchMove(t->getLocation());
    }
}
void BaseTestLayer::onToucheEnd(Touch* t, Event* e)
{
    if(testItemLayer)
    {
        testItemLayer->onTouchEnd(t->getLocation());
    }
}
void BaseTestLayer::b_creatTestItem(int index)
{
    if(index < b_testList.size())
    {
        if(testItemLayer)
        {
            removeChild(testItemLayer);
            testItemLayer = nullptr;
            Director::getInstance()->purgeCachedData();
        }
        b_label->setString(b_testList[index].name);
        testItemLayer = b_testList[index].func();
        testItemLayer->testIndex = index;
        testItemLayer->autorelease();
        addChild(testItemLayer);
    }
}
void BaseTestLayer::onCallBack(Ref* sender, string t)
{
    log("%s",t.c_str());
    if(t == "L")
    {
        if(b_testList.size() > 0)
        {
            b_testIndex = (b_testIndex-1+b_testList.size())%b_testList.size();
            b_creatTestItem(b_testIndex);
        }
    }else if(t == "R")
    {
        if(b_testList.size() > 0)
        {
            b_testIndex = (b_testIndex+1)%b_testList.size();
            b_creatTestItem(b_testIndex);
        }
    }else if(t == "C")
    {
        b_creatTestItem(b_testIndex);
    }else if(t == "B" && closeCall)
    {
        closeCall();
    }else if(t == "v")
    {
        Node* node = dynamic_cast<Node*>(sender);
        bool v = !node->getParent()->getChildren().at(0)->isVisible();
        b_label->setVisible(v);
        for(int i = 0;i<node->getParent()->getChildren().size();i++)
        {
            if (node->getParent()->getChildren().at(i) != node) {
                node->getParent()->getChildren().at(i)->setVisible(v);
            }
        }
    }
}