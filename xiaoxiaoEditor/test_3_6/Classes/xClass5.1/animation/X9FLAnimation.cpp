//
//  X9FLAnimation.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/14.
//
//

#include "X9FLAnimation.h"

//MARK:X9FLBaseAnimationElement
X9FLBaseAnimationElement::X9FLBaseAnimationElement(const string& key):X9AutoObject(key)
{
    _elementData = nullptr;
}
void X9FLBaseAnimationElement::removed()
{
    _elementData = nullptr;
//    _selfNode = nullptr;
    X9AutoObject::removed();
}
void X9FLBaseAnimationElement::setElementData(X9FLBaseEmtData* baseElementData,int showFrameIndex,bool isAdd, const Color3B& stickCOlor, float stickThickness)
{
    X9FLAniElementData* elementData = dynamic_cast<X9FLAniElementData*>(baseElementData);
    if (elementData) {
        _selfNode->setPosition(elementData->x, elementData->y);
        _selfNode->setScale(elementData->scaleX, elementData->scaleY);
        _selfNode->setRotationSkewX(elementData->rotationSkewX);
        _selfNode->setRotationSkewY(elementData->rotationSkewY);
        _selfNode->setColor(Color3B(elementData->color));
        _selfNode->setOpacity(elementData->color.a);
        Sprite* spr = dynamic_cast<Sprite*>(_selfNode);
        if (spr) {
            if (elementData->isAddBlend || isAdd) {
                spr->setBlendFunc(BlendFunc::ADDITIVE);
            }else{
                spr->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
            }
        }
    }
}
//MARK:X9FLAnimationStickElement
vector<ui::Scale9Sprite*> X9FLAnimationStickElement::_stickPools;
X9FLAnimationStickElement* X9FLAnimationStickElement::create()
{
    CREATE_NEW_AUTO_OBJ(X9FLAnimationStickElement,);
}
X9FLAnimationStickElement* X9FLAnimationStickElement::create(X9FLBaseEmtData *data, const Color3B& stickCOlor, float stickThickness)
{
    X9FLAnimationStickElement* mc = create();
    mc->setElementData(data, 0, false,stickCOlor,stickThickness);
    return mc;
}
X9FLAnimationStickElement::X9FLAnimationStickElement():X9FLBaseAnimationElement("X9FLAnimationStickElement")
{
    _selfNode = this;
    init();
}
void X9FLAnimationStickElement::removed()
{
    for (int i = 0; i<getChildren().size(); i++) {
        _stickPools.push_back(dynamic_cast<ui::Scale9Sprite*>(getChildren().at(i)));
    }
    removeAllChildren();
    X9FLBaseAnimationElement::removed();
}
void X9FLAnimationStickElement::setElementData(X9FLBaseEmtData* elementData,int showFrameIndex,bool isAdd, const Color3B& stickCOlor, float stickThickness)
{
    X9FLStickData* stickData = dynamic_cast<X9FLStickData*>(elementData);
    resetChildCount(stickData->points.size());
    for (int i = 0; i < getChildren().size(); i++) {
        ui::Scale9Sprite* mc = dynamic_cast<ui::Scale9Sprite*>(getChildren().at(i));
        X9FLStickData::Vec22& v = stickData->points[i];
        float vr = atan2(v.a.y-v.b.y, v.b.x-v.a.x)/M_PI*180;
        float vl = sqrt((v.a.x-v.b.x)*(v.a.x-v.b.x)+(v.a.y-v.b.y)*(v.a.y-v.b.y));
        float ns = stickThickness/64;
        float nw = (vl+stickThickness)/ns;
        mc->setPosition(v.a);
        mc->setScale(ns);
        mc->setContentSize(Size(nw,64));
        mc->setAnchorPoint(Vec2(32/nw,0.5));
        mc->setRotation(vr);
        mc->setColor(stickCOlor);
    }
}
void X9FLAnimationStickElement::resetChildCount(int stickCount)
{
    if (getChildrenCount() < stickCount) {
        for (int i = getChildrenCount(); i<stickCount; i++) {
            if (_stickPools.empty()) {
                ui::Scale9Sprite* mc = ui::Scale9Sprite::create("stick_scale9.png", Rect(0,0,128,64));
                mc->retain();
                addChild(mc);
            }else{
                ui::Scale9Sprite* mc = _stickPools.back();
                _stickPools.erase(_stickPools.end()-1);
                addChild(mc);
            }
        }
    }
}

