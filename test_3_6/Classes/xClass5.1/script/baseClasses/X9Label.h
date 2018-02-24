//
//  X9Label.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/10.
//
//

#ifndef X9Label_hpp
#define X9Label_hpp

#include "X9DisplayObject.h"

class X9Label : public X9DisplayObject
{
    void initObject(const vector<X9ValueObject*>& vs);
public:
    Label* _label;
public:
    static X9Label* create();
    X9Label();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void removed()override;
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
};

#endif /* X9Label_hpp */
