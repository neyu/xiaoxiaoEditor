//
//  X9MovieClipEvent.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/24.
//
//

#ifndef X9MovieClipEvent_hpp
#define X9MovieClipEvent_hpp

#include "../X9Event.h"
class X9MovieClipEvent : public X9Event
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    static string ENTER_FRAME;
    static string ENTER_LABEL;
    static string PLAY_COMPLETE;
    int frameIndex;
    string frameLabel;
    int loop;
    int loopIndex;
public:
    static X9MovieClipEvent* newMovieClipEvent(X9Library* library,const string& type, int frameIndex, int loop, int loopIndex, const string& label);
    static X9MovieClipEvent* newMovieClipEvent(X9Library* library,const string& type, int frameIndex, int loop, int loopIndex);
    static X9MovieClipEvent* create();
    X9MovieClipEvent();
    
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
};

#endif /* X9MovieClipEvent_hpp */
