//
//  X9Animation.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/20.
//
//

#ifndef X9Animation_hpp
#define X9Animation_hpp

#include "X9DisplayObject.h"
#include "../../animation/X9FLAnimation.h"
class X9MovieClip : public X9DisplayObject
{
//    static string ENTER_FRAME;
//    static string ENTER_LABEL;
//    static string PLAY_COMPLETE;
    void playSound(int idx);
public:
    string _aniName;
    X9FLAnimationFrame* _aniFrame;
    X9FLAnimationData* _aniData;
    int _playLoop;
    int _playLoopNum;
    int _startIndex;
    int _endIndex;
    int _playFrames;
    float _playTimeLength;
    bool _isPlaying;
    bool _isPlayStart;
    float _playTime;
    float _frameTime;
    
    Color3B _stickColor;
    float _stickThickness;
    
    void initObject(const vector<X9ValueObject*>& vs);
public:
    static X9MovieClip* create();
    static X9MovieClip* newMovieClip(X9Library* library,const string& name,int loop = 0, int stIdx = 0, int edIdx = -1);
    X9MovieClip();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual void addedToStage()override;
    virtual void removedFromStage()override;
    virtual void update(float delay)override;
public:
    void setAniName(const string& name, const Color3B& stickColor, float stickThickness);
    void setPlayLoop(int loop, int start = 0, int end = -1, bool isPlay = false);
    int getPlayLoop(){return _playLoop;};
    bool isPlaying(){return _isPlaying;};
    void play();
    void stop();
    void gotoAndPlay(int frameIndex, bool upend = false);
    void gotoAndPlay(const string& frameLabel, bool upend = false);
    void gotoAndStop(int frameIndex);
    void gotoAndStop(const string& frameLabel);
    void nextFrame();
    void prevFrame();
};

#endif /* X9Animation_hpp */
