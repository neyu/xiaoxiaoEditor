#include "HelloWorldScene.h"
#include "csAppClient.h"
//#include "test/ConfigData.h"

//#include "test/x9Test/X9Test.h"
#include "xClass5.1/script/X9Player.h"

//USING_NS_CC;

//#define CreatCall(ClassName) ([](){ \
//Layer* L = new ClassName(); \
//L->autorelease(); \
//return L; \
//})
//
//#define NEW_TEST(__CLASS__) \
//{#__CLASS__,[](std::function<void()> closeCall){return new __CLASS__(closeCall);}}

IconNode* IconNode::create(const string& path,Node* stage,std::function<void(const string&)> callBack, std::function<void(HelloWorld::DragType,const Vec2&,float)> dragCallBack)
{
    auto icon = new IconNode();
    if (icon && icon->init(path, stage, callBack, dragCallBack)) {
        icon->autorelease();
        return icon;
    }
    delete icon;
    return nullptr;
}
IconNode::IconNode()
{
    _isDrag = false;
}
IconNode::~IconNode()
{
    
}
bool IconNode::init(const string& path,Node* stage,std::function<void(const string&)> callBack, std::function<void(HelloWorld::DragType,const Vec2&,float)> dragCallBack)
{
    if (Node::init()) {
        
        
        this->stage = stage;
        this->path = path;
        this->callBack = callBack;
        this->dragCallBack = dragCallBack;
        touchRect.setRect(-40, -60, 80, 100);
        Label* label = Label::createWithTTF(path, "droid_sans_fallback.ttf", 20);
        label->setPositionY(-52);
        addChild(label);
        clip = ClippingNode::create(Sprite::create("iconBG.png"));
        clip->setAlphaThreshold(0.5);
        Sprite* icon;
        Texture2D *texture = Director::getInstance()->getTextureCache()->addImage("x9project/"+path+"/icon.png");
        if (texture) {
            icon = Sprite::createWithTexture(texture);
        }else{
            icon = Sprite::create("tempIcon.png");
        }
        icon->cocos2d::Node::setScale(80/icon->getContentSize().width, 80/icon->getContentSize().height);
        addChild(clip);
        clip->addChild(icon);
        auto listener = EventListenerTouchOneByOne::create();
        listener->setSwallowTouches(true);
        //    listener->onT
        listener->onTouchBegan = CC_CALLBACK_2(IconNode::onToucheBegin, this);
        listener->onTouchMoved = CC_CALLBACK_2(IconNode::onToucheMove, this);
        listener->onTouchEnded = CC_CALLBACK_2(IconNode::onToucheEnd, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
        return true;
    }
    return false;
}
bool IconNode::onToucheBegin(Touch* t, Event* e)
{
    Vec2 p = this->convertTouchToNodeSpace(t);
    _beginTouchLocation = p;
    _oldMoveLocation = p;
    _isDrag = false;
    if(touchRect.containsPoint(p)) {
        clip->setScale(1.1);
        return true;
    }
    return false;
}
void IconNode::onToucheMove(Touch* t, Event* e)
{
    Vec2 p = this->convertTouchToNodeSpace(t);
    if (!_isDrag) {
        if(abs(p.x - _beginTouchLocation.x) > 10)
        {
            clip->setScale(1);
            _isDrag = true;
            dragCallBack(HelloWorld::DragType::DRAG_BEGIN,stage->convertToNodeSpace(this->convertToWorldSpace(_oldMoveLocation)),HelloWorld::getTimer()-0.016);
        }
    }
    if (_isDrag) {
        dragCallBack(HelloWorld::DragType::DRAG_MOVED,stage->convertToNodeSpace(this->convertToWorldSpace(p)),HelloWorld::getTimer());
    }
    _oldMoveLocation = p;
}
void IconNode::onToucheEnd(Touch* t, Event* e)
{
    Vec2 p = this->convertTouchToNodeSpace(t);
    if (_isDrag) {
        dragCallBack(HelloWorld::DragType::DRAG_ENDED,stage->convertToNodeSpace(this->convertToWorldSpace(p)),HelloWorld::getTimer());
        return;
    }
    clip->setScale(1);
    if(touchRect.containsPoint(p)) {
        callBack(path);
    }
}



long HelloWorld::_startTimeSec = 0;
Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}
HelloWorld::~HelloWorld()
{
    stageGroup->release();
}
vector<string> HelloWorld::getFilePathAtVec(string filePath)
{
    vector<std::string> path_vec;
    filePath = FileUtils::getInstance()->fullPathForFilename(filePath);
    if (filePath == "") {
        return path_vec;
    }
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
//        log("name:%s",entry->d_name);
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

bool HelloWorld::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    winSize = Director::getInstance()->getWinSize();
    struct timeval now;
    gettimeofday(&now, NULL);
    _startTimeSec = now.tv_sec;
    
//    FileUtils::getInstance()->setSearchPaths(searchPath);
    isAutoDraging = false;
    autoMoveTime = 0.5;
    scheduleUpdate();
    auto l = LayerColor::create(Color4B(128, 128, 128, 255));
    addChild(l);
    stageGroup = Node::create();
    stageGroup->retain();
    addChild(stageGroup);
    
    
    auto listener = EventListenerTouchOneByOne::create();
    //    listener->onT
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onToucheBegin, this);
    listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onToucheMove, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onToucheEnd, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    //    string path = FileUtils::getInstance()->fullPathForFilename("x9project");
    int ws = 6;
    int hs = 4;
    
    itemGroup = Node::create();
    stageGroup->addChild(itemGroup);
    Size sceneSize = winSize;
    Vec2 space(sceneSize.width/(ws+1),sceneSize.height/(hs+1));
    Vec2 startPos(space.x,sceneSize.height-space.y);
    vector<string> dir = getFilePathAtVec("x9project");
    _maxListIndex = ceil((float)dir.size()/(ws*hs))-1;
    for (int i = 0; i<dir.size(); i++) {
        int sceneIdx = i/(ws*hs);
        int xIdx = i%ws;
        int yidx = (i/ws)%hs;
        IconNode* icon = IconNode::create(dir[i], this, CC_CALLBACK_1(HelloWorld::gotoGame, this), CC_CALLBACK_3(HelloWorld::onDrag, this));
        icon->setPosition(startPos.x+xIdx*space.x+sceneIdx*sceneSize.width, startPos.y-yidx*space.y);
        itemGroup->addChild(icon);
    }
    _listIndex = 0;
    
    dotSpace = 20;
    dotGroup = Node::create();
    stageGroup->addChild(dotGroup);
    dotGroup->setPositionX(winSize.width/2-(_maxListIndex*dotSpace)/2);
    dotGroup->setPositionY(20);
    dotItem = DrawNode::create();
    dotItem->drawDot(Vec2::ZERO, 5, Color4F::WHITE);
    for (int i = 0; i<=_maxListIndex; i++) {
        DrawNode* _dot = DrawNode::create();
        _dot->drawDot(Vec2::ZERO, 5, Color4F(0.75,0.75,0.75,1));
        _dot->setPositionX(i*dotSpace);
        dotGroup->addChild(_dot);
    }
    dotGroup->addChild(dotItem);
    auto keyListener = EventListenerKeyboard::create();
    keyListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyDown, this);
    x9player = nullptr;
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
    
