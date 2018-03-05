//
//  X9MovieClip.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/20.
//
//

#include "X9MovieClip.h"
#include "X9Player.h"
#include "../X9TargetManager.h"
#include "../X9SoundPlayer.h"
#include "event/X9MovieClipEvent.h"
#include "X9Array.h"
#include "X9Color.h"
#include "X9Point.h"
#include "X9Vec3.h"
#include "X9AniPosFrame.h"

X9ValueObject* base_ani_setPlayLoop(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() > 0 && values.size() <= 4 && values[0]->isNumber(),"setPlayLoop() Error!!!");
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    int loopCount = values[0]->getNumber();
    int stIdx = 0;
    int endIdx = -1;
    bool isPlay = false;
//    if (values.size() == 1) {
//        ani->setPlayLoop(values[0]->getNumber());
//    }
    if(values.size() >= 2)
    {
        X9ASSERT(values[1]->isNumber(),"setPlayLoop() Error!!!");
        stIdx = values[1]->getNumber()-1;
    }
    if(values.size() >= 3)
    {
        X9ASSERT(values[2]->isNumber(),"setPlayLoop() Error!!!");
        endIdx = values[2]->getNumber();
    }
    if(values.size() == 4)
    {
        X9ASSERT(values[3]->isBool(),"setPlayLoop() Error!!!");
        isPlay = values[3]->getBool();
    }
    ani->setPlayLoop(loopCount,stIdx,endIdx,isPlay);
    return nullptr;
}
X9ValueObject* base_ani_play(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"play() Error!!!");
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    ani->play();
    return nullptr;
}
X9ValueObject* base_ani_stop(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"stop() Error!!!");
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    ani->stop();
    return nullptr;
}
X9ValueObject* base_ani_gotoAndPlay(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 || values.size() == 2,"gotoAndPlay() Error!!!");
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    bool isUpend = false;
    if (values.size() == 2) {
        X9ASSERT(values[1]->isBool(),"gotoAndPlay() Error!!!");
        isUpend = values[1]->getBool();
    }
    if (values[0]->isString()) {
        ani->gotoAndPlay(values[0]->getString(), isUpend);
    }else{
        X9ASSERT(values[0]->isNumber(),"gotoAndPlay() Error!!!");
        ani->gotoAndPlay(values[0]->getNumber()-1, isUpend);
    }
    return nullptr;
}
X9ValueObject* base_ani_gotoAndStop(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1,"gotoAndStop() Error!!!");
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    if (values[0]->isString()) {
        ani->gotoAndStop(values[0]->getString());
    }else{
        ani->gotoAndStop(values[0]->getNumber()-1);
    }
    return nullptr;
}
X9ValueObject* base_ani_nextFrame(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"nextFrame() Error!!!");
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    ani->nextFrame();
    return nullptr;
}
X9ValueObject* base_ani_prevFrame(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.empty(),"nextFrame() Error!!!");
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    ani->prevFrame();
    return nullptr;
}
X9ValueObject* base_ani_getIsPosFrame(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 1 && values[0]->isString(),"getIsPosFrame() Error!!!");
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    return X9ValueObject::createWithBool(ani->_aniFrame->getIsPosFrame(values[0]->getString()));
}
X9ValueObject* base_ani_getPosFrame(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->isString() && values[1]->isObject<X9DisplayObject*>(),"getPosFrame() Error!!!");
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    X9DisplayObject* mc = values[1]->getObject<X9DisplayObject*>();
    if (!ani->getIsInStage() || !mc->getIsInStage()) {
        return X9ValueObject::create();
    }
    X9PosFrame pf = ani->_aniFrame->getPosFrame(values[0]->getString());
    if(pf.isEmpty)
    {
        return X9ValueObject::create();
    }
    X9AniPosFrame* apf = X9AniPosFrame::newAniPosFrame(target->getLibrary(), &pf);
    apf->a = mc->getNode()->convertToNodeSpace(apf->a);
    apf->b = mc->getNode()->convertToNodeSpace(apf->b);
    apf->r = atan2(apf->a.y-apf->b.y, apf->b.x-apf->a.x);
    return X9ValueObject::createWithObject(apf);
}
X9ValueObject* base_ani_setStick(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() == 2 && values[0]->isObject<X9Color*>() && values[1]->isNumber(),"setStick() Error!!!");
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    ani->_aniFrame->setStick(values[0]->getObject<X9Color*>()->_color, values[1]->getNumber());
    return X9ValueObject::create();
}
X9ValueObject* base_ani_setAniName(X9RunObject* target,const vector<X9ValueObject*>& values)
{
    X9ASSERT(values.size() >= 1 && values.size() <= 3 && values[0]->isString(),"setAniName() Error!!!");
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    Color3B color = Color3B::BLACK;
    float stickThickness = 10;
    ani->setAniName(values[0]->getString(), color, stickThickness);
    return X9ValueObject::create();
}



