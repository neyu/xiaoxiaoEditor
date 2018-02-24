//
//  X9KeyFrameScriptNode.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/10/5.
//
//

#include "X9KeyFrameScriptNode.h"
#include "X9MovieClip.h"
#include "X9Array.h"

X9ValueObject* base_keyFrameScriptNode_setAni(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isString(),"setAni Error!!!");
    X9KeyFrameScriptNode* node = dynamic_cast<X9KeyFrameScriptNode*>(target);
    node->ani = X9MovieClip::newMovieClip(target->getLibrary(), values[0]->getString());
    node->addElement(node->ani);
    return nullptr;
}
X9ValueObject* base_keyFrameScriptNode_setKeyFrames(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(!values.empty() && ((int)values.size())%2 == 0,"setKeyFrames Error!!!");
    X9KeyFrameScriptNode* node = dynamic_cast<X9KeyFrameScriptNode*>(target);
    for (int i = 0; i<values.size(); i+=2) {
        X9ASSERT(values[i]->isNumber() && values[i+1]->isString(),"setKeyFrames Error!!!");
        int idx = values[i]->getNumber();
        node->keyFrames[idx] = values[i+1]->getString();
    }
    return nullptr;
}
X9ValueObject* base_keyFrameScriptNode_setLoopCount(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() >= 1 && values.size() <= 2 && values[0]->isNumber(),"setLoopCount Error!!!");
    X9KeyFrameScriptNode* node = dynamic_cast<X9KeyFrameScriptNode*>(target);
    bool isPlay = false;
    if (values.size() == 2) {
        X9ASSERT(values[1]->isBool(),"setLoopCount Error!!!");
        isPlay = values[1]->getBool();
    }
    node->setLoopCount(values[0]->getNumber(), isPlay);
    return nullptr;
}
X9ValueObject* base_keyFrameScriptNode_play(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"play Error!!!");
    X9KeyFrameScriptNode* node = dynamic_cast<X9KeyFrameScriptNode*>(target);
    node->play();
    return nullptr;
}
X9ValueObject* base_keyFrameScriptNode_stop(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"stop Error!!!");
    X9KeyFrameScriptNode* node = dynamic_cast<X9KeyFrameScriptNode*>(target);
    node->stop();
    return nullptr;
}
X9ValueObject* base_keyFrameScriptNode_gotoAndPlay(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"gotoAndPlay Error!!!");
    X9KeyFrameScriptNode* node = dynamic_cast<X9KeyFrameScriptNode*>(target);
    node->gotoAndPlay(values[0]->getNumber()-1);
    return nullptr;
}
X9ValueObject* base_keyFrameScriptNode_gotoAndStop(X9RunObject* target, const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isNumber(),"gotoAndStop Error!!!");
    X9KeyFrameScriptNode* node = dynamic_cast<X9KeyFrameScriptNode*>(target);
    node->gotoAndStop(values[0]->getNumber()-1);
    return nullptr;
}

X9ValueObject* baseGet_keyFrameScriptNode_currentFrame(X9RunObject* target)
{
    X9KeyFrameScriptNode* node = dynamic_cast<X9KeyFrameScriptNode*>(target);
    return X9ValueObject::createWithNumber(node->frameIndex+1);
}
X9ValueObject* baseGet_keyFrameScriptNode_totalFrames(X9RunObject* target)
{
    X9KeyFrameScriptNode* node = dynamic_cast<X9KeyFrameScriptNode*>(target);
    return X9ValueObject::createWithNumber(node->ani->_aniData->totalFrames);
}
X9ValueObject* baseGet_keyFrameScriptNode_loopCount(X9RunObject* target)
{
    X9KeyFrameScriptNode* node = dynamic_cast<X9KeyFrameScriptNode*>(target);
    return X9ValueObject::createWithNumber(node->loopCount);
}
void X9KeyFrameScriptNode::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(keyFrameScriptNode_,setAni);
    x9_AddBaseFunc(keyFrameScriptNode_,setKeyFrames);
    x9_AddBaseFunc(keyFrameScriptNode_,setLoopCount);
    x9_AddBaseFunc(keyFrameScriptNode_,play);
    x9_AddBaseFunc(keyFrameScriptNode_,stop);
    x9_AddBaseFunc(keyFrameScriptNode_,gotoAndPlay);
    x9_AddBaseFunc(keyFrameScriptNode_,gotoAndStop);
    
    
    
    x9_AddBaseGet(keyFrameScriptNode_,currentFrame);
    x9_AddBaseGet(keyFrameScriptNode_,totalFrames);
    x9_AddBaseGet(keyFrameScriptNode_,loopCount);
