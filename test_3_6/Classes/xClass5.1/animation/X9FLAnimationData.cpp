//
//  X9FLAnimationData.cpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/14.
//
//

#include "X9FLAnimationData.h"
#include "XJsonMacrocoding.h"
//#include "../script/x9Utils.h"
//#include "../XUtils.h"
#include "../XMath.h"
#define DELETE_X9ANI_VECTOR(_V_) \
for (int i = 0; i<_V_.size(); i++) { \
delete _V_[i]; \
} \
_V_.clear()

#define _JS_DGETF(_D_,_V_,_N_,_T_,_F_) _V_ = _D_.HasMember(#_N_) ? _D_[#_N_].Get##_T_() : _F_
#define _JS_GETF(_V_,_N_,_T_,_F_) _V_ = _JS_DGETF(jsonData,_V_,_N_,_T_,_F_)


//MARK::X9FLStickData
X9FLStickData::Vec22::Vec22(float x1, float y1, float x2, float y2)
{
    a.set(x1, y1);
    b.set(x2, y2);
}
X9FLStickData::X9FLStickData(X9FLFrameData* parentData):X9FLBaseEmtData(parentData)
{
}
X9FLStickData::X9FLStickData(const rapidjson::Value &jsonData, X9FLFrameData* parentData):X9FLBaseEmtData(parentData)
{
    sourceType = ST_STICK;
    for (int i = 0; i<jsonData["s"].Size(); i+=4) {
        points.push_back(Vec22(jsonData["s"][i].GetDouble(),
                               -jsonData["s"][i+1].GetDouble(),
                               jsonData["s"][i+2].GetDouble(),
                               -jsonData["s"][i+3].GetDouble()
                               ));
    }
}
//MARK::X9FLHeadData
X9FLHeadData::X9FLHeadData(X9FLFrameData* parentData):X9FLBaseEmtData(parentData)
{
}
X9FLHeadData::X9FLHeadData(const rapidjson::Value &jsonData, X9FLFrameData* parentData):X9FLBaseEmtData(parentData)
{
    sourceType = ST_HEAD;
    _JS_GETF(x,x,Double,0);
    _JS_GETF(y,y,Double,0);
    y = -y;
    _JS_GETF(scaleX,sx,Double,1);
    _JS_GETF(scaleY,sy,Double,1);
    _JS_GETF(rotationSkewX,rx,Double,0);
    _JS_GETF(rotationSkewY,ry,Double,0);
}
//MARK::X9FLAniElementData
X9FLAniElementData::X9FLAniElementData(const rapidjson::Value &jsonData,X9FLFrameData* parentData):X9FLBaseEmtData(parentData)
{
    loopType = LT_LOOP;
    sourceType = ST_IMAGE;
    if (jsonData.HasMember("t")) {
        sourceType = (SourceType)jsonData["t"].GetInt();
    }
    _JS_GETF(source,s,Int,0);
    isScriptFrame = sourceType == ST_IMAGE && parentData->parentData->parentData->imgs.size() > 1;
    setSource(source);
    _JS_GETF(firstFrame,ff,Int,0);
    _JS_GETF(x,x,Double,0);
    _JS_GETF(y,y,Double,0);
    y = -y;
    _JS_GETF(scaleX,sx,Double,1);
    _JS_GETF(scaleY,sy,Double,1);
    _JS_GETF(rotationSkewX,rx,Double,0);
    _JS_GETF(rotationSkewY,ry,Double,0);
    isAddBlend = jsonData.HasMember("a");
    color = Color4B::WHITE;
    int a = 0;
//    colorB = Color4B::BLACK;
    if (jsonData.HasMember("c")) {
        color.r = jsonData["c"][a].GetInt()*2.55;
        color.g = jsonData["c"][(int)1].GetInt()*2.55;
        color.b = jsonData["c"][(int)2].GetInt()*2.55;
        color.a = jsonData["c"][(int)3].GetInt()*2.55;
//        colorB.r = jsonData["c"][(int)4].GetInt();
//        colorB.g = jsonData["c"][(int)5].GetInt();
//        colorB.b = jsonData["c"][(int)6].GetInt();
//        colorB.a = jsonData["c"][(int)7].GetInt();
    }
    //log("%d, %d, %d, %d",color.r,color.g,color.b,color.a);
    //log("isAddBlend:%d",isAddBlend);
}
X9FLAniElementData::X9FLAniElementData(X9FLFrameData* parentData):X9FLBaseEmtData(parentData)
{
}
void X9FLAniElementData::setSource(int s)
{
    if (sourceType == ST_IMAGE) {
        sourceName = parentData->parentData->parentData->imgs[s];
        if (isScriptFrame) {
            sourceURI = parentData->parentData->parentData->library->path->getAnimationPath(parentData->parentData->parentData->name+"/"+sourceName);
        }else{
            sourceURI = parentData->parentData->parentData->library->path->getAnimationPath(sourceName);
        }
    }else{
        sourceName = parentData->parentData->parentData->anis[s];
        sourceURI = parentData->parentData->parentData->library->path->getAnimationPath(sourceName+".x9ani");
    }
}
//const string& X9FLElementData::getSourceName()
//{
//    if (sourceType == ST_IMAGE) {
//        return parentData->parentData->parentData->imgs[source];
//    }
//    return parentData->parentData->parentData->anis[source];
//}
//const string& X9FLElementData::getSourceURI()
//{
//    if (sourceType == ST_IMAGE) {
//        return parentData->parentData->parentData->imgs[source];
//    }
//    return parentData->parentData->parentData->anis[source];
//}