//void baseSet_ani_name(X9RunObject* target, X9ValueObject* value)
//{
//    X9ASSERT(value->isString(),"set name Error!!!");
//    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
//    ani->setAniName(value->getString(), Color3B::BLACK, 10);
//}
//X9ValueObject* baseGet_ani_name(X9RunObject* target)
//{
//    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
//    return X9ValueObject::createWithString(ani->_aniName);
//}
X9ValueObject* baseGet_ani_isPlaying(X9RunObject* target)
{
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    return X9ValueObject::createWithBool(ani->isPlaying());
}
X9ValueObject* baseGet_ani_playLoop(X9RunObject* target)
{
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    return X9ValueObject::createWithNumber(ani->_playLoop);
}
X9ValueObject* baseGet_ani_startFrame(X9RunObject* target)
{
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    return X9ValueObject::createWithNumber(ani->_startIndex+1);
}
X9ValueObject* baseGet_ani_endFrame(X9RunObject* target)
{
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    return X9ValueObject::createWithNumber(ani->_endIndex);
}
X9ValueObject* baseGet_ani_time(X9RunObject* target)
{
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    return X9ValueObject::createWithNumber(ani->_playTimeLength);
}
X9ValueObject* baseGet_ani_frameIndex(X9RunObject* target)
{
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    return X9ValueObject::createWithNumber(ani->_aniFrame->getFrameIndex()+1);
}
X9ValueObject* baseGet_ani_totalFrames(X9RunObject* target)
{
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    return X9ValueObject::createWithNumber(ani->_aniData->totalFrames);
}
X9ValueObject* baseGet_ani_frameRate(X9RunObject* target)
{
    X9MovieClip* ani = dynamic_cast<X9MovieClip*>(target);
    return X9ValueObject::createWithNumber(ani->_aniData->frameRate);
}
//string X9MovieClip::ENTER_FRAME = "enterFrame";
//string X9MovieClip::ENTER_LABEL = "enterLabel";
//string X9MovieClip::PLAY_COMPLETE = "playComplete";
void X9MovieClip::setBaseFunctions(X9Library* library, const string& className)
{
    x9_AddBaseFunc(ani_,setPlayLoop);
    x9_AddBaseFunc(ani_,play);
    x9_AddBaseFunc(ani_,stop);
    x9_AddBaseFunc(ani_,gotoAndPlay);
    x9_AddBaseFunc(ani_,gotoAndStop);
    x9_AddBaseFunc(ani_,nextFrame);
    x9_AddBaseFunc(ani_,prevFrame);
    x9_AddBaseFunc(ani_,getIsPosFrame);
    x9_AddBaseFunc(ani_,getPosFrame);
    x9_AddBaseFunc(ani_,setStick);
    x9_AddBaseFunc(ani_,setAniName);
    
//    x9_AddBaseSGet(ani_,name);
    x9_AddBaseGet(ani_,isPlaying);
    x9_AddBaseGet(ani_,playLoop);
    x9_AddBaseGet(ani_,startFrame);
    x9_AddBaseGet(ani_,endFrame);
    x9_AddBaseGet(ani_,time);
    x9_AddBaseGet(ani_,frameIndex);
    x9_AddBaseGet(ani_,totalFrames);
    x9_AddBaseGet(ani_,frameRate);
}
void X9MovieClip::setConstValues(X9ScriptClassData* classData)
{
//    x9_AddConstValue(String, ENTER_FRAME, ENTER_FRAME);
//    x9_AddConstValue(String, ENTER_LABEL, ENTER_LABEL);
//    x9_AddConstValue(String, PLAY_COMPLETE, PLAY_COMPLETE);
}
X9MovieClip* X9MovieClip::newMovieClip(X9Library* library, const string& name,int loop, int stIdx, int edIdx)
{
    vector<X9ValueObject*> values;
    values.push_back(X9ValueObject::createWithString(name));
    values.push_back(X9ValueObject::createWithNumber(loop));
    values.push_back(X9ValueObject::createWithNumber(stIdx));
    values.push_back(X9ValueObject::createWithNumber(edIdx));
    return dynamic_cast<X9MovieClip*>(library->createObject("MovieClip", values));
}
X9MovieClip* X9MovieClip::create()
{
    CREATE_NEW_AUTO_OBJ(X9MovieClip,);
}
X9MovieClip::X9MovieClip():X9DisplayObject("X9MovieClip")
{
    _aniFrame = nullptr;
    _aniData = nullptr;
    _aniName = "";
    _isPlaying = false;
    _isPlayStart = false;
    _playLoop = 0;
    _playLoopNum = 0;
    _startIndex = 0;
    _endIndex = -1;
    _playFrames = 0;
    _playTime = 0;
    _playTimeLength = 0;
    _frameTime = 0;
    x9_setCtor(MovieClip);
}
void X9MovieClip::removed()
{
    if(_aniFrame)
    {
        _aniFrame->nonuse();
    }
    _aniFrame = nullptr;
    _aniData = nullptr;
    _aniName = "";
    _isPlaying = false;
    _isPlayStart = false;
    _playLoop = 0;
    _playLoopNum = 0;
    _startIndex = 0;
    _endIndex = -1;
    _playFrames = 0;
    _playTime = 0;
    _playTimeLength = 0;
    _frameTime = 0;
    X9DisplayObject::removed();
}
/**
 * @class:MovieClip
 * @param:string animation name
 * @param:number? loop
 * @param:number? startIndex
 * @param:number? endIndex
 *
 */
