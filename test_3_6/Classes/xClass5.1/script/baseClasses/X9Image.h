//
//  X9Image.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/5.
//
//

#ifndef X9Image_hpp
#define X9Image_hpp

#include "X9DisplayObject.h"

class X9Image : public X9DisplayObject
{
    void initObject(const vector<X9ValueObject*>& vs);
    Sprite* _mc;
public:
    string _source;
    string _blendMode;
    float _glow;
public:
    static X9Image* newImage(X9Library* library, const string& name = "");
    static X9Image* create();
    X9Image(const string& key);
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    
    void setImageFile(const string& fileName, const Rect& rect = Rect::ZERO);
    void setBlendMode(const string& blendMode);
    void setGlow(float glow);
};

#endif /* X9Image_hpp */
