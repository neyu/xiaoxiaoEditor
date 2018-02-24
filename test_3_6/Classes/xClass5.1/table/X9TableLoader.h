//
//  X9TableLoader.hpp
//  test_3_6
//
//  Created by xiaoxiaozzq on 17/9/27.
//
//

#ifndef X9TableLoader_hpp
#define X9TableLoader_hpp

#include "csAppClient.h"
class X9TableItem
{
public:
    struct TValue
    {
        union{
            float f;
            bool b;
            string* s;
        }value;
        TValue(){}
        void clear(bool isString);
    };
    typedef enum {
        NUMBER = 1,
        BOOL,
        STRING
    }Type;
    vector<string> names;
    vector<Type> types;
    vector<vector<TValue*>> values;
    ~X9TableItem();
};
class X9TableLoader
{
public:
    static X9TableItem* load(const string& path);
};

#endif /* X9TableLoader_hpp */