void X9MovieClip::initObject(const vector<X9ValueObject*>& vs)
{
    X9ASSERT(vs.size() >= 1 && vs[0]->isString() && vs[0]->getString() != "","new X9MovieClip initObject Error!!!");
    _aniName = vs[0]->getString();
    _aniData = X9FLAnimationLibrary::getInstance()->getAniData(_aniName);
    _frameTime = _aniData->frameTime;
    _playLoop = 0;
    _startIndex = 0;
    _endIndex = _aniData->totalFrames;
    _stickColor = Color3B::BLACK;
    _stickThickness = 10;
    if (vs.size() > 1) {
        X9ASSERT(vs[1]->isNumber(),"new X9MovieClip initObject Error!!!");
        _playLoop = MAX(0,vs[1]->getNumber());
    }
    if (vs.size() > 2) {
        X9ASSERT(vs[2]->isNumber(),"new X9MovieClip initObject Error!!!");
        _startIndex = MAX(0,MIN(_aniData->totalFrames-1,vs[2]->getNumber()-1));
    }
    if (vs.size() > 3) {
        X9ASSERT(vs[3]->isNumber(),"new X9MovieClip initObject Error!!!");
        if (vs[3]->getNumber() > 0) {
            _endIndex = vs[3]->getNumber();
        }
        _endIndex = MAX(_startIndex+1,_endIndex);
    }
    _playFrames = _endIndex-_startIndex;
    _playTimeLength = _aniData->time;
    
    _aniFrame = X9FLAnimationFrame::create(_aniName, _stickColor, _stickThickness, _startIndex);
    _aniFrame->use();
    _node = _aniFrame->getNode();
    //_label->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
}
void X9MovieClip::addedToStage()
{
    X9DisplayObject::addedToStage();
    if (_isPlaying) {
        getPlayer()->targetManager->addUpdateTarget(this);
    }
}
void X9MovieClip::removedFromStage()
{
    X9DisplayObject::removedFromStage();
    if (_isPlaying) {
        getPlayer()->targetManager->removeUpdateTarget(this);
    }
}


