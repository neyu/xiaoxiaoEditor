//
//  X9TouchEvent.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/24.
//
//

#ifndef X9TouchEvent_hpp
#define X9TouchEvent_hpp

#include "../X9Event.h"
class X9TouchEvent : public X9Event
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    //cancelled
    static string TOUCH_BEGIN;
    static string TOUCH_MOVED;
    static string TOUCH_ENDED;
    static string TOUCH_OUTSIDE_MOVED;
    static string TOUCH_OUTSIDE_ENDED;
    static string TOUCH_OUT;
    static string TOUCH_OVER;
    static string TOUCH_CANCELLED;
    static string DRAG_BEGIN;
    static string DRAG_MOVED;
    static string DRAG_ENDED;
    Vec2 stageLocatiion;
    Vec2 location;
    int ID;
public:
    static X9TouchEvent* newTouchEvent(X9Library* library,const string& type, int ID, const Vec2& location, const Vec2& stageLocation);
    static X9TouchEvent* create();
    X9TouchEvent();
    
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
};


#endif /* X9TouchEvent_hpp */
