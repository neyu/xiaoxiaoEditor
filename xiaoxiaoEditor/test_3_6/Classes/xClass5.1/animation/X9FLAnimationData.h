//
//  X9FLAnimationData.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/14.
//
//

#ifndef X9FLAnimationData_hpp
#define X9FLAnimationData_hpp

#include "../X9Path.h"

class X9FLFrameData;
class X9FLLayerData;
class X9FLAnimationData;
class X9FLAnimationLibrary;
class X9FLBaseEmtData
{
public:
    typedef enum {
        ST_IMAGE = 0,
        ST_ANI,
        ST_STICK,
        ST_HEAD,
    }SourceType;
    SourceType sourceType;
    virtual SourceType getSourceType(){return sourceType;}
    X9FLFrameData* parentData;
    X9FLBaseEmtData(X9FLFrameData* pData):parentData(pData){}
};
class X9FLHeadData : public X9FLBaseEmtData
{
public:
    float x;
    float y;
    float scaleX;
    float scaleY;
    float rotationSkewX;
    float rotationSkewY;
    X9FLHeadData(X9FLFrameData* parentData);
    X9FLHeadData(const rapidjson::Value &jsonData,X9FLFrameData* parentData);
};
class X9FLStickData : public X9FLBaseEmtData
{
public:
    struct Vec22
    {
        Vec2 a;
        Vec2 b;
        Vec22(){};
        Vec22(const Vec2& a, const Vec2& b):a(a),b(b){};
        Vec22(float x1, float y1, float x2, float y2);
    };
    vector<Vec22> points;
    X9FLStickData(X9FLFrameData* parentData);
    X9FLStickData(const rapidjson::Value &jsonData,X9FLFrameData* parentData);
};
class X9FLAniElementData: public X9FLBaseEmtData
{
public:
    typedef enum {
        LT_LOOP = 0,
        LT_PLAYER_ONCE,
        LT_SINGLE_FRAME
    }LoopType;
public:
    LoopType loopType;
    int firstFrame;
    int source;
    bool isSpriteFrame;
    string sourceName;
    string sourceURI;
    bool isScriptFrame;
    float x;
    float y;
    float scaleX;
    float scaleY;
    float rotationSkewX;
    float rotationSkewY;
    Color4B color;
//    Color4B colorB;
    bool isAddBlend;
public:
    X9FLAniElementData(X9FLFrameData* parentData);
    X9FLAniElementData(const rapidjson::Value &jsonData,X9FLFrameData* parentData);
    void setSource(int s);
//    const string& getSourceName();
//    const string& getSourceURI();
};
class X9FLFrameData
{
    vector<float> motionValues;
    vector<Vec2> rotationSkews;
    void makeMotionValues();
public:
    typedef enum {
        RT_NONE = 0,
        RT_AUTO,
        RT_CLOCKWISE,
        RT_COUNTER_CLOCKWISE,
    }RotateType;
public:
//    int _useLNum;
    X9FLLayerData* parentData;
    X9FLFrameData* prevData;
    X9FLFrameData* nextData;
    bool isEmpty;
public:
    int startFrame;
    int duration;
    RotateType rotateType;
    int rotateTimes;
    vector<Vec2> eases;
    bool isMotion;
    bool isMotionScale;
    vector<X9FLBaseEmtData*> elements;
public:
    X9FLFrameData(X9FLLayerData* parentData);
    X9FLFrameData(const rapidjson::Value &jsonData,X9FLLayerData* parentData,X9FLFrameData* prevData);
    ~X9FLFrameData();
public:
    X9FLFrameData* getMotionFrame(int f);
};
class X9FLLayerData
{
    vector<X9FLFrameData*> _realFrames;
public:
    typedef enum {
        LT_NORMAL = 0,
        LT_MASK,
        LT_MASKED,
    }LayerType;
public:
    X9FLAnimationData* parentData;
    LayerType layerType;
    bool isStickLayer;
    bool isHeadLayer;
    float maskNum;
    vector<X9FLFrameData*> frames;
public:
    X9FLLayerData(const rapidjson::Value &jsonData,X9FLAnimationData* parentData);
    ~X9FLLayerData();
};
//class X9FLSoundData
//{
//public:
//    string name;
//    int loop;
//public:
//    X9FLSoundData(const rapidjson::Value &jsonData, int start, X9FLAnimationData* parentData);
//};
class X9PosFrame
{
public:
    bool isEmpty;
    int startFrame;
    int duration;
    Vec2 a;
    Vec2 b;
    float r;
    bool reverse;
public:
    X9PosFrame():isEmpty(true){};
    X9PosFrame(const rapidjson::Value &jsonData);
};
class X9FLAnimationData
{
    bool _isFramesReady;
    int _useCount;
public:
    X9FLAnimationLibrary* library;
public:
    string name;
    float frameRate;
    float frameTime;
    float time;
    int totalFrames;
    vector<X9FLLayerData*> layers;
    vector<string> imgs;
    vector<string> anis;
    map<string,int> labels;
    map<int, vector<string>> labelIndexs;
    map<int,vector<string>> sounds;
    map<string,map<int,X9PosFrame*>> posFrames;
    vector<X9PosFrame*> usePosFrames;
public:
    X9FLAnimationData(const string& fileName, X9FLAnimationLibrary* library);
    ~X9FLAnimationData();
    void addSpriteFrames();
};

class X9FLAnimationLibrary
{
    static X9FLAnimationLibrary* _instance;
public:
    X9Path* path;
    map<string,X9FLAnimationData*> anis;
public:
    static X9FLAnimationLibrary* getInstance();
    X9FLAnimationLibrary();
    ~X9FLAnimationLibrary();
public:
    void setPath(X9Path* path);
    X9FLAnimationData* getAniData(const string& name);
//    void addSpriteFrames(X9FLAnimationData* aniData);
    void clear(const string& path = "");
};


#endif /* X9FLAnimationData_hpp */