//    X9TableLoader::load("temp.txt");
    
//
//    
//    char *pData = 0;
//    unsigned long len;
//    Data content = FileUtils::getInstance()->getDataFromFile("temp.txt");
//    pData = (char*)content.getBytes();
//    string str = pData;
//    len = content.getSize();
//    if(pData) {
//        char *m_pData = const_cast<char*>(pData);
//        while(*m_pData != '\0' && *m_pData != '\n') ++m_pData;
//        ++m_pData;
//        log("%d, %s",(int)len,m_pData);
//    }else{
//        log("没找到");
//    }
    
    ui::Scale9Sprite* mc = ui::Scale9Sprite::create("stick_scale9.png", Rect(0,0,128,64));
    mc->setAnchorPoint(Vec2(0.25,0.5));
    addChild(mc);
    
    Vec2 a(480,320);
    Vec2 b = a+Vec2(100,50);
    float thickness = 30;
    Color3B c(255,128,0);
    
    float vr = atan2(b.y-a.y, b.x-a.x)/M_PI*180;
    float vl = sqrt((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
    float ns = thickness/64;
    float nw = (vl+thickness)/ns;
    mc->setScale(ns);
    mc->setPosition(a);
    mc->setContentSize(Size(nw,64));
    mc->setRotation(vr);
    mc->setColor(c);
    
    DrawNode* dnode = DrawNode::create();
    addChild(dnode);
    _bdot = DrawNode::create();
    _bdot->drawPoint(Vec2(), 5, Color4F::BLUE);
    addChild(_bdot);
    
    vector<XBezier> arr;
    Vec2 p1(random(200, 500),random(200, 500));
    for (int i = 0; i<3; i++) {
        Vec2 p2(random(200, 500),random(200, 500));
        Vec2 p3(random(200, 500),random(200, 500));
        arr.push_back(XBezier(p1, p2, p3, true));
        dnode->drawQuadBezier(p1,p2,p3, 20, Color4F::GREEN);
        p1 = p3;
    }
    _blist = new XBezierList(arr);
    return true;
}
float HelloWorld::getTimer()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return (now.tv_sec-_startTimeSec) + now.tv_usec / 1000000.0;
}
void HelloWorld::onKeyDown(EventKeyboard::KeyCode keyCode, Event* event)
{
//    log("%d",(int)keyCode);
    if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE && x9player) {
        removeChild(x9player);
        x9player = nullptr;
        addChild(stageGroup);
    }
}
void HelloWorld::gotoGame(const string& path)
{
    removeChild(stageGroup);
//    string pStr = "x9project/"+path+"/";
    x9player = X9Player::create(path,winSize.width,winSize.height);
//    player->setAppCommand("test", CC_CALLBACK_1(X9Test_script2::test, this));
    addChild(x9player);
}
bool HelloWorld::onToucheBegin(Touch* t, Event* e)
{
    if (!stageGroup->isVisible()) {
        return false;
    }
    onDrag(DRAG_BEGIN,t->getLocation(),getTimer());
    return true;
}
void HelloWorld::onToucheMove(Touch* t, Event* e)
{
    onDrag(DRAG_MOVED,t->getLocation(),getTimer());
//    float _t = _blist->_list[0].getClosest(t->getLocation());
    float _t = _blist->getClosest(t->getLocation());
    _bdot->setPosition(_blist->getPoint(_t));
}
void HelloWorld::onToucheEnd(Touch* t, Event* e)
{
    onDrag(DRAG_ENDED,t->getLocation(),getTimer());
}
void HelloWorld::onDrag(DragType type,const Vec2& p, float time)
{
    switch (type) {
        case DRAG_BEGIN:
            itemGroup->stopAllActions();
            dragSpeeds.clear();
            dragSpeeds.push_back(Vec3(p.x,p.y,time));
            _ox = itemGroup->getPositionX();
            _mx = p.x;
            break;
        case DRAG_MOVED:
        {
            float ax = _ox+p.x-_mx;
            if (ax >= 0) {
                ax = ax/2;
            }else if(ax < -_maxListIndex*winSize.width)
            {
                ax = (ax+_maxListIndex*winSize.width)/2-_maxListIndex*winSize.width;
            }
            itemGroup->setPositionX(ax);
            dragSpeeds.push_back(Vec3(p.x,p.y,time));
            if(dragSpeeds.size() > 4) dragSpeeds.erase(dragSpeeds.begin());
            break;
        }
        case DRAG_ENDED:
            float speed = 0;
            for (int i = 1; i<dragSpeeds.size(); i++) {
                float l = dragSpeeds[i].x-dragSpeeds[i-1].x;
                float t = dragSpeeds[i].z-dragSpeeds[i-1].z;
                speed += l/t;
            }
            speed /= dragSpeeds.size()-1;
            float toLoc = speed*autoMoveTime+itemGroup->getPositionX();
            int idx = MAX(0,MIN(_maxListIndex,-toLoc/winSize.width));
            
            itemGroup->runAction(EaseSineOut::create(MoveTo::create(autoMoveTime, Vec2(-idx*winSize.width,itemGroup->getPositionY()))));
            dotItem->setPositionX(idx*dotSpace);
            break;
    }
}