//MARK:X9FLAnimationHeadElement
X9FLAnimationHeadElement* X9FLAnimationHeadElement::create()
{
    CREATE_NEW_AUTO_OBJ(X9FLAnimationHeadElement,);
}
X9FLAnimationHeadElement* X9FLAnimationHeadElement::create(X9FLBaseEmtData* data, const Color3B& stickCOlor, float stickThickness)
{
    X9FLAnimationHeadElement* mc = create();
    mc->setElementData(data, 0, false, stickCOlor, stickThickness);
    return mc;
}
X9FLAnimationHeadElement::X9FLAnimationHeadElement():X9FLBaseAnimationElement("X9FLAnimationHeadElement"),Sprite()
{
    _selfNode = this;
    CCASSERT(initWithFile("stick_head.png"),"new X9FLAnimationHeadElement Error!!!");
}
void X9FLAnimationHeadElement::removed()
{
    X9FLBaseAnimationElement::removed();
}
void X9FLAnimationHeadElement::setElementData(X9FLBaseEmtData* elementData,int showFrameIndex,bool isAdd, const Color3B& stickCOlor, float stickThickness)
{
    X9FLHeadData* headData = dynamic_cast<X9FLHeadData*>(elementData);
    setPosition(headData->x,headData->y);
    const float s = stickThickness*3/128;
    setScale(headData->scaleX*s, headData->scaleY*s);
    setRotationSkewX(headData->rotationSkewX);
    setRotationSkewY(headData->rotationSkewY);
    setColor(stickCOlor);
}

//MARK:X9FLAnimationImageElement
X9FLAnimationImageElement* X9FLAnimationImageElement::create()
{
    CREATE_NEW_AUTO_OBJ(X9FLAnimationImageElement,);
}
X9FLBaseAnimationElement* X9FLAnimationImageElement::create(X9FLBaseEmtData* data,int showFrameIndex,bool isAdd)
{
    X9FLBaseAnimationElement* mc = create();
    mc->setElementData(data,showFrameIndex,isAdd,Color3B::BLACK,0);
    return mc;
}
X9FLAnimationImageElement::X9FLAnimationImageElement():X9FLBaseAnimationElement("X9FLAnimationImageElement"),Sprite()
{
    _selfNode = this;
    init();
}
void X9FLAnimationImageElement::removed()
{
    X9FLBaseAnimationElement::removed();
}
void X9FLAnimationImageElement::setElementData(X9FLBaseEmtData* baseElementData,int showFrameIndex,bool isAdd, const Color3B& stickCOlor, float stickThickness)
{
    X9FLAniElementData* elementData = dynamic_cast<X9FLAniElementData*>(baseElementData);
    if(elementData)
    {
        this->setTexture(elementData->sourceURI);
//        if(elementData->sourceType == X9FLAniElementData::ST_IMAGE) {
//            this->setTexture(elementData->sourceURI);
//        }else{
//            this->setSpriteFrame(elementData->sourceName);
//        }
        this->setAnchorPoint(Vec2::UNIT_Y);
        X9FLBaseAnimationElement::setElementData(elementData,showFrameIndex,isAdd,stickCOlor,stickThickness);
    }
}

//MARK:X9FLAnimationMaskElement
X9FLAnimationMaskElement* X9FLAnimationMaskElement::create()
{
    CREATE_NEW_AUTO_OBJ(X9FLAnimationMaskElement,);
}
X9FLAnimationMaskElement::X9FLAnimationMaskElement():X9AutoObject("X9FLAnimationMaskElement"),ClippingNode()
{
    init();
    _selfNode = this;
    _group = Node::create();
    setStencil(_group);
}
void X9FLAnimationMaskElement::removed()
{
    X9AutoObject::removed();
}
void X9FLAnimationMaskElement::addMask(X9FLBaseAnimationElement* element)
{
    _group->addChild(element->getNode());
}