//MARK::X9FLFrameData
X9FLFrameData::X9FLFrameData(X9FLLayerData* parentData)
{
    this->parentData = parentData;
    this->nextData = nullptr;
    this->prevData = nullptr;
    isMotion = false;
    isEmpty = true;
}
X9FLFrameData::X9FLFrameData(const rapidjson::Value &jsonData,X9FLLayerData* parentData,X9FLFrameData* prevData)
{
    this->parentData = parentData;
    this->nextData = nullptr;
    this->prevData = nullptr;
    isEmpty = false;
    _JS_GETF(startFrame,f,Int,0);
    _JS_GETF(duration,d,Int,0);
    if (prevData && prevData->startFrame+prevData->duration == startFrame) {
        this->prevData = prevData;
        prevData->nextData = this;
    }
    
    rotateType = RT_NONE;
    if (jsonData.HasMember("rt")) {
        rotateType = (RotateType)jsonData["rt"].GetInt();
    }
    _JS_GETF(rotateTimes,rn,Int,0);
    if (jsonData.HasMember("a")) {
        log("%d",(int)jsonData["a"].Size());
        for (int i = 0; i<jsonData["a"].Size(); i+=2) {
            float x = jsonData["a"][i].GetDouble();
            float y = jsonData["a"][i+1].GetDouble();
            eases.push_back(Vec2(x,y));
        }
    }
    isMotion = jsonData.HasMember("m");
    isMotionScale = !jsonData.HasMember("s");
    for (int i = 0; i<jsonData["e"].Size(); i++) {
        if (parentData->isStickLayer) {
            elements.push_back(new X9FLStickData(jsonData["e"][i],this));
        }else if(parentData->isHeadLayer) {
            elements.push_back(new X9FLHeadData(jsonData["e"][i],this));
        }else{
            elements.push_back(new X9FLAniElementData(jsonData["e"][i],this));
        }
    }
    if (!this->parentData->isStickLayer && this->prevData && this->prevData->isMotion) {
        this->prevData->makeMotionValues();
    }
}