void X9MovieClip::update(float delay)
{
    _playTime += delay;
    bool __checkPlayComplete = false;
    if (_playTime >= _playTimeLength) {
        __checkPlayComplete = true;
        int N = _playTime/_playTimeLength;
        _playTime -= N*_playTimeLength;
        if(_playLoop > 0) {
            _playLoopNum -= N;
            if (_playLoopNum <= 1) {
                _isPlayStart = false;
                _isPlaying = false;
                getPlayer()->targetManager->removeUpdateTarget(this);
                X9Event* completeEvent = X9MovieClipEvent::newMovieClipEvent(getLibrary(),X9MovieClipEvent::PLAY_COMPLETE,_endIndex,_playLoop,_playLoop);
                dispatchEvent(completeEvent);
                //complete
                return;
            }
        }
    }
    int cidx = _playTime/_frameTime+_startIndex;
    //log("%d, %d, %d",cidx,_playLoop,_playLoopNum);
    if (_aniFrame->getFrameIndex() != cidx) {
        _aniFrame->setFrameIndex(cidx);
        int num = delay/_frameTime;
        int idx = _aniFrame->getFrameIndex()-num;
        for (int i = idx; i<=_aniFrame->getFrameIndex(); i++) {
            int _i = i;
            if (_i < _startIndex) {
                _i+=_playFrames;
            }
            playSound(_i);
            X9Event* event = X9MovieClipEvent::newMovieClipEvent(getLibrary(),X9MovieClipEvent::ENTER_FRAME,_i+1,_playLoop,_playLoopNum);
            dispatchEvent(event);
            vector<string> ls;
            if (_aniFrame->_isUpend) {
                ls = _aniFrame->getFrameLabels(_aniData->totalFrames-_i-1);
            }else
            {
                ls = _aniFrame->getFrameLabels(_i);
            }
            if (!ls.empty()) {
                for (int k = 0; k<ls.size(); k++) {
                    X9Event* labelEvent = X9MovieClipEvent::newMovieClipEvent(getLibrary(),X9MovieClipEvent::ENTER_LABEL,_i+1,_playLoop,_playLoop-_playLoopNum+1,ls[k]);
                    dispatchEvent(labelEvent);
                }
            }
//            if(_aniData->labelIndexs.find(_i) != _aniData->labelIndexs.end())
//            {
//                for (int k = 0; k<_aniData->labelIndexs[_i].size(); k++) {
//                    X9Event* labelEvent = X9MovieClipEvent::newMovieClipEvent(getLibrary(),X9MovieClipEvent::ENTER_LABEL,_i+1,_playLoop,_playLoop-_playLoopNum+1,_aniData->labelIndexs[_i][k]);
//                    dispatchEvent(labelEvent);
//                }
//            }
        }
    }
}
void X9MovieClip::setAniName(const string& name, const Color3B& stickColor, float stickThickness)
{
    if (_isPlaying) {
        stop();
    }
    _aniName = name;
    _aniData = X9FLAnimationLibrary::getInstance()->getAniData(_aniName);
    _frameTime = _aniData->frameTime;
    _playLoop = 0;
    _startIndex = 0;
    _endIndex = _aniData->totalFrames;
    _aniFrame->setAni(_aniName,stickColor,stickThickness);
}
void X9MovieClip::setPlayLoop(int loop, int start, int end, bool isPlay)
{
    if (_isPlaying) {
        stop();
    }
    _isPlayStart = false;
    _playLoop = loop;
    if(end < 0) end = _aniData->totalFrames;
    _startIndex = MAX(0,MIN(_aniData->totalFrames-1,start));
    _endIndex = MAX(_startIndex+1,MIN(_aniData->totalFrames,end));
    _playFrames = _endIndex-_startIndex;
    _playTimeLength = _playFrames*_aniData->frameTime;
    if (isPlay) {
        play();
    }
}
void X9MovieClip::playSound(int idx)
{
    if (_aniFrame && _aniData) {
        if(_aniFrame->_isUpend)
        {
            idx = _aniData->totalFrames-idx-1;
        }
        if(_aniData->sounds.find(idx) != _aniData->sounds.end()) {
            for (int i = 0; i<_aniData->sounds[idx].size(); i++) {
                const string& soundName = _aniData->sounds[idx][i];
                getLibrary()->soundPlayer->playEffect(soundName,true);
//                SimpleAudioEngine::getInstance()->
            }
        }
    }
}
void X9MovieClip::play()
{
    if (_isPlaying) return;
    _isPlaying = true;
    if (!_isPlayStart) {
        _isPlayStart = true;
        playSound(_aniFrame->getFrameIndex());
        _playTime = 0;
        if (_playLoop > 0) {
            _playLoopNum = _playLoop;
        }
    }
    if (_isInStage) {
        getPlayer()->targetManager->addUpdateTarget(this);
    }
}
void X9MovieClip::stop()
{
    if (!_isPlaying) return;
    _isPlaying = false;
    if (_isInStage) {
        getPlayer()->targetManager->removeUpdateTarget(this);
    }
}
void X9MovieClip::gotoAndPlay(int frameIndex, bool upend)
{
    _aniFrame->_isUpend = upend;
    frameIndex = MAX(_startIndex,MIN(_endIndex-1,frameIndex));
    _playTime = (frameIndex-_startIndex)*_frameTime;
    _aniFrame->setFrameIndex(_playTime/_frameTime+_startIndex);
    if (!_isPlaying) {
        _isPlaying = true;
        if (!_isPlayStart) {
            _isPlayStart = true;
//            _playTime = 0;
            if (_playLoop > 0) {
                _playLoopNum = _playLoop;
            }
        }
        if (_isInStage) {
            getPlayer()->targetManager->addUpdateTarget(this);
        }
    }
}
void X9MovieClip::gotoAndPlay(const string& frameLabel, bool upend)
{
    if(_aniData->labels.find(frameLabel) == _aniData->labels.end()) {
        gotoAndPlay(_startIndex, upend);
    }else{
        gotoAndPlay(_aniData->labels[frameLabel], upend);
    }
}
void X9MovieClip::gotoAndStop(int frameIndex)
{
    frameIndex = MAX(_startIndex,MIN(_endIndex-1,frameIndex));
    _playTime = (frameIndex-_startIndex)*_frameTime;
    _aniFrame->setFrameIndex(_playTime/_frameTime+_startIndex);
    if (_isPlaying) {
        _isPlaying = false;
        if (_isInStage) {
            getPlayer()->targetManager->removeUpdateTarget(this);
        }
    }
}
void X9MovieClip::gotoAndStop(const string& frameLabel)
{
    if(_aniData->labels.find(frameLabel) == _aniData->labels.end()) {
        gotoAndStop(_startIndex);
    }else{
        gotoAndStop(_aniData->labels[frameLabel]);
    }
}
void X9MovieClip::nextFrame()
{
    if (_isPlaying) {
        gotoAndPlay(_aniFrame->getFrameIndex()+1);
    }else{
        gotoAndStop(_aniFrame->getFrameIndex()+1);
    }
}
void X9MovieClip::prevFrame()
{
    if (_isPlaying) {
        gotoAndPlay(_aniFrame->getFrameIndex()-1);
    }else{
        gotoAndStop(_aniFrame->getFrameIndex()-1);
    }
}