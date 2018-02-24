//
//  X9AutoObject.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/8/21.
//
//

#ifndef X9AutoObject_hpp
#define X9AutoObject_hpp

#include "x9AppClient.h"
#define X9_SAFE_DELETE(p)           do { delete (p); } while(0)
#define CREATE_NEW_AUTO_OBJ(_N_,_K_) \
if (getPoolObjectCount(#_N_) == 0) { \
return new _N_(_K_); \
} \
return dynamic_cast<_N_*>(createPoolObject(#_N_))


class X9AutoObject
{
    static map<string,vector<X9AutoObject*>> _typePoolMemory;
    static map<string,map<X9AutoObject*,int>> _typeMemory;
    static map<string,map<X9AutoObject*,int>> _typeRemoveMemory;
protected:
//    bool _isInPool;
    static int getPoolObjectCount(const string& key);
    static X9AutoObject* createPoolObject(const string& key);
public:
    static void recycleUpdate();
    static void clearMemory();
protected:
    string _key;
    bool _isUse;
    int _useCount;
public:
    X9AutoObject(const string& key);
    ~X9AutoObject();
    virtual void use();
    virtual void nonuse();
    void recycle();
    void nonuseAndRecycle();
    int getUseCount();
    virtual void removed(){};
    //    bool isInPool(){return _isInPool;};
};
#endif /* X9AutoObject_hpp */
