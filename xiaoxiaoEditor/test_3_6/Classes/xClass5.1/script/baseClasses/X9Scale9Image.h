//
//  X9Scale9Image.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 18/1/18.
//
//

#ifndef X9Scale9Image_hpp
#define X9Scale9Image_hpp

#include "X9DisplayObject.h"

class X9Scale9Image : public X9DisplayObject
{
    void initObject(const vector<X9ValueObject*>& vs);
    ui::Scale9Sprite* _mc;
public:
    string _source;
    string _blendMode;
    float _glow;
public:
    static X9Scale9Image* newScale9Image(X9Library* library, const string& name, const Rect& rect);
    static X9Scale9Image* create();
    X9Scale9Image(const string& key);
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    
//    void setImageFile(const string& fileName, const Rect& rect);
    void setBlendMode(const string& blendMode);
};

#endif /* X9Scale9Image_hpp */