//MARK:X9FLAnimationFrame
X9FLAnimationFrame* X9FLAnimationFrame::create()
{
    CREATE_NEW_AUTO_OBJ(X9FLAnimationFrame,);
    
}
X9FLBaseAnimationElement* X9FLAnimationFrame::create(X9FLBaseEmtData* data,int showFrameIndex,bool isAdd, const Color3B& stickCOlor, float stickThickness)
{
    X9FLBaseAnimationElement* mc = create();
    mc->setElementData(data,showFrameIndex,isAdd,stickCOlor,stickThickness);
    return mc;
}
X9FLAnimationFrame* X9FLAnimationFrame::create(const string& aniName, const Color3B& stickCOlor, float stickThickness,int index)
{
    X9FLAnimationFrame* mc = create();
    mc->setAni(aniName,stickCOlor,stickThickness,index);
    return mc;
}
X9FLAnimationFrame::X9FLAnimationFrame(const string& key):X9FLBaseAnimationElement(key)
{
    init();
    this->setCascadeColorEnabled(true);
    this->setCascadeOpacityEnabled(true);
    _selfNode = this;
    _data = nullptr;
    _isAdd = false;
    _frameIndex = -1;
    _isUpend = false;
}
void X9FLAnimationFrame::removed()
{
    clear();
    _isAdd = false;
    _frameIndex = -1;
    _isUpend = false;
    _data = nullptr;
    setPosition(0, 0);
    setScale(1, 1);
    setOpacity(255);
    setColor(Color3B::WHITE);
    setRotationSkewX(0);
    setRotationSkewY(0);
    setVisible(true);
    X9FLBaseAnimationElement::removed();
}
void X9FLAnimationFrame::showFrame()
{
    clear();
    X9FLAnimationMaskElement* _mask = nullptr;
    vector<X9FLBaseAnimationElement*> _maskeds;
    for (auto layerData : _data->layers)
    {
        for (auto baseElementData : layerData->frames[_frameIndex]->elements)
        {
            X9FLBaseAnimationElement* _element;
            if (baseElementData->sourceType == X9FLBaseEmtData::ST_STICK) {
                _element = X9FLAnimationStickElement::create(baseElementData,_stickCOlor,_stickThickness);
            }else if(baseElementData->sourceType == X9FLBaseEmtData::ST_HEAD) {
                _element = X9FLAnimationHeadElement::create(baseElementData, _stickCOlor, _stickThickness);
            }else{
                X9FLAniElementData* elementData = dynamic_cast<X9FLAniElementData*>(baseElementData);
                if (elementData->sourceType == X9FLAniElementData::ST_ANI) {
                    _element = X9FLAnimationFrame::create(elementData,_frameIndex,_isAdd,_stickCOlor,_stickThickness);
                }else{
                    _element = X9FLAnimationImageElement::create(elementData,_frameIndex,_isAdd);
                }
            }
            _elements.push_back(_element);
            _element->use();
//            addChild(_element->getNode());
            switch (layerData->layerType) {
                case X9FLLayerData::LT_NORMAL:
                    addChild(_element->getNode());
                    _mask = nullptr;
                    if(!_maskeds.empty()) _maskeds.clear();
                    break;
                case X9FLLayerData::LT_MASK:
                    if (_mask == nullptr) {
                        _mask = X9FLAnimationMaskElement::create();
                        _mask->setAlphaThreshold(layerData->maskNum);
                        addChild(_mask->getNode());
                        _mask->use();
                        _masks.push_back(_mask);
                        for (auto e : _maskeds) {
                            _mask->addChild(e->getNode());
                        }
                    }
                    _mask->addMask(_element);
                    break;
                case X9FLLayerData::LT_MASKED:
                    _maskeds.push_back(_element);
                    break;
            }
        }
    }
}
void X9FLAnimationFrame::clear()
{
    for (auto element : _elements) {
        element->getNode()->getParent()->removeChild(element->getNode());
        element->nonuse();
    }
    _elements.clear();
    for (auto mask : _masks)
    {
        removeChild(mask->getNode());
        mask->nonuse();
    }
    _masks.clear();
}
void X9FLAnimationFrame::setElementData(X9FLBaseEmtData* baseElementData,int showFrameIndex, bool isAdd, const Color3B& stickCOlor, float stickThickness)
{
    X9FLAniElementData* elementData = dynamic_cast<X9FLAniElementData*>(baseElementData);
    int f = showFrameIndex-elementData->parentData->startFrame;
    this->_isAdd = isAdd || elementData->isAddBlend;
    X9FLAnimationData* aniData = elementData->parentData->parentData->parentData->library->getAniData(elementData->sourceName);
    int idx = elementData->firstFrame;
    switch (elementData->loopType) {
        case X9FLAniElementData::LT_SINGLE_FRAME:
            break;
        case X9FLAniElementData::LT_PLAYER_ONCE:
            idx = MIN(elementData->firstFrame+f,aniData->totalFrames);
            break;
        case X9FLAniElementData::LT_LOOP:
            idx = (elementData->firstFrame+f)%aniData->totalFrames;
            break;
    }
    setAni(elementData->sourceName,stickCOlor,stickThickness,idx);
    X9FLBaseAnimationElement::setElementData(elementData,showFrameIndex, isAdd,stickCOlor,stickThickness);
}
void X9FLAnimationFrame::setAni(const string& name, const Color3B& stickCOlor, float stickThickness, int frameIndex)
{
    CCASSERT(name != "","setAni Error!!!");
    bool isNewData = false;
    if (_data == nullptr || _data->name != name) {
        isNewData = true;
        _data = X9FLAnimationLibrary::getInstance()->getAniData(name);
    }
    CCASSERT(frameIndex >= 0 && frameIndex < _data->totalFrames,"setFrameIndex Error!!!");
    _stickCOlor = stickCOlor;
    _stickThickness = stickThickness;
    _data->addSpriteFrames();
    if (frameIndex != _frameIndex || isNewData) {
        _frameIndex = frameIndex;
        showFrame();
    }
}
void X9FLAnimationFrame::setAni(X9FLAnimationData* data, const Color3B& stickCOlor, float stickThickness, int frameIndex)
{
    CCASSERT(data,"setAni Error!!!");
    bool isNewData = _data != data;
    _data = data;
    CCASSERT(frameIndex >= 0 && frameIndex < _data->totalFrames,"setFrameIndex Error!!!");
    _stickCOlor = stickCOlor;
    _stickThickness = stickThickness;
    _data->addSpriteFrames();
    if (frameIndex != _frameIndex || isNewData) {
        _frameIndex = frameIndex;
        showFrame();
    }
}
bool X9FLAnimationFrame::setFrameIndex(int index)
{
    CCASSERT(_data,"setFrameIndex Error!!!");
    CCASSERT(index >= 0 && index < _data->totalFrames,"setFrameIndex Error!!!");
    if (_isUpend) {
        index = _data->totalFrames-index-1;
    }
    if (index != _frameIndex) {
        _frameIndex = index;
        showFrame();
        return true;
    }
    return false;
}
bool X9FLAnimationFrame::setFrameTime(float time)
{
    CCASSERT(_data,"setAni Error!!!");
    int index = time/_data->frameTime;
    CCASSERT(index >= 0 && index < _data->totalFrames,"setFrameIndex Error!!!");
    if (_isUpend) {
        index = _data->totalFrames-index-1;
    }
    if (index != _frameIndex) {
        _frameIndex = index;
        showFrame();
        return true;
    }
    return false;
}
int X9FLAnimationFrame::getFrameIndex()
{
    if (_isUpend) {
        return _data->totalFrames-_frameIndex-1;
    }
    return _frameIndex;
}
vector<string> X9FLAnimationFrame::getFrameLabels(int index)
{
    if(_data->labelIndexs.find(index) != _data->labelIndexs.end())
    {
        return _data->labelIndexs[index];
    }
    return vector<string>();
}
float X9FLAnimationFrame::getFrameTiem()
{
    CCASSERT(_data,"setAni Error!!!");
    return _frameIndex*_data->frameTime;
}
bool X9FLAnimationFrame::getIsPosFrame(const string& name)
{
    bool b = _data->posFrames.find(name) != _data->posFrames.end();
    if (b) {
        return true;
    }
    for (int i = 0; i<_elements.size(); i++) {
        X9FLAnimationFrame* f = dynamic_cast<X9FLAnimationFrame*>(_elements[i]);
        if (f && f->getIsPosFrame(name)) {
            return true;
        }
    }
    return false;
}
X9PosFrame X9FLAnimationFrame::getPosFrame(const string& name)
{
    if(_data->posFrames.find(name) != _data->posFrames.end()) {
        if(_data->posFrames[name].find(_frameIndex) != _data->posFrames[name].end()) {
            X9PosFrame pf = *_data->posFrames[name][_frameIndex];
            pf.a = convertToWorldSpace(pf.a);
            pf.b = convertToWorldSpace(pf.b);
            return pf;
        }else{
            return X9PosFrame();
        }
    }
    for (int i = 0; i<_elements.size(); i++) {
        X9FLAnimationFrame* f = dynamic_cast<X9FLAnimationFrame*>(_elements[i]);
        if (f && f->getIsPosFrame(name)) {
            return f->getPosFrame(name);
        }
    }
    return X9PosFrame();
}
void X9FLAnimationFrame::setStick(const Color3B& stickCOlor, float stickThickness)
{
    _stickCOlor = stickCOlor;
    _stickThickness = stickThickness;
    showFrame();
}

