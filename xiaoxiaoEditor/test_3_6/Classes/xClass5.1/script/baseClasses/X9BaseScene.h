//
//  X9BaseScene.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/20.
//
//

#ifndef X9BaseScene_hpp
#define X9BaseScene_hpp

#include "X9Node.h"

class X9BaseScene : public X9Node
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    float _sceneWidth;
    float _sceneHeight;
    bool _isFillScreen;
    string nextSceneName;
    string prevSceneName;
    Color3B _backgroundColor;
public:
    static X9BaseScene* create();
    X9BaseScene();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
//    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    
};

#endif /* X9BaseScene_hpp */
