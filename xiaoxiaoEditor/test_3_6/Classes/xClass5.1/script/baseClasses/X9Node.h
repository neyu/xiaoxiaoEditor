//
//  X9Node.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/1.
//
//

#ifndef X9Node_hpp
#define X9Node_hpp

#include "X9DisplayObject.h"

class X9Node : public X9DisplayObject
{
    //    Scheduler* _scheduler;
    bool _beginSliderDistance;
//    bool _isCanDispatchTouchEvent;
    Vec2 _beginTouchLocation;
    bool _isReadyGetSliderNode;
    X9Node* _sliderNode;
    Vec2 _oldTouchLocation;
    Rect _dragRounds;
    bool _isDrag;
    Vec2 _dragOP;
    Vec2 _dragMP;
    bool _isDragLockCenter;
    X9Node* getSliderNode(bool bx, bool by, float x, float y);
    void initObject(const vector<X9ValueObject*>& vs);
//    vector<X9Node*> _touchNodes;
public:
    struct TouchPoint
    {
        Vec2 location;
        int id;
        TouchPoint(){};
    };
    typedef enum {
        X9MT_BEGIN = 1,
        X9MT_MOVED,
        X9MT_ENDED,
        X9MT_OVER,
        X9MT_OUT,
        X9MT_OUTSIDE_MOVED,
        X9MT_OUTSIDE_ENDED,
        X9MT_CANCELLED,
    }X9MainTouchType;
    virtual void _sendTouch(X9MainTouchType type, const vector<TouchPoint>& locations);
    virtual void _sendSliderTouch(X9MainTouchType type, const Vec2& location){};
    void readyDispatchTouchEvent(X9MainTouchType type, const string& typeStr, const Vec2& location, const Vec2& playerLocation, int ID);
    virtual void dispatchTouchEvent(X9MainTouchType type, const string& typeStr, const Vec2& location, const Vec2& playerLocation,int ID);
//    int touchChildCount;
//    void addTouchChild(X9Node* node);
//    void removeTouchChild(X9Node* node);
//    const vector<X9Node*>& getTouchNodes(){return _touchNodes;};
public:
    ClippingNode* maskNode;
    X9ValueObject* maskValueObj;
    bool _horizontalSliderEnalbed;
    bool _verticalSliderEnalbed;
    bool _isOnlyTouchEvent;
    bool _isCacheAsBitmap;
    Size _cacheAsBitmapSize;
    bool isUpdate;
    bool isCanUpdate;
    vector<X9DisplayObject*> elements;
    bool touchEnabled;
    Rect touchRect;
//    Size size;
    vector<bool> _oldTouchInsides;
public:
    static X9Node* newNode(X9Library* library);
    static X9Node* create();
    X9Node(const string& key);
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void use()override;
    virtual void nonuse()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual void setIsInStage(bool value)override;
    virtual void update(float delay)override;
    virtual void addedToStage()override;
    virtual void removedFromStage()override;
    virtual void setTouchEnabled(bool enabled, const Rect& rect = Rect::ZERO);
    void setTouchRect(const Rect& rect);
    const Rect& getTouchRect();
    Vec2 getTouchLocation(int ID);
    void setCacheAsBitmap(bool value,const Size& size = Size::ZERO);
    bool getCacheAsBitmap(){return _isCacheAsBitmap;};
    virtual void setMask(X9DisplayObject* node, float alpha = 0.5);
    virtual void clearMask();
    void startUpdate();
    void stopUpdate();
    void addElement(X9DisplayObject* element);
    void addElementAt(X9DisplayObject* element, int index);
    void setElementIndex(X9DisplayObject* element, int index);
    X9DisplayObject* getElement(int index);
    int getElementIndex(X9DisplayObject* element);
    void removeElement(X9DisplayObject* element);
    void removeElementByIndex(int index);
    void removeElements(int stIdx = 0,int edIdx = -1);
    
    
    virtual void updateDisplayedTimeSpeed(float parentSpeed)override;
    virtual void disableCascadeTimeSpeed()override;
    
    void startDrag(bool lockCenter, const Rect& touchRect, const Rect& bounds);
    void stopDrag();
};


#endif /* X9Node_hpp */