//MARK:X9FLAnimation
X9FLAnimation* X9FLAnimation::create()
{
    CREATE_NEW_AUTO_OBJ(X9FLAnimation,);
}
X9FLAnimation* X9FLAnimation::create(const string& name, const Color3B& stickCOlor, float stickThickness)
{
    X9FLAnimation* mc = create();
    mc->setAni(name,stickCOlor,stickThickness);
    return mc;
}
X9FLAnimation::X9FLAnimation():X9FLAnimationFrame("X9FLAnimation")
{
    _time = 0;
//    _startIndex = 0;
//    _endindex = 0;
    _aniTimeLength = 0;
//    _loopCount = 0;
    _isPlaying = false;
}
void X9FLAnimation::removed()
{
    _time = 0;
//    _startIndex = 0;
//    _endindex = 0;
    _aniTimeLength = 0;
//    _loopCount = 0;
    _isPlaying = false;
    unscheduleUpdate();
    X9FLAnimationFrame::removed();
}
void X9FLAnimation::setAni(const string& name, const Color3B& stickCOlor, float stickThickness, int frameIndex)
{
    X9FLAnimationFrame::setAni(name,stickCOlor,stickThickness, frameIndex);
    _time = frameIndex*_data->frameTime;
//    _startIndex = 0;
//    _endindex = _data->totalFrames;
    _aniTimeLength = _data->time;
//    _loopCount = -1;
}
void X9FLAnimation::update(float delay)
{
    _time += delay;
    if (_time >= _aniTimeLength) {
        _time = _time-floor(_time/_aniTimeLength)*_aniTimeLength;
//        if (_loopCount > 0) {
//            _loopCount--;
//        }
    }
    setFrameIndex(_time/_data->frameTime);
//    if (_loopCount == 0) {
//        unscheduleUpdate();
//    }
}
void X9FLAnimation::play()
{
    if(_isPlaying) return;
    scheduleUpdate();
}
void X9FLAnimation::gotoAndPlay(int index)
{
    play();
    _time = index*_data->frameTime;
    setFrameTime(_time);
}
void X9FLAnimation::gotoAndPlay(const string& label)
{
    if(_data->labels.find(label) != _data->labels.end())
    {
        gotoAndPlay(_data->labels[label]);
    }
}
void X9FLAnimation::gotoAndStop(int index)
{
    stop();
    _time = index*_data->frameTime;
    setFrameIndex(index);
}
void X9FLAnimation::gotoAndStop(const string& label)
{
    if(_data->labels.find(label) != _data->labels.end())
    {
        gotoAndStop(_data->labels[label]);
    }
}
void X9FLAnimation::stop()
{
    if(!_isPlaying) return;
    unscheduleUpdate();
}