//    x9_AddBaseGet(scene_,sceneHeight);
//    x9_AddBaseGet(scene_,isLastScene);
//    x9_AddBaseGet(scene_,isFirstScene);
//    
//    x9_AddBaseGet(scene_,isFillScreen);
//    x9_AddBaseSGet(scene_,backgroundColor);
}
void X9KeyFrameScriptNode::setConstValues(X9ScriptClassData* classData)
{
    
}
X9KeyFrameScriptNode* X9KeyFrameScriptNode::create()
{
    CREATE_NEW_AUTO_OBJ(X9KeyFrameScriptNode,"X9KeyFrameScriptNode");
}
X9KeyFrameScriptNode::X9KeyFrameScriptNode(const string& key):X9Node(key)
{
    ani = nullptr;
    time = 0;
    frameIndex = 0;
    isFirstPlay = true;
    loopCount = 0;
    loopIndex = 0;
    isPlaying = false;
    x9_setCtor(KeyFrameScriptNode);
}

void X9KeyFrameScriptNode::removed()
{
    keyFrames.clear();
    ani = nullptr;
    time = 0;
    frameIndex = 0;
    isFirstPlay = true;
    loopCount = 0;
    loopIndex = 0;
    isPlaying = false;
    X9Node::removed();
}
void X9KeyFrameScriptNode::initObject(const vector<X9ValueObject*>& vs)
{
    runSuperCtor("Node",vs);
    X9ASSERT(vs.empty(),"new KeyFrameScriptNode Error!!!");
}
void X9KeyFrameScriptNode::initPlay()
{
    if(isFirstPlay) {
        time = 0;
        frameIndex = 0;
        loopIndex = 0;
    }
}
void X9KeyFrameScriptNode::gotoAndPlay(int index)
{
    play();
    index = MAX(0,MIN(ani->_aniData->totalFrames-1,index));
    time = index*ani->_aniData->frameTime;
    setFrameIndex(index);
}
void X9KeyFrameScriptNode::gotoAndStop(int index)
{
    stop();
    index = MAX(0,MIN(ani->_aniData->totalFrames-1,index));
    time = index*ani->_aniData->frameTime;
    setFrameIndex(index);
}
void X9KeyFrameScriptNode::play()
{
    if(!isPlaying)
    {
        isPlaying = true;
        initPlay();
        startUpdate();
    }
}
void X9KeyFrameScriptNode::stop()
{
    if(isPlaying)
    {
        isPlaying = false;
        stopUpdate();
    }
}
void X9KeyFrameScriptNode::setLoopCount(int loopCount, bool isPlay)
{
    stop();
    time = 0;
    frameIndex = 0;
    this->loopCount = loopCount;
    loopIndex = 0;
    isFirstPlay = true;
    if(isPlay)
    {
        play();
    }
}
void X9KeyFrameScriptNode::update(float delay)
{
    vector<float> delays;
    if (delay >= ani->_aniData->frameTime) {
        if (delay == ani->_aniData->frameTime) {
            delays.push_back(delay/2);
            delays.push_back(delay/2);
        }else{
            int count = ceil(delay/ani->_aniData->frameTime);
            for (int i = 0; i<count; i++) {
                delays.push_back(delay/count);
            }
        }
    }else{
        delays.push_back(delay);
    }
    for (int i = 0; i<delays.size(); i++) {
        float d = delays[i];
        X9Node::update(d);
        time += d;
        int oldFIdx = frameIndex;
        int _frameIndex = time/ani->_aniData->frameTime;
        if (_frameIndex >= ani->_aniData->totalFrames) {
            if (loopCount > 0) {
                loopIndex++;
                if (loopIndex >= loopCount) {
                    setFrameIndex(ani->_aniData->totalFrames-1);
                    stop();
                    isFirstPlay = true;
                    return;
                }
            }
            frameIndex = 0;
            time -= ani->_aniData->totalFrames*ani->_aniData->frameTime;
        }
        if(oldFIdx != _frameIndex)
        {
            setFrameIndex(_frameIndex);
        }
    }
}
void X9KeyFrameScriptNode::setFrameIndex(int idx)
{
    frameIndex = idx;
    ani->gotoAndStop(frameIndex);
    if (keyFrames.find(frameIndex) != keyFrames.end()) {
        runFunction(MemberType::MT_FUNCTION, keyFrames[frameIndex],_emptyValues,getClassData());
    }
}