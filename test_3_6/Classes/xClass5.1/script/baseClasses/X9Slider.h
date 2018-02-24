//
//  X9BaseSlider.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/2.
//
//

#ifndef X9BaseSlider_hpp
#define X9BaseSlider_hpp

#include "X9Node.h"
class X9Slider : public X9Node
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    X9Node* _contentNode;
    DrawNode* _stencil;
    Size _size;
    Size _contentSize;
    float _horizontalValue;
    float _verticalValue;
//    virtual void _sendTouch(X9MainTouchType type, const vector<TouchPoint>& locations)override;
    virtual void _sendSliderTouch(X9MainTouchType type, const Vec2& location)override;
    void resetSize(const Size& size);
    void resetContentSize(const Size& size);
public:
//    static X9Slider* newSlider(X9Library* library);
    static X9Slider* create();
    X9Slider(const string& key);
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual void setMask(X9DisplayObject* node, float alpha = 0.5)override{};
};

#endif /* X9BaseSlider_hpp */
