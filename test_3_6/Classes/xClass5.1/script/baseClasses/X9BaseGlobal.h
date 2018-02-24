//
//  X9BaseGlobal.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/7/26.
//
//

#ifndef X9BaseGlobal_hpp
#define X9BaseGlobal_hpp

#include "X9Object.h"
class X9UserDefault;
class X9BaseGlobal : public X9Object
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    X9UserDefault* userDefault;
public:
    static X9BaseGlobal* create();
    X9BaseGlobal();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
};

#endif /* X9BaseGlobal_hpp */
