//
//  X9UserDefined.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/14.
//
//

#ifndef X9UserDefined_hpp
#define X9UserDefined_hpp

#include "X9Object.h"

class X9UserDefault : public X9Object
{
    string _key;
    void initObject(const vector<X9ValueObject*>& vs);
public:
    map<string,X9Object*> dict;
    X9Object* _data;
    bool _isAutoSave;
public:
    static X9UserDefault* create();
    X9UserDefault();
    static void setBaseFunctions(X9Library* library, const string& className);
    static void setConstValues(X9ScriptClassData* classData);
    virtual void runCtor(const vector<X9ValueObject*>& vs)override{initObject(vs);};
    virtual void removed()override;
    void save();
    void clear();
};

#endif /* X9UserDefined_hpp */
