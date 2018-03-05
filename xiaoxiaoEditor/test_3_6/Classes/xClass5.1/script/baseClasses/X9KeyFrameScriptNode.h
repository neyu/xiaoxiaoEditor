//
//  X9KeyFrameScriptNode.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/10/5.
//
//

#ifndef X9KeyFrameScriptNode_hpp
#define X9KeyFrameScriptNode_hpp

#include "X9Node.h"
class X9MovieClip;
class X9ValueObject;
class X9KeyFrameScriptNode : public X9Node
{
    vector<X9ValueObject*> _emptyValues;
    void initObject(const vector<X9ValueObject*>& vs);
public:
    X9MovieClip* ani;
    map<int,string> keyFrames;
    float time;
    int frameIndex;
    int loopCount;
    int loopIndex;
    bool isPlaying;
    bool isFirstPlay;
    void initPlay();
public:
    static X9KeyFrameScriptNode* create();
    X9KeyFrameScriptNode(const string& key);
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    
    void gotoAndPlay(int index);
    void gotoAndStop(int index);
    void play();
    void stop();
    void setLoopCount(int loopCount, bool isPlay);
    virtual void update(float delay)override;
    void setFrameIndex(int idx);
};


#endif /* X9KeyFrameScriptNode_hpp */
