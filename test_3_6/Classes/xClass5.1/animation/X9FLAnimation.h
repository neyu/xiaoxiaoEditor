//
//  X9FLAnimation.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/14.
//
//

#ifndef X9FLAnimation_hpp
#define X9FLAnimation_hpp

#include "../X9AutoObject.h"
#include "X9FLAnimationData.h"
//MARK:X9FLBaseAnimationElement
class X9FLBaseAnimationElement : public X9AutoObject
{
protected:
    X9FLBaseEmtData* _elementData;
    Node* _selfNode;
public:
    X9FLBaseAnimationElement(const string& key);
    virtual void removed()override;
public:
    virtual void setElementData(X9FLBaseEmtData* elementData,int showFrameIndex,bool isAdd, const Color3B& stickCOlor, float stickThickness);
    X9FLBaseEmtData* getElementData(){return _elementData;};
    Node* getNode(){return _selfNode;}
};

//MARK:X9FLAnimationStickElement
class X9FLAnimationStickElement: public X9FLBaseAnimationElement, public Node
{
    static vector<ui::Scale9Sprite*> _stickPools;
public:
    static X9FLAnimationStickElement* create();
    static X9FLAnimationStickElement* create(X9FLBaseEmtData* data, const Color3B& stickCOlor, float stickThickness);
    X9FLAnimationStickElement();
    virtual void removed()override;
    virtual void setElementData(X9FLBaseEmtData* elementData,int showFrameIndex,bool isAdd, const Color3B& stickCOlor, float stickThickness)override;
    void resetChildCount(int stickCount);
};

//MARK:X9FLAnimationHeadElement
class X9FLAnimationHeadElement: public X9FLBaseAnimationElement, public Sprite
{
public:
    static X9FLAnimationHeadElement* create();
    static X9FLAnimationHeadElement* create(X9FLBaseEmtData* data, const Color3B& stickCOlor, float stickThickness);
    X9FLAnimationHeadElement();
    virtual void removed()override;
    virtual void setElementData(X9FLBaseEmtData* elementData,int showFrameIndex,bool isAdd, const Color3B& stickCOlor, float stickThickness)override;
};

//MARK:X9FLAnimationImageElement
class X9FLAnimationImageElement : public X9FLBaseAnimationElement, public Sprite
{
public:
    static X9FLAnimationImageElement* create();
    static X9FLBaseAnimationElement* create(X9FLBaseEmtData* data,int showFrameIndex,bool isAdd);
    X9FLAnimationImageElement();
    virtual void removed()override;
public:
    virtual void setElementData(X9FLBaseEmtData* elementData,int showFrameIndex,bool isAdd, const Color3B& stickCOlor, float stickThickness)override;
};


//MARK:X9FLAnimationMaskElement
class X9FLAnimationMaskElement : public X9AutoObject, public ClippingNode
{
    Node* _selfNode;
    Node* _group;
public:
public:
    static X9FLAnimationMaskElement* create();
    X9FLAnimationMaskElement();
    virtual void removed()override;
    void addMask(X9FLBaseAnimationElement* element);
    Node* getNode(){return _selfNode;}
};


//MARK:X9FLAnimationFrame
class X9FLAnimationFrame : public X9FLBaseAnimationElement, public Node
{
protected:
    X9FLAnimationData* _data;
    bool _isAdd;
    int _frameIndex;
    Color3B _stickCOlor;
    float _stickThickness;
    vector<X9FLAnimationMaskElement*> _masks;
    vector<X9FLBaseAnimationElement*> _elements;
    void showFrame();
public:
    static X9FLAnimationFrame* create();
    static X9FLBaseAnimationElement* create(X9FLBaseEmtData* data,int showFrameIndex,bool isAdd, const Color3B& stickCOlor, float stickThickness);
    static X9FLAnimationFrame* create(const string& aniName, const Color3B& stickCOlor, float stickThickness,int index = 0);
    X9FLAnimationFrame(const string& key="X9FLAnimationFrame");
    virtual void removed()override;
    virtual void clear();
    bool _isUpend;
public:
    virtual void setElementData(X9FLBaseEmtData* elementData,int showFrameIndex,bool isAdd, const Color3B& stickCOlor, float stickThickness)override;
    virtual void setAni(const string& name, const Color3B& stickCOlor, float stickThickness, int frameIndex = 0);
    virtual void setAni(X9FLAnimationData* data, const Color3B& stickCOlor, float stickThickness, int frameIndex = 0);
    bool setFrameIndex(int index);
    bool setFrameTime(float time);
    int getFrameIndex();
    vector<string> getFrameLabels(int index);
    float getFrameTiem();
    X9FLAnimationData* getData(){return _data;}
    X9PosFrame getPosFrame(const string& name);
    bool getIsPosFrame(const string& name);
    void setStick(const Color3B& stickCOlor, float stickThickness);
};
//MARK:X9FLAnimation
class X9FLAnimation : public X9FLAnimationFrame
{
    float _time;
    float _aniTimeLength;
//    int _startIndex;
//    int _endindex;
//    int _loopCount;
    bool _isPlaying;
public:
//    X9FLAnimationData* data;
    virtual void update(float delay)override;
public:
    static X9FLAnimation* create();
    static X9FLAnimation* create(const string& name, const Color3B& stickCOlor, float stickThickness);
    X9FLAnimation();
    virtual void removed()override;
    virtual void setAni(const string& name, const Color3B& stickCOlor, float stickThickness, int frameIndex = 0)override;
    void play();
    void gotoAndPlay(int index);
    void gotoAndPlay(const string& label);
    void gotoAndStop(int index);
    void gotoAndStop(const string& label);
    void stop();
};

#endif /* X9FLAnimation_hpp */