X9FLFrameData::~X9FLFrameData()
{
    DELETE_X9ANI_VECTOR(elements);
}
void X9FLFrameData::makeMotionValues()
{
    for (int i = 1; i<duration; i++) {
        motionValues.push_back((float)i/(duration));
    }
    if (!eases.empty()) {
        for (int i = 0; i<motionValues.size(); i++) {
            float v = motionValues[i];
            const float x1 = eases[0].x;
            const float y1 = eases[0].y;
            const float x2 = eases[1].x;
            const float y2 = eases[1].y;
            vector<float> arr = XMath::solve3xEquation((x2-x1)*3-1,(x1*2-x2)*3,-x1*3,v);
            for (int k = 0; k<arr.size(); k++) {
                if (arr[k] >= 0 && arr[k] <= 1) {
                    v = arr[k];
                    break;
                }
            }
            motionValues[i] = XMath::mix3x(0, y1, y2, 1, v);
        }
    }
    
    
    X9FLAniElementData* A = dynamic_cast<X9FLAniElementData*>(elements.front());
    X9FLAniElementData* B = dynamic_cast<X9FLAniElementData*>(nextData->elements.front());
    switch (rotateType) {
        case RT_NONE:
            for (int i = 1; i<motionValues.size(); i++) {
                rotationSkews.push_back(Vec2(A->rotationSkewX,A->rotationSkewY));
            }
            break;
        case RT_AUTO:
        {
            float rxl = XMath::rotationLength(A->rotationSkewX, B->rotationSkewX);
            float ryl = XMath::rotationLength(A->rotationSkewY, B->rotationSkewY);
            if (rotateTimes > 0) {
                rxl += (rxl > 0 ? 360 : -360)*rotateTimes;
                ryl += (ryl > 0 ? 360 : -360)*rotateTimes;
            }
            for (int i = 0; i<motionValues.size(); i++) {
                rotationSkews.push_back(Vec2(motionValues[i]*rxl+A->rotationSkewX,motionValues[i]*ryl+A->rotationSkewY));
            }
            break;
        }
        case RT_CLOCKWISE:
        {
            float rxl = XMath::rotationLength(A->rotationSkewX, B->rotationSkewX,true);
            float ryl = XMath::rotationLength(A->rotationSkewY, B->rotationSkewY,true);
            if (rotateTimes > 0) {
                rxl += 360*rotateTimes;
                ryl += 360*rotateTimes;
            }
            //log("xr:%f, yr:%f",rxl,ryl);
            for (int i = 0; i<motionValues.size(); i++) {
                rotationSkews.push_back(Vec2(motionValues[i]*rxl+A->rotationSkewX,motionValues[i]*ryl+A->rotationSkewY));
            }
            break;
        }
        case RT_COUNTER_CLOCKWISE:
        {
            float rxl = -XMath::rotationLength(B->rotationSkewX, A->rotationSkewX,true);
            float ryl = -XMath::rotationLength(B->rotationSkewY, A->rotationSkewY,true);
            if (rotateTimes > 0) {
                rxl -= 360*rotateTimes;
                ryl -= 360*rotateTimes;
            }
            for (int i = 0; i<motionValues.size(); i++) {
                rotationSkews.push_back(Vec2(motionValues[i]*rxl+A->rotationSkewX,motionValues[i]*ryl+A->rotationSkewX));
            }
            break;
        }
    }
}
//#define X9F_Motion(_V_) newFrame->_V_ =
X9FLFrameData* X9FLFrameData::getMotionFrame(int f)
{
    X9FLFrameData* newFrame = new X9FLFrameData(parentData);
    newFrame->isEmpty = false;
    newFrame->startFrame = this->startFrame+f;
    
    
    X9FLAniElementData* A = dynamic_cast<X9FLAniElementData*>(elements.front());
    X9FLAniElementData* B = dynamic_cast<X9FLAniElementData*>(nextData->elements.front());
    X9FLAniElementData* N = new X9FLAniElementData(newFrame);
    N->sourceType = A->sourceType;
    N->source = A->source;
    N->sourceName = A->sourceName;
    N->sourceURI = A->sourceURI;
    N->loopType = A->loopType;
    if (N->sourceType == X9FLAniElementData::ST_ANI) {
        X9FLAnimationData* aniData = parentData->parentData->library->getAniData(A->sourceName);
        switch (A->loopType) {
            case X9FLAniElementData::LT_SINGLE_FRAME:
                N->firstFrame = A->firstFrame;
                break;
            case X9FLAniElementData::LT_PLAYER_ONCE:
                N->firstFrame = MIN(A->firstFrame+f,aniData->totalFrames);
                break;
            case X9FLAniElementData::LT_LOOP:
                N->firstFrame = (A->firstFrame+f)%aniData->totalFrames;
                break;
        }
    }
    float v = motionValues[f];
    N->x = XMath::mix(A->x, B->x, v);
    N->y = XMath::mix(A->y, B->y, v);
    if (isMotionScale) {
        N->scaleX = XMath::mix(A->scaleX, B->scaleX, v);
        N->scaleY = XMath::mix(A->scaleY, B->scaleY, v);
    }else{
        N->scaleX = A->scaleX;
        N->scaleY = A->scaleY;
    }
    N->rotationSkewX = rotationSkews[f].x;
    N->rotationSkewY = rotationSkews[f].y;
    N->color.r = XMath::mix(A->color.r, B->color.r, v);
    N->color.g = XMath::mix(A->color.g, B->color.g, v);
    N->color.b = XMath::mix(A->color.b, B->color.b, v);
    N->color.a = XMath::mix(A->color.a, B->color.a, v);
//    N->colorB.r = mix(A->colorB.r, B->colorB.r, v);
//    N->colorB.g = mix(A->colorB.g, B->colorB.g, v);
//    N->colorB.b = mix(A->colorB.b, B->colorB.b, v);
//    N->colorB.a = mix(A->colorB.a, B->colorB.a, v);
    N->isAddBlend = A->isAddBlend;
    newFrame->elements.push_back(N);
    return newFrame;
}
//MARK::X9FLLayerData
X9FLLayerData::X9FLLayerData(const rapidjson::Value &jsonData,X9FLAnimationData* parentData)
{
    this->parentData = parentData;
    _realFrames.push_back(new X9FLFrameData(this));
    layerType = LT_NORMAL;
    isStickLayer = jsonData.HasMember("s");
    isHeadLayer = jsonData.HasMember("h");
    if (jsonData.HasMember("t")) {
        layerType = (LayerType)jsonData["t"].GetInt();
    }
    _JS_GETF(maskNum,mn,Double,0.5);
    vector<X9FLFrameData*> temp;
    for (int i = 0; i<jsonData["f"].Size(); i++) {
        temp.push_back(new X9FLFrameData(jsonData["f"][i],this,i > 0 ? temp[i-1] : nullptr));
    }
    for (int i = 0; i<temp.size(); i++) {
        X9FLFrameData* frame = temp[i];
        if (frame->prevData == nullptr) {
            while (frames.size() < frame->startFrame) {
                frames.push_back(_realFrames.front());
            }
        }
        _realFrames.push_back(frame);
        frames.push_back(frame);
        if (frame->isMotion) {
            for (int k = 1;k<frame->duration; k++) {
                X9FLFrameData* newFrame = frame->getMotionFrame(k-1);
                _realFrames.push_back(newFrame);
                frames.push_back(newFrame);
            }
        }else{
            for (int k = 1;k<frame->duration; k++) {
                frames.push_back(frame);
            }
        }
    }
    while (frames.size() < parentData->totalFrames) {
        frames.push_back(_realFrames.front());
    }
}
X9FLLayerData::~X9FLLayerData()
{
    frames.clear();
    DELETE_X9ANI_VECTOR(_realFrames);
}
////MARK::X9FLSoundData
//X9FLSoundData::X9FLSoundData(const rapidjson::Value &jsonData, int start, X9FLAnimationData* parentData)
//{
//    this->name = jsonData[start].GetString();
//    this->loop = jsonData[start+1].GetInt();
//}
//MARK::X9PosFrame
X9PosFrame::X9PosFrame(const rapidjson::Value &jsonData)
{
    isEmpty = false;
    _JS_GETF(startFrame, f, Int, 0);
    _JS_GETF(duration, d, Int, 1);
    _JS_GETF(a.x, x, Double, 0);
    _JS_GETF(a.y, y, Double, 0);
    a.y = -a.y;
    _JS_GETF(r, r, Double, 0);
    b.x = cos(r)+a.x;
    b.y = sin(r)+a.y;
    reverse = jsonData.HasMember("s");
}
//MARK::X9FLAnimationData
X9FLAnimationData::X9FLAnimationData(const string& fileName, X9FLAnimationLibrary* library)
{
    _isFramesReady = false;
    this->library = library;
    loadJsonDoc(_doc,fileName);
    this->name = _doc["name"].GetString();
    this->frameRate = _doc["frameRate"].GetDouble();
    this->totalFrames = _doc["totalFrames"].GetInt();
    this->frameTime = 1.0f/this->frameRate;
    this->time = this->totalFrames*this->frameTime;
    if (_doc.HasMember("imgs")) {
        for (int i = 0; i<_doc["imgs"].Size(); i++) {
            this->imgs.push_back(_doc["imgs"][i].GetString());
        }
    }
    if (_doc.HasMember("anis")) {
        for (int i = 0; i<_doc["anis"].Size(); i++) {
            this->anis.push_back(_doc["anis"][i].GetString());
        }
    }
    if (_doc.HasMember("labels")) {
        for (auto it = _doc["labels"].MemberonBegin(); it != _doc["labels"].MemberonEnd(); it++) {
            int n = it->value.GetInt();
            const string& name = it->name.GetString();
            labels[name] = n;
            if (labelIndexs.find(n) == labelIndexs.end()) {
                labelIndexs[n] = vector<string>();
            }
            labelIndexs[n].push_back(name);
        }
    }
    if (_doc.HasMember("sounds")) {
        for (int i = 0; i<_doc["sounds"].Size(); i+=2) {
            int idx = _doc["sounds"][i].GetInt();
            if (sounds.find(idx) == sounds.end()) {
                sounds[idx] = vector<string>();
            }
            sounds[idx].push_back(_doc["sounds"][i+1].GetString());
        }
    }
    if (_doc.HasMember("pos")) {
        for (auto it = _doc["pos"].MemberonBegin(); it != _doc["pos"].MemberonEnd(); it++) {
            const string& posName = it->name.GetString();
            posFrames[posName] = map<int,X9PosFrame*>();
            for (int i = 0; i<it->value.Size(); i++) {
                X9PosFrame* posFrame = new X9PosFrame(it->value[i]);
                usePosFrames.push_back(posFrame);
                for (int k = 0; k<posFrame->duration; k++) {
                    posFrames[posName][k+posFrame->startFrame] = posFrame;
                }
            }
//            _JS_GETF(rotateTimes,rn,Int,0);
        }
    }
    for (int i = 0; i<_doc["layers"].Size(); i++) {
        this->layers.push_back(new X9FLLayerData(_doc["layers"][i],this));
    }
}
X9FLAnimationData::~X9FLAnimationData()
{
    DELETE_X9ANI_VECTOR(layers);
    DELETE_X9ANI_VECTOR(usePosFrames);
    sounds.clear();
}
void X9FLAnimationData::addSpriteFrames()
{
    if(_isFramesReady) return;
    _isFramesReady = true;
    if (imgs.size() > 1) {
        //SpriteFrameCache::getInstance()->addSpriteFramesWithFile(library->path->getAnimationPath(name+".plist"));
    }
}

X9FLAnimationLibrary* X9FLAnimationLibrary::_instance = nullptr;
X9FLAnimationLibrary* X9FLAnimationLibrary::getInstance()
{
    if (_instance == nullptr) {
        _instance = new X9FLAnimationLibrary();
    }
    return _instance;
}
X9FLAnimationLibrary::X9FLAnimationLibrary()
{
    path = nullptr;
}
X9FLAnimationLibrary::~X9FLAnimationLibrary()
{
    
}
void X9FLAnimationLibrary::setPath(X9Path* path)
{
    this->path = path;
}
X9FLAnimationData* X9FLAnimationLibrary::getAniData(const string& name)
{
    if (anis.find(name) == anis.end()) {
        anis[name] = new X9FLAnimationData(path->getAnimationPath(name+".x9ani"),this);
    }
    return anis[name];
}
//void X9FLAnimationLibrary::addSpriteFrames(X9FLAnimationData* aniData)
//{
//    SpriteFrameCache::getInstance()->addSpriteFramesWithFile(aniPath+aniData->name+".plist");
//}
void X9FLAnimationLibrary::clear(const string& path)
{
    